#include <glad/gl.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

// #include "linmath.h"

#include <stdlib.h>
#include <stdio.h>
#include <string>


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <fstream>
#include <sstream>
#include <iostream>

struct ViewUniforms {
	glm::mat4 view;
	glm::mat4 proj;
	glm::vec3 pos;
	float padding;  // Padding to align with vec4
};
#define CORNFLOWER_BLUE 100 / 255.f, 149 / 255.f, 237 / 255.f, 1


// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;


// Camera
glm::vec3 cameraPos = glm::vec3(0.0f, 1.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
bool keys[1024];

// Delta Time
GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

static void error_callback(int error, const char* description)
{
	fprintf(stderr, "Error: %s\n", description);
}

// Moves/alters the camera positions based on user input
void do_movement() {
    // Camera controls
    GLfloat cameraSpeed = 5.0f * deltaTime;
    if (keys[GLFW_KEY_W])
        cameraPos += cameraSpeed * cameraFront;
    if (keys[GLFW_KEY_S])
        cameraPos -= cameraSpeed * cameraFront;
    if (keys[GLFW_KEY_A])
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (keys[GLFW_KEY_D])
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
}

// Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
    if (action == GLFW_PRESS)
        keys[key] = true;
    else if (action == GLFW_RELEASE)
        keys[key] = false;
}

GLuint compileShader(GLenum type, const char *source)
{
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);

    GLint compiled;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
    if (!compiled)
		{
        GLint infoLen = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
        if (infoLen > 1)
				{
            char *infoLog = new char[infoLen];
            glGetShaderInfoLog(shader, infoLen, nullptr, infoLog);
            std::cerr << "Error compiling shader:\n" << infoLog << std::endl;
            delete[] infoLog;
        }
        glDeleteShader(shader);
        return 0;
    }

    return shader;
}

GLuint createProgram(const char *vert, const char *frag)
{
	std::string vertexCode;
	std::string fragmentCode;
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;
	// ensure ifstream objects can throw exceptions:
	vShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
	try 
        {
            // open files
            vShaderFile.open(vert);
            fShaderFile.open(frag);
            std::stringstream vShaderStream, fShaderStream;
            // read file's buffer contents into streams
            vShaderStream << vShaderFile.rdbuf();
            fShaderStream << fShaderFile.rdbuf();		
            // close file handlers
            vShaderFile.close();
            fShaderFile.close();
            // convert stream into string
			vertexCode = vShaderStream.str();
			fragmentCode = fShaderStream.str();
        }
        catch (std::ifstream::failure& e)
        {
            std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << e.what() << std::endl;
        }
    GLuint vertexShader = compileShader(GL_VERTEX_SHADER, vertexCode.c_str());
    GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentCode.c_str());

    GLuint program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    GLint linked;
    glGetProgramiv(program, GL_LINK_STATUS, &linked);
    if (!linked) {
        GLint infoLen = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLen);
        if (infoLen > 1)
				{
            char *infoLog = new char[infoLen];
            glGetProgramInfoLog(program, infoLen, nullptr, infoLog);
            std::cerr << "Error linking program:\n" << infoLog << std::endl;
            delete[] infoLog;
        }
        glDeleteProgram(program);
        return 0;
    }

    return program;
}



int main(void)
{
	GLFWwindow* window;
	GLuint vertex_buffer, vertex_shader, fragment_shader, program;
	GLint mvp_location, vpos_location, vcol_location;

	glfwSetErrorCallback(error_callback);

	if (!glfwInit())
		exit(EXIT_FAILURE);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

	window = glfwCreateWindow(WIDTH, HEIGHT, "Simple example", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwSetKeyCallback(window, key_callback);

	glfwMakeContextCurrent(window);
	gladLoadGL(glfwGetProcAddress);
	glfwSwapInterval(1);

	// NOTE: OpenGL error checks have been omitted for brevity

    // Define the viewport dimensions
    glViewport(0, 0, WIDTH, HEIGHT);



	// OpenGL options
    glEnable(GL_DEPTH_TEST);

    // Set up vertex data (and buffer(s)) and attribute pointers
GLfloat planeVertices[] = {
    // Positions
    10000.0f,  0.0f,  10000.0f,  // Vertex 0
   -10000.0f,  0.0f,  10000.0f,  // Vertex 1
   -10000.0f,  0.0f, -10000.0f,  // Vertex 2
    10000.0f,  0.0f, -10000.0f   // Vertex 3
};
GLuint planeIndices[] = {
    0, 1, 2,  // First triangle
    0, 2, 3   // Second triangle
};

	float vertices[]{
    -1.0f, -1.0f,
    -1.0f,  1.0f,
     1.0f,  1.0f,
     1.0f, -1.0f,
	};
	uint32_t cw_indices[]{
		0, 1, 2,
		0, 2, 3,
	};
	uint32_t ccw_indices[]{
		0, 2, 1,
		0, 3, 2,
	};

    GLuint planeVAO, planeVBO;
    glGenVertexArrays(1, &planeVAO);
    glBindVertexArray(planeVAO);
	
    glGenBuffers(1, &planeVBO);

    glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

	 // Generate and bind index buffer
    GLuint ebo;
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(ccw_indices), ccw_indices, GL_STATIC_DRAW); 
    // glBindVertexArray(0);
    
	program = createProgram("../shaders/plane.vert", "../shaders/plane.frag");
	GLint viewLoc = glGetUniformLocation(program, "view");
	GLint projLoc = glGetUniformLocation(program, "projection");
	GLint nearFarLoc = glGetUniformLocation(program, "u_nearfar");
	
	
	float near = 0.1f;
    float far  = 1000.0f;
	auto projection = glm::perspective(glm::radians(45.0f), (float)WIDTH / (float)HEIGHT, near, far);
	auto near_far = glm::vec2(near, far);
	

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);

	while (!glfwWindowShouldClose(window))
	{
 // Calculate deltatime of current frame
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Check if any events have been activated (key pressed, mouse moved, etc.) and call corresponding response functions
        glfwPollEvents();
        do_movement();

        // Clear the colorbuffer
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // glClearColor(CORNFLOWER_BLUE);
        glClearColor(0.f, 0.f, 0.f, 1.0f);


        // Render
    	glUseProgram(program);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		
        // Draw the plane
        glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        // Set shader uniforms (assuming you have a shader program set up))
        glUniform1fv(nearFarLoc, 2, glm::value_ptr(near_far));
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);

	glfwTerminate();
	exit(EXIT_SUCCESS);
}


