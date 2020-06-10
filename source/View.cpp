#include "../include/View.h"

void mouse_button_callback(GLFWwindow* window,int button, int action, int mods);

int View::mousestate = 0;
int View::prevmousestate = 0;

View::View()
{
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    window = glfwCreateWindow(640,480,"LearnOpenGL",NULL,NULL);
    // glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    window2 = glfwCreateWindow(640,480,"LearnOpenGL2ndWindow",NULL, window);
    if (window == NULL)
    {
        cout("Failed to create GLFW window 1");
        glfwTerminate();
        exit(-1);
    }
    if (window2 == NULL)
    {
        cout("Failed to create GLFW window 2");
        glfwTerminate();
        exit(-1);
    }
    glfwMakeContextCurrent(window);
    glewExperimental = GL_TRUE;
    if(glewInit() != GLEW_OK)
    {
        cout("GLEW initialization failed");
        glfwDestroyWindow(window);
        glfwTerminate();
        exit(1);
    }

    glfwMakeContextCurrent(window2);
    glewExperimental = GL_TRUE;
    if(glewInit() != GLEW_OK)
    {
        cout("GLEW initialization failed");
        glfwDestroyWindow(window);
        glfwTerminate();
        exit(1);
    }

    glfwMakeContextCurrent(window);

    //Tell GLFW to capture mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    //Enabling Depth Buffer
    glEnable(GL_DEPTH_TEST);

    //Setting a Callback for Mouse State
    glfwSetMouseButtonCallback(window, View::mouse_button_callback);
    // glfwSetMouseButtonCallback(window2, View::mouse_button_callback);

    glfwSetWindowPos(window, 0, 0);

    glfwMakeContextCurrent(window2);
    glfwSetInputMode(window2,GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glEnable(GL_DEPTH_TEST);
    glfwSetWindowPos(window2, 700, 0);

    glfwMakeContextCurrent(window);

}

View::View(View* ov)
{
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    window = glfwCreateWindow(800,600,"LearnOpenGL",NULL,ov->window);
    if (window == NULL)
    {
        cout("Failed to create GLFW window");
        glfwTerminate();
        exit(-1);
    }
    glfwMakeContextCurrent(window);
    glewExperimental = GL_TRUE;
    if(glewInit() != GLEW_OK)
    {
        cout("GLEW initialization failed");
        glfwDestroyWindow(window);
        glfwTerminate();
        exit(1);
    }

    //Tell GLFW to capture mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    //Enabling Depth Buffer
    glEnable(GL_DEPTH_TEST);

    //Setting a Callback for Mouse State
    glfwSetMouseButtonCallback(window, View::mouse_button_callback);
    
}

void View::DrawWithTexture(Shader* s, vector<Model> m, unsigned int texture1)
{
    for(int i=0;i<m.size();i++)
    {
        glm::mat4 model = glm::mat4(1.0f);
        model = m[i].GetModelMatrix();
        s->setMat4("model", model);
        glBindTexture(GL_TEXTURE_2D, texture1);
        m[i].drawGeodesic(s); // To draw the geodesic
    }
}
void View::DrawWithTexture(Shader* s, Model m, unsigned int texture1)
{
    glm::mat4 model = glm::mat4(1.0f);
    model = m.GetModelMatrix();
    s->setMat4("model", model);
    glBindTexture(GL_TEXTURE_2D, texture1);
    m.drawGeodesic(s); // To draw the geodesic
}
void View::DrawWithoutTexture(Shader* s, vector<Model> m)
{
    for(int i=0;i<m.size();i++)
    {
        glm::mat4 model = glm::mat4(1.0f);
        model = m[i].GetModelMatrix();
        s->setMat4("model", model);
        m[i].drawGeodesic(s); // To draw the geodesic
    }
}
void View::DrawWithoutTexture(Shader* s, Model m)
{
    glm::mat4 model = glm::mat4(1.0f);
    model = m.GetModelMatrix();
    s->setMat4("model", model);
    // m.drawGeodesic(s); // To draw the geodesic   
    if(glfwGetCurrentContext() == window)
        m.Draw(s);
    else 
        m.Draw2(s);
}

void View::DrawIJWithTexture(Shader* s, vector<Model> m, int i, int j, unsigned int texture1)
{
    for(int k=i;k<=j;k++)
    {
        glm::mat4 model = glm::mat4(1.0f);
        model = m[k].GetModelMatrix();
        s->setMat4("model", model);
        glBindTexture(GL_TEXTURE_2D, texture1);
        m[k].drawGeodesic(s); // To draw the geodesic
    }
}

void View::DrawIJWithoutTexture(Shader* s, vector<Model> m, int i, int j)
{
    for(int k=i;k<=j;k++)
    {
        glm::mat4 model = glm::mat4(1.0f);
        model = m[k].GetModelMatrix();
        s->setMat4("model", model);
        m[k].drawGeodesic(s); // To draw the geodesic
    }
}

void View::SetView(Shader* s, Camera* c)
{
    glm::mat4 view = glm::mat4(1.0f);
    view = c->GetViewMatrix();
    s->setMat4("view",view);
}

void View::SetPerspective(Shader* s, Camera* c, int type)
{
    glm::mat4 projection = glm::mat4(1.0f);
    if(type == 1)
        projection = glm::perspective(glm::radians(c->Zoom), (float)SCR_WIDTH/ (float) SCR_HEIGHT, 0.1f, 100.0f);
    else if(type == 0)
        projection = glm::ortho(-10.0f,10.0f,-10.0f,10.0f,0.0f,100.0f);
    s->setMat4("projection", projection);
}

void View::SetColouringMode(Shader* s, int ccmode, int nnmode)
{
    s->setInt("cmode",ccmode);
    s->setInt("nmode",nnmode);
}
