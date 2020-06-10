#include "../include/Model.h"
// #define STB_IMAGE_IMPLEMENTATION
// #include "../include/stb_image.h"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/vector_angle.hpp> 
#include <glm/gtc/type_ptr.hpp>

Model::Model(string path, int maptype, int methodNumber) //Parses the PLY file to extract Vertex and Indices
{
    //Not parsing comments as of now.
    fstream file;
    string word;
    unsigned int noOfVertices, noOfFaces, temp;
    file.open(path);
    fstream debug;
    debug.open("Debugoutput.txt");
    int ignoreColumns = 0;
    while(file >> word)
    {
        if(word.compare("PLY") == 0)
        {
            debug<<"PLY FILE STARTED READING"<<endl;
            continue;
        }

        if(word.compare("element") == 0)
        {
            file >> word;
            if(word.compare("vertex") == 0)
            {
                file >> noOfVertices;
            }
            if(word.compare("face") == 0)
            {
                file >> noOfFaces;
            }
        }

        if(word.compare("format") == 0)
        {
            file >> word;
            file >> word;
            continue;
        }
        
        // Assumption is that all values will be float
        if(word.compare("property") == 0)
        {
            file >> word;
            if(word.compare("float32") == 0)
            {
                file >> word;
                if(word.compare("x") != 0 && word.compare("y") != 0 && word.compare("z") != 0)
                {
                    ignoreColumns++;
                }
                continue;
            }
            if(word.compare("list") == 0)
            {
                file >> word;
                file >> word;
                file >> word;
                continue;
            }
        }

        if(word.compare("end_header") == 0)
        {
            break;
        }
    }

    // cout << ignoreColumns << endl;

    unsigned int i = 0;
    float numberx, numbery, numberz,t;
    Vertex v;
    while(i < noOfVertices)
    {
        file >> numberx;
        file >> numbery;
        file >> numberz;

        v.Position = glm::vec3(numberx, numbery, numberz);
        v.Normal = glm::vec3(0.0f, 0.0f, 0.0f);
        v.TexCoords = glm::vec2(0.0f,0.0f);

        this->vertices.push_back(v);

        for(int j=0;j<ignoreColumns;j++)
            file >> t;

        i++;
    }


    vertices = Normalize(vertices);

    //Only Triangle Faces are supported 
    i = 0;
    unsigned int face1, face2, face3, size;
    while(i < noOfFaces)
    {
        file >> size;
        if(size != 3)
        {
            cout<<"MODEL::FACE::SIZE_OF_FACE_NOT_3" << endl;
            exit(0);
        }
        file >> face1;
        file >> face2;
        file >> face3;

        this->indices.push_back(face1);
        this->indices.push_back(face2);
        this->indices.push_back(face3);
        
        // Normal calculation for every face
        glm::vec3 v1 = vertices[face1].Position;
        glm::vec3 v2 = vertices[face2].Position;
        glm::vec3 v3 = vertices[face3].Position;

        glm::vec3 u = v2 - v1;
        glm::vec3 v = v3 - v1;
        glm::vec3 n = glm::cross(u,v);
        n = glm::normalize(n);
        // n = -n; 

        // Updating the normals for each vertex
        vertices[face1].Normal = n;
        vertices[face2].Normal = n;
        vertices[face3].Normal = n;


        // Adding to the list of normals
        normals[face1].push_back(n);
        normals[face2].push_back(n);
        normals[face3].push_back(n);
        i++;
    }

    splatmode = 0;
    
    glm::mat4 m = glm::mat4(1.0f);
    model = m; 

        //Generate Texure Coordinates for the object for the three methods
    //We need to implement a spherical / cylindrical mapping

    minvert = glm::vec4(10000.0f,10000.0f,0.0f,1.0f);
    maxvert = glm::vec4(-10000.0f,-10000.0f,0.0f,1.0f);

    for(int i=0;i<vertices.size();i++)
    {
        numberx = vertices[i].Position.x;
        numbery = vertices[i].Position.y;
        numberz = vertices[i].Position.z;

        if(numberx > maxvert.x) maxvert.x = numberx;
        if(numberx < minvert.x) minvert.x = numberx;
        if(numbery > maxvert.y) maxvert.y = numbery;
        if(numbery < minvert.y) minvert.y = numbery;

        if(numberz < minZ) minZ = numberz;
    }

    if(maptype == 1) // Cylindrical
    {
        // Method 3
        double theta,h;
        if (methodNumber == 3)
        {
            double thetamin=100000, thetamax=-100000;
            for(unsigned int i=0;i<vertices.size();i++)
            {
                glm::vec2 cnormal = glm::vec2(vertices[i].Position.x,vertices[i].Position.z);
                glm::vec2 xaxis = glm::vec2(1.0f,0.0f);
                cnormal = glm::normalize(cnormal);
                xaxis = glm::normalize(xaxis);

                if(cnormal.x >= 0 && cnormal.y >= 0)
                {
                    theta = glm::acos(cnormal.x);
                    theta /= 3.1415926536/2;
                    theta /= 4;
                }
                else if(cnormal.x >= 0 && cnormal.y <= 0)
                {
                    theta = glm::acos(cnormal.x);
                    theta = 3.1416926536/2 - theta;
                    theta /= 3.1415926536/2;
                    theta /= 4;
                    theta += 0.75;
                }
                else if(cnormal.x <= 0 && cnormal.y >= 0)
                {
                    theta = glm::asin(cnormal.y);
                    theta = 3.1415926536/2 - theta;
                    theta /= 3.1415926536/2;
                    theta /=4;
                    theta += 0.25;
                }
                else 
                {
                    theta = glm::asin(cnormal.y);
                    theta = 3.1415926536/2 - theta;
                    theta /= 3.1415926536/2;
                    theta /=4;
                    theta += 0.25;
                    if(thetamin > theta) thetamin = theta;
                    if(thetamax < theta) thetamax = theta;
                }

                //Addition of duplicate vertices to remove seams
                if(theta == 0)
                {

                }

                vertices[i].TexCoords.x = theta;

                h =  (vertices[i].Position.y - minvert.y) / (maxvert.y - minvert.y); 

                vertices[i].TexCoords.y = h;
            }
            // cout << glGetError() << thetamin << " " << thetamax << endl;
        }
        else if (methodNumber == 2)
        {
            double thetamin=100000, thetamax=-100000;
            // Method 2
            for(unsigned int i=0;i<vertices.size();i++)
            {
                glm::vec2 cnormal = glm::vec2(vertices[i].Normal.x,vertices[i].Normal.z);
                glm::vec2 xaxis = glm::vec2(1.0f,0.0f);
                cnormal = glm::normalize(cnormal);
                xaxis = glm::normalize(xaxis);

                double theta;
                if(cnormal.x >= 0 && cnormal.y >= 0)
                {
                    theta = glm::acos(cnormal.x);
                    theta /= 3.1415926536/2;
                    theta /= 4;
                }
                else if(cnormal.x >= 0 && cnormal.y <= 0)
                {
                    theta = glm::acos(cnormal.x);
                    theta = 3.1416926536/2 - theta;
                    theta /= 3.1415926536/2;
                    theta /= 4;
                    theta += 0.75;
                }
                else if(cnormal.x <= 0 && cnormal.y >= 0)
                {
                    theta = glm::asin(cnormal.y);
                    theta = 3.1415926536/2 - theta;
                    theta /= 3.1415926536/2;
                    theta /=4;
                    theta += 0.25;
                }
                else 
                {
                    theta = glm::asin(cnormal.y);
                    theta = 3.1415926536/2 - theta;
                    theta /= 3.1415926536/2;
                    theta /=4;
                    theta += 0.25;
                    if(thetamin > theta) thetamin = theta;
                    if(thetamax < theta) thetamax = theta;
                }
                vertices[i].TexCoords.x = theta;

                h =  (vertices[i].Position.y - minvert.y) / (maxvert.y - minvert.y); 

                vertices[i].TexCoords.y = h;
            }
        }
    } 
    else if(maptype == 2) // Spherical
    {
        double theta,phi;
        double thetamin=1000,thetamax=-1000,phimin=10000,phimax=-10000;
        if(methodNumber == 3)
        {
            //Method 3
            for(unsigned int i=0;i<vertices.size();i++)
            {
                glm::vec3 cnormal = glm::vec3(vertices[i].Position.x,vertices[i].Position.y,vertices[i].Position.z);
                glm::vec3 axis = glm::vec3(1.0f,0.0f,0.0f);
                cnormal = glm::normalize(cnormal);
                axis = glm::normalize(axis);

                double theta;
                if(cnormal.x >= 0 && cnormal.z >= 0)
                {
                    theta = glm::acos(cnormal.x);
                    theta /= 3.1415926536/2;
                    theta /= 4;
                }
                else if(cnormal.x >= 0 && cnormal.z <= 0)
                {
                    theta = glm::acos(cnormal.x);
                    theta = 3.1416926536/2 - theta;
                    theta /= 3.1415926536/2;
                    theta /= 4;
                    theta += 0.75;
                }
                else if(cnormal.x <= 0 && cnormal.z >= 0)
                {
                    theta = glm::asin(cnormal.z);
                    theta = 3.1415926536/2 - theta;
                    theta /= 3.1415926536/2;
                    theta /=4;
                    theta += 0.25;
                }
                else 
                {
                    theta = glm::asin(cnormal.z);
                    theta = 3.1415926536/2 - theta;
                    theta /= 3.1415926536/2;
                    theta /=4;
                    theta += 0.25;
                    if(thetamin > theta) thetamin = theta;
                    if(thetamax < theta) thetamax = theta;
                }
                vertices[i].TexCoords.x = theta;

                double phi;
                if(cnormal.x >= 0 && cnormal.y >= 0)
                {
                    phi = glm::acos(cnormal.x);
                    phi /= 3.1415926536/2;
                    phi /= 4;
                }
                else if(cnormal.x >= 0 && cnormal.y <= 0)
                {
                    phi = glm::acos(cnormal.x);
                    phi = 3.1416926536/2 - phi;
                    phi /= 3.1415926536/2;
                    phi /= 4;
                    phi += 0.75;
                }
                else if(cnormal.x <= 0 && cnormal.y >= 0)
                {
                    phi = glm::asin(cnormal.y);
                    phi = 3.1415926536/2 - phi;
                    phi /= 3.1415926536/2;
                    phi /=4;
                    phi += 0.25;
                }
                else 
                {
                    phi = glm::asin(cnormal.y);
                    phi = 3.1415926536/2 - phi;
                    phi /= 3.1415926536/2;
                    phi /=4;
                    phi += 0.25;
                    if(phimin > phi) phimin = phi;
                    if(phimax < phi) phimax = phi;
                }
                vertices[i].TexCoords.y = phi;

            }   
        }
        else if (methodNumber == 2)
        {        
            //Method 2
            for(unsigned int i=0;i<vertices.size();i++)
            {
                glm::vec3 cnormal = glm::vec3(vertices[i].Normal.x,vertices[i].Normal.y,vertices[i].Normal.z);
                glm::vec3 axis = glm::vec3(1.0f,0.0f,0.0f);
                cnormal = glm::normalize(cnormal);
                axis = glm::normalize(axis);

                double theta;
                if(cnormal.x >= 0 && cnormal.z >= 0)
                {
                    theta = glm::acos(cnormal.x);
                    theta /= 3.1415926536/2;
                    theta /= 4;
                }
                else if(cnormal.x >= 0 && cnormal.z <= 0)
                {
                    theta = glm::acos(cnormal.x);
                    theta = 3.1416926536/2 - theta;
                    theta /= 3.1415926536/2;
                    theta /= 4;
                    theta += 0.75;
                }
                else if(cnormal.x <= 0 && cnormal.z >= 0)
                {
                    theta = glm::asin(cnormal.z);
                    theta = 3.1415926536/2 - theta;
                    theta /= 3.1415926536/2;
                    theta /=4;
                    theta += 0.25;
                }
                else 
                {
                    theta = glm::asin(cnormal.z);
                    theta = 3.1415926536/2 - theta;
                    theta /= 3.1415926536/2;
                    theta /=4;
                    theta += 0.25;
                    if(thetamin > theta) thetamin = theta;
                    if(thetamax < theta) thetamax = theta;
                }
                vertices[i].TexCoords.x = theta;

                double phi;
                if(cnormal.x >= 0 && cnormal.y >= 0)
                {
                    phi = glm::acos(cnormal.x);
                    phi /= 3.1415926536/2;
                    phi /= 4;
                }
                else if(cnormal.x >= 0 && cnormal.y <= 0)
                {
                    phi = glm::acos(cnormal.x);
                    phi = 3.1416926536/2 - phi;
                    phi /= 3.1415926536/2;
                    phi /= 4;
                    phi += 0.75;
                }
                else if(cnormal.x <= 0 && cnormal.y >= 0)
                {
                    phi = glm::asin(cnormal.y);
                    phi = 3.1415926536/2 - phi;
                    phi /= 3.1415926536/2;
                    phi /=4;
                    phi += 0.25;
                }
                else 
                {
                    phi = glm::asin(cnormal.y);
                    phi = 3.1415926536/2 - phi;
                    phi /= 3.1415926536/2;
                    phi /=4;
                    phi += 0.25;
                    if(phimin > phi) phimin = phi;
                    if(phimax < phi) phimax = phi;
                }
                vertices[i].TexCoords.y = phi;
            }
        }
        // cout << glGetError() << " " << mintheta << " " << maxtheta << " " << minphi << " " << maxphi << endl;
    }


    SetupModel();
    numberFaces = noOfFaces;

    geoiteration = -1;
    animateGeodesic(); // Setup Function for animation geodesic

    // aVAO[0] = VAO;
    // aVBO[0] = VBO;
    // aEBO[0] = EBO;

    // Splats Not Working for Stanford Bunny. Too small?
    // SetupSplat();
    // cout << "lol" << endl;

    

    // this->texpath = texpath;
    // this->spectexpath = spectexpath;
    // loadAndSetTexture();

}

void Model::SetupModel()
{
    //Normalizing the vertices
    // Normalize(vertices);

    glm::vec3 avg;
    int count = 0;

    // fstream o("output.txt");
    
    //Assigning Normals to the vertices
    for(unsigned int i=0; i<vertices.size();i++)
    {
        
        for(int j=0;j<normals[i].size();j++)
        {
            avg += normals[i][j];
            count++;
        }
        // avg /= count;
        avg = glm::normalize(avg);
        avg.x = clamp(avg.x,0.0f,1.0f);
        avg.y = clamp(avg.y,0.0f,1.0f);
        avg.z = clamp(avg.z,0.0f,1.0f);
        // o<<glGetError()<<endl<<avg.x<<endl<<avg.y<<endl<<avg.z;
        vertices[i].Normal = avg;
    }
    // o.close();

    //Default Drawing mode
    drawmode = GL_TRIANGLES;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));

    glBindVertexArray(0);
}

void Model::SetupModelForSecondScreen()
{
    glGenVertexArrays(1, &VAO2);
    glBindVertexArray(VAO2);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));

    glBindVertexArray(0);
}

void Model::SetupSplat()
{
    splatvertices.clear();
    fanvertices.clear();

    unsigned int f1,f2,f3;
    Vertex A,B,C;
    float a,b,c,s;
    Vertex k;

    // fstream f("output.txt");
    for(unsigned int i = 0 ; i < indices.size();)
    {
        f1=indices[i++];
        f2=indices[i++];
        f3=indices[i++];

        A = vertices[f1];
        B = vertices[f2];
        C = vertices[f3];

        a = glm::distance(B.Position,C.Position);
        b = glm::distance(C.Position,A.Position);
        c = glm::distance(A.Position,B.Position);
        s = (a+b+c)/2;

        k.Position = a/(a+b+c)*A.Position + b/(a+b+c)*B.Position + c/(a+b+c)*C.Position;
        k.Radius = glm::sqrt(s*(s-a)*(s-b)*(s-c))/s;
        
        //Normal calculation for each face
        glm::vec3 u = B.Position - A.Position;
        glm::vec3 v = C.Position - A.Position;
        glm::vec3 n = glm::cross(u,v);
        n = glm::normalize(n);
        k.Normal = n;

        // f << glGetError() << " " << n.x << " " << n.y<< " " << n.z << endl;

        splatvertices.push_back(k);
    }
    // f.close();

    fanvertices.reserve(noOfSplatVertices*splatvertices.size()*sizeof(Vertex));

    Vertex vtemp;
    glm::vec4 temp;
    glm::mat4 trans = glm::mat4(1.0f);
    glm::mat4 final = glm::mat4(1.0f);
    for(unsigned int i=0;i<splatvertices.size();i++)
    {
        trans = glm::mat4(1.0f);
        
        //Find out Rotation angle and axis
        glm::vec3 axisOfR = glm::cross(splatvertices[i].Normal,glm::vec3(0.0f,0.0f,1.0f));
        axisOfR = glm::normalize(axisOfR);
        // float x = glm::dot<glm::vec3>(axisOfR,glm::vec3(0.0f,0.0f,1.0f))/glm::length(axisOfR);
        // float x = axisOfR.z/glm::sqrt((axisOfR.x*axisOfR.x)+(axisOfR.y*axisOfR.y)+(axisOfR.z*axisOfR.z));
        float angleOfR = glm::acos(glm::dot(axisOfR,glm::vec3(0.0f,0.0f,1.0f))/glm::sqrt((axisOfR.x*axisOfR.x)+(axisOfR.y*axisOfR.y)+(axisOfR.z*axisOfR.z)));
        
        // fstream f("output.txt");
        //Applying them to all points generated.
        // trans = glm::rotate(trans,angleOfR,axisOfR);
        // final = glm::translate(trans,splatvertices[i].Position);
        trans = glm::translate(trans,splatvertices[i].Position);        
        final = glm::rotate(trans,angleOfR,axisOfR);
        // f.close();

        //Radius Addition Checker/Result
        splatvertices[i].Radius = clamp(radiusAdd+splatvertices[i].Radius,0.0f,10000.0f);
        
        //Generate the points 
        for(double j=0.0; j<2 * glm::pi<double>();j+=2*glm::pi<double>()/noOfSplatVertices)
        {
            // temp.Position = glm::vec3(cos(j)*splats[i].Radius,sin(i)*splats[i].Radius,0);
            temp = glm::vec4(cos(j)*splatvertices[i].Radius,sin(j)*splatvertices[i].Radius,0.0,1.0f);
            // temp = glm::vec4(cos(j)*0.5,sin(j)*0.5,0.0,1.0f);
            // f << glGetError() << endl << cos(j) << endl;
            temp = final * temp;
            // f << glGetError() << endl << temp.x << temp.y << temp.z << endl;
            vtemp.Position = glm::vec3(temp.x,temp.y,temp.z);
            vtemp.Normal = splatvertices[i].Normal;
            vtemp.Radius = splatvertices[i].Radius;
            fanvertices.push_back(vtemp);
        }
        
    }

    fanvertices = Normalize(fanvertices);
    // fstream o("output.txt");
    // for(int i=0;i<splatvertices.size();i++)
    // {
    //     o << glGetError() << splatvertices[i].Radius << endl;
    // }
    // o.close();
    
    unsigned int location = 0;
    for(unsigned int i=0;i<splatvertices.size();i++) // 14 should be replaced with number of splat vertices - 2
    {   
        glGenVertexArrays(1, &sVAO[i]);
        glGenBuffers(1, &sVBO[i]);

        glBindVertexArray(sVAO[i]);
        glBindBuffer(GL_ARRAY_BUFFER, sVBO[i]);

        glBufferData(GL_ARRAY_BUFFER,16*sizeof(Vertex),&fanvertices[location],GL_STATIC_DRAW);
        location+=16;
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE, sizeof(Vertex),(void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));

        glBindBuffer(GL_ARRAY_BUFFER,0);
        glBindVertexArray(0);
    }

    

    // unsigned int loc = 0;
    // for(int i=0;i<splatvertices.size();i++)
    // {
    //     fan_starts[i] = loc;
    //     fan_sizes[i] = noOfSplatVertices;
    //     loc+=noOfSplatVertices;
    // }

    // fstream o("output.txt");
    // for(int i=0;i<fanvertices.size();i++)
    // {
    //     o << glGetError() << fan_starts[i] << " "<< fan_sizes[i] << "\n";
    // }
    // o.close();
}

void Model::Draw(Shader* shader)
{
    if(splatmode == 0)
    {
        glBindVertexArray(VAO);
        glDrawElements(drawmode, indices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }
    else 
    {
        for(int i=0;i<splatvertices.size();i++)
        {  
            glBindVertexArray(sVAO[i]);
            glDrawArrays(GL_TRIANGLE_FAN,0,15);
            glBindVertexArray(0);
        }
    }
}

void Model::Draw2(Shader* shader)
{
    if(splatmode == 0)
    {
        glBindVertexArray(VAO2);
        glDrawElements(drawmode, indices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }
    else 
    {
        for(int i=0;i<splatvertices.size();i++)
        {  
            glBindVertexArray(sVAO[i]);
            glDrawArrays(GL_TRIANGLE_FAN,0,15);
            glBindVertexArray(0);
        }
    }
}



void Model::DrawSplats(Shader shader)
{

    // glBindVertexArray(sVAO);
    // // glMultiDrawArrays(GL_LINE_LOOP,fan_starts,fan_sizes,splatvertices.size());
    // glMultiDrawArrays(GL_LINE_LOOP,fan_starts,fan_sizes,1);
    // glBindVertexArray(0);

    // for(int i=0;i<splatvertices.size();i++)
    // {  
    //     glBindVertexArray(sVAO[i]);
    //     glDrawArrays(GL_TRIANGLE_FAN,0,15);
    //     glBindVertexArray(0);
    // }
    
}

void Model::setWireframe()
{
    drawmode = GL_LINES;
}
void Model::setFilled()
{
    drawmode = GL_TRIANGLES;
}

void Model::setSplatMode(int s)
{
    splatmode = s;
}

glm::mat4 Model::GetModelMatrix()
{
    return this->model;
}

void Model::setModelMatrix(glm::mat4 model)
{
    this->model = model;
}

vector<Vertex> Model::Normalize(vector<Vertex> vertices)
{
    float max = 0, min = 0;
    for(int i=0;i<vertices.size();i++)
    {
        if(max < vertices[i].Position.x)
            max = vertices[i].Position.x;
        if(max < vertices[i].Position.y)
            max = vertices[i].Position.y;
        if(max < vertices[i].Position.z)
            max = vertices[i].Position.z;  

        if(min > vertices[i].Position.x )
            min = vertices[i].Position.x;
        if(min > vertices[i].Position.y )
            min = vertices[i].Position.y;
        if(min > vertices[i].Position.z )
            min = vertices[i].Position.z;
    }

    if( max < 1 && min > -1)
        return vertices;
    else 
    {
        for(int i=0;i<vertices.size();i++)
        {
            vertices[i].Position.x = (vertices[i].Position.x - min ) / (max - min);
            vertices[i].Position.x *= 2.0;
            vertices[i].Position.x += -1;

            vertices[i].Position.y = (vertices[i].Position.y - min ) / (max - min);
            vertices[i].Position.y *= 2.0;
            vertices[i].Position.y += -1;

            vertices[i].Position.z = (vertices[i].Position.z - min ) / (max - min);
            vertices[i].Position.z *= 2.0;
            vertices[i].Position.z += -1;
        }
    }

    return vertices;
}

void Model::animateGeodesic()
{
    // geovertices.reserve(maxgeoiteration);
    // geoindices.reserve(maxgeoiteration);
    geoiteration++;

    if(geoiteration != 0) 
    {

        geovertices.reserve(vertices.size());
        geoindices.reserve(indices.size());

        // Animation Model Generation
        map <int, int> imapping; // Old Index to New Index mapping
        // for(int i = 0;i<maxgeoiteration;i++)
        // {   
        imapping.clear();
        // int normaltoggle=0;

        for(int j=0;j<indices.size();j+=3)
        {
            glm::vec3 v1 = vertices[indices[j]].Position;
            glm::vec3 v2 = vertices[indices[j+1]].Position;
            glm::vec3 v3 = vertices[indices[j+2]].Position;
            glm::vec3 u = v2 - v1;
            glm::vec3 v = v3 - v1;
            glm::vec3 n = glm::cross(u,v);
            n = glm::normalize(n);
            // n = -n;
            glm::vec3 centroid = glm::vec3((v1.x+v2.x+v3.x)/3,(v1.y+v2.y+v3.y)/3,(v1.z+v2.z+v3.z)/3);
            if(geoiteration%2!=1) centroid += glm::vec3(n.x/geoiteration,n.y/geoiteration,n.z/geoiteration);
            else centroid -= glm::vec3(n.x/geoiteration,n.y/geoiteration,n.z/geoiteration);

            int iv1,iv2,iv3,iv4;
            auto it = imapping.find(indices[j]);
            // cout << glGetError() << "LOL" << i << j << endl;
            // imapping.end();
            if(it != imapping.end())
            {
                iv1 = imapping.find(indices[j])->second;
            }
            else 
            {
                iv1 = geovertices.size();
                // for(int k=0;k<geovertices.size();k++)
                    // cout << geovertices[k].Position.x << endl;
                // geovertices[iv1];
                // cout << glGetError() << "LOL" << i << j <<"LOL" << iv1 << " " << offset << endl;
                geovertices.push_back(vertices[indices[j]]);
                imapping.insert({indices[j],iv1});
            }
            if(imapping.find(indices[j+1]) != imapping.end())
                iv2 = imapping.find(indices[j+1])->second;
            else 
            {
                iv2 = geovertices.size();
                geovertices.push_back(vertices[indices[j+1]]);
                imapping.insert({indices[j+1],iv2});
            }    
            if(imapping.find(indices[j+2]) != imapping.end())
                iv3 = imapping.find(indices[j+2])->second;
            else 
            {
                iv3 = geovertices.size();
                geovertices.push_back(vertices[indices[j+2]]);
                imapping.insert({indices[j+2],iv3});
            }
            iv4 = geovertices.size();
            Vertex cenVertex;
            cenVertex.Position = centroid;
            cenVertex.Normal = -n;
            cenVertex.Radius = 0.0f;
            cenVertex.TexCoords = glm::vec2(0.0f,0.0f);
            geovertices.push_back(cenVertex);
            // imapping.insert({indices[j],iv4}); No mapping for a completely new vertex

            //indices insertion - the 3 new faces
            geoindices.push_back(iv1);geoindices.push_back(iv4);geoindices.push_back(iv2);
            geoindices.push_back(iv2);geoindices.push_back(iv4);geoindices.push_back(iv3);
            geoindices.push_back(iv3);geoindices.push_back(iv4);geoindices.push_back(iv1);
        }

        vertices = geovertices;
        indices = geoindices;

    cout<<glGetError() << "GEOITERATION:" << geoiteration << endl;
    }


    int j = geoiteration;

    glm::vec3 avg;
    int count = 0;

    //Default Drawing mode
    drawmode = GL_TRIANGLES;

    glGenVertexArrays(1, &aVAO[j]);
    glGenBuffers(1, &aVBO[j]);
    glGenBuffers(1, &aEBO[j]);

    glBindVertexArray(aVAO[j]);
    glBindBuffer(GL_ARRAY_BUFFER, aVBO[j]);

    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, aEBO[j]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));

    glBindVertexArray(0);
}

void Model::drawGeodesic(Shader *shader)
{
    glBindVertexArray(aVAO[geoiteration]);
    glDrawElements(drawmode, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void Model::vertexUpdateByModel()
{
    for(int i=0;i<vertices.size();i++)
    {
        glm::vec4 newPoint = model * glm::vec4(vertices[i].Position,1.0f);
        vertices[i].Position = glm::vec3(newPoint.x,newPoint.y,newPoint.z);
        glm::vec4 newNormal = model * glm::vec4(vertices[i].Normal,1.0f);
        vertices[i].Normal = glm::vec3(newNormal.x,newNormal.y,newNormal.z);
    }

    model = glm::mat4(1.0f);
}