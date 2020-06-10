#ifndef ANIMATION_H
#define ANIMATION_H

#include <bits/stdc++.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "SceneNode.h"
#include "SpotLightNode.h"
#include "CameraNode.h"

class Animation
{
    public:
        float simTime = 0.0;
        bool simOn = false;
        float startTime = glfwGetTime();
        float currentTime = glfwGetTime();
        bool part1 = false;

        // Animation Parameters
        //  Ballon Parameters
        float ballonhspeed = 0.1;
        float ballonvspeed = 0.1;
        float ballonradiusscale = 1.0;
        float ballonradiusscalespeed = 0.01;
        float ballonexplosiontime = 1000;

        glm::vec3 basketvelocity;
        glm::vec3 basketinitialvelocity;
        glm::mat4 basketinitialposition;
        glm::vec3 newbposition;
        glm::mat4 newbasketposition;
        bool groundcollision = false;

        float rabbitzinitialspeed = 3.0;
        glm::vec3 rabbitvelocity;
        glm::vec3 rabbitinitialvelocity;
        glm::mat4 rabbitinitialposition;
        glm::vec3 newrposition;
        glm::mat4 newrabbitposition;
        bool groundrcollision = false;

        float birdspeed = 10;
        glm::vec3 birdvelocity;
        glm::vec3 ballonposvec;
        glm::vec3 birdposvec;
        glm::vec3 birdnewpos;
        glm::mat4 birdnewposition;
        float prevtime;
        float curtime;
        bool toballon = true;
        bool awayballon = false;
        float collisiontime;
        float collisiongaptime = 3.0;
        glm::vec3 oppdirvelocity;

        // Avatar 1 Data
        glm::vec3 headdirection;
        glm::vec3 headposition;

        // Avatar 2 Data
        glm::vec3 headdirection2;
        glm::vec3 headposition2;

        // Avatar 3 Data
        glm::vec3 headdirection3;
        glm::vec3 headposition3;
        float movespeed = 0.5f;
        glm::vec3 movevelocity;
        float socialdistance = 18.0f;
        bool towards = false; // True is A, False is B

        void startSim()
        {
            simOn = true;
            startTime = glfwGetTime();
            currentTime = glfwGetTime();
            simTime = currentTime - startTime;
            prevtime = glfwGetTime();
            curtime = glfwGetTime();
        }

        void stopSim()
        {
            simOn = false;
        }

        void continueSim()
        {
            simOn = true;
        }

        void restartSim()
        {
            // NEED TO REPLACE EVERY OBJECT BACK TO ORIGINAL POSITION

            // startTime = glfwGetTime();
            // currentTime = glfwGetTime();
            // simTime = currentTime - startTime;
            simOn = true;
            startTime = glfwGetTime();
            currentTime = glfwGetTime();
            simTime = currentTime - startTime;
            prevtime = glfwGetTime();
            curtime = glfwGetTime();

            toballon = true;
            awayballon = false;

            groundrcollision = false;

            groundcollision = false;

            ballonradiusscale = 1.0;

            ballonexplosiontime = 1000;

            part1 = false;
        }

        void updateBallon(SceneNode* ballon);
        void updateBallonSpeed(float h, float v); // Could make this accept a vector

        void updateRabbit(SceneNode* rabbit);
        // Could Allow Setting of initial Jump velocity
        
        void updateBasket(SceneNode* basket);

        void updateBird(SceneNode* bird, SceneNode* ballon, SpotLightNode* spotlight, CameraNode* camera_bird);

        void updateAvatar1(SceneNode* avatar1head);

        void updateAvatar3Position(SceneNode* avatar1, SceneNode* avatar2, SceneNode* avatar3);

        void updateAvatar3(SceneNode* avatar3head, SceneNode* avatar2head, SceneNode* avatar1head);

};

#endif