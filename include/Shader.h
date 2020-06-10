#ifndef SHADER_H
#define SHADER_H

#include <GL/glew.h>
#include <glm/glm.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;

class Shader
{
    //
    public:
    
    // Program ID
    unsigned int ID;

    //Constructor - Reads and Builds the Shader
    Shader(string vertexPath, string fragmentPath);

    // Use/Activate the Shader
    void use();

    //Utility Uniform Functions
    void setBool(const string &name, bool value) const;
    void setInt(const string &name, int value) const;
    void setFloat(const string &name, float value) const;

    void setVec4(const string &name, const glm::vec4 &value) const;
    void setVec4(const string &name, float x, float y, float z, float w);

    void setVec3(const string &name, const glm::vec3 &value) const;
    void setVec3(const string &name, float x, float y, float z);

    void setMat2(const std::string &name, const glm::mat2 &mat) const;
    void setMat3(const std::string &name, const glm::mat3 &mat) const;
    void setMat4(const std::string &name, const glm::mat4 &mat) const;

};

#endif