#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <bits/stdc++.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "SceneNode.h"
#include "LightNode.h"
#include "SpotLightNode.h"
#include "CameraNode.h"
#include "Animation.h"

class Controller
{
    public:
    Shader* multiLightShader;
    Shader* clearShader;
    Shader* textureShader;
    Shader* lampShader;
    vector<Model> models;
    vector<Model> lights;
    int selection = 1;
    int selection2 = 2;
    
    bool walkingA = false;
    bool walkingB = false;
    bool walkingC = false;
    bool walkingmaincamera = true;

    unsigned int textures[16];

    SceneNode *scene_root;
    SceneNode *avatar1;
    SceneNode *avatar1head;
    SceneNode *avatar2;
    SceneNode *avatar2head;
    SceneNode *ballon;
    SceneNode *basket;
    SceneNode *rabbit;
    SceneNode *bird;
    SceneNode *field;
    SceneNode *avatar3;
    SceneNode *avatar3head;

    LightNode *light_root;
    LightNode *mainlight;
    LightNode *plight1;
    LightNode *plight2;
    LightNode *plight3;
    LightNode *plight4;
    SpotLightNode *spotlight;

    // Camera* camera; //
    // float lastX; //
    // float lastY; //
    // bool firstMouse; //

    CameraNode *camera_root;
    CameraNode *camera_avatar1left;
    CameraNode *camera_avatar1right;
    CameraNode *camera_avatar2left;
    CameraNode *camera_avatar2right;
    CameraNode *camera_avatar3left;
    CameraNode *camera_avatar3right;
    CameraNode *camera_bird;
    CameraNode *camera_main;

    float deltaTime;
    float lastFrame;
    float currentFrame;

    Trackball trackball;
    float rotation_transform[4][4];
    double px,py,rx,ry;

    int ccmode = 0;
    int nnmode = 1;

    int ptype = 1;

    glm::vec3 lightleft;
    glm::vec3 lightright;

    time_t startTime;
    time_t currentTime;

    // unsigned int texture1;
    // unsigned char* data;

    Animation a;

    bool enable1 = true;
    bool enable2 = true;
    bool enable3 = true;
    bool enable4 = true;
    bool enable5 = true;
    bool enablespot = true;

    Controller(vector<string> models, vector<string> lights, vector<string> shaders);

    void loadTexture(string texpath);
    
    void render(View* v, View* v2);

    void selectionShaderCameraView(View* v, Shader* shader, int leftOrRight);

    void resetPosition();

    void processInput(GLFWwindow *window, GLFWwindow *window2, int inputchoice);

    void mouseCamera(GLFWwindow *window);

    void mouseRotation(GLFWwindow *window);

    void mouseSelection(GLFWwindow *window);
};

#endif