#version 330 core

struct DirLight{
    vec3 lightColor;
    vec3 lightDir;
    float lightStrength;
};
out vec4 FragColor;

in vec2 TexCoord;
in vec3 worldNormal;
in vec3 worldPos;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform samplerCube skybox;
uniform DirLight dirLight;
uniform vec3 cameraPos;
float P=150.0;
float CalcLambert(vec3 L,vec3 N){
    return max(dot(L,N),0.0);
}
float CalcHalfLambert(vec3 L,vec3 N){
    return 0.5*max(dot(L,N),0.0)+0.5;
}
float CalcSpecular(vec3 H,vec3 N,float p){
    return pow(max(dot(H,N),0.0),p);
}
vec3 CalcDirLight(DirLight light,vec3 V,vec3 N,vec3 base){
    vec3 H=normalize(light.lightDir+V);
    vec3 diffuse=light.lightStrength*light.lightColor*CalcHalfLambert(light.lightDir,N);
    vec3 specular=light.lightStrength*light.lightColor*CalcSpecular(H,N,P);
    return diffuse*base+specular;
}
vec3 CalcReflect(vec3 N,vec3 V){
    vec3 I = -V;
    vec3 R = reflect(I, N);
    vec3 result =texture(skybox, R).rgb;
    return result;
}
void main(){
    vec3 viewDir=normalize(cameraPos-worldPos);
    vec4 base=texture(texture_diffuse1,TexCoord);
    vec3 col = pow(base.xyz,vec3(2.2));
    vec3 dlCol = CalcDirLight(dirLight,viewDir,worldNormal,col);
    vec3 reflectCol = CalcReflect(worldNormal,viewDir);
    dlCol=dlCol*reflectCol;
    dlCol=pow(dlCol,vec3(1.0/2.2));
    FragColor = vec4(dlCol,1.0);
}