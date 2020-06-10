#include <bits/stdc++.h>

#include "../include/Controller.h"

using namespace std;

int main()
{
    glfwInit();

    View *v = new View();
    // View *v2 = new View(v);

    vector<string> m;
    
    m.push_back("./data/cylinder.ply"); // Avatar 1
    m.push_back("./data/sphere.ply"); // Avatar 1 head
    m.push_back("./data/cylinder.ply"); // Avatar 2
    m.push_back("./data/sphere.ply"); // Avatar 2 head
    m.push_back("./data/sphere.ply"); // Ballon
    m.push_back("./data/basket.ply"); // Basket
    m.push_back("./data/bun_zipper.ply"); // Rabbit
    // m.push_back("./data/sphere.ply"); // Bird
    m.push_back("./data/airplane.ply"); // Bird
    m.push_back("./data/cube.ply"); // Field
    m.push_back("./data/cylinder.ply"); // Avatar 3
    m.push_back("./data/sphere.ply"); // Avatar 3 head


    vector<string> l;
    l.push_back("./data/sphere.ply");
    l.push_back("./data/sphere.ply");
    l.push_back("./data/sphere.ply");
    l.push_back("./data/sphere.ply");
    l.push_back("./data/cylinder.ply");
    l.push_back("./data/sphere.ply");


    vector<string> s;
    // s.push_back("./source/tex_only2.vert");
    // s.push_back("./source/tex_only2.frag");

    // Shaders for Simulating Lighting
    s.push_back("./source/mult_lights.vert");
    s.push_back("./source/mult_lights.frag");

    // Shaders for Clear Viewing (No Effects)
    s.push_back("./source/lamp.vert");
    s.push_back("./source/lamp.frag");

    // Shaders for Mapping Texture
    s.push_back("./source/texture.vs");
    s.push_back("./source/texture.fs");

    // Shaders for Clear Viewing for Lamps
    s.push_back("./source/lamp.vert");
    s.push_back("./source/lamp.frag");

    Controller *c = new Controller(m,l,s);

    // c->render(v,v2);
    c->render(v,v);


    glfwTerminate();

    return 0;
}
