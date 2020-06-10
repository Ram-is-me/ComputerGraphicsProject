#include "../include/SpotLightNode.h"

SpotLightNode::SpotLightNode(string path, int type, int method, SceneNode* parent, int lightnum, glm::vec3 position, glm::vec3 direction) : LightNode(path, type, method, parent, lightnum, position) 
{
    lightdir = direction - lightpos;
    // There's a need to re-orient the cylinder spotlight as it is actually pointing towards z axis.
    glm::mat4 temp = *relTransform;
    const float* pSource = (const float*) glm::value_ptr(temp);
    // cout << pSource[12] << " " << pSource[13] << " " << pSource[14] << endl;
    ogposmatrix = *relTransform;
    float costheta = glm::dot(lightdir,glm::vec3(0.0f,0.0f,1.0f))/(glm::dot(lightdir,lightdir));
    *relTransform = glm::rotate(*relTransform,glm::acos(costheta),glm::cross(glm::vec3(0.0f,0.0f,1.0f),lightdir));
    // glm::vec3 yaxis = glm::cross(lightdir,glm::vec3(0.0f,0.0f,1.0f));
    // *relTransform = glm::rotate(*relTransform,glm::radians(90.0f),yaxis);
    oldlightdir = lightdir;

    temp = *relTransform;
    pSource = (const float*) glm::value_ptr(temp);
    // cout << pSource[12] << " " << pSource[13] << " " << pSource[14] << endl;
};

void SpotLightNode::updateShader(Shader *s)
{
    s->setVec3("spotLight.position", lightpos);
    s->setVec3("spotLight.direction", lightdir);
    s->setVec3("spotLight.ambient", 0.2f, 0.2f, 0.2f);
    s->setVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
    s->setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
    s->setFloat("spotLight.constant", 1.0f);
    s->setFloat("spotLight.linear", 0.04);
    s->setFloat("spotLight.quadratic", 0.000);
    s->setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
    s->setFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));  

    // Am updating the rotations here as well
    // NEED TO STORE AND AXIS AND CROSS IT WITH THE DIRECTION I WISH TO CHANGE TO
    // float costheta = glm::dot(lightdir,oldlightdir)/(glm::dot(lightdir,lightdir) * glm::dot(oldlightdir,oldlightdir));
    // float costheta = glm::dot(glm::normalize(lightdir),glm::normalize(oldlightdir));
    // cout << "THESE ARE THE ANGLES " << costheta << endl;
    // cout << "THESE ARE LIGHT DIRECTIONS " << endl;
    // cout << "NEW LIGHT POS " << lightdir.x << " " << lightdir.y << " " << lightdir.z << endl;
    // cout << "OLD LIGHT POS " << oldlightdir.x << " " << oldlightdir.y << " " << oldlightdir.z << endl; 

    if(lightdir == oldlightdir) return;

    // *relTransform = glm::rotate(*relTransform,glm::acos(costheta),glm::cross(oldlightdir,lightdir));
    // *relTransform = glm::rotate(*relTransform,glm::acos(costheta),glm::cross(oldlightdir,lightdir)); // THERE WAS A GIMBAL LOCK HERE
    float costheta = glm::dot(lightdir,glm::vec3(0.0f,0.0f,1.0f))/(glm::dot(lightdir,lightdir));
    *relTransform = glm::rotate(ogposmatrix,glm::acos(costheta),glm::cross(glm::vec3(0.0f,0.0f,1.0f),lightdir));
    // cout << costheta << endl;
    // const float* pSource = (const float*) glm::value_ptr(lightdir);
    // cout << pSource[12] << " " << pSource[13] << " " << pSource[14] << endl;
    // pSource = (const float*) glm::value_ptr(oldlightdir);
    // cout << pSource[12] << " " << pSource[13] << " " << pSource[14] << endl;
    oldlightdir = lightdir;

}