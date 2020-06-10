#ifndef SPOTLIGHTNODE_H
#define SPOTLIGHTNODE_H

#include "LightNode.h"

class SpotLightNode : public LightNode 
{
    public:
        SpotLightNode(string path, int type, int method, SceneNode* parent, int lightnum, glm::vec3 position, glm::vec3 direction);

        virtual void updateShader(Shader *s);
        glm::vec3 lightdir;
        glm::vec3 oldlightdir;
        glm::mat4 ogposmatrix;

};

#endif