#include <assimp/Importer.hpp>
#include <assimp/material.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "../vendor/stb_image.h"

#include "../include/mesh.h"
#include "../include/shader.h"

#include <iostream>
#include <map>
using namespace std;

struct BoneInfo {
    int id;
    glm::mat4 offset;
};

class Model {
    public:
        Model(string const &path){
            loadModel(path);
        }

        void Draw(Shader &shader){
            for(unsigned int i=0; i<meshes.size(); i++){
                meshes[i].Draw(shader);
            }
        }
    private:
        // model data
        vector<Mesh> meshes;
        vector<Texture> textures_loaded;
        string directory;

        // animation
        std::map<string, BoneInfo> m_BoneInfoMap;
        int m_BoneCounter = 0;

        auto& GetBoneInfoMap() {return m_BoneInfoMap;}
        int& GetBoneCount() {return m_BoneCounter;}

        void SetVertexBoneDataToDefault(Vertex& vertex){
            for(int i=0; i<MAX_BONE_WEIGHTS; i++){
                vertex.m_BoneIDs[i] = -1;
                vertex.m_Weights[i] = 0.0f;
            }
        }

        void loadModel(string const &path){
            // Create Instance of Importer class 
            Assimp::Importer importer;
            // Let's read the file
            const aiScene* scene = importer.ReadFile(path,
                    aiProcess_CalcTangentSpace |
                    aiProcess_Triangulate |
                    aiProcess_FlipUVs);

            // if the importer failed, report it
            if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
                cout << "ERROR::ASSIMP::" << importer.GetErrorString() << endl;
                return;
            }

            directory = path.substr(0, path.find_last_of('/'));

            // Now we can access the file's contents
            processNode(scene->mRootNode, scene);
        }

        void processNode(aiNode *node, const aiScene *scene){
            // process all the node's meshes
            for(unsigned int i=0; i<node->mNumMeshes; i++){
                aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
                meshes.push_back(processMesh(mesh, scene));
            }

            // repeat for all child nodes
            for(unsigned int i=0; i<node->mNumChildren; i++){
                processNode(node->mChildren[i], scene);
            }
        }

        Mesh processMesh(aiMesh *mesh, const aiScene *scene){
            vector<Vertex> vertices;
            vector<unsigned int> indices;
            vector<Texture> textures;

            for(unsigned int i=0; i<mesh->mNumVertices; i++){
                // process vertices
                Vertex vertex;

                SetVertexBoneDataToDefault(vertex);

                glm::vec3 vector;
                vector.x = mesh->mVertices[i].x;
                vector.y = mesh->mVertices[i].y;
                vector.z = mesh->mVertices[i].z;
                vertex.Position = vector;

                // process normals 
                if(mesh->HasNormals()){
                    vector.x = mesh->mNormals[i].x;
                    vector.y = mesh->mNormals[i].y;
                    vector.z = mesh->mNormals[i].z;
                    vertex.Normal = vector; 
                }

                // process texCoords 
                if(mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
                {
                    glm::vec2 vec;
                    vec.x = mesh->mTextureCoords[0][i].x; 
                    vec.y = mesh->mTextureCoords[0][i].y;
                    vertex.TexCoords = vec;
                }
                else
                    vertex.TexCoords = glm::vec2(0.0f, 0.0f);

                vertices.push_back(vertex);
            }

            // process indices
            for(unsigned int i=0; i<mesh->mNumFaces; i++){
                aiFace face = mesh->mFaces[i];
                for(unsigned int j=0; j<face.mNumIndices; j++){
                    indices.push_back(face.mIndices[j]);
                }
            }

            // process material
            if(mesh->mMaterialIndex >= 0){
                aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
                vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
                textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

                vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
                textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
            }

            ExtractBoneWeightForVertices(vertices, mesh, scene);

            return Mesh(vertices, indices, textures);
        }

        void SetVertexBoneData(Vertex& vertex, int boneID, float weight){
            for(int i=0; i<MAX_BONE_WEIGHTS; ++i){
                if(vertex.m_BoneIDs[i] < 0){
                    vertex.m_Weights[i] = weight;
                    vertex.m_BoneIDs[i] = boneID;
                    break;
                }
            }
        }

        void ExtractBoneWeightForVertices(std::vector& vertices, aiMesh* mesh, const aiScene* scene) {
            for(int boneIndex = 0; boneIndex < mesh->mNumBones; ++boneIndex){
                int boneID = -1;
                std::string boneName = mesh->mBones[boneIndex]->mName.C_Str();
                if(m_BoneInfoMap.find(boneName) == m_BoneInfoMap.end()){
                    BoneInfo newBoneInfo;
                    newBoneInfo.id = m_BoneCounter;
                    newBoneInfo.offset = AssimpGLMHelpers::ConvertMatrixToGLMFormat(
                            mesh->mBones[boneIndex]->mOffsetMatrix);
                    m_BoneInfoMap[boneName] = newBoneInfo;
                    boneID = m_BoneCounter;
                    m_BoneCounter++;
                }
                else {
                    boneID = m_BoneInfoMap[boneName].id;
                }

                assert(boneID != -1);
                auto weights = mesh->mBones[boneIndex]->mWeights;
                int numWeights = mesh->mBones[boneIndex]->mNumWeights;

                for(int weightIndex = 0; weightIndex < numWeights; ++weightIndex){
                    int vertexId = weights[weightIndex].mVertexId;
                    float weight = weights[weightIndex].mVertexId;
                }
            }
        }

        vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, string typeName){
            vector<Texture> textures;
            for(unsigned int i = 0; i < mat->GetTextureCount(type); i++)
            {
                aiString str;
                mat->GetTexture(type, i, &str);
                bool skip = false;
                for(unsigned int j = 0; j < textures_loaded.size(); j++)
                {
                    if(std::strcmp(textures_loaded[j].path.c_str(), str.C_Str()) == 0)
                    {
                        textures.push_back(textures_loaded[j]);
                        skip = true; 
                        break;
                    }
                }
                if(!skip)
                {   // if texture hasn't been loaded already, load it
                    Texture texture;
                    texture.id = TextureFromFile(str.C_Str(), directory);
                    texture.type = typeName;
                    texture.path = str.C_Str();
                    textures.push_back(texture);
                    textures_loaded.push_back(texture); // add to loaded textures
                }
            }
    return textures;
}
        unsigned int TextureFromFile(string const &path, const string &directory)
        {
            string filename = string(path);
            filename = directory + '/' + filename;

            unsigned int textureID;
            glGenTextures(1, &textureID);

            int width, height, nrComponents;
            unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
            if (data)
            {
                GLenum format;
                if (nrComponents == 1)
                    format = GL_RED;
                else if (nrComponents == 3)
                    format = GL_RGB;
                else if (nrComponents == 4)
                    format = GL_RGBA;

                glBindTexture(GL_TEXTURE_2D, textureID);
                glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
                glGenerateMipmap(GL_TEXTURE_2D);

                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

                stbi_image_free(data);
            }
            else
            {
                std::cout << "Texture failed to load at path: " << path << std::endl;
                stbi_image_free(data);
            }

            return textureID;
        }
};

