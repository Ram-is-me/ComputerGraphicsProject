#include "../include/Controller.h"
#define STB_IMAGE_IMPLEMENTATION
#include "../include/stb_image.h"
#include <time.h>
#include <unistd.h>

void Controller::loadTexture(string texpath)
{
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
    // The FileSystem::getPath(...) is part of the GitHub repository so we can find files on any IDE/platform; replace it with your own image path.
    string path = texpath;
    unsigned char *data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        // glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);

        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

}

Controller::Controller(vector<string> models, vector<string> lights, vector<string> shaders)
{
    // Loading Shaders into Memory
    multiLightShader = new Shader(shaders[0],shaders[1]);
    clearShader = new Shader(shaders[2],shaders[3]);
    textureShader = new Shader(shaders[4],shaders[5]);
    lampShader = new Shader(shaders[6],shaders[7]);

    // Loading Texture
    glGenTextures(1, &textures[0]);
    glBindTexture(GL_TEXTURE_2D, textures[0]);
    string path;
    path = "textures/rainbow.png";
    loadTexture(path); 

    glGenTextures(1, &textures[1]);
    glBindTexture(GL_TEXTURE_2D, textures[1]); 
    path = "textures/Ram.jpg";
    loadTexture(path);

    glGenTextures(1, &textures[2]);
    glBindTexture(GL_TEXTURE_2D, textures[2]); 
    path = "textures/green.jpg";
    loadTexture(path);

    glGenTextures(1, &textures[3]);
    glBindTexture(GL_TEXTURE_2D, textures[3]); 
    path = "textures/peach.jpg";
    loadTexture(path);

    glGenTextures(1, &textures[4]);
    glBindTexture(GL_TEXTURE_2D, textures[4]); 
    path = "textures/Red.jpg";
    loadTexture(path);

    glGenTextures(1, &textures[5]);
    glBindTexture(GL_TEXTURE_2D, textures[5]); 
    path = "textures/lightbrown.jpg";
    loadTexture(path);

    // glGenTextures(1, &textures[6]);
    // glBindTexture(GL_TEXTURE_2D, textures[6]); 
    // path = "textures/lightbrown.jpg";
    // loadTexture(path);

        
    
    // Loading Models
    scene_root = new SceneNode(models[0],0,0,NULL);
    scene_root->enable = false;
    
    avatar1 = new SceneNode(models[0],1,3,scene_root);
    *(avatar1->relTransform) = glm::translate(glm::mat4(1.0f),glm::vec3(-25.0f,0.0f,-25.0f));
    *(avatar1->initialTransform) = glm::rotate(glm::mat4(1.0f),glm::radians(90.0f),glm::vec3(1.0f,0.0f,0.0f));


    avatar1head = new SceneNode(models[1],1,3,avatar1);
    *(avatar1head->relTransform) = glm::translate(glm::mat4(1.0f),glm::vec3(0.0f,2.0f,0.0f));
    *(avatar1head->initialTransform) = glm::rotate(glm::mat4(1.0f),glm::radians(90.0f),glm::vec3(0.0f,0.0f,1.0f));
    *(avatar1head->initialTransform) = glm::rotate(*(avatar1head->initialTransform),glm::radians(180.0f),glm::vec3(0.0f,1.0f,0.0f));
    // *(avatar1head->initialTransform) = glm::rotate(glm::mat4(1.0f),glm::radians(30.0f),glm::vec3(1.0f,0.0f,0.0f)) * *(avatar1head->initialTransform);

    avatar2 = new SceneNode(models[2],1,3,scene_root);
    *(avatar2->relTransform) = glm::translate(glm::mat4(1.0f),glm::vec3(25.0f,0.0f,25.0f));
    *(avatar2->initialTransform) = glm::rotate(glm::mat4(1.0f),glm::radians(90.0f),glm::vec3(1.0f,0.0f,0.0f));


    avatar2head = new SceneNode(models[3],1,3,avatar2);
    *(avatar2head->relTransform) = glm::translate(glm::mat4(1.0f),glm::vec3(0.0f,2.0f,0.0f));
    *(avatar2head->initialTransform) = glm::rotate(glm::mat4(1.0f),glm::radians(-90.0f),glm::vec3(0.0f,1.0f,0.0f));
    // *(avatar2head->initialTransform) = glm::rotate(*(avatar2head->initialTransform),glm::radians(180.0f),glm::vec3(0.0f,1.0f,0.0f));
    // *(avatar2head->initialTransform) = glm::rotate(glm::mat4(1.0f),glm::radians(30.0f),glm::vec3(1.0f,0.0f,0.0f)) * *(avatar2head->initialTransform);



    ballon = new SceneNode(models[4],1,3,scene_root);
    *(ballon->relTransform) = glm::translate(glm::mat4(1.0f),glm::vec3(0.0f,5.0f,0.0f));
    // *(ballon->drawTransform) = glm::scale(glm::mat4(1.0f),glm::vec3(1.0f,2.0f,1.0f));
    *(ballon->drawTransform) = glm::scale(glm::mat4(1.0f),glm::vec3(1.0f,1.0f,1.0f));

    basket = new SceneNode(models[5],1,3,ballon);
    *(basket->relTransform) = glm::translate(glm::mat4(1.0f),glm::vec3(0.0f,-5.0f,0.0f)); // Have hardcoded this -5 in Animation class, don't feel the need to change it anywhere.

    rabbit = new SceneNode(models[6],1,3,ballon);
    *(rabbit->relTransform) = glm::translate(glm::mat4(1.0f),glm::vec3(0.0f,-5.0f,0.0f));
    *(rabbit->drawTransform) = glm::scale(glm::mat4(1.0f),glm::vec3(3.0,3.0,3.0));

    bird = new SceneNode(models[7],1,3,scene_root);
    *(bird->relTransform) = glm::translate(glm::mat4(1.0f),glm::vec3(40.0f,10.0f,0.0f));

    field = new SceneNode(models[8],1,3,scene_root);
    *(field->relTransform) = glm::translate(glm::mat4(1.0f),glm::vec3(0.0f,-3.0f,0.0f));
    *(field->relTransform) = glm::scale(*(field->relTransform),glm::vec3(100.0f,0.4f,100.0f));

    avatar3 = new SceneNode(models[9],1,3,scene_root);
    *(avatar3->relTransform) = glm::translate(glm::mat4(1.0f),glm::vec3(-25.0f,0.0f,25.0f));
    *(avatar3->initialTransform) = glm::rotate(glm::mat4(1.0f),glm::radians(90.0f),glm::vec3(1.0f,0.0f,0.0f));


    avatar3head = new SceneNode(models[10],1,3,avatar3);
    *(avatar3head->relTransform) = glm::translate(glm::mat4(1.0f),glm::vec3(0.0f,2.0f,0.0f));
    *(avatar3head->initialTransform) = glm::rotate(glm::mat4(1.0f),glm::radians(90.0f),glm::vec3(0.0f,0.0f,1.0f));
    *(avatar3head->initialTransform) = glm::rotate(*(avatar3head->initialTransform),glm::radians(210.0f),glm::vec3(0.0f,1.0f,0.0f));
    *(avatar3head->initialTransform) = glm::rotate(glm::mat4(1.0f),glm::radians(30.0f),glm::vec3(1.0f,0.0f,0.0f)) * *(avatar3head->initialTransform);


    // Lights loading
    light_root = new LightNode(models[0],0,0,NULL,0,glm::vec3(0.0f,0.0f,0.0f));
    light_root->enable = false; 


    plight1 = new LightNode(lights[0],0,0,light_root,0,glm::vec3(50.0f,0.0f,50.0f));

    plight2 = new LightNode(lights[1],0,0,light_root,1,glm::vec3(50.0f,0.0f,-50.0f));

    plight3 = new LightNode(lights[2],0,0,light_root,2,glm::vec3(-50.0f,0.0f,50.0f));

    plight4 = new LightNode(lights[3],0,0,light_root,3,glm::vec3(-50.0f,0.0f,-50.0f));

    mainlight = new LightNode(lights[5],0,0,light_root,4,glm::vec3(0.0f,40.0f,0.0f));

    spotlight = new SpotLightNode(lights[4],0,0,light_root,5,glm::vec3(50.0f,5.0f,5.0f),glm::vec3(40.0f,10.0f,0.0f));// The point the spotlight must point to is the position of the bird


    // Camera Setup
    // camera = new Camera(glm::vec3(10.0f, 7.0f, 25.0f));
    // lastX = View::SCR_WIDTH / 2.0f;
    // lastY = View::SCR_HEIGHT / 2.0f;
    // firstMouse = true;

    camera_root = new CameraNode(models[0],0,0,NULL,glm::vec3(0.0f,0.0f,0.0f));
    camera_root->enable = false;

    camera_main = new CameraNode(models[0],0,0,camera_root,glm::vec3(18.0f,10.0f,-24.0f),80.0f,0.0f);

    camera_bird = new CameraNode(models[0],0,0,camera_root,glm::vec3(38.0f,10.0f,0.0f),-180.0f,0.0f);

    camera_avatar1left = new CameraNode(models[0],0,0,camera_root,glm::vec3(-24.0f,2.0f,-24.0f),-135.0f+180.0f,0.0f);

    camera_avatar1right = new CameraNode(models[0],0,0,camera_root,glm::vec3(-24.0f,2.0f,-24.0f),-135.0f+180.0f,0.0f);

    camera_avatar2left = new CameraNode(models[0],0,0,camera_root,glm::vec3(24.0f,2.0f,24.0f),45.0f+180.0f,0.0f);

    camera_avatar2right = new CameraNode(models[0],0,0,camera_root,glm::vec3(24.0f,2.0f,24.0f),45.0f+180.0f,0.0f);

    camera_avatar3left = new CameraNode(models[0],0,0,camera_root,glm::vec3(-24.0f,2.0f,24.0f),-45.0f,0.0f);

    camera_avatar3right = new CameraNode(models[0],0,0,camera_root,glm::vec3(-24.0f,2.0f,24.0f),-45.0f,0.0f);

    // Frame and Time Setup
    deltaTime = 0.0f;
    lastFrame = 0.0f;
    currentFrame = 0.0f;
    startTime = time(0);

}

void Controller::render(View* v,View* v2)
{

    // avatar1->tex = &textures[0];
    avatar1->texnumber = 3;
    // avatar1head->tex = &textures[1];
    avatar1head->texnumber = 1;
    // avatar2->tex = &textures[0];
    avatar2->texnumber = 3;
    // avatar2head->tex = &textures[1];
    avatar2head->texnumber = 1;
    // avatar3->tex = &textures[0];
    avatar3->texnumber = 3;
    // avatar3head->tex = &textures[1];
    avatar3head->texnumber = 1;
    // ballon->tex = &textures[0];
    ballon->texnumber = 4;
    // bird->tex = &textures[0];
    bird->texnumber = 0;
    // basket->tex = &textures[0];
    basket->texnumber = 0;
    // rabbit->tex = &textures[0];
    rabbit->texnumber = 3;
    // field->tex = &textures[0];
    field->texnumber = 2;


    glfwMakeContextCurrent(v->window2);
    
    // scene_root->updateSecondContext(scene_root);
    // light_root->updateSecondContext(light_root);
    // IMMA TRI DOING IT MANUALLY NOW
    avatar1->object->SetupModelForSecondScreen();
    avatar1head->object->SetupModelForSecondScreen();
    avatar2->object->SetupModelForSecondScreen();
    avatar2head->object->SetupModelForSecondScreen();
    avatar3->object->SetupModelForSecondScreen();
    avatar3head->object->SetupModelForSecondScreen();
    ballon->object->SetupModelForSecondScreen();
    bird->object->SetupModelForSecondScreen();
    basket->object->SetupModelForSecondScreen();
    rabbit->object->SetupModelForSecondScreen();
    field->object->SetupModelForSecondScreen();

    plight1->object->SetupModelForSecondScreen();
    plight2->object->SetupModelForSecondScreen();
    plight3->object->SetupModelForSecondScreen();
    plight4->object->SetupModelForSecondScreen();
    mainlight->object->SetupModelForSecondScreen();
    spotlight->object->SetupModelForSecondScreen();

    glfwMakeContextCurrent(v->window);

    sleep(1);

    // Start Simulation
    a.startSim();

    // Start of Render Loop
    while(!glfwWindowShouldClose(v->window) && !glfwWindowShouldClose(v->window2))
    {
        glfwMakeContextCurrent(v->window);

        // Current Frame and Time Calculations
        currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        if(deltaTime < 0.03) continue;

        lastFrame = currentFrame;
        currentTime = time(0);
        int elapsed_time = currentTime - startTime;
        // cout << glGetError() << elapsed_time << endl;
        
        // Input Processing
        processInput(v->window,v->window2,1); // TODO: COMBINED FORM OF INPUT PROCESSING

        // Background Color and Clear Screen
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        clearShader->use();
        selectionShaderCameraView(v,clearShader,1);
        v->SetColouringMode(clearShader,0,1);
        
        multiLightShader->use();
        // v->SetView(multiLightShader, camera);
        // v->SetPerspective(multiLightShader, camera, ptype);
        selectionShaderCameraView(v, multiLightShader,1);

        multiLightShader->setBool("enable1",enable1);
        multiLightShader->setBool("enable2",enable2);
        multiLightShader->setBool("enable3",enable3);
        multiLightShader->setBool("enable4",enable4);
        multiLightShader->setBool("enable5",enable5);
        multiLightShader->setBool("enablespot",enablespot);


        v->SetColouringMode(multiLightShader, ccmode, nnmode);
        // multiLightShader->setVec3("viewPos", camera->Position);
        // multiLightShader->setInt("material.diffuse", 0);
        // multiLightShader->setInt("material.specular", 1);
        multiLightShader->setFloat("material.shininess", 32.0f);
        
        // Light Updates
        plight1->update();
        plight2->update();
        plight3->update();
        plight4->update();
        mainlight->update();
        spotlight->update();

        plight1->updateShader(multiLightShader);
        plight2->updateShader(multiLightShader);
        plight3->updateShader(multiLightShader);
        plight4->updateShader(multiLightShader);
        mainlight->updateShader(multiLightShader);
        spotlight->updateShader(multiLightShader); // NEED TO FIGURE OUT THE ORDER LATER


        // Setting Parent Model values 
        scene_root->update();
        avatar1->update();
        avatar1head->update();
        avatar2->update();
        avatar2head->update();
        ballon->update();
        basket->update();
        rabbit->update();
        bird->update();
        field->update();
        avatar3->update();
        avatar3head->update();

        // Dependency Value Updates
        // Let's set the actual direction of the camera into the Animation for avatar 2.
        a.headdirection2 = glm::normalize(camera_avatar2right->c->Front);

        // Animation Updates
        a.updateBallon(ballon);

        a.updateBasket(basket);

        a.updateRabbit(rabbit);

        a.updateBird(bird, ballon, spotlight, camera_bird);

        a.updateAvatar1(avatar1head);

        a.updateAvatar3Position(avatar1,avatar2,avatar3);

        a.updateAvatar3(avatar3head,avatar2head,avatar1head);

        // Updaing Shader
        // glBindTexture(GL_TEXTURE_2D, textures[0]);
        scene_root->render(multiLightShader,v,textures);

        // cout << "WORKING" << endl;

        lampShader->use();
        // v->SetView(lampShader, camera);
        // v->SetPerspective(lampShader, camera, ptype);
        selectionShaderCameraView(v, lampShader,1);
        v->SetColouringMode(lampShader, 1, 0);
        
        light_root->render(lampShader,v,textures);

        // glfwMakeContextCurrent(v->window);
        // Buffer Swap and Polling
        glfwSwapBuffers(v->window);
        glfwPollEvents();

        // For Rendering In Second Window

        glfwMakeContextCurrent(v->window2);

        processInput(v->window,v->window2,2);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        // clearShader->use();
        // selectionShaderCameraView(v,clearShader);
        // v->SetColouringMode(clearShader,0,1);
        
        multiLightShader->use();
        // v->SetView(multiLightShader, camera);
        // v->SetPerspective(multiLightShader, camera, ptype);
        selectionShaderCameraView(v, multiLightShader,2);

        multiLightShader->setBool("enable1",enable1);
        multiLightShader->setBool("enable2",enable2);
        multiLightShader->setBool("enable3",enable3);
        multiLightShader->setBool("enable4",enable4);
        multiLightShader->setBool("enable5",enable5);
        multiLightShader->setBool("enablespot",enablespot);

        v->SetColouringMode(multiLightShader, ccmode, nnmode);
        // multiLightShader->setVec3("viewPos", camera->Position);
        multiLightShader->setInt("material.diffuse", 0);
        multiLightShader->setInt("material.specular", 1);
        multiLightShader->setFloat("material.shininess", 32.0f);

        plight1->updateShader(multiLightShader);
        plight2->updateShader(multiLightShader);
        plight3->updateShader(multiLightShader);
        plight4->updateShader(multiLightShader);
        mainlight->updateShader(multiLightShader);
        spotlight->updateShader(multiLightShader); // NEED TO FIGURE OUT THE ORDER LATER

        // glBindTexture(GL_TEXTURE_2D, textures[0]);
        scene_root->render(multiLightShader,v,textures);

        // cout << "WORKING" << endl;

        lampShader->use();
        // v->SetView(lampShader, camera);
        // v->SetPerspective(lampShader, camera, ptype);
        selectionShaderCameraView(v, lampShader,2);
        v->SetColouringMode(lampShader, 1, 0);
        
        // glBindTexture(GL_TEXTURE_2D, textures[0]);
        light_root->render(lampShader,v,textures);

        glfwSwapBuffers(v->window2);
        glfwPollEvents();

        // glfwMakeContextCurrent(NULL);
    }
    
}

