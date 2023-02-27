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


float vertices1[] = {

    -0.25f, -0.25f, -0.10f,  0.0f, 0.0f,
     0.25f, -0.25f, -0.10f,  1.0f, 0.0f,
     0.25f,  0.25f, -0.10f,  1.0f, 1.0f,
     0.25f,  0.25f, -0.10f,  1.0f, 1.0f,
    -0.25f,  0.25f, -0.10f,  0.0f, 1.0f,
    -0.25f, -0.25f, -0.10f,  0.0f, 0.0f,

    -0.25f, -0.25f,  0.10f,  0.0f, 0.0f,
     0.25f, -0.25f,  0.10f,  1.0f, 0.0f,
     0.25f,  0.25f,  0.10f,  1.0f, 1.0f,
     0.25f,  0.25f,  0.10f,  1.0f, 1.0f,
    -0.25f,  0.25f,  0.10f,  0.0f, 1.0f,
    -0.25f, -0.25f,  0.10f,  0.0f, 0.0f,

    -0.25f,  0.25f,  0.10f,  1.0f, 0.0f,
    -0.25f,  0.25f, -0.10f,  1.0f, 1.0f,
    -0.25f, -0.25f, -0.10f,  0.0f, 1.0f,
    -0.25f, -0.25f, -0.10f,  0.0f, 1.0f,
    -0.25f, -0.25f,  0.10f,  0.0f, 0.0f,
    -0.25f,  0.25f,  0.10f,  1.0f, 0.0f,

     0.25f,  0.25f,  0.10f,  1.0f, 0.0f,
     0.25f,  0.25f, -0.10f,  1.0f, 1.0f,
     0.25f, -0.25f, -0.10f,  0.0f, 1.0f,
     0.25f, -0.25f, -0.10f,  0.0f, 1.0f,
     0.25f, -0.25f,  0.10f,  0.0f, 0.0f,
     0.25f,  0.25f,  0.10f,  1.0f, 0.0f,

    -0.25f, -0.25f, -0.10f,  0.0f, 1.0f,
     0.25f, -0.25f, -0.10f,  1.0f, 1.0f,
     0.25f, -0.25f,  0.10f,  1.0f, 0.0f,
     0.25f, -0.25f,  0.10f,  1.0f, 0.0f,
    -0.25f, -0.25f,  0.10f,  0.0f, 0.0f,
    -0.25f, -0.25f, -0.10f,  0.0f, 1.0f,

    -0.25f,  0.25f, -0.10f,  0.0f, 1.0f,
     0.25f,  0.25f, -0.10f,  1.0f, 1.0f,
     0.25f,  0.25f,  0.10f,  1.0f, 0.0f,
     0.25f,  0.25f,  0.10f,  1.0f, 0.0f,
    -0.25f,  0.25f,  0.10f,  0.0f, 0.0f,
    -0.25f,  0.25f, -0.10f,  0.0f, 1.0f

};

// world space positions of our cubes
glm::vec3 cubePositions[] = {

    glm::vec3(0.0f, 0.0f,  0.0f),  // Body

    glm::vec3(-0.95f, 0.0f,  0.0f), // Handle

    glm::vec3(0.0f, 0.0f, 0.0f)  // Plane

    //glm::vec3(-3.8f, -2.0f, -12.3f),
    //glm::vec3( 2.4f, -0.4f, -3.5f),
    //glm::vec3(-1.7f,  3.0f, -7.5f),
    //glm::vec3( 1.3f, -2.0f, -2.5f),
    //glm::vec3( 1.5f,  2.0f, -2.5f),
    //glm::vec3( 1.5f,  0.2f, -1.5f),
    //glm::vec3(-1.3f,  1.0f, -1.5f)

};

unsigned int VBO, VAO;
glGenVertexArrays(1, &VAO);
glGenBuffers(1, &VBO);

glBindVertexArray(VAO);

glBindBuffer(GL_ARRAY_BUFFER, VBO);
glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);



glBindVertexArray(VAO);
glActiveTexture(GL_TEXTURE0);
//glBindTexture(GL_TEXTURE_2D, texture1);
glm::mat4 model = glm::mat4(1.0f);
model = glm::translate(model, cubePositions[0]);
model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
ourShader.setMat4("model", model);
glDrawArrays(GL_TRIANGLES, 0, 36);