#include "../include/CameraNode.h"

CameraNode::CameraNode(string path, int type, int method, SceneNode* parent, glm::vec3 position) : SceneNode(path, type, method, parent) 
{
    c = new Camera(position);
    *relTransform = glm::translate(*relTransform,position);
};

// CameraNode::CameraNode(string path, int type, int method, SceneNode* parent, glm::vec3 position) : SceneNode(path, type, method, parent) 
// {
//     // cout << fr.x << " " << fr.y << " " << fr.z << endl;
//     c = new Camera(position);
//     // cout << c->Front.x << " " << c->Front.y << " " << c->Front.z << endl;
//     *relTransform = glm::translate(*relTransform,position);
// };

CameraNode::CameraNode(string path, int type, int method, SceneNode* parent, glm::vec3 position, float yaw, float pitch) : SceneNode(path, type, method, parent) 
{
    c = new Camera(position,glm::vec3(0.0f,1.0f,0.0f),yaw, pitch);
    *relTransform = glm::translate(*relTransform,position);
};
