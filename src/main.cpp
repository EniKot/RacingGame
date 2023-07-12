
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "stb_image.h"
#include "Camera.h"
#include <iostream>
#include "Car.h"
#include "SkyBox.h"
#include "ShadowMap.h"

#define SCR_WIDTH 1200
#define SCR_HEIGHT 1200
#define SHADOW_MAP_SIZE 10240
float deltaTime=0.0f;
float lastFrame=0.0f;

float sensitivity = 0.05f;
bool firstMouse=true;
bool pause =true;
bool fixed = false;

Vec2 center = {SCR_WIDTH/2,SCR_HEIGHT/2};

std::vector<Shared<Shader>> shaderList;

Shared<DirectionalLight> light=createShared<DirectionalLight>(1.0,Vec3(1,1,1),Vec3(0.5,-1,0.5));
Shared<Camera> camera = createShared<Camera>();

Shared<ShadowMap> shadowMap=createShared<ShadowMap>();
Car car;
Entity plane;
bool show_panel = false;
void setShaderMat4(const std::string& name ,const Mat4& mat) {
    for(auto shader : shaderList){
        shader->bind();
        shader->setMat4(name,mat);
    }
}

void scroll_callback(GLFWwindow* window,double xoffset,double yoffset){
    float fov = camera->getFov();
    fov-=yoffset;
    if(fov>=1.0f&&fov<=45.0f){
        fov-=yoffset;
    }
    if(fov<=1.0f){
        fov=1.0f;
    }
    if(fov>80.0f){
        fov=80.0f;
    }
    camera->setFov(fov);

}
void mouse_callback(GLFWwindow* window,double xpos,double ypos){
    if(!pause&&!fixed) {
        float pitch, yaw;
        pitch = yaw = 0.0f;
        float xoffset = xpos - center.x;
        float yoffset = ypos - center.y;
        glfwSetCursorPos(window,center.x,center.y);

        xoffset *= sensitivity;
        yoffset *= sensitivity;
        yaw = xoffset;//绕y轴动，左右
        pitch = -1 * yoffset;//绕x轴动，上下
        camera->rotate(Vec3(pitch, yaw, 0.0f));
    }
}
void framebuffer_size_callback(GLFWwindow* window,int width,int height){
    glViewport(0,0,SCR_WIDTH,SCR_HEIGHT);
}
void processInput(GLFWwindow *window){
    if(pause){
        glfwSetInputMode(window,GLFW_CURSOR,GLFW_CURSOR_NORMAL);
    }else{
        glfwSetInputMode(window,GLFW_CURSOR,GLFW_CURSOR_DISABLED);
    }
    if(glfwGetKey(window,GLFW_KEY_ESCAPE)==GLFW_PRESS){
        glfwSetCursorPos(window,center.x,center.y);
        pause=!pause;
    }
    if(glfwGetKey(window,GLFW_KEY_Q)==GLFW_PRESS){
        glfwSetWindowShouldClose(window,true);
    }

    float cameraSpeed=2.5f*deltaTime;
    if(!pause) {
        if(glfwGetKey(window,GLFW_KEY_T)==GLFW_PRESS) {
            fixed = !fixed;
        }
        if(!fixed){
            Vec3 front = camera->getFront();
            Vec3 right = camera->getRight();
            Vec3 up = camera->getUp();
            if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
                camera->move(cameraSpeed * front);
            }
            if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
                camera->move(-1 * cameraSpeed * front);
            }
            if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
                camera->move(cameraSpeed * right);
            }
            if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
                camera->move(-1 * cameraSpeed * right);
            }
            if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
                camera->move(cameraSpeed * up);
            }
            if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) {
                camera->move(-1 * cameraSpeed * up);
            }
        }else{
            if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
                car.speedUp();
            }
            if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
                car.back();
            }
            if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
                car.turnLeft();
            }
            if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
                car.turnRight();
            }
            if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
                car.transform.move(Vec3(0,1,0)*2.5f*deltaTime);
            }
            if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) {
                car.transform.move(Vec3(0,-1,0)*2.5f*deltaTime);
            }
        }
    }
}
void setDeltaTime(){
    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
}
void initImGui(GLFWwindow *window){
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void) io;
    ImGui::StyleColorsClassic();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");
}
int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "RacingGame", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);//Enable vsync
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        std::cout << "Failed to initialized GLAD" << std::endl;
        return -1;
    }
    initImGui(window);

    std::vector<std::string> cubemaps{
                    "../resources/textures/skybox/right.jpg",
                    "../resources/textures/skybox/left.jpg",
                    "../resources/textures/skybox/top.jpg",
                    "../resources/textures/skybox/bottom.jpg",
                    "../resources/textures/skybox/front.jpg",
                    "../resources/textures/skybox/back.jpg",};
    Shared<Shader> carShader = createShared<Shader>("../Shader/carVertex.glsl","../Shader/carFragment.glsl");
    carShader->setInt("skybox",2);
    shaderList.push_back(carShader);
    Shared<Mesh> carModel = createShared<Mesh>("../resources/objects/Lamborghini/untitled.obj");
    car.transform.setLocalPosition(Vec3(0,0.3,0));
    car.setMesh(carModel);
    car.setShader(carShader);

    Shared<SkyBox> skyBox=createShared<SkyBox>(cubemaps);
    Shared<CubeTexture> cubeTexture = skyBox->getCubeTexture();
    Shared<Shader> skyBoxShader = skyBox->getShader();
    shaderList.push_back(skyBoxShader);

    Shared<Mesh> planeModel = createShared<Mesh>("../resources/objects/race-track/race-track.obj");
    Shared<Shader> planeShader = createShared<Shader>("../Shader/phongVertex.glsl","../Shader/phongFragment.glsl");
    shaderList.push_back(planeShader);
    plane.setMesh(planeModel);
    plane.setShader(planeShader);
    planeShader->bind();
    planeShader->setInt("shadowMap",2);

    camera->setPos(Vec3(0,0.3,-5));
    shadowMap->initShadowMap(light,SHADOW_MAP_SIZE,SHADOW_MAP_SIZE);
    Shared<Shader> depthShader = shadowMap->getDepthShader();
    Shared<CubeTexture> skyBoxMap = skyBox->getCubeTexture();
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    while(!glfwWindowShouldClose(window)){
        setDeltaTime();
        processInput(window);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        Mat4 projection = glm::perspective(glm::radians(camera->getFov()), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 1000.0f);
        camera->setProjection(projection);
        setShaderMat4("projection",projection);
        car.slowDown();
        car.carMove();
        if(fixed){
            Vec3 forward = car.transform.getForward();
            forward+=Vec3(0,-0.2,0);
            Vec3 pos = car.transform.getLocalPosition();
            pos+=Vec3(0,1,0);
            camera->setFront(forward);
            camera->setPos(pos);
            Vec3 front = camera->getFront();
            Vec3 up = camera->getUp();
            camera->move(-6.5f*front);
        }
        Mat4 view = camera->getViewMatrix();
        setShaderMat4("view",view);

        //1.Draw ShadowMap
        shadowMap->drawDepthMap();
        shadowMap->loadData(camera->getPos());
        car.Draw(depthShader);
        plane.Draw(depthShader);

        //2.Recover FrameBuffer&Viewport
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);

        //3.Draw Scene

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        planeShader->bind();
        planeShader->setFloat3("cameraPos",camera->getPos());
        planeShader->setFloat3("dirLight.lightDir",normalize(-light->getDirection()));
        planeShader->setFloat("dirLight.lightStrength",light->getIntensity());
        planeShader->setFloat3("dirLight.lightColor",light->getColor());
        planeShader->setMat4("lightSpaceMatrix", shadowMap->getLightSpaceMatrix());
        planeShader->setFloat("shadowSize",SHADOW_MAP_SIZE);
        glActiveTexture(GL_TEXTURE2);
        shadowMap->bind();
        plane.Draw();

        carShader->bind();
        carShader->setFloat3("dirLight.lightDir",normalize(-light->getDirection()));
        carShader->setFloat("dirLight.lightStrength",light->getIntensity());
        carShader->setFloat3("dirLight.lightColor",light->getColor());
        carShader->setFloat3("cameraPos",camera->getPos());
        glActiveTexture(GL_TEXTURE2);
        skyBoxMap->bind();
        car.Draw();

        skyBox->DrawSkyBox();

        //4.Draw ImGUI Panel
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("Data");
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate,
                        ImGui::GetIO().Framerate);
        Vec3 front = camera->getFront();
        Vec3 right = camera->getRight();
        Vec3 pos = camera->getPos();
        Vec3 carPos = car.transform.getLocalPosition();
        ImGui::Text("Camera Front:(%.3f,%.3f,%.3f)",front.x,front.y,front.z);
        ImGui::Text("Camera Right:(%.3f,%.3f,%.3f)",right.x,right.y,right.z);
        ImGui::Text("Camera Pos:(%.3f,%.3f,%.3f)",pos.x,pos.y,pos.z);
        ImGui::Text("Car Pos:(%.3f,%.3f,%.3f)",carPos.x,carPos.y,carPos.z);
        ImGui::Text("Car Speed: %.3f",car.getSpeed());
        ImGui::End();
        light->showControlPanel();
        shadowMap->showControlPanel();
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
