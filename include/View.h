#ifndef VIEW_H
#define VIEW_H

#include <bits/stdc++.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Shader.h"
#include "Camera.h"
#include "Model.h"

using namespace std;

#define cout(a) cout<<a<<endl

class View
{
    public:
    
    GLFWwindow* window;
    GLFWwindow* window2;
    const static unsigned int SCR_WIDTH = 800;
    const static unsigned int SCR_HEIGHT = 600;
    static int mousestate;
    static int prevmousestate;

    View();
    View(View* ov);
    void DrawWithTexture(Shader* s, vector<Model> m, unsigned int texture1);
    void DrawWithTexture(Shader* s, Model m, unsigned int texture1);
    void DrawWithoutTexture(Shader* s, vector<Model> m);
    void DrawWithoutTexture(Shader* s, Model m);

    void DrawIJWithTexture(Shader* s, vector<Model> m, int i, int j, unsigned int texture1);
    void DrawIJWithoutTexture(Shader* s, vector<Model> m, int i, int j);
    
    void SetView(Shader* s, Camera* c);
    void SetPerspective(Shader* s,Camera* c, int type);
    void SetColouringMode(Shader* s, int ccmode, int nnmode);
    static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
    {
        View::mousestate = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
    }
};

#endif