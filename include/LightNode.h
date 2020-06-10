#ifndef LIGHTNODE_H
#define LIGHTNODE_H

#include "SceneNode.h"
#include <string>

class LightNode : public SceneNode 
{
    public:
        LightNode(string path, int type, int method, SceneNode* parent, int lightnum, glm::vec3 position);

        glm::vec3 lightpos;
        int lightnumber;

        virtual void updateShader(Shader *s);

        void render(Shader *s, View *v, unsigned int* textures);
};

#endif