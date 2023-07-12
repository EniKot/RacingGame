#version 330 core
#define NUM_SAMPLES 30
#define NUM_RINGS 10
#define EPS 1e-3
#define PI 3.141592653589793
#define PI2 6.283185307179586
out vec4 FragColor;

in VS_OUT {
    vec3 worldPos;
    vec3 normal;
    vec2 texCoords;
    vec4 FragPosLightSpace;
} fs_in;
struct DirLight{
    vec3 lightColor;
    vec3 lightDir;
    float lightStrength;
};
uniform float shadowSize;
uniform sampler2D texture_diffuse1;
uniform sampler2D shadowMap;
uniform vec3 cameraPos;
uniform DirLight dirLight;
float P=150.0;
float rand_1to1(float x ) {
    return fract(sin(x)*10000.0);
}
float unpack(vec4 rgbaDepth) {
    const vec4 bitShift = vec4(1.0, 1.0/256.0, 1.0/(256.0*256.0), 1.0/(256.0*256.0*256.0));
    return dot(rgbaDepth, bitShift);
}
float rand_2to1(vec2 uv ) {
    const float a = 12.9898, b = 78.233, c = 43758.5453;
     float dt = dot( uv.xy, vec2( a,b ) ), sn = mod( dt, PI );
    return fract(sin(sn) * c);
}
vec2 poissonDisk[NUM_SAMPLES];
//泊松分布采样
void poissonDiskSamples( const in vec2 randomSeed ) {
    float ANGLE_STEP = PI2 * float( NUM_RINGS ) / float( NUM_SAMPLES );
    float INV_NUM_SAMPLES = 1.0 / float( NUM_SAMPLES );

    float angle = rand_2to1( randomSeed ) * PI2;
    float radius = INV_NUM_SAMPLES;
    float radiusStep = radius;

    for( int i = 0; i < NUM_SAMPLES; i ++ ) {
        poissonDisk[i] = vec2( cos( angle ), sin( angle ) ) * pow( radius, 0.75 );
        radius += radiusStep;
        angle += ANGLE_STEP;
    }
}
//正态分布采样
void uniformDiskSamples( const in vec2 randomSeed ) {
    float randNum = rand_2to1(randomSeed);
    float sampleX = rand_1to1( randNum ) ;
    float sampleY = rand_1to1( sampleX ) ;
    float angle = sampleX * PI2;
    float radius = sqrt(sampleY);
    for( int i = 0; i < NUM_SAMPLES; i ++ ) {
        poissonDisk[i] = vec2( radius * cos(angle) , radius * sin(angle)  );

        sampleX = rand_1to1( sampleY ) ;
        sampleY = rand_1to1( sampleX ) ;

        angle = sampleX * PI2;
        radius = sqrt(sampleY);
    }
}
float Bias(){
    vec3 lightDir =normalize(dirLight.lightDir);
    vec3 normal = normalize(fs_in.normal);
    float bias = max(0.005 * (1.0 - dot(normal, lightDir)), 0.005);
    return bias;
}
float PCF(vec3 shadowCoord,float filterSize){
    float distance = shadowCoord.z;
    float vis = 0;
    float bias = Bias();
    filterSize = 1.0/shadowSize*filterSize;
    poissonDiskSamples(shadowCoord.xy);
    for(int i=0;i<NUM_SAMPLES;i++){
        vec2 uv = poissonDisk[i]*filterSize+shadowCoord.xy;
        float depth = unpack(texture(shadowMap,uv));
        vis+=distance>depth+bias+EPS?0.0:1.0;
    }
    vis=vis/float(NUM_SAMPLES);
    if(distance>1.0){
        vis = 1.0;
    }

    return vis;
}
vec3 blinnPhong() {
    vec3 color = texture(texture_diffuse1, fs_in.texCoords).rgb;
    vec3 worldPos = fs_in.worldPos;
    color = pow(color, vec3(2.2));
    vec3 ambient = 0.3 * color;
    vec3 lightDir =normalize(dirLight.lightDir);
    vec3 lightColor = dirLight.lightColor;
    float intensity = dirLight.lightStrength;
    vec3 normal = normalize(fs_in.normal);
    float diff = max(dot(lightDir, normal), 0.0);

    vec3 diffuse = intensity *diff * color;

    vec3 viewDir = normalize(cameraPos -worldPos);
    vec3 halfDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(halfDir, normal), 0.0), 64.0);
    vec3 specular = intensity*spec*lightColor;

    vec3 radiance = (ambient + diffuse + specular);
    vec3 phongColor = pow(radiance,vec3(1.0/2.2));
    return phongColor;
}
void main()
{
    // 计算阴影
    float visibility;
    vec4 fragPosLightSpace = fs_in.FragPosLightSpace;
    //计算得到当前着色点在lightSpace下的NDC坐标,映射到[0,1]
    vec3 shadowCoord = fragPosLightSpace.xyz/fragPosLightSpace.w*0.5+0.5;
    visibility = PCF(shadowCoord,3.0);
    vec3 phongColor = blinnPhong();

    FragColor = vec4(phongColor*visibility, 1.0);
}
