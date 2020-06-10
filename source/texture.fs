#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoords;
in vec3 FragPos;

// texture samplers
uniform sampler2D texture1;

void main()
{
	FragColor = texture(texture1, TexCoords);
	//FragColor = vec4(1.0f);
}