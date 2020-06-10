#include "../include/Animation.h"

void Animation::updateBallonSpeed(float h, float v)
{
    ballonhspeed = h;
    ballonvspeed = v;
}

void Animation::updateBallon(SceneNode *ballon)
{
    currentTime = glfwGetTime();
    float delTime = currentTime - startTime;
    if(simOn)
    {
        glm::mat4 update;
        // cout << glGetError() << delTime << endl;
        if(ballonradiusscale <= 2)
        {
            ballonradiusscale += ballonradiusscalespeed;
            update = glm::scale(glm::mat4(1.0f),glm::vec3(ballonradiusscale,ballonradiusscale,ballonradiusscale));
            // update = glm::scale(glm::mat4(1.0f),glm::vec3(1.0,1.0,1.0));
            *ballon->drawTransform = update;

            update = glm::mat4(1.0f);
            update = glm::translate(update,glm::vec3(ballonhspeed*delTime,ballonvspeed*delTime,0.0));
            *ballon->relTransform = update * *ballon->relTransform;
            // const float *rPos = (const float *) glm::value_ptr(*ballon->relTransform);
            // cout << glGetError() << " " << rPos[12] << " " << rPos[13] << " " << rPos[14] << endl; 
            
            // KEEP UPDATING THIS VALUE IN REAL TIME, SO THAT NO WEIRD CHANGES HAPPEN
            ballonexplosiontime = glfwGetTime();

        }
        else if(part1 == false)
        {
            ballonradiusscale += ballonradiusscalespeed;
            update = glm::scale(glm::mat4(1.0f),glm::vec3(ballonradiusscale,ballonradiusscale,ballonradiusscale));
            // update = glm::scale(glm::mat4(1.0f),glm::vec3(1.0,1.0,1.0));
            *ballon->drawTransform = update;

            update = glm::mat4(1.0f);
            update = glm::translate(update,glm::vec3(ballonhspeed*delTime,ballonvspeed*delTime,0.0));
            *ballon->relTransform = update * *ballon->relTransform;
            ballonexplosiontime = glfwGetTime();
            // basketinitialposition = *ballon->children[0]->comTransform;
            
            ballon->enable = false;
            basketinitialposition = glm::translate(glm::mat4(1.0f),glm::vec3(0.0,-5.0,0.0)); // Hardcoded position from ballon
            basketinitialvelocity = glm::vec3(ballonhspeed,ballonvspeed,0.0f);
            rabbitinitialposition = glm::translate(glm::mat4(1.0f),glm::vec3(0.0,-5.0,0.0)); // Hardcoded position from ballon
            rabbitinitialvelocity = glm::vec3(ballonhspeed,ballonvspeed,rabbitzinitialspeed);

            // const float *rPos = (const float *) glm::value_ptr(*ballon->relTransform);
            // cout << glGetError() << " " << rPos[12] << " " << rPos[13] << " " << rPos[14] << endl; 
            part1 = true;
            return;
        }
        else if(part1 == true)
        {
            ballon->enable = false;
            return;
        }
    }
    ballon->update();
}

void Animation::updateBasket(SceneNode *basket)
{
    if(simOn)
    {
        if(ballonradiusscale > 2 && groundcollision == false)
        {
            float delTime = glfwGetTime() - ballonexplosiontime;
            // delTime = delTime/1000;
            // s = ut - 1/2 g t ^ 2
            // glm::vec3 newposition;
            glm::vec3 checking = basketinitialvelocity * delTime;
            // cout << glGetError() << checking.x << " " << checking.y << " " << checking.z << endl;
            // cout << glGetError() << " " << ballonexplosiontime << endl;
            // newbposition = (basketinitialvelocity * delTime) + (glm::vec3(0.0f,-4.9f,0.0f) * delTime * delTime);
            newbposition = (basketinitialvelocity * delTime) + (glm::vec3(0.0f,-4.9f,0.0f) * delTime * delTime);
            // cout << glGetError() << newbposition.x << " " << newbposition.y << " " << newbposition.z << endl;
            newbasketposition = glm::translate(basketinitialposition,newbposition);
            
            glm::mat4 floorcheck = newbasketposition * *basket->comTransform;
            floorcheck = glm::translate(floorcheck,glm::vec3(0.0f,-ballonvspeed*ballonexplosiontime,0.0)); // Hardcoded the floor check with ballonvspeed. Should change if an intentional y velocity is given at the start
            const float *bPos = (const float *) glm::value_ptr(*basket->parent->comTransform);
            float offset = bPos[13];
            floorcheck = glm::translate(floorcheck,glm::vec3(0.0f,offset,0.0f));
            const float *pSource = (const float*) glm::value_ptr(floorcheck);
            // cout << glGetError() << pSource[13] << endl;

            if(pSource[13] < -2.0) groundcollision = true; 

            *basket->relTransform = newbasketposition;
            basket->update();
        }
    }
}

