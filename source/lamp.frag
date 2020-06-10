#version 330 core

in vec3 Normal;
in vec2 TexCoord;

out vec4 FragColor;
 
uniform int cmode;
uniform int nmode;
uniform sampler2D texture1;
uniform vec3 lightColor;
void main()
{
   //White Color Lamp
   //FragColor = vec4(1.0f);
   
   //FragColor = vec4(Normal, 1.0f);
   //FragColor = vec4(1.0f,1.0f,1.0f,1.0f);
   //FragColor = vec4(0.57735f,0.57735f,0.57735f,1.0f);
   //float ambientStrength = 1.2;
   //vec3 ambient = ambientStrength * vec3(1.0f,1.0f,1.0f); //light color
   //vec3 result = ambient * (nmode * Normal + cmode * vec3(1.0f,1.0f,1.0f));
   //FragColor = vec4(result,1.0f);
   
   //Simple Color Program
   FragColor = nmode * vec4(Normal, 1.0f) + cmode * vec4(1.0f,1.0f,1.0f,1.0f);
   
   //Simple Color Program with lighting color 
   //FragColor = nmode * vec4(Normal * lightColor, 1.0f) + cmode * vec4(vec3(1.0f,1.0f,1.0f)*lightColor,1.0f);

   //Texture color
   //FragColor = texture(texture1, TexCoord);
}