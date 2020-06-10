#ifndef CAMERANODE_H
#define CAMERANODE_H

#include "SceneNode.h"
#include <string>

class CameraNode : public SceneNode 
{
    public:
        CameraNode(string path, int type, int method, SceneNode* parent, glm::vec3 position);
        CameraNode(string path, int type, int method, SceneNode* parent, glm::vec3 position, float yaw, float pitch);

        Camera *c;
        float lastX = View::SCR_HEIGHT/2;
        float lastY = View::SCR_WIDTH/2;
        bool firstMouse = true;

};

#endif