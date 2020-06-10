#include "../include/LightNode.h"

LightNode::LightNode(string path, int type, int method, SceneNode* parent, int lightnum, glm::vec3 position) : SceneNode(path, type, method, parent) 
{
    lightnumber = lightnum;
    lightpos = position;
    *relTransform = glm::translate(*relTransform,position);
};

void LightNode::updateShader(Shader *s)
{
    s->setVec3("pointLights[" + to_string(lightnumber) + "].position", lightpos);
    s->setVec3("pointLights[" + to_string(lightnumber) + "].ambient", 0.4f, 0.4f, 0.4f);
    s->setVec3("pointLights[" + to_string(lightnumber) + "].diffuse", 0.8f, 0.8f, 0.8f);
    s->setVec3("pointLights[" + to_string(lightnumber) + "].specular", 1.0f, 1.0f, 1.0f);
    
    s->setFloat("pointLights[" + to_string(lightnumber) + "].constant", 1.0f);
    s->setFloat("pointLights[" + to_string(lightnumber) + "].linear", 0.04);
    s->setFloat("pointLights[" + to_string(lightnumber) + "].quadratic", 0.00);
}

void LightNode::render(Shader *s, View *v, unsigned int* textures)
{
    // cout << glm::to_string(*relTransform) << endl;
    // s->setInt("material.diffuse", texnumber);
    // if(texnumber == 0)
    //     glActiveTexture(GL_TEXTURE0);
    // else if(texnumber == 1)
    //     glActiveTexture(GL_TEXTURE1);
    // glBindTexture(GL_TEXTURE_2D, textures[texnumber]);
    // cout << "LOL " << endl;
    object->setModelMatrix(*comTransform * *drawTransform);
    if(enable)
        v->DrawWithoutTexture(s,*object);
    // object->setModelMatrix(*relTransform);
    for(int i=0;i<children.size();i++)
    {
        children[i]->render(s,v,textures);
    }
}