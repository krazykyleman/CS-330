/*
Author: Kyle Wucik
Assignment: 5-5 Texture Milestone
Class: CS-330
Date: 2/10/2023

Some things to say: 
1. I made the handle with different corner colors to show that the texture was loading properly and not just a black texturless item
2. The tile floor will not be where the items go, I'm making a counter as well.
3. N/A

*/
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <learnopengl/filesystem.h>
#include <learnopengl/shader_m.h>
#include <learnopengl/camera.h>

#include <iostream>


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
unsigned int loadTexture(const char* path);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

bool isPerspective = true;
bool keyPressed = false;

// camera stuff
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = 800.0f / 2.0;
float lastY = 600.0 / 2.0;
bool firstMouse = true;

// frame timing
float deltaTime = 0.0f; // time between current frame and last frame
float lastFrame = 0.0f; // time of last frame

// lighting
glm::vec3 lightPos(1.0f, 0.0f, 1.5f);


int main() {

    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Project One - Kyle W", NULL, NULL);

    if (window == NULL) {

        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();

        return -1;

    }

    glfwMakeContextCurrent(window);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    glfwSetCursorPosCallback(window, mouse_callback);

    glfwSetScrollCallback(window, scroll_callback);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {

        std::cout << "Failed to initialize GLAD" << std::endl;

        return -1;

    }

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    // build and compile our shader zprogram
    // ------------------------------------
    Shader lightingShader("shader/1.colors.vs", "shader/1.colors.fs");
    Shader lightCubeShader("shader/1.light_cube.vs", "shader/1.light_cube.fs");

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
#define PI 3.14159265358979323846
#define SLICES 20

    std::vector<float> cylinderVertices;
    std::vector<float> cylinderNormals;

    float a = 0.5; // major axis length
    float b = 0.25; // minor axis length

    for (int i = 0; i < SLICES; i++) {
        float theta1 = 2 * PI * i / SLICES;
        float theta2 = 2 * PI * (i + 1) / SLICES;

        // bottom vertex
        cylinderVertices.push_back(a * cos(theta1));
        cylinderVertices.push_back(b * sin(theta1));
        cylinderVertices.push_back(-0.5);
        cylinderNormals.push_back(cos(theta1));
        cylinderNormals.push_back(sin(theta1));
        cylinderNormals.push_back(0.0);

        // side vertices
        cylinderVertices.push_back(a * cos(theta2));
        cylinderVertices.push_back(b * sin(theta2));
        cylinderVertices.push_back(0.5);
        cylinderNormals.push_back(cos(theta2));
        cylinderNormals.push_back(sin(theta2));
        cylinderNormals.push_back(0.0);

        cylinderVertices.push_back(a * cos(theta1));
        cylinderVertices.push_back(b * sin(theta1));
        cylinderVertices.push_back(0.5);
        cylinderNormals.push_back(cos(theta1));
        cylinderNormals.push_back(sin(theta1));
        cylinderNormals.push_back(0.0);

        cylinderVertices.push_back(a * cos(theta2));
        cylinderVertices.push_back(b * sin(theta2));
        cylinderVertices.push_back(-0.5);
        cylinderNormals.push_back(cos(theta2));
        cylinderNormals.push_back(sin(theta2));
        cylinderNormals.push_back(0.0);

        cylinderVertices.push_back(a * cos(theta1));
        cylinderVertices.push_back(b * sin(theta1));
        cylinderVertices.push_back(-0.5);
        cylinderNormals.push_back(cos(theta1));
        cylinderNormals.push_back(sin(theta1));
        cylinderNormals.push_back(0.0);

        cylinderVertices.push_back(0.0);
        cylinderVertices.push_back(0.0);
        cylinderVertices.push_back(0.5);
        cylinderNormals.push_back(0.0);
        cylinderNormals.push_back(0.0);
        cylinderNormals.push_back(1.0);
    }


    float planeVertices[] = {

        -2.0f, 0.0f, -2.0f,    0.0f, 1.0f, 0.0f, -0.5f, -0.5f,
         2.0f, 0.0f,  2.0f,    0.0f, 1.0f, 0.0f,  2.0f,  2.0f,
        -2.0f, 0.0f,  2.0f,    0.0f, 1.0f, 0.0f,  2.0f, -0.5f,
        
         2.0f, 0.0f, -2.0f,    0.0f, 1.0f, 0.0f,  -0.5f,  2.0f,
         2.0f, 0.0f,  2.0f,    0.0f, 1.0f, 0.0f,  -0.5f, -0.5f,
        -2.0f, 0.0f, -2.0f,    0.0f, 1.0f, 0.0f,   2.0f,  2.0f

    };


    float vertices[] = {

        // positions             // normals           // texture coords
        -0.5f, -1.0f, -0.25f,    0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
         0.5f, -1.0f, -0.25f,    0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
         0.5f,  1.0f, -0.25f,    0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
         0.5f,  1.0f, -0.25f,    0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
        -0.5f,  1.0f, -0.25f,    0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
        -0.5f, -1.0f, -0.25f,    0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
                               
        -0.5f, -1.0f,  0.25f,     0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
         0.5f, -1.0f,  0.25f,     0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
         0.5f,  1.0f,  0.25f,     0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
         0.5f,  1.0f,  0.25f,     0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
        -0.5f,  1.0f,  0.25f,     0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
        -0.5f, -1.0f,  0.25f,     0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
                               
        -0.5f,  1.0f,  0.25f,    -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  1.0f, -0.25f,    -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
        -0.5f, -1.0f, -0.25f,    -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -1.0f, -0.25f,    -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -1.0f,  0.25f,    -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  1.0f,  0.25f,    -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
                               
         0.5f,  1.0f,  0.25f,     1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
         0.5f,  1.0f, -0.25f,     1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
         0.5f, -1.0f, -0.25f,     1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -1.0f, -0.25f,     1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -1.0f,  0.25f,     1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
         0.5f,  1.0f,  0.25f,     1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
                               
        -0.5f, -1.0f, -0.25f,     0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -1.0f, -0.25f,     0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
         0.5f, -1.0f,  0.25f,     0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
         0.5f, -1.0f,  0.25f,     0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f, -1.0f,  0.25f,     0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f, -1.0f, -0.25f,     0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
                      
        -0.5f,  1.0f, -0.25f,     0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
         0.5f,  1.0f, -0.25f,     0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
         0.5f,  1.0f,  0.25f,     0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
         0.5f,  1.0f,  0.25f,     0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  1.0f,  0.25f,     0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  1.0f, -0.25f,     0.0f,  1.0f,  0.0f,  0.0f,  1.0f

    };


    // positions all containers
    glm::vec3 cubePositions[] = {
        glm::vec3(0.0f,  0.0f,  0.0f),
    };

    glm::vec3 pointLightColors[] = {

        glm::vec3(1.0f, 1.0f, 1.0f),
        glm::vec3(0.0f, 1.0f, 0.0f),

    };



                                                                                                   

    unsigned int VBOs[3], VAOs[3];                                                                        // VAOs and VBOs for cube objects
    glGenVertexArrays(3, VAOs);                                                                           // ------------------------------
    glGenBuffers(3, VBOs);                                                                                // ------------------------------

    glBindVertexArray(VAOs[0]);                                                                           // ------------------------------

    glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);                                                               // toaster body
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);                          // ------------------------------


    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);                         // cube 1 position attribute
    glEnableVertexAttribArray(0);                                                                         // -------------------------

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));       //  cube 1texture attribute
    glEnableVertexAttribArray(1);                                                                         // ------------------------

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindVertexArray(VAOs[1]);                                                                           // ------------------------------


    glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);                                                               // handle for toaster
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);                          // ------------------------------


    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);                         // cube 2 position attribute
    glEnableVertexAttribArray(0);                                                                         // -------------------------

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));       // cube 2 texture attribute
    glEnableVertexAttribArray(1);                                                                         // ------------------------

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);


    unsigned int planeVBO, planeVAO;                                                                      // VAO and VBO for plane/floor
    glGenVertexArrays(1, &planeVAO);                                                                      // ---------------------
    glGenBuffers(1, &planeVBO);                                                                           // ---------------------

    glBindVertexArray(planeVAO);                                                                          // ---------------------

    glBindBuffer(GL_ARRAY_BUFFER, planeVBO);                                                              // ---------------------
    glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), planeVertices, GL_STATIC_DRAW);                  // ---------------------

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);                         // plane position attribute
    glEnableVertexAttribArray(0);                                                                         // ------------------------

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));       // plane texture attribute
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));       // plane texture attribute
    glEnableVertexAttribArray(2);                                                                         // -----------------------




    unsigned int cylinderVAOs[3], cylinderVBOs[3], cylinderNBOs[3];
    glGenVertexArrays(3, cylinderVAOs);
    glGenBuffers(3, cylinderVBOs);
    glGenBuffers(3, cylinderNBOs);

    glBindVertexArray(cylinderVAOs[0]);

    glBindBuffer(GL_ARRAY_BUFFER, cylinderVBOs[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float)* cylinderVertices.size(),
        cylinderVertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, cylinderNBOs[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float)* cylinderNormals.size(),
        cylinderNormals.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    glBindVertexArray(cylinderVAOs[1]);

    glBindBuffer(GL_ARRAY_BUFFER, cylinderVBOs[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float)* cylinderVertices.size(),
        cylinderVertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, cylinderNBOs[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float)* cylinderNormals.size(),
        cylinderNormals.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    glBindVertexArray(cylinderVAOs[2]);

    glBindBuffer(GL_ARRAY_BUFFER, cylinderVBOs[2]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float)* cylinderVertices.size(),
        cylinderVertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, cylinderNBOs[2]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float)* cylinderNormals.size(),
        cylinderNormals.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);


    glBindBuffer(GL_ARRAY_BUFFER, VBOs[2]);                                                               // stem for soap
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);                            // ------------------------------


    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);                         // cube 3 position attribute
    glEnableVertexAttribArray(0);                                                                         // -------------------------

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));       // cube 3 texture attribute
    glEnableVertexAttribArray(1);                                                                         // ------------------------

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);






    unsigned int lightCubeVAO;
    glGenVertexArrays(1, &lightCubeVAO);
    glBindVertexArray(lightCubeVAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
    // note that we update the lamp's position attribute's stride to reflect the updated buffer data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);


    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // load image, create texture and generate mipmaps
    unsigned int diffuseMap = loadTexture("resources/textures/toaster.png");

    // load image, create texture and generate mipmaps
    unsigned int diffuseMap1 = loadTexture("resources/textures/handle.png");

    // load image, create texture and generate mipmaps
    unsigned int diffuseMap2 = loadTexture("resources/textures/floor.png");

    unsigned int diffuseMap3 = loadTexture("resources/textures/soapBody.png");

    unsigned int diffuseMap4 = loadTexture("resources/textures/soapPump.png");

    unsigned int specularMap = loadTexture("resources/textures/toaster.png");


    // shader configuration
    // --------------------
    lightingShader.use();
    lightingShader.setInt("material.diffuse[0]", 0);
    lightingShader.setInt("material.diffuse[1]", 1);
    lightingShader.setInt("material.diffuse[2]", 2);
    lightingShader.setInt("material.diffuse[3]", 3);
    lightingShader.setInt("material.diffuse[4]", 4);

    lightingShader.setInt("material.specular", 5);
   

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window)) {
        // per-frame time logic
        // --------------------
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // be sure to activate shader when setting uniforms/drawing objects
        lightingShader.use();
        lightingShader.setVec3("light.position", 0.0f, 1.0f, 0.0f);
        lightingShader.setVec3("light.direction", 0.0f, -1.0f, 0.0f);
        lightingShader.setFloat("light.cutOff", glm::cos(glm::radians(45.0f)));
        //lightingShader.setFloat("light.outerCutOff", glm::cos(glm::radians(75.5f)));
        lightingShader.setVec3("viewPos", camera.Position);

        // light properties
        lightingShader.setVec3("light.ambient", 0.1f, 0.1f, 0.1f);
        // we configure the diffuse intensity slightly higher; the right lighting conditions differ with each lighting method and environment.
        // each environment and lighting type requires some tweaking to get the best out of your environment.
        lightingShader.setVec3("light.diffuse", 0.8f, 0.8f, 0.8f);
        lightingShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
        lightingShader.setFloat("light.constant", 1.0f);
        lightingShader.setFloat("light.linear", 0.09f);
        lightingShader.setFloat("light.quadratic", 0.032f);

        // material properties
        lightingShader.setFloat("material.shininess", 32.0f);

        // set projection matrix
        glm::mat4 projection;
        if (isPerspective) {
            projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        }
        else {
            projection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 0.1f, 100.0f);
        }

        glm::mat4 view = camera.GetViewMatrix();
        lightingShader.setMat4("projection", projection);
        lightingShader.setMat4("view", view);

        // world transformation
        glm::mat4 model = glm::mat4(1.0f);
        lightingShader.setMat4("model", model);


        // render first cube toaster bod
        glBindVertexArray(VAOs[0]);
        lightingShader.setInt("sampler_index", 0);
        glActiveTexture(GL_TEXTURE);
        glBindTexture(GL_TEXTURE_2D, diffuseMap);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, specularMap);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f)); // make toaster body smaller
        lightingShader.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // render second cube toaster handle/lever
        glBindVertexArray(VAOs[1]);
        lightingShader.setInt("sampler_index", 1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, diffuseMap1);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-0.35f, 0.0f, 0.0f)); // move toaster handle to be on the side of toaster
        model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::scale(model, glm::vec3(0.25f, 0.25f, 0.25f)); // make handle body smaller
        lightingShader.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);


        // render plane/kitchen floor
        glBindVertexArray(planeVAO);
        lightingShader.setInt("sampler_index", 2);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, diffuseMap2);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, -0.25f, 0.0f));
        model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        lightingShader.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);


        // render capsule cylinder for soap: body
        glBindVertexArray(cylinderVAOs[0]);
        lightingShader.use();
        lightingShader.setInt("sampler_index", 3);
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, diffuseMap3);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(1.0f, 0.0f, 0.0f));
        //model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::rotate(model, glm::radians(90.0f), glm::vec3(-1.0f, 0.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.35f, 0.55f, 0.5f));
        lightingShader.setMat4("model", model);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, SLICES * 6 + 2);
        glBindVertexArray(0);

        // render cylinder for hand soap pump: stem
        glBindVertexArray(cylinderVAOs[1]);
        lightingShader.use();
        lightingShader.setInt("sampler_index", 4);
        glActiveTexture(GL_TEXTURE4);
        glBindTexture(GL_TEXTURE_2D, diffuseMap4);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(1.0f, 0.35f, 0.0f));
        //model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::rotate(model, glm::radians(90.0f), glm::vec3(-1.0f, 0.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.065f, 0.125f, 0.25f));
        lightingShader.setMat4("model", model);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, SLICES * 6 + 2);
        glBindVertexArray(0);        
        
        // render cylinder for hand soap pump: base
        glBindVertexArray(cylinderVAOs[2]);
        lightingShader.use();
        lightingShader.setInt("sampler_index", 4);
        glActiveTexture(GL_TEXTURE4);
        glBindTexture(GL_TEXTURE_2D, diffuseMap4);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(1.0f, 0.25f, 0.0f));
        //model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::rotate(model, glm::radians(90.0f), glm::vec3(-1.0f, 0.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.125f, 0.25f, 0.25f));
        lightingShader.setMat4("model", model);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, SLICES * 6 + 2);
        glBindVertexArray(0);
        





        //render cube for hand soap pump: flat part  CAN'T FIND OUT WHY IT WONT RENDER
        //glBindVertexArray(VAOs[2]);
        //lightingShader.use();
        //lightingShader.setInt("sampler_index", 4);
        //glActiveTexture(GL_TEXTURE4);
        //glBindTexture(GL_TEXTURE_2D, diffuseMap4);
        //model = glm::mat4(1.0f);
        //model = glm::translate(model, glm::vec3(1.0f, 1.0f, 0.0f));
        //model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        //model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
        //lightingShader.setMat4("model", model);
        //glDrawArrays(GL_TRIANGLES, 0, 36);





        //// also draw the lamp object        
        //lightCubeShader.use();
        //lightCubeShader.setMat4("projection", projection);
        //lightCubeShader.setMat4("view", view);
        //model = glm::mat4(1.0f);
        //model = glm::translate(model, lightPos);
        //model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        //model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f)); 
        //lightCubeShader.setMat4("model", model);

        //glBindVertexArray(lightCubeVAO);
       // glDrawArrays(GL_TRIANGLES, 0, 36);



        // swap buffers and poll IO events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // de-allocate resources
    glDeleteVertexArrays(3, VAOs);
    glDeleteBuffers(3, VBOs); 

    glDeleteVertexArrays(1, &planeVAO);
    glDeleteBuffers(1, &planeVBO);

    glDeleteVertexArrays(1, &lightCubeVAO);

    glDeleteVertexArrays(3, cylinderVAOs);
    glDeleteBuffers(3, cylinderVBOs);
    glDeleteBuffers(3, cylinderNBOs);

    // terminate GLFW
    glfwTerminate();

    return 0;

}


// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window) {

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    const float cameraSpeed = static_cast<float>(2.5 * deltaTime); // adjust as you see fit; this is just a base variable

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {

        camera.ProcessKeyboard(FORWARD, deltaTime);

    }

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {

        camera.ProcessKeyboard(BACKWARD, deltaTime);

    }

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {

        camera.ProcessKeyboard(LEFT, deltaTime);

    }

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {

        camera.ProcessKeyboard(RIGHT, deltaTime);

    }

    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {

        camera.ProcessKeyboard(UPWARDS, deltaTime);

    }

    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {

        camera.ProcessKeyboard(DOWNWARDS, deltaTime);

    }

    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) {

        if (!keyPressed) {

            isPerspective = !isPerspective;
            keyPressed = true;

        }

    }
    else {
    
        keyPressed = false;
    
    }

}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {

    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);

}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn) { // helps track mouse to move camera

    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse) {

        lastX = xpos;
        lastY = ypos;
        firstMouse = false;

    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);

}


void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) { // knows when scrolling to zoom

    camera.ProcessMouseScroll(static_cast<float>(yoffset));

}


// utility function for loading a 2D texture from file
// ---------------------------------------------------
unsigned int loadTexture(char const* path)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}