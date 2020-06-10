#ifndef MODEL_H
#define MODEL_H

#include "Shader.h"
#include <vector>
#include <map>

#define GL_ENABLE_EXPERIMENTAL
#include "glm/gtc/constants.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
// #include <glm/gtx/norm.hpp>
// #include <glm/gtx/string_cast.hpp>

#define clamp(x, lower, upper) (min(upper, max(x,lower)))


struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
    float Radius;
};


class Model {

    public:
        vector<Vertex> vertices;
        vector<Vertex> splatvertices;
        vector<Vertex> fanvertices;
        // Vertex* vertices;
        vector<unsigned int> indices;
        map<unsigned int, vector<glm::vec3>> normals;
        glm::mat4 model;

        //Wireframe/filled mode
        int drawmode;
        int splatmode;
        const int noOfSplatVertices = 15;
        glm::vec4 minvert;
        glm::vec4 maxvert;
        float minZ = 100000.0;
        int numberFaces;
        float radiusAdd = 0.0;
        unsigned int texture;
        unsigned int spectexture;
        int textureMode = 1;
        string texpath;
        string spectexpath;

        // Data Structure to store multiple geovertices of Tetrahedron and Octahedron
        vector<Vertex> geovertices;
        vector<unsigned int> geoindices;
        unsigned int geoiteration = 0; // 0 - Original Model
        const unsigned int maxgeoiteration = 6;


        Model(string path, int maptype, int methodNumber); // 1 will be cylindrical and 2 will be spherical 
        void Draw(Shader* shader);
        void Draw2(Shader* shader);
        void DrawSplats(Shader shader);
        glm::mat4 GetModelMatrix();
        void setModelMatrix(glm::mat4 model);
        void setWireframe();
        void setFilled();
        void setSplatMode(int);

        void vertexUpdateByModel();

        // Texture Setup Function
        void loadAndSetTexture();

        // Geodesic Functions
        void animateGeodesic();
        void drawGeodesic(Shader* shader);

        unsigned int VAO, VBO, EBO;
        unsigned int VAO2;
        unsigned int sVAO[20000], sVBO[20000];
        unsigned int aVAO[100],aVBO[100],aEBO[100]; // Hardcoded only 100 animation iterations - More than enough for geovertices
        void SetupModel();
        void SetupModelForSecondScreen();
        void SetupSplat();
        vector<Vertex> Normalize(vector<Vertex> vertices);
        void VertexNormals();
};


#endif

//TODO
// Store model vertices based on Triangle Faces as key
// Generate the vertices and indices array to run SetupModel()
// When running Animation function, perform the required changes to this Dictionary