void Controller::selectionShaderCameraView(View* v, Shader* shader, int leftOrRight)
{
    if(leftOrRight == 1)
    {
        if(selection == 1)
        {
            // v->SetView(clearShader,camera);
            // v->SetPerspective(clearShader,camera,ptype);
            v->SetView(shader, camera_main->c);
            v->SetPerspective(shader, camera_main->c,ptype);
            shader->setVec3("viewPos", camera_main->c->Position);
        }
        else if(selection == 2)
        {
            v->SetView(shader, camera_bird->c);
            v->SetPerspective(shader, camera_bird->c,ptype);
            shader->setVec3("viewPos", camera_bird->c->Position);
        }
        else if(selection == 3)
        {
            v->SetView(shader, camera_avatar1left->c);
            v->SetPerspective(shader, camera_avatar1left->c,ptype);
            shader->setVec3("viewPos", camera_avatar1left->c->Position);
        }
        else if(selection == 4)
        {
            v->SetView(shader, camera_avatar2left->c);
            v->SetPerspective(shader, camera_avatar2left->c,ptype);
            shader->setVec3("viewPos", camera_avatar2left->c->Position);
        }
        else if(selection == 5)
        {
            v->SetView(shader, camera_avatar3left->c);
            v->SetPerspective(shader, camera_avatar3left->c,ptype);
            shader->setVec3("viewPos", camera_avatar3left->c->Position);
        }
        else if(selection == 6)
        {
            v->SetView(shader, camera_avatar1left->c);
            v->SetPerspective(shader, camera_avatar1left->c,ptype);
            shader->setVec3("viewPos", camera_avatar1left->c->Position);
        }
    }
    else 
    {
        if(selection2 == 1)
        {
            // v->SetView(clearShader,camera);
            // v->SetPerspective(clearShader,camera,ptype);
            v->SetView(shader, camera_main->c);
            v->SetPerspective(shader, camera_main->c,ptype);
            shader->setVec3("viewPos", camera_main->c->Position);
        }
        else if(selection2 == 2)
        {
            v->SetView(shader, camera_bird->c);
            v->SetPerspective(shader, camera_bird->c,ptype);
            shader->setVec3("viewPos", camera_bird->c->Position);
        }
        else if(selection2 == 3)
        {
            v->SetView(shader, camera_avatar1right->c);
            v->SetPerspective(shader, camera_avatar1right->c,ptype);
            shader->setVec3("viewPos", camera_avatar1right->c->Position);
        }
        else if(selection2 == 4)
        {
            v->SetView(shader, camera_avatar2right->c);
            v->SetPerspective(shader, camera_avatar2right->c,ptype);
            shader->setVec3("viewPos", camera_avatar2right->c->Position);
        }
        else if(selection2 == 5)
        {
            v->SetView(shader, camera_avatar3right->c);
            v->SetPerspective(shader, camera_avatar3right->c,ptype);
            shader->setVec3("viewPos", camera_avatar3right->c->Position);
        }
        else if(selection2 == 6)
        {
            v->SetView(shader, camera_avatar2right->c);
            v->SetPerspective(shader, camera_avatar2right->c,ptype);
            shader->setVec3("viewPos", camera_avatar2right->c->Position);
        }
    }
}

void Controller::processInput(GLFWwindow *window, GLFWwindow *window2, int inputchoice)
{
    int width, height;
    glfwGetWindowSize(window,&width,&height);
    glViewport(0,0, width, height);

    // Exit Keys
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if(glfwGetKey(window2, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window2, true);
    
    // Num Keys
    if(glfwGetKey(window2, GLFW_KEY_RIGHT_SHIFT) != GLFW_PRESS)
    {
        if(glfwGetKey(window2, GLFW_KEY_1) == GLFW_PRESS)
        {
            // if(inputchoice == 1)
            // {
            //     selection = 1;
            //     glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            // }
            // else 
            // {
            //     selection2 = 1;
            //     glfwSetInputMode(window2, GLFW_CURSOR, GLFW_CURSOR_DISABLED);    
            // }
            selection = 1;
            selection2 = 2;
        }
        if(glfwGetKey(window2, GLFW_KEY_2) == GLFW_PRESS)
        {   
            // if(inputchoice == 1)
            // {
            //     selection = 2;
            //     glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            // }
            // else 
            // {
            //     selection2 = 2;
            //     glfwSetInputMode(window2, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            // }
            selection = 6;
            selection2 = 6;
        }
        if(glfwGetKey(window2, GLFW_KEY_3) == GLFW_PRESS)
        {
            if(inputchoice == 1)
            {
                selection = 3;
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            }
            else 
            {
                selection2 = 3;
                glfwSetInputMode(window2, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            }
        }
        if(glfwGetKey(window2, GLFW_KEY_4) == GLFW_PRESS)
        {
            if(inputchoice == 1)
            {
                selection = 4;
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            }
            else 
            {
                selection2 = 4;
                glfwSetInputMode(window2, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            }
        }
        if(glfwGetKey(window2, GLFW_KEY_5) == GLFW_PRESS)
        {
            if(inputchoice == 1)
            {
                selection = 5;
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            }
            else 
            {
                selection2 = 5;
                glfwSetInputMode(window2, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            }
        }
        if(glfwGetKey(window2, GLFW_KEY_6) == GLFW_PRESS)
        {
            if(inputchoice == 1)
            {
                selection = 6;
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            }
            else 
            {
                selection2 = 5;
                glfwSetInputMode(window2, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            }
        }
        if(glfwGetKey(window2, GLFW_KEY_7) == GLFW_PRESS)
        {
            if(inputchoice == 1)
            {
                selection = 7;
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            }
            else 
            {
                selection2 = 7;
                glfwSetInputMode(window2, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            }
        }
        if(glfwGetKey(window2, GLFW_KEY_8) == GLFW_PRESS)
        {
            if(inputchoice == 1)
            {
                selection = 8;
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            }
            else 
            {
                selection2 = 8;
                glfwSetInputMode(window2, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            }
        }
        if(glfwGetKey(window2, GLFW_KEY_9) == GLFW_PRESS)
        {
            if(inputchoice == 1)
            {
                selection = 9;
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            }
            else 
            {
                selection2 = 9;
                glfwSetInputMode(window2, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            }
        }
    }

    if(glfwGetKey(window2, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS)
    {
        if(glfwGetKey(window2, GLFW_KEY_1) == GLFW_PRESS)
        {
            if(enable1) 
            {
                enable1 = false;
                plight1->enable = false;
            }
            else 
            {
                enable1 = true;
                plight1->enable = true;
            }
            
        }
        if(glfwGetKey(window2, GLFW_KEY_2) == GLFW_PRESS)
        {
            if(enable2) 
            {
                enable2 = false;
                plight2->enable = false;
            }
            else 
            {
                enable2 = true;
                plight2->enable = true;
            }
            
        }
        if(glfwGetKey(window2, GLFW_KEY_3) == GLFW_PRESS)
        {
            if(enable3) 
            {
                enable3 = false;
                plight3->enable = false;
            }
            else 
            {
                enable3 = true;
                plight3->enable = true;
            }
            
        }
        if(glfwGetKey(window2, GLFW_KEY_4) == GLFW_PRESS)
        {
            if(enable4) 
            {
                enable4 = false;
                plight4->enable = false;
            }
            else 
            {
                enable4 = true;
                plight4->enable = true;
            }
            
        }
        if(glfwGetKey(window2, GLFW_KEY_5) == GLFW_PRESS)
        {
            if(enable5) 
            {
                enable5 = false;
                mainlight->enable = false;
            }
            else 
            {
                enable5 = true;
                mainlight->enable = true;
            }
            
        }
        if(glfwGetKey(window2, GLFW_KEY_6) == GLFW_PRESS)
        {
            if(enablespot) 
            {
                enablespot = false;
                spotlight->enable = false;
            }
            else 
            {
                enablespot = true;
                spotlight->enable = true;
            }
            
        }


    }
    
    // CONTROL KEYS
    if(glfwGetKey(window2, GLFW_KEY_M) == GLFW_PRESS)
    {
        // selection = 1;
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        glfwSetInputMode(window2, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        // firstMouse = true;
        camera_main->firstMouse = true;
        camera_avatar1left->firstMouse = true;
        camera_avatar1right->firstMouse = true;
        camera_avatar2left->firstMouse = true;
        camera_avatar2right->firstMouse = true;
        camera_avatar3left->firstMouse = true;
        camera_avatar3right->firstMouse = true;
        camera_bird->firstMouse = true;
        camera_root->firstMouse = true;

    }
    // if(glfwGetKey(window2, GLFW_KEY_C) == GLFW_PRESS)
    // {
    //     selection = 1;
    //     glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    //     camera_root->firstMouse = true;
    // }
    // if(glfwGetKey(window2, GLFW_KEY_SPACE) == GLFW_PRESS)
    // {
    //     selection = -2;
    //     glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    // }

    // if(glfwGetKey(window2, GLFW_KEY_V) == GLFW_PRESS)
    // {
    //     ccmode = 1;
    //     nnmode = 0;
    // }
    // if(glfwGetKey(window2, GLFW_KEY_N) == GLFW_PRESS)
    // {
    //     ccmode = 0;
    //     nnmode = 1;
    // }

    if(glfwGetKey(window2, GLFW_KEY_UP) == GLFW_PRESS) // Default is 0.1
    { 
        a.ballonvspeed += 0.02;
    }
    if(glfwGetKey(window2, GLFW_KEY_DOWN) == GLFW_PRESS)
    {
        a.ballonvspeed -= 0.02;
    }
    if(glfwGetKey(window2, GLFW_KEY_LEFT) == GLFW_PRESS)
    {
        a.ballonhspeed -= 0.02;
    }
    if(glfwGetKey(window2, GLFW_KEY_RIGHT) == GLFW_PRESS)
    {
        a.ballonhspeed += 0.02;
    }

    if(glfwGetKey(window2, GLFW_KEY_P) == GLFW_PRESS)
    {
        walkingA = true;
        walkingB = false;
        walkingC = false;
        walkingmaincamera = false;
    }
    if(glfwGetKey(window2, GLFW_KEY_O) == GLFW_PRESS)
    {
        walkingA = false;
        walkingB = true;
        walkingC = false;
        walkingmaincamera = false;
    }
    if(glfwGetKey(window2, GLFW_KEY_I) == GLFW_PRESS)
    {
        walkingA = false;
        walkingB = false;
        walkingC = true;
        walkingmaincamera = false;
    }
    if(glfwGetKey(window2, GLFW_KEY_U) == GLFW_PRESS)
    {
        walkingA = false;
        walkingB = false;
        walkingC = false;
        walkingmaincamera = true;
    }

    // Movement Keys
    if (glfwGetKey(window2, GLFW_KEY_W) == GLFW_PRESS)
    {
        if(selection == -1)
            camera_root->c->ProcessKeyboard(FORWARD, deltaTime);
        if(walkingmaincamera)
            camera_main->c->ProcessKeyboard(FORWARD, deltaTime);
        if(selection == 2) // REDUNDANT CODE, I THINK
            camera_bird->c->ProcessKeyboard(FORWARD, deltaTime);
        if(walkingA)
        {
            camera_avatar1right->c->ProcessKeyboard(FORWARD, deltaTime);
            camera_avatar1left->c->ProcessKeyboard(FORWARD, deltaTime);
            float velocity = (camera_avatar1right->c->MovementSpeed * deltaTime);
            glm::vec3 change = camera_avatar1right->c->Front;
            change = change * velocity;
            *avatar1->relTransform = glm::translate(*avatar1->relTransform,change);
        }
        if(walkingB)
        {
            camera_avatar2right->c->ProcessKeyboard(FORWARD, deltaTime);
            camera_avatar2left->c->ProcessKeyboard(FORWARD, deltaTime);
            float velocity = (camera_avatar2right->c->MovementSpeed * deltaTime);
            glm::vec3 change = camera_avatar2right->c->Front;
            change = change * velocity;
            *avatar2->relTransform = glm::translate(*avatar2->relTransform,change);
        }
        if(walkingC)
        {
            camera_avatar3right->c->ProcessKeyboard(FORWARD, deltaTime);
            camera_avatar3left->c->ProcessKeyboard(FORWARD, deltaTime);
            float velocity = (camera_avatar3right->c->MovementSpeed * deltaTime);
            glm::vec3 change = camera_avatar3right->c->Front;
            change = change * velocity;
            *avatar3->relTransform = glm::translate(*avatar3->relTransform,change);
        }
    }
    if (glfwGetKey(window2, GLFW_KEY_S) == GLFW_PRESS)
    {
        if(selection == -1)
            camera_root->c->ProcessKeyboard(BACKWARD, deltaTime);
        if(walkingmaincamera)
            camera_main->c->ProcessKeyboard(BACKWARD, deltaTime);
        if(selection == 2)
            camera_bird->c->ProcessKeyboard(BACKWARD, deltaTime);
        if(walkingA)
        {
            camera_avatar1right->c->ProcessKeyboard(BACKWARD, deltaTime);
            camera_avatar1left->c->ProcessKeyboard(BACKWARD, deltaTime);
            float velocity = (camera_avatar1right->c->MovementSpeed * deltaTime);
            glm::vec3 change = camera_avatar1right->c->Front;
            change = change * velocity; 
            change = -change;
            *avatar1->relTransform = glm::translate(*avatar1->relTransform,change);

        }
        if(walkingB)
        {
            camera_avatar2right->c->ProcessKeyboard(BACKWARD, deltaTime);
            camera_avatar2left->c->ProcessKeyboard(BACKWARD, deltaTime);
            float velocity = (camera_avatar2right->c->MovementSpeed * deltaTime);
            glm::vec3 change = camera_avatar2right->c->Front;
            change = change * velocity;
            change = -change;
            *avatar2->relTransform = glm::translate(*avatar2->relTransform,change);
        }
        if(walkingC)
        {
            camera_avatar3right->c->ProcessKeyboard(BACKWARD, deltaTime);
            camera_avatar3left->c->ProcessKeyboard(BACKWARD, deltaTime);
            float velocity = (camera_avatar3right->c->MovementSpeed * deltaTime);
            glm::vec3 change = camera_avatar3right->c->Front;
            change = change * velocity;
            change = -change;
            *avatar3->relTransform = glm::translate(*avatar3->relTransform,change);    
        }
    }
    if (glfwGetKey(window2, GLFW_KEY_A) == GLFW_PRESS)
    {
        if(selection == -1)
            camera_root->c->ProcessKeyboard(LEFT, deltaTime);
        if(walkingmaincamera)
            camera_main->c->ProcessKeyboard(LEFT, deltaTime);
        if(selection == 2)
            camera_bird->c->ProcessKeyboard(LEFT, deltaTime);
        if(walkingA)
        {
            camera_avatar1right->c->ProcessKeyboard(LEFT, deltaTime);
            camera_avatar1left->c->ProcessKeyboard(LEFT, deltaTime);
            float velocity = (camera_avatar1right->c->MovementSpeed * deltaTime);
            glm::vec3 change = camera_avatar1right->c->Right;
            change = change * velocity; 
            change = -change;
            *avatar1->relTransform = glm::translate(*avatar1->relTransform,change);
        }
        if(walkingB)
        {
            camera_avatar2right->c->ProcessKeyboard(LEFT, deltaTime);
            camera_avatar2left->c->ProcessKeyboard(LEFT, deltaTime);
            float velocity = (camera_avatar2right->c->MovementSpeed * deltaTime);
            glm::vec3 change = camera_avatar2right->c->Right;
            change = change * velocity;
            change = -change;
            *avatar2->relTransform = glm::translate(*avatar2->relTransform,change);
        }
        if(walkingC)
        {
            camera_avatar3right->c->ProcessKeyboard(LEFT, deltaTime);
            camera_avatar3left->c->ProcessKeyboard(LEFT, deltaTime);
            float velocity = (camera_avatar3right->c->MovementSpeed * deltaTime);
            glm::vec3 change = camera_avatar3right->c->Right;
            change = change * velocity;
            change = -change;
            *avatar3->relTransform = glm::translate(*avatar3->relTransform,change);
        }
    }
    if (glfwGetKey(window2, GLFW_KEY_D) == GLFW_PRESS)
    {
        if(selection == -1)
            camera_root->c->ProcessKeyboard(RIGHT, deltaTime);
        if(walkingmaincamera)
            camera_main->c->ProcessKeyboard(RIGHT, deltaTime);
        if(selection == 2)
            camera_bird->c->ProcessKeyboard(RIGHT, deltaTime);
        if(walkingA)
        {
            camera_avatar1right->c->ProcessKeyboard(RIGHT, deltaTime);
            camera_avatar1left->c->ProcessKeyboard(RIGHT, deltaTime);
            float velocity = (camera_avatar1right->c->MovementSpeed * deltaTime);
            glm::vec3 change = camera_avatar1right->c->Right;
            change = change * velocity; 
            *avatar1->relTransform = glm::translate(*avatar1->relTransform,change);
        }
        if(walkingB)
        {
            camera_avatar2right->c->ProcessKeyboard(RIGHT, deltaTime);
            camera_avatar2left->c->ProcessKeyboard(RIGHT, deltaTime);
            float velocity = (camera_avatar2right->c->MovementSpeed * deltaTime);
            glm::vec3 change = camera_avatar2right->c->Right;
            change = change * velocity;
            *avatar2->relTransform = glm::translate(*avatar2->relTransform,change);
        }
        if(walkingC)
        {
            camera_avatar3right->c->ProcessKeyboard(RIGHT, deltaTime);
            camera_avatar3left->c->ProcessKeyboard(RIGHT, deltaTime);
            float velocity = (camera_avatar3right->c->MovementSpeed * deltaTime);
            glm::vec3 change = camera_avatar3right->c->Right;
            change = change * velocity;
            *avatar3->relTransform = glm::translate(*avatar3->relTransform,change);
        }
    }
    if (glfwGetKey(window2, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
    {
        if(selection == -1)
            camera_root->c->ProcessKeyboard(UP, deltaTime);
        if(walkingmaincamera)
            camera_main->c->ProcessKeyboard(UP, deltaTime);
        if(selection == 2)
            camera_bird->c->ProcessKeyboard(UP, deltaTime);
        if(walkingA)
        {
            camera_avatar1right->c->ProcessKeyboard(UP, deltaTime);
            camera_avatar1left->c->ProcessKeyboard(UP, deltaTime);
            float velocity = (camera_avatar1right->c->MovementSpeed * deltaTime);
            glm::vec3 change = camera_avatar1right->c->Up;
            change = change * velocity; 
            *avatar1->relTransform = glm::translate(*avatar1->relTransform,change);
        } 
        if(walkingB)
        {
            camera_avatar2right->c->ProcessKeyboard(UP, deltaTime);
            camera_avatar2left->c->ProcessKeyboard(UP, deltaTime);
            float velocity = (camera_avatar2right->c->MovementSpeed * deltaTime);
            glm::vec3 change = camera_avatar2right->c->Up;
            change = change * velocity;
            *avatar2->relTransform = glm::translate(*avatar2->relTransform,change);
        }
        if(walkingC)
        {
            camera_avatar3right->c->ProcessKeyboard(UP, deltaTime);
            camera_avatar3left->c->ProcessKeyboard(UP, deltaTime);
            float velocity = (camera_avatar3right->c->MovementSpeed * deltaTime);
            glm::vec3 change = camera_avatar3right->c->Up;
            change = change * velocity;
            *avatar3->relTransform = glm::translate(*avatar3->relTransform,change);
        }
    }
    if (glfwGetKey(window2, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
    {
        if(selection == -1)
            camera_root->c->ProcessKeyboard(DOWN, deltaTime);
        if(walkingmaincamera)
            camera_main->c->ProcessKeyboard(DOWN, deltaTime);
        if(selection == 2)
            camera_bird->c->ProcessKeyboard(DOWN, deltaTime);
        if(walkingA)
        {
            camera_avatar1right->c->ProcessKeyboard(DOWN, deltaTime);
            camera_avatar1left->c->ProcessKeyboard(DOWN, deltaTime);
            float velocity = (camera_avatar1right->c->MovementSpeed * deltaTime);
            glm::vec3 change = camera_avatar1right->c->Up;
            change = change * velocity; 
            change = -change;
            *avatar1->relTransform = glm::translate(*avatar1->relTransform,change);
        }
        if(walkingB)
        {
            camera_avatar2right->c->ProcessKeyboard(DOWN, deltaTime);
            camera_avatar2left->c->ProcessKeyboard(DOWN, deltaTime);
            float velocity = (camera_avatar2right->c->MovementSpeed * deltaTime);
            glm::vec3 change = camera_avatar2right->c->Up;
            change = change * velocity;
            change = -change;
            *avatar2->relTransform = glm::translate(*avatar2->relTransform,change);
        }
        if(walkingC)
        {
            camera_avatar3right->c->ProcessKeyboard(DOWN, deltaTime);
            camera_avatar3left->c->ProcessKeyboard(DOWN, deltaTime);
            float velocity = (camera_avatar3right->c->MovementSpeed * deltaTime);
            glm::vec3 change = camera_avatar3right->c->Up;
            change = change * velocity;
            change = -change;
            *avatar3->relTransform = glm::translate(*avatar3->relTransform,change);
        }
    }
    if (glfwGetKey(window2, GLFW_KEY_K) == GLFW_PRESS)
    {
        if(a.towards == true)
            a.towards = false;
        else 
            a.towards = true;
    }
    // if (glfwGetKey(window2, GLFW_KEY_O) == GLFW_PRESS)
    // {
    //     if(selection == -1)
    //         ptype = 0;    
    // }
        
    //Choosing Drawing Mode
    if(glfwGetKey(window2, GLFW_KEY_R) == GLFW_PRESS)
    {
        resetPosition();
        a.restartSim();
    }
    //Changing Splat Radius
    // if(glfwGetKey(window2, GLFW_KEY_E) == GLFW_PRESS)
    // {
    //     a.continueSim();
    // }
    // if(glfwGetKey(window2, GLFW_KEY_T) == GLFW_PRESS)
    //     models[selection].setSplatMode(1);
    // if(glfwGetKey(window2, GLFW_KEY_Y) == GLFW_PRESS)
    //     models[selection].setSplatMode(0);
    // if(glfwGetKey(window2, GLFW_KEY_R) == GLFW_PRESS)
    // {
    //     models[selection].radiusAdd -= 0.01;
    //     models[selection].SetupSplat();
    // }
    
    // if(glfwGetKey(window2, GLFW_KEY_I) == GLFW_PRESS)
    // {
    //     // models[selection].radiusAdd -= 0.01;
    //     // models[selection].SetupSplat();
    //     if(models[selection].geoiteration < models[selection].maxgeoiteration - 1)
    //     {
    //         models[selection].geoiteration++;
    //         models[selection].animateGeodesic();
    //         // getchar();
    //         // usleep(1000);
    //     }
    // }
    // if(glfwGetKey(window2, GLFW_KEY_U) == GLFW_PRESS)
    // {
    //     // models[selection].radiusAdd -= 0.01;
    //     // models[selection].SetupSplat();
    //     if(models[selection].geoiteration > 0)
    //         models[selection].geoiteration--;
    // }
    
    // Mouse Control
    // if(glfwGetCurrentContext() == window)
    mouseCamera(window2);
    if(View::mousestate != View::prevmousestate)
    {
        // mouseSelection(window);
        // mouseRotation(window);
    }
}

void Controller::mouseCamera(GLFWwindow *window)
{
    // if(selection == -1)
    // {
    //     double xpos;
    //     double ypos; 
    //     glfwGetCursorPos(window, &xpos, &ypos);

    //     if (firstMouse)
    //     {
    //         lastX = xpos;
    //         lastY = ypos;
    //         firstMouse = false;
    //     }

    //     float xoffset = xpos - lastX;
    //     float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    //     lastX = xpos;
    //     lastY = ypos;
        
    //     camera->ProcessMouseMovement(xoffset, yoffset);
    // }

    if (glfwGetCurrentContext() == window) // I want to capture mouse movement only
    {
        double xpos;
        double ypos; 
        glfwGetCursorPos(window, &xpos, &ypos);
        
        if(selection == 1)
        {
            
            if (camera_main->firstMouse)
            {
                camera_main->lastX = xpos;
                camera_main->lastY = ypos;
                camera_main->firstMouse = false;
            }
            float xoffset = xpos - camera_main->lastX;
            float yoffset = camera_main->lastY - ypos; // reversed since y-coordinates go from bottom to top
            camera_main->lastX = xpos;
            camera_main->lastY = ypos;
            camera_main->c->ProcessMouseMovement(xoffset, yoffset);
        }
        if(selection == 2)
        {
            
            if (camera_bird->firstMouse)
            {
                camera_bird->lastX = xpos;
                camera_bird->lastY = ypos;
                camera_bird->firstMouse = false;
            }
            float xoffset = xpos - camera_bird->lastX;
            float yoffset = camera_bird->lastY - ypos; // reversed since y-coordinates go from bottom to top
            camera_bird->lastX = xpos;
            camera_bird->lastY = ypos;
            camera_bird->c->ProcessMouseMovement(xoffset, yoffset);
        }
        if(selection == 3)
        {
            
            if (camera_avatar1right->firstMouse)
            {
                camera_avatar1right->lastX = xpos;
                camera_avatar1right->lastY = ypos;
                camera_avatar1right->firstMouse = false;
                camera_avatar1left->lastX = xpos;
                camera_avatar1left->lastY = ypos;
                camera_avatar1left->firstMouse = false;
            }
            float xoffset = xpos - camera_avatar1right->lastX;
            float yoffset = camera_avatar1right->lastY - ypos; // reversed since y-coordinates go from bottom to top
            camera_avatar1right->lastX = xpos;
            camera_avatar1right->lastY = ypos;
            camera_avatar1right->c->ProcessMouseMovement(xoffset, yoffset);
            
            xoffset = xpos - camera_avatar1left->lastX;
            yoffset = camera_avatar1left->lastY - ypos; // reversed since y-coordinates go from bottom to top
            camera_avatar1left->lastX = xpos;
            camera_avatar1left->lastY = ypos;
            camera_avatar1left->c->ProcessMouseMovement(xoffset, yoffset);
        }
        if(selection == 4)
        {
            
            if (camera_avatar2right->firstMouse)
            {
                camera_avatar2right->lastX = xpos;
                camera_avatar2right->lastY = ypos;
                camera_avatar2right->firstMouse = false;
                camera_avatar2left->lastX = xpos;
                camera_avatar2left->lastY = ypos;
                camera_avatar2left->firstMouse = false;
            }
            float xoffset = xpos - camera_avatar2right->lastX;
            float yoffset = camera_avatar2right->lastY - ypos; // reversed since y-coordinates go from bottom to top
            camera_avatar2right->lastX = xpos;
            camera_avatar2right->lastY = ypos;
            camera_avatar2right->c->ProcessMouseMovement(xoffset, yoffset);

            xoffset = xpos - camera_avatar2left->lastX;
            yoffset = camera_avatar2left->lastY - ypos; // reversed since y-coordinates go from bottom to top
            camera_avatar2left->lastX = xpos;
            camera_avatar2left->lastY = ypos;
            camera_avatar2left->c->ProcessMouseMovement(xoffset, yoffset);

        }
        if(selection == 5)
        {
            
            if (camera_avatar3right->firstMouse)
            {
                camera_avatar3right->lastX = xpos;
                camera_avatar3right->lastY = ypos;
                camera_avatar3right->firstMouse = false;
                camera_avatar3left->lastX = xpos;
                camera_avatar3left->lastY = ypos;
                camera_avatar3left->firstMouse = false;
            }
            float xoffset = xpos - camera_avatar3right->lastX;
            float yoffset = camera_avatar3right->lastY - ypos; // reversed since y-coordinates go from bottom to top
            camera_avatar3right->lastX = xpos;
            camera_avatar3right->lastY = ypos;
            camera_avatar3right->c->ProcessMouseMovement(xoffset, yoffset);

            xoffset = xpos - camera_avatar3left->lastX;
            yoffset = camera_avatar3left->lastY - ypos; // reversed since y-coordinates go from bottom to top
            camera_avatar3left->lastX = xpos;
            camera_avatar3left->lastY = ypos;
            camera_avatar3left->c->ProcessMouseMovement(xoffset, yoffset);

        }
    }
    else 
    {

    }
}   

void Controller::mouseRotation(GLFWwindow *window)
{
    if(selection > -1)
    {
        int state = View::mousestate;
        if(state == GLFW_PRESS)
        {
            glfwGetCursorPos(window, &px, &py);
            trackball.rotate(0,0,0,0);
        }
        if(state == GLFW_RELEASE)
        {
            glfwGetCursorPos(window, &rx, &ry);

            float xoffset = rx - px;
            float yoffset = ry - py;

            trackball.rotate((2.0 * px - View::SCR_WIDTH) / float(View::SCR_WIDTH),
            (View::SCR_HEIGHT - 2.0 * py) / float(View::SCR_HEIGHT),
            (2.0 * rx - View::SCR_WIDTH) / (float(View::SCR_WIDTH)),
            (View::SCR_HEIGHT - 2.0 * ry) / float(View::SCR_HEIGHT));
            // x_ang = xx ; y_ang = yy ;

            glm::mat4 m = models[selection].GetModelMatrix();
            trackball.rotationMatrix(rotation_transform);

            //Converting to floats
            float temp[16];
            int tempno = 0;
            for(int i=0;i<4;i++)
                for(int j=0;j<4;j++)
                    temp[tempno++] = rotation_transform[j][i];

            glm::mat4 transform = glm::make_mat4x4(temp); //Checked for column major ordering above
            m = m * transform;
            models[selection].setModelMatrix(m);
        }
    }
    View::prevmousestate = View::mousestate;
}

void Controller::mouseSelection(GLFWwindow *window)
{
    if(selection == -2)
    {
        double xpos,ypos;
        glfwGetCursorPos(window, &xpos, &ypos);

        glm::mat4 unscreen = glm::mat4(1.0f);
        glm::mat4 view = glm::mat4(1.0f);
        glm::mat4 projection = glm::mat4(1.0f);

        view = camera_root->c->GetViewMatrix();
        projection = glm::ortho(-10.0f,10.0f,-10.0f,10.0f,0.0f,100.0f);
        // projection = glm::perspective(glm::radians(camera->Zoom), (float)View::SCR_WIDTH/ (float) View::SCR_HEIGHT, 0.1f, 100.0f);
        unscreen = projection * view;
        unscreen = glm::inverse(unscreen);

        glm::vec4 worldpos = glm::vec4(1.0f);
        worldpos.x = (2.0f*((float)(xpos-0)/(View::SCR_WIDTH-0)))-1.0f;
        worldpos.y = 1.0f-(2.0f*((float)(ypos-0)/(View::SCR_HEIGHT-0)));
        float z;
        glReadPixels(xpos,ypos,1,1,GL_DEPTH_COMPONENT,GL_FLOAT,&z);
        worldpos.z = z;
        worldpos.w = 1.0f;

        worldpos = unscreen * worldpos;

        // cout << glGetError() << worldpos.x << " " << worldpos.y << endl; 
        vector<int> depthSelection;                   

        //Check between coordinates for selection
        for(int i=0;i<models.size();i++)
        {

            if(worldpos.x >= models[i].minvert.x && worldpos.x <= models[i].maxvert.x)
            {
                if(worldpos.y >= models[i].minvert.y && worldpos.y <= models[i].maxvert.y)
                {
                    // cout << glGetError() << worldpos.x << " " << worldpos.y << endl;                    
                    
                    // selection = i;
                    depthSelection.push_back(i);

                    // break;
                }
            }
        }

        float minDepth = 1000000.0;
        int minDepthIndex = -2;
        for(int i=0;i<depthSelection.size();i++)
        {
            if(minDepth > models[depthSelection[i]].minZ)
            {
                minDepth = models[depthSelection[i]].minZ;
                minDepthIndex = depthSelection[i];
            }
        }

        selection = minDepthIndex;
        // cout << glGetError() << selection << endl;

        if(selection == -2)
        {
            selection = -1;
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            camera_root->firstMouse = true;
        }
    }
}

void Controller::resetPosition()
{
    *(avatar1->relTransform) = glm::translate(glm::mat4(1.0f),glm::vec3(-25.0f,0.0f,-25.0f));

    *(avatar1head->relTransform) = glm::translate(glm::mat4(1.0f),glm::vec3(0.0f,2.0f,0.0f));

    *(avatar2->relTransform) = glm::translate(glm::mat4(1.0f),glm::vec3(25.0f,0.0f,25.0f));

    *(avatar2head->relTransform) = glm::translate(glm::mat4(1.0f),glm::vec3(0.0f,2.0f,0.0f));
    
    *(ballon->relTransform) = glm::translate(glm::mat4(1.0f),glm::vec3(0.0f,5.0f,0.0f));
    // *(ballon->drawTransform) = glm::scale(glm::mat4(1.0f),glm::vec3(1.0f,2.0f,1.0f));
    *(ballon->drawTransform) = glm::scale(glm::mat4(1.0f),glm::vec3(1.0f,1.0f,1.0f));
    ballon->enable = true;

    *(basket->relTransform) = glm::translate(glm::mat4(1.0f),glm::vec3(0.0f,-5.0f,0.0f)); // Have hardcoded this -5 in Animation class, don't feel the need to change it anywhere.

    *(rabbit->relTransform) = glm::translate(glm::mat4(1.0f),glm::vec3(0.0f,-5.0f,0.0f));
    *(rabbit->drawTransform) = glm::scale(glm::mat4(1.0f),glm::vec3(3.0,3.0,3.0));

    *(bird->relTransform) = glm::translate(glm::mat4(1.0f),glm::vec3(40.0f,10.0f,0.0f));

    *(field->relTransform) = glm::translate(glm::mat4(1.0f),glm::vec3(0.0f,-3.0f,0.0f));
    *(field->relTransform) = glm::scale(*(field->relTransform),glm::vec3(100.0f,0.4f,100.0f));

    *(avatar3->relTransform) = glm::translate(glm::mat4(1.0f),glm::vec3(-25.0f,0.0f,25.0f));

    *(avatar3head->relTransform) = glm::translate(glm::mat4(1.0f),glm::vec3(0.0f,2.0f,0.0f));

    // camera_main->c->Position = glm::vec3(0.0f,30.0f,0.0f);
    // camera_main->c->Yaw = 0.0f;
    // camera_main->c->Pitch = -60.0f;
    // camera_main->c->updateCameraVectors();

    // camera_bird = new CameraNode(models[0],0,0,camera_root,glm::vec3(38.0f,10.0f,0.0f),-180.0f,0.0f);

    camera_avatar1left->c->Position = glm::vec3(-24.0f,2.0f,-24.0f);
    camera_avatar1left->c->Yaw =-135.0f+180.0f;
    camera_avatar1left->c->Pitch =0.0f;
    camera_avatar1left->c->updateCameraVectors();

    camera_avatar1right->c->Position = glm::vec3(-24.0f,2.0f,-24.0f);;
    camera_avatar1right->c->Yaw = -135.0f+180.0f;
    camera_avatar1right->c->Pitch = 0.0f;
    camera_avatar1right->c->updateCameraVectors();

    camera_avatar2left->c->Position = glm::vec3(24.0f,2.0f,24.0f);
    camera_avatar2left->c->Yaw = 45.0f+180.0f;
    camera_avatar2left->c->Pitch =0.0f;
    camera_avatar2left->c->updateCameraVectors();

    camera_avatar2right->c->Position = glm::vec3(24.0f,2.0f,24.0f);
    camera_avatar2right->c->Yaw = 45.0f+180.0f;
    camera_avatar2right->c->Pitch = 0.0f;
    camera_avatar2right->c->updateCameraVectors();

    // camera_avatar3left->c->Position = glm::vec3(-24.0f,2.0f,24.0f);
    // camera_avatar3left->c->Yaw = -45.0f;
    // camera_avatar3left->c->Pitch = 0.0f;
    // camera_avatar3left->c->updateCameraVectors();

    // camera_avatar3right->c->Position = glm::vec3(-24.0f,2.0f,24.0f);
    // camera_avatar3right->c->Yaw = -45.0f;
    // camera_avatar3right->c->Pitch = 0.0f;
    // camera_avatar3right->c->updateCameraVectors();

}

// TODO
// Might need to move these settings to the View. 
// Need Keyboard mode to control lighting and more than 1 light for Shading


