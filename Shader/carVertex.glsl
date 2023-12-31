#version 330 core

layout (location=0) in vec3 aPos;

layout (location=1) in vec3 aNormal;

layout (location=2) in vec2 aTexCoord;


out vec2 TexCoord;
out vec3 worldNormal;
out vec3 worldPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main(){
    gl_Position=projection*view*model*vec4(aPos,1.0);
    vec3 worldPos=(model*vec4(aPos.xyz,1.0)).xyz;
    TexCoord=aTexCoord;
    worldNormal=normalize(transpose(inverse(mat3(model)))*aNormal);
}