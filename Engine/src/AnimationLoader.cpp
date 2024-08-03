#include "AnimationLoader.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "AssimpGLMHelpers.h"
#include "Bone.h"

void readSceneNodeData(MeshNodeData& nodeData, const aiNode* scene)
{
    assert(src);

    nodeData.name = scene->mName.data;
    nodeData.transformation = AssimpGLMHelpers::convertMat4ToGLMFormat(scene->mTransformation);
    nodeData.childrenCount = scene->mNumChildren;

    for (int i = 0; i < scene->mNumChildren; i++)
    {
        MeshNodeData newData;
        readSceneNodeData(newData, scene->mChildren[i]);
        nodeData.children.push_back(newData);
    }
}

void readAnimationBones(const aiAnimation* animation, std::unordered_map<std::string, std::shared_ptr<Bone>>& bones)
{
    unsigned int size = animation->mNumChannels;

    for (int i = 0; i < size; i++)
    {
        auto channel = animation->mChannels[i];
        std::string boneName = channel->mNodeName.data;

        std::vector<KeyPosition> positions;
        for (int i = 0; i < channel->mNumPositionKeys; i++)
        {
            KeyPosition keyPosition;
            keyPosition.timeStamp = channel->mPositionKeys[i].mTime;
            keyPosition.position = AssimpGLMHelpers::convertVec3ToGLMFormat(channel->mPositionKeys[i].mValue);
            positions.push_back(keyPosition);
        }

        std::vector<KeyRotation> rotations;
        for (int i = 0; i < channel->mNumRotationKeys; i++)
        {
            KeyRotation keyRotation;
            keyRotation.timeStamp = channel->mRotationKeys[i].mTime;
            keyRotation.orientation = AssimpGLMHelpers::convertQuatToGLMFormat(channel->mRotationKeys[i].mValue);
            rotations.push_back(keyRotation);
        }

        std::vector<KeyScale> scalings;
        for (int i = 0; i < channel->mNumScalingKeys; i++)
        {
            KeyScale keyScale;
            keyScale.timeStamp = channel->mScalingKeys[i].mTime;
            keyScale.scale = AssimpGLMHelpers::convertVec3ToGLMFormat(channel->mScalingKeys[i].mValue);
            scalings.push_back(keyScale);
        }
        auto& bone = std::make_shared<Bone>(positions, rotations, scalings);

        bones[boneName] = bone;
    }
}

std::shared_ptr<Animation> AnimationLoader::loadAnimation(const std::string& path)
{
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate);
    assert(scene && scene->mRootNode && scene->HasAnimations());
    auto aiAnimation = scene->mAnimations[0];

    MeshNodeData rootNode; //todo fix
    readSceneNodeData(rootNode, scene->mRootNode);

    std::unordered_map<std::string, std::shared_ptr<Bone>> bones;
    readAnimationBones(aiAnimation, bones);

    std::shared_ptr<Animation> animation = std::make_shared<Animation>(aiAnimation->mName, aiAnimation->mDuration, aiAnimation->mTicksPerSecond, rootNode, bones);


}