void Animation::updateRabbit(SceneNode* rabbit)
{
    if(simOn)
    {
        if(ballonradiusscale > 2 && groundrcollision == false)
        {
            float delTime = glfwGetTime() - ballonexplosiontime;
            // delTime = delTime/1000;
            // s = ut - 1/2 g t ^ 2
            // glm::vec3 newposition;
            // newbposition = (basketinitialvelocity * delTime) + (glm::vec3(0.0f,-4.9f,0.0f) * delTime * delTime);
            
            newrposition = (rabbitinitialvelocity * delTime) + (glm::vec3(0.0f,-4.9f,0.0f) * delTime * delTime);
            // cout << glGetError() << newrposition.x << " " << newrposition.y << " " << newrposition.z << endl;
            newrabbitposition = glm::translate(rabbitinitialposition,newrposition);
            
            glm::mat4 floorcheck = newrabbitposition * *rabbit->comTransform;
            floorcheck = glm::translate(floorcheck,glm::vec3(0.0f,-ballonvspeed*ballonexplosiontime,0.0)); // Hardcoded the floor check with ballonvspeed. Should change if an intentional y velocity is given at the start
            const float *rPos = (const float *) glm::value_ptr(*rabbit->parent->comTransform);
            float offset = rPos[13];
            floorcheck = glm::translate(floorcheck,glm::vec3(0.0f,offset,0.0f));
            const float *pSource = (const float*) glm::value_ptr(floorcheck);
            // cout << glGetError() << pSource[13] << endl;

            if(pSource[13] < -3.5) groundrcollision = true; // -2 for rabbit floor check, small model
            // if(offset + (-ballonvspeed*ballonexplosiontime) < -1.6) groundrcollision = true; // -2 for rabbit floor check, small model

            *rabbit->relTransform = newrabbitposition;
            rabbit->update();
        }
    }
}

void Animation::updateBird(SceneNode* bird, SceneNode* ballon, SpotLightNode* spotlight, CameraNode* camera_bird)
{
    if(simOn)
    {
        curtime = glfwGetTime();
        float delTime = curtime - prevtime;
        // CHECK BALLON DISTANCE
        if(toballon == true)
        {
            const float *pSource = (const float*) glm::value_ptr(*ballon->comTransform);             
            ballonposvec = glm::vec3(pSource[12],pSource[13],pSource[14]);
            pSource = (const float*) glm::value_ptr(*bird->comTransform); 
            birdposvec = glm::vec3(pSource[12],pSource[13],pSource[14]);
            birdvelocity = ballonposvec - birdposvec;
            birdvelocity = glm::normalize(birdvelocity);
            birdnewpos = birdvelocity + birdposvec;

            pSource = (const float*) glm::value_ptr(*spotlight->comTransform); 
            glm::vec3 spotlightpos = glm::vec3(pSource[12],pSource[13],pSource[14]);
            spotlight->lightdir = birdnewpos - spotlightpos; // Setting spotlight focus point
            // cout << spotlightpos.x << " " << spotlightpos.y << " " << spotlightpos.z << " A " << endl;
            // cout << spotlightpos.x << " " << spotlightpos.y << " " << spotlightpos.z << " A " << endl;
        

            glm::vec3 dist = birdnewpos - ballonposvec;
            float mag = sqrt(glm::dot(dist,dist));
            if(mag < 4.0f) // Collision Distance Hardcoded
            {
                // glm::mat4 temp = glm::rotate(glm::translate(glm::mat4(1.0f),glm::vec3(birdvelocity)),glm::radians(90.0f),glm::vec3(0.0f,1.0f,0.0f));
                glm::mat4 temp = glm::translate(glm::rotate(glm::mat4(1.0f),glm::radians(90.0f),glm::vec3(0.0f,1.0f,0.0f)),glm::vec3(birdvelocity));
                pSource = (const float*) glm::value_ptr(temp);
                birdvelocity.x = pSource[12];
                birdvelocity.y = pSource[13];
                birdvelocity.z = pSource[14];

                oppdirvelocity = birdvelocity;
                
                toballon = false;
                awayballon = true;
                collisiontime = glfwGetTime();          
            }

            birdvelocity = birdvelocity * birdspeed * (delTime);

            // cout << birdvelocity.x << " " << birdvelocity.y << " " << birdvelocity.z << endl;
            // cout << delTime << endl;
            birdnewposition = glm::translate(*bird->relTransform,birdvelocity);

            pSource = (const float*) glm::value_ptr(birdnewposition);
            // cout << pSource[12] << " " << pSource[13] << " " << pSource[14] << endl; 

            *bird->relTransform = birdnewposition;

        }
        else if(awayballon == true)
        {
            // float curtime = glfwGetTime();
            if(curtime - collisiontime < collisiongaptime)
            {
                const float *pSource = (const float*) glm::value_ptr(*ballon->comTransform); 
                ballonposvec = glm::vec3(pSource[12],pSource[13],pSource[14]);
                pSource = (const float*) glm::value_ptr(*bird->comTransform); 
                birdposvec = glm::vec3(pSource[12],pSource[13],pSource[14]);
                // birdvelocity = ballonposvec - birdposvec;
                // birdvelocity = glm::normalize(birdvelocity);
                // birdvelocity = -birdvelocity; // opposite direction of ballon
                birdvelocity = oppdirvelocity;
                
                // if(birdvelocity.y < 0) birdvelocity.y = -birdvelocity.y;

                if (!(curtime - collisiontime < collisiongaptime/5))
                    birdvelocity.y = 0.0; // Could randomize the direction of bird flying away
                
                
                // if(curtime - collisiontime < collisiongaptime/2)
                //     birdvelocity = oppdirvelocity;
                birdvelocity = birdvelocity * birdspeed * (delTime);

                birdnewpos = birdvelocity + birdposvec;
                            
                pSource = (const float*) glm::value_ptr(*spotlight->comTransform); 
                glm::vec3 spotlightpos = glm::vec3(pSource[12],pSource[13],pSource[14]);
                spotlight->lightdir = birdnewpos - spotlightpos; // Setting spotlight focus point

                // cout << spotlightpos.x << " " << spotlightpos.y << " " << spotlightpos.z << " B " << endl;
                // cout << delTime << endl;

                birdnewposition = glm::translate(*bird->relTransform,birdvelocity);

                *bird->relTransform = birdnewposition;                
            }
            else 
            {
                toballon = true;
                awayballon = false;

                const float* pSource = (const float*) glm::value_ptr(*spotlight->comTransform); 
                glm::vec3 spotlightpos = glm::vec3(pSource[12],pSource[13],pSource[14]);
                spotlight->lightdir = birdnewpos - spotlightpos; // Setting spotlight focus point

                // cout << spotlightpos.x << spotlightpos.y << spotlightpos.z << " C " << endl;

            }
        
        }
        // Camera Changes
        camera_bird->c->SetMotion(birdnewpos+glm::vec3(0.0f,1.0f,0.0f));
        camera_bird->c->Front = glm::normalize(birdvelocity);

        //Rotation Changes
        float costheta = glm::dot(glm::normalize(birdvelocity),glm::vec3(0.0f,-1.0f,0.0f));
        glm::mat4 rotation = glm::rotate(glm::mat4(1.0f),glm::acos(costheta),glm::cross(glm::vec3(0.0f,-1.0f,0.0f),birdvelocity));
        *bird->drawTransform = rotation; 
        bird->update();

        prevtime = curtime;
    }
}

void Animation::updateAvatar1(SceneNode* avatar1head)
{
    // TODO: LINK THE ACTUAL CAMERA TO THE MODEL ORIENTATION

    const float* pSource = (const float*) glm::value_ptr(*avatar1head->comTransform); 
    headposition.x = pSource[12]; headposition.y = pSource[13]; headposition.z = pSource[14];
    headdirection = ballonposvec - headposition;

    // cout << headdirection.x << " " << headdirection.y << " " << headdirection.z << endl;

    float costheta = glm::dot(glm::normalize(headdirection),glm::vec3(0.0f,1.0f,0.0f));
    // glm::mat4 ogpos = glm::translate(glm::mat4(1.0f),glm::vec3(0.0f,2.0f,0.0f)); // Hardcoding the ogpos matrix
    // *avatar1head->relTransform = ogpos;
    glm::mat4 rotation = glm::rotate(glm::mat4(1.0f),glm::acos(costheta),glm::cross(glm::vec3(0.0f,1.0f,0.0f),headdirection));
    // *avatar1head->relTransform = rotation * ogpos;
    // glm::mat4 rotation = glm::rotate(glm::mat4(1.0f),glm::radians(90.0f),glm::vec3(0.0f,-1.0f,0.0f));
    // *avatar1head->relTransform = ogpos;
    *avatar1head->drawTransform = rotation; // STEP HERE IGNORES ANY SCALING ON THE FACE! 
    // *avatar1head->relTransform = glm::mat4(1.0f);

    avatar1head->update();
    
}

// GENERAL RULE
// IF YOU WISH TO APPLY ROTATION, USE DRAW TRANSFORM, it's better than relTransform, but it cannot be passed down
// It works for spotlight because it's not the children.

// IDEA
//  One possible idea is to keep a separate track for translation, rotation and scaling matrices for each scenenode and compose them in the right order in update/render

void Animation::updateAvatar3Position(SceneNode* avatar1, SceneNode* avatar2, SceneNode* avatar3)
{
    const float* pSource = (const float*) glm::value_ptr(*avatar3->comTransform); 
    headposition3.x = pSource[12]; headposition3.y = pSource[13]; headposition3.z = pSource[14];

    pSource = (const float*) glm::value_ptr(*avatar2->comTransform); 
    headposition2.x = pSource[12]; headposition2.y = pSource[13]; headposition2.z = pSource[14];
    
    pSource = (const float*) glm::value_ptr(*avatar1->comTransform); 
    headposition.x = pSource[12]; headposition.y = pSource[13]; headposition.z = pSource[14];
    
    if(simOn)
    {
        if(towards == true && glm::distance(headposition,headposition3) > socialdistance )
        {
            movevelocity = headposition - headposition3;
            movevelocity = glm::normalize(movevelocity);
            movevelocity = movevelocity * movespeed;
            *avatar3->relTransform = glm::translate(*avatar3->relTransform,movevelocity);
            headposition3 += movevelocity;
        }
        else if(towards == false && glm::distance(headposition2,headposition3) > socialdistance) // For B 
        {
            movevelocity = headposition2 - headposition3;
            movevelocity = glm::normalize(movevelocity);
            movevelocity = movevelocity * movespeed;
            *avatar3->relTransform = glm::translate(*avatar3->relTransform,movevelocity);
            headposition3 += movevelocity;
        }
        avatar3->update();

    }

}

void Animation::updateAvatar3(SceneNode* avatar3head, SceneNode* avatar2head, SceneNode* avatar1head)
{
    // NEED TO REDESIGN THE PARAMETERS FOR THIS FUNCTION
    // TODO: LINK THE ACTUAL CAMERA TO THE MODEL ORIENTATION

    const float* pSource = (const float*) glm::value_ptr(*avatar3head->comTransform); 
    headposition3.x = pSource[12]; headposition3.y = pSource[13]; headposition3.z = pSource[14];

    pSource = (const float*) glm::value_ptr(*avatar2head->comTransform); 
    headposition2.x = pSource[12]; headposition2.y = pSource[13]; headposition2.z = pSource[14];
    
    pSource = (const float*) glm::value_ptr(*avatar1head->comTransform); 
    headposition.x = pSource[12]; headposition.y = pSource[13]; headposition.z = pSource[14];
    
    // if(towards == true && glm::distance(headposition,headposition3)) // For A
    // {
    //     movevelocity = headposition - headposition3;
    //     movevelocity = glm::normalize(movevelocity);
    //     movevelocity = movevelocity * movespeed;
    //     *avatar3head->parent->comTransform = glm::translate(*avatar3head->parent->comTransform,movevelocity);
    //     headposition3 += movevelocity;
    // }
    // else if(towards == false && glm::distance(headposition2,headposition3)) // For B 
    // {
    //     movevelocity = headposition2 - headposition3;
    //     movevelocity = glm::normalize(movevelocity);
    //     movevelocity = movevelocity * movespeed;
    //     *avatar3head->parent->comTransform = glm::translate(*avatar3head->parent->comTransform,movevelocity);
    //     headposition3 += movevelocity;
    // }
    // avatar3head->update();

    // ASSUMING updateAvatar1 has already been called, headdirection already has the right value.
    // ASSUMING the camera outside will set the right headdirection2 value so that this updation need not happen
    // Before camera, it is assumed that the facing direction of +yaxis
    // IF User took control of camera, then camera must update head direction for person 1 as well

    // headdirection2.x = 0.0f;
    // headdirection2.y = 1.0f;
    // headdirection2.z = 0.0f;

    // Setting headdirection3 to be towards P1
    if(towards == false)
        headdirection3 = headposition2 - headposition3;
    else 
        headdirection3 = headposition - headposition3;

    headdirection3 = glm::normalize(headdirection3);
    // cout << headposition2.x << " " << headposition2.y << " " << headposition2.z << " " << endl;
    // cout << headposition3.x << " " << headposition3.y << " " << headposition3.z << " " << endl;
    // cout << headdirection3.x << " " << headdirection3.y << " " << headdirection3.z << " " << endl;

    float costheta = glm::dot(glm::normalize(headdirection3),glm::vec3(0.0f,1.0f,0.0f));
    glm::mat4 rotation = glm::rotate(glm::mat4(1.0f),glm::acos(costheta),glm::cross(glm::vec3(0.0f,1.0f,0.0f),headdirection3));
    *avatar3head->drawTransform = rotation; // Losing the scaling aspect of draw transform
    
    // glm::vec3 sum1 = glm::normalize(headdirection) + glm::normalize(-(headposition3));
    // sum1 = glm::normalize(sum1);
    // float value = glm::dot(sum1,glm::normalize(-(headposition)));
    // value = glm::abs(value);
    // cout << sum1.x << " " << sum1.y << " " << sum1.z << " " << value << endl;
    // if(1 - value < 0.1 && value > 0)
    // {
    //     glm::mat4 rotation = glm::mat4(1.0f);
    //     *avatar3head->drawTransform = rotation; // Losing the scaling aspect of draw transform
    //     return;
    // }
    float value = glm::dot(headdirection3,glm::normalize(-headdirection));
    value = glm::abs(value);
    if(1 - value < 0.03)
    {
        glm::mat4 rotation = glm::mat4(1.0f);
        *avatar3head->drawTransform = rotation; // Losing the scaling aspect of draw transform
        return;
    }

    // glm::vec3 sum2 = glm::normalize(headdirection2) + glm::normalize(-(headposition3));
    // sum2 = glm::normalize(sum2);
    // value = glm::dot(sum2,glm::normalize(-(headposition2)));
    // // cout << sum2.x << " " << sum2.y << " " << sum2.z << " " << value << endl;
    // if(1 - value < 0.05 && value > 0)
    // {
    //     glm::mat4 rotation = glm::mat4(1.0f);
    //     *avatar3head->drawTransform = rotation; // Losing the scaling aspect of draw transform
    //     return;
    // }

    value = glm::dot(headdirection3,glm::normalize(-headdirection2));
    value = glm::abs(value);
    if(1 - value < 0.03)
    {
        glm::mat4 rotation = glm::mat4(1.0f);
        *avatar3head->drawTransform = rotation; // Losing the scaling aspect of draw transform
        return;
    }


    
}
