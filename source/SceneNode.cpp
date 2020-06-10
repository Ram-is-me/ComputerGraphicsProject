#include "../include/SceneNode.h"

SceneNode::SceneNode(string path, int type, int method, SceneNode* parent)
{
    object = new Model(path,type,method);
    this->parent = parent;
    if(parent == NULL)
    {
        relTransform = new glm::mat4(1.0f);
        comTransform = new glm::mat4(1.0f);
        drawTransform = new glm::mat4(1.0f);
        initialTransform = new glm::mat4(1.0f);
    }
    else 
    {
        parent->children.push_back(this);
        relTransform = new glm::mat4(1.0f);
        comTransform = new glm::mat4(1.0f);
        drawTransform = new glm::mat4(1.0f);
        *comTransform = (*parent->comTransform);
        initialTransform = new glm::mat4(1.0f);
    }
}

void SceneNode::update()
{
    if(parent != NULL)
    {
        *comTransform = *relTransform * *(parent->comTransform);
    }
    else 
    {
        *comTransform = *relTransform;
    }
    object->setModelMatrix(*comTransform);

}

void SceneNode::render(Shader *s, View *v, unsigned int* textures)
{
    // cout << glm::to_string(*relTransform) << endl;
    s->setInt("material.diffuse", texnumber);
    s->setInt("material.specular", texnumber);

    if(texnumber == 0)
        glActiveTexture(GL_TEXTURE0);
    else if(texnumber == 1)
        glActiveTexture(GL_TEXTURE1);
    else if(texnumber == 2)
        glActiveTexture(GL_TEXTURE2);
    else if(texnumber == 3)
        glActiveTexture(GL_TEXTURE3);
    else if(texnumber == 4)
        glActiveTexture(GL_TEXTURE4);
    else if(texnumber == 5)
        glActiveTexture(GL_TEXTURE5);
    else if(texnumber == 6)
        glActiveTexture(GL_TEXTURE6);
    
    // glEnable(GL_TEXTURE_2D);  
    glBindTexture(GL_TEXTURE_2D, textures[texnumber]);
    // cout << "LOL " << endl;
    object->setModelMatrix(*comTransform * *drawTransform * *initialTransform);
    if(enable)
        v->DrawWithoutTexture(s,*object);
    // object->setModelMatrix(*relTransform);
    for(int i=0;i<children.size();i++)
    {
        children[i]->render(s,v,textures);
    }
}

void SceneNode::updateSecondContext(SceneNode* root)
{
    object->SetupModelForSecondScreen();
    for(int i=0;i<children.size();i++)
    {
        children[i]->updateSecondContext(children[i]);
    }
}

SceneNode::~SceneNode()
{
    free(relTransform);
    free(drawTransform);
    free(comTransform);
    free(initialTransform);

}