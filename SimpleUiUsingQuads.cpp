#include <glad/gl.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.cpp"
#include "UploadImage.cpp"

#include <iostream>

int screenWidth = 2560/2, screenHeight = 1440/2;

GLuint quadVAO = 0;
GLuint quadVBO;
GLuint shaderProgram;
GLuint texture;


// Vertex shader
const char* vertexShaderSource = R"VERTEX(

#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoords;

out vec2 TexCoords;

uniform mat4 uModel;
uniform mat4 uProjection;

void main()
{
    gl_Position = uProjection * uModel * vec4(aPos, 0.0, 1.0);
    TexCoords = aTexCoords;
}

)VERTEX";

// Fragment shader
const char* fragmentShaderSource = R"FRAGMENT(

#version 330 core
in vec2 TexCoords;
out vec4 FragColor;

uniform sampler2D textureSampler;

void main()
{
    FragColor = texture(textureSampler, TexCoords);
    // FragColor = vec4(1,1,1,1);
}

)FRAGMENT";



void initQuad() {
    float quadVertices[] = {
        // positions   // texture coords
         0.0f,  1.0f,  0.0f, 1.0f,
         1.0f,  1.0f,  1.0f, 1.0f,
         0.0f,  0.0f,  0.0f, 0.0f,
         1.0f,  0.0f,  1.0f, 0.0f
    };

    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &quadVBO);
    glBindVertexArray(quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Compile and link shaders here to create shaderProgram
    // ...
    shaderProgram = create_shader_program(vertexShaderSource, fragmentShaderSource);

    // Load texture
    texture = UploadImage("../res/textures/shoot.png");
}

void drawUIQuad(float x, float y, float width, float height)
{
    glUseProgram(shaderProgram);

    glm::mat4 uProjection = glm::ortho(0.0f, (float)screenWidth, (float)screenHeight, 0.0f, -1.0f, 1.0f);
    glm::mat4 uModel = glm::mat4(1.0f);
    uModel = glm::translate(uModel, glm::vec3(x, y, 0.0f));
    uModel = glm::scale(uModel, glm::vec3(width, height, 1.0f));

    GLint modelLoc = glGetUniformLocation(shaderProgram, "uModel");
    GLint projectionLoc = glGetUniformLocation(shaderProgram, "uProjection");

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(uModel));
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(uProjection));

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glUniform1i(glGetUniformLocation(shaderProgram, "textureSampler"), 0);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);

    glDisable(GL_BLEND);
    glUseProgram(0);
}




GLFWwindow* window;


int main(void)
{	
 
	if (!glfwInit())
    {
        std::cout << "EXIT_FAILURE" << '\n';
        exit(EXIT_FAILURE);
    }
 
	window = glfwCreateWindow(screenWidth, screenHeight, "Simple MeshGeom Example", NULL, NULL);
	if (!window)
	{
        std::cout << "No Window" << '\n';
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
 
 
	glfwMakeContextCurrent(window);
	gladLoadGL(glfwGetProcAddress);
	glfwSwapInterval(1);
 

    // set ogl states and defaults
    glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glViewport(0, 0, screenWidth, screenHeight);

    initQuad();
    do
    {
        
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        drawUIQuad(0,0, 100,100);
            
        // Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    } while (glfwWindowShouldClose(window) == 0);
 
    return 0;
}