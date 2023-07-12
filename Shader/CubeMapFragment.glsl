#version 330

in vec3 TexCoords;
uniform samplerCube cubeMap;

void main(){
    gl_FragColor=texture(cubeMap,TexCoords);

}