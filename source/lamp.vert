#version 330 core 
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec3 aTexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
// uniform mat4 transform;

out vec3 Normal;
out vec2 TexCoord;
void main()
{
   // gl_Position = projection * view * model * transform * vec4(aPos, 1.0);
   //Normal = aNormal;
   Normal = mat3(transpose(inverse(model))) * aNormal;
   TexCoord = vec2(aTexCoord.x,aTexCoord.y);
   gl_Position = projection * view * model * vec4(aPos, 1.0);
   //gl_Position = transform * vec4(aPos, 1.0);
   //gl_Position = vec4(aPos, 1.0);
   //gl_Position = model * vec4(aPos, 1.0);
}