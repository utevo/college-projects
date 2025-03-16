#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoord;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoord;
out vec4 FragPosLightSpace;

uniform mat4 MODEL;
uniform mat4 VIEW;
uniform mat4 PROJECTION;
uniform mat4 MVP;
uniform mat4 lightSpaceMatrix;

void main() {
    gl_Position = PROJECTION *  VIEW  * MODEL * vec4(position, 1.0f);
    FragPos = vec3(MODEL * vec4(position, 1.0f));
    Normal = mat3(transpose(inverse(MODEL))) * normal;
    TexCoord = texCoord;
	FragPosLightSpace = lightSpaceMatrix * vec4(FragPos, 1.0);
}