#pragma once

#include <glad/gl.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include "linmath.h"

#include <cmath>
#include <vector>
#include <string>
#include <sstream>
#include <iostream>

#define M_PI 3.141592653589793238462

#define CHECK_GL { GLenum glStatus = glGetError(); if( glStatus != GL_NO_ERROR ) { std::cout << "File: " << __FILE__ << "(" << __LINE__ << ") " << "OpenGL error: " << openglGetErrorString( glStatus ); } }
static std::string openglGetErrorString(GLenum status)
{
	std::stringstream ss;

	switch (status)
	{
	case GL_INVALID_ENUM:
		ss << "GL_INVALID_ENUM";
		break;
	case GL_INVALID_VALUE:
		ss << "GL_INVALID_VALUE";
		break;
	case GL_INVALID_OPERATION:
		ss << "GL_INVALID_OPERATION";
		break;
	case GL_OUT_OF_MEMORY:
		ss << "GL_OUT_OF_MEMORY";
		break;
	default:
		ss << "GL_UNKNOWN_ERROR" << " - " << status;
		break;
	}

	return ss.str();
}

class GLMeshData
{
public:
	GLMeshData();
	~GLMeshData();

	void createBox(float w, float h, float l);
	void createPlane(float base, float size, float uvScale = 1.0f);
	void createSphere(float rad, uint32_t hSegs, uint32_t vSegs);
    void createCone(float radius, float height, uint32_t segments);
    void createCylinder(float radius, float height, uint32_t segments);
    void createTrapezoid(float baseWidth, float topWidth, float height, float depth);
    void createRectangle(float width, float height);
    void createCircle(float radius, uint32_t segments);

	void render();
	void clear();

protected:
	void createGLObjects();

	GLenum primitiveType;
	unsigned int numVertices;
	unsigned int numPrimitives;

	GLuint meshVAID;
	GLuint meshIBID;
	GLuint meshVBID_pos;
	GLuint meshVBID_uv;
	
	std::vector<GLuint> indexData;
	std::vector<GLfloat> posData;
	std::vector<GLfloat> uvData;
};


GLMeshData::GLMeshData()
{
	meshVAID = meshVBID_pos = meshVBID_uv = meshIBID = 0;

	numVertices = numPrimitives = 0;

	primitiveType = GL_TRIANGLES;
}

GLMeshData::~GLMeshData()
{
	clear();
}

void GLMeshData::clear()
{
	if (meshVBID_pos)
	{
		glDeleteBuffers(1, &meshVBID_pos);
		meshVBID_pos = 0;
	}

	if (meshVBID_uv)
	{
		glDeleteBuffers(1, &meshVBID_uv);
		meshVBID_uv = 0;
	}

	if (meshIBID)
	{
		glDeleteBuffers(1, &meshIBID);
		meshIBID = 0;
	}

	if (meshVAID)
	{
		glDeleteVertexArrays(1, &meshVAID);
		meshVAID = 0;
	}
}

void GLMeshData::createPlane(float base, float size, float uvScale)
{
	numPrimitives = 2;

	posData = {
		-size,base,-size,
		 size,base,-size,
		 size,base, size,
		-size,base, size,
	};

	uvData = {
		0.0f*uvScale, 0.0f*uvScale,
		1.0f*uvScale, 0.0f*uvScale,
		1.0f*uvScale, 1.0f*uvScale,
		0.0f*uvScale, 1.0f*uvScale,
	};

	indexData = {
		0, 1, 2,
		2, 3, 0
	};

	createGLObjects();
}

void GLMeshData::createBox(float w, float h, float l)
{
	numPrimitives = 6*2;

	w *= 0.5f;
	h *= 0.5f;
	l *= 0.5f;

	// bottom face
	posData.insert(posData.end(), { w, -h,  l}); uvData.insert(uvData.end(), {0.0f, 0.0f});
	posData.insert(posData.end(), {-w, -h,  l}); uvData.insert(uvData.end(), {1.0f, 0.0f});
	posData.insert(posData.end(), {-w, -h, -l}); uvData.insert(uvData.end(), {1.0f, 1.0f});
	posData.insert(posData.end(), { w, -h, -l}); uvData.insert(uvData.end(), {0.0f, 1.0f});

	// top face
	posData.insert(posData.end(), {-w,  h,  l}); uvData.insert(uvData.end(), {0.0f, 0.0f});
	posData.insert(posData.end(), { w,  h,  l}); uvData.insert(uvData.end(), {1.0f, 0.0f});
	posData.insert(posData.end(), { w,  h, -l}); uvData.insert(uvData.end(), {1.0f, 1.0f});
	posData.insert(posData.end(), {-w,  h, -l}); uvData.insert(uvData.end(), {0.0f, 1.0f});

	// left face
	posData.insert(posData.end(), {-w, -h, -l}); uvData.insert(uvData.end(), {0.0f, 0.0f});
	posData.insert(posData.end(), {-w, -h,  l}); uvData.insert(uvData.end(), {1.0f, 0.0f});
	posData.insert(posData.end(), {-w,  h,  l}); uvData.insert(uvData.end(), {1.0f, 1.0f});
	posData.insert(posData.end(), {-w,  h, -l}); uvData.insert(uvData.end(), {0.0f, 1.0f});

	// right face
	posData.insert(posData.end(), { w, -h,  l}); uvData.insert(uvData.end(), {0.0f, 0.0f});
	posData.insert(posData.end(), { w, -h, -l}); uvData.insert(uvData.end(), {1.0f, 0.0f});
	posData.insert(posData.end(), { w,  h, -l}); uvData.insert(uvData.end(), {1.0f, 1.0f});
	posData.insert(posData.end(), { w,  h,  l}); uvData.insert(uvData.end(), {0.0f, 1.0f});

	// front face
	posData.insert(posData.end(), {-w, -h,  l}); uvData.insert(uvData.end(), {0.0f, 0.0f});
	posData.insert(posData.end(), { w, -h,  l}); uvData.insert(uvData.end(), {1.0f, 0.0f});
	posData.insert(posData.end(), { w,  h,  l}); uvData.insert(uvData.end(), {1.0f, 1.0f});
	posData.insert(posData.end(), {-w,  h,  l}); uvData.insert(uvData.end(), {0.0f, 1.0f});

	// back face
	posData.insert(posData.end(), { w, -h, -l}); uvData.insert(uvData.end(), {0.0f, 0.0f});
	posData.insert(posData.end(), {-w, -h, -l}); uvData.insert(uvData.end(), {1.0f, 0.0f});
	posData.insert(posData.end(), {-w,  h, -l}); uvData.insert(uvData.end(), {1.0f, 1.0f});
	posData.insert(posData.end(), { w,  h, -l}); uvData.insert(uvData.end(), {0.0f, 1.0f});

	std::vector< unsigned int > tri00;
	std::vector< unsigned int > tri01;
	tri00.push_back(0); tri00.push_back(1); tri00.push_back(2);
	tri01.push_back(0); tri01.push_back(2); tri01.push_back(3);

	std::vector< unsigned int > tri02;
	std::vector< unsigned int > tri03;
	tri02.push_back(4); tri02.push_back(5); tri02.push_back(6);
	tri03.push_back(4); tri03.push_back(6); tri03.push_back(7);

	std::vector< unsigned int > tri04;
	std::vector< unsigned int > tri05;
	tri04.push_back(8); tri04.push_back(9); tri04.push_back(10);
	tri05.push_back(8); tri05.push_back(10); tri05.push_back(11);

	std::vector< unsigned int > tri06;
	std::vector< unsigned int > tri07;
	tri06.push_back(12); tri06.push_back(13); tri06.push_back(14);
	tri07.push_back(12); tri07.push_back(14); tri07.push_back(15);

	std::vector< unsigned int > tri08;
	std::vector< unsigned int > tri09;
	tri08.push_back(16); tri08.push_back(17); tri08.push_back(18);
	tri09.push_back(16); tri09.push_back(18); tri09.push_back(19);

	std::vector< unsigned int > tri10;
	std::vector< unsigned int > tri11;
	tri10.push_back(20); tri10.push_back(21); tri10.push_back(22);
	tri11.push_back(20); tri11.push_back(22); tri11.push_back(23);

	indexData.insert(indexData.end(), tri00.begin(), tri00.end());
	indexData.insert(indexData.end(), tri01.begin(), tri01.end());
	indexData.insert(indexData.end(), tri02.begin(), tri02.end());
	indexData.insert(indexData.end(), tri03.begin(), tri03.end());
	indexData.insert(indexData.end(), tri04.begin(), tri04.end());
	indexData.insert(indexData.end(), tri05.begin(), tri05.end());
	indexData.insert(indexData.end(), tri06.begin(), tri06.end());
	indexData.insert(indexData.end(), tri07.begin(), tri07.end());
	indexData.insert(indexData.end(), tri08.begin(), tri08.end());
	indexData.insert(indexData.end(), tri09.begin(), tri09.end());
	indexData.insert(indexData.end(), tri10.begin(), tri10.end());
	indexData.insert(indexData.end(), tri11.begin(), tri11.end());

	createGLObjects();
}

void GLMeshData::createSphere(float rad, uint32_t hSegs, uint32_t vSegs)
{
	numPrimitives = hSegs * vSegs * 2;

	float dphi = (float)(2.0*M_PI) / (float)(hSegs);
	float dtheta = (float)(M_PI) / (float)(vSegs);

	for (uint32_t v = 0; v <= vSegs; ++v)
	{
		float theta = v * dtheta;

		for (uint32_t h = 0; h <= hSegs; ++h)
		{
			float phi = h * dphi;

			float x = std::sin(theta) * std::cos(phi);
			float y = std::cos(theta);
			float z = std::sin(theta) * std::sin(phi);

			posData.insert(posData.end(), { rad * x, rad * y, rad * z });
			uvData.insert(uvData.end(), { 1.0f - (float)h / hSegs, (float)v / vSegs });
		}
	}

	for (uint32_t v = 0; v < vSegs; v++)
	{
		for (uint32_t h = 0; h < hSegs; h++)
		{
			uint32_t topRight = v * (hSegs + 1) + h;
			uint32_t topLeft = v * (hSegs + 1) + h + 1;
			uint32_t lowerRight = (v + 1) * (hSegs + 1) + h;
			uint32_t lowerLeft = (v + 1) * (hSegs + 1) + h + 1;

			std::vector< unsigned int > tri0;
			std::vector< unsigned int > tri1;

			tri0.push_back(lowerLeft);
			tri0.push_back(lowerRight);
			tri0.push_back(topRight);

			tri1.push_back(lowerLeft);
			tri1.push_back(topRight);
			tri1.push_back(topLeft);

			indexData.insert(indexData.end(), tri0.begin(), tri0.end());
			indexData.insert(indexData.end(), tri1.begin(), tri1.end());
		}
	}

	createGLObjects();
}

void GLMeshData::createCone(float radius, float height, uint32_t segments)
{
    numPrimitives = segments * 2;

    // Create the base circle
    for (uint32_t i = 0; i <= segments; ++i)
    {
        float angle = 2.0f * M_PI * i / segments;
        float x = radius * std::cos(angle);
        float z = radius * std::sin(angle);
        
        posData.insert(posData.end(), {x, 0, z});
        uvData.insert(uvData.end(), {(float)i / segments, 0});
    }

    // Add the apex
    posData.insert(posData.end(), {0, height, 0});
    uvData.insert(uvData.end(), {0.5f, 1.0f});

    // Create the base triangles
    for (uint32_t i = 0; i < segments; ++i)
    {
        indexData.insert(indexData.end(), {0, i + 1, i + 2});
    }

    // Create the side triangles
    uint32_t apexIndex = segments + 1;
    for (uint32_t i = 1; i <= segments; ++i)
    {
        indexData.insert(indexData.end(), {i, apexIndex, i + 1});
    }

    createGLObjects();
}

void GLMeshData::createCylinder(float radius, float height, uint32_t segments)
{
    numPrimitives = segments * 4;

    // Create the bottom circle
    for (uint32_t i = 0; i <= segments; ++i)
    {
        float angle = 2.0f * M_PI * i / segments;
        float x = radius * std::cos(angle);
        float z = radius * std::sin(angle);
        
        posData.insert(posData.end(), {x, 0, z});
        uvData.insert(uvData.end(), {(float)i / segments, 0});
    }

    // Create the top circle
    for (uint32_t i = 0; i <= segments; ++i)
    {
        float angle = 2.0f * M_PI * i / segments;
        float x = radius * std::cos(angle);
        float z = radius * std::sin(angle);
        
        posData.insert(posData.end(), {x, height, z});
        uvData.insert(uvData.end(), {(float)i / segments, 1});
    }

    // Create the bottom triangles
    for (uint32_t i = 0; i < segments; ++i)
    {
        indexData.insert(indexData.end(), {0, i + 1, i + 2});
    }

    // Create the top triangles
    uint32_t topStart = segments + 1;
    for (uint32_t i = 0; i < segments; ++i)
    {
        indexData.insert(indexData.end(), {topStart, topStart + i + 1, topStart + i + 2});
    }

    // Create the side triangles
    for (uint32_t i = 0; i < segments; ++i)
    {
        uint32_t bottomLeft = i + 1;
        uint32_t bottomRight = i + 2;
        uint32_t topLeft = topStart + i + 1;
        uint32_t topRight = topStart + i + 2;

        indexData.insert(indexData.end(), {bottomLeft, topLeft, bottomRight});
        indexData.insert(indexData.end(), {bottomRight, topLeft, topRight});
    }
    createGLObjects();
}


void GLMeshData::createTrapezoid(float baseWidth, float topWidth, float height, float depth)
{
    numPrimitives = 12;  // 6 faces, 2 triangles each

    float baseHalfWidth = baseWidth / 2.0f;
    float topHalfWidth = topWidth / 2.0f;
    float halfDepth = depth / 2.0f;

    // Bottom face
    posData.insert(posData.end(), {-baseHalfWidth, 0, -halfDepth});
    posData.insert(posData.end(), { baseHalfWidth, 0, -halfDepth});
    posData.insert(posData.end(), { baseHalfWidth, 0,  halfDepth});
    posData.insert(posData.end(), {-baseHalfWidth, 0,  halfDepth});

    // Top face
    posData.insert(posData.end(), {-topHalfWidth, height, -halfDepth});
    posData.insert(posData.end(), { topHalfWidth, height, -halfDepth});
    posData.insert(posData.end(), { topHalfWidth, height,  halfDepth});
    posData.insert(posData.end(), {-topHalfWidth, height,  halfDepth});

    // UV coordinates (simplified)
    for (int i = 0; i < 8; ++i)
    {
        uvData.insert(uvData.end(), {(i % 2) * 1.0f, (i / 2) % 2 * 1.0f});
    }

    // Indices for all faces
    std::vector<uint32_t> faceIndices = {
        0, 1, 2, 2, 3, 0,  // Bottom
        4, 5, 6, 6, 7, 4,  // Top
        0, 4, 7, 7, 3, 0,  // Left
        1, 5, 6, 6, 2, 1,  // Right
        0, 1, 5, 5, 4, 0,  // Front
        3, 2, 6, 6, 7, 3   // Back
    };

    indexData.insert(indexData.end(), faceIndices.begin(), faceIndices.end());

    createGLObjects();
}


void GLMeshData::createCircle(float radius, uint32_t segments)
{
    numPrimitives = segments;

    // Center vertex
    posData.insert(posData.end(), {0.0f, 0.0f, 0.0f});
    uvData.insert(uvData.end(), {0.5f, 0.5f});

    // Create the circle vertices
    for (uint32_t i = 0; i <= segments; ++i)
    {
        float angle = 2.0f * M_PI * i / segments;
        float x = radius * std::cos(angle);
        float y = radius * std::sin(angle);
        
        posData.insert(posData.end(), {x, y, 0.0f});
        
        // UV coordinates
        float u = (std::cos(angle) + 1.0f) * 0.5f;
        float v = (std::sin(angle) + 1.0f) * 0.5f;
        uvData.insert(uvData.end(), {u, v});
    }

    // Create the triangles
    for (uint32_t i = 1; i <= segments; ++i)
    {
        indexData.insert(indexData.end(), {0, i, i + 1});
    }

    createGLObjects();
}

void GLMeshData::createRectangle(float width, float height)
{
    numPrimitives = 2;

    float halfWidth = width * 0.5f;
    float halfHeight = height * 0.5f;
    

    // Vertices
    posData = {
        -halfWidth, -halfHeight, 0.0f,
         halfWidth, -halfHeight, 0.0f,
         halfWidth,  halfHeight, 0.0f,
        -halfWidth,  halfHeight, 0.0f
    };

    // UV coordinates
    uvData = {
        0.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f
    };

    // Indices
    indexData = {
        0, 1, 2,
        2, 3, 0
    };

    createGLObjects();
}

void GLMeshData::createGLObjects()
{
	// create vertex buffer objects for pos, uv
	glGenBuffers(1, &meshVBID_pos);
	glBindBuffer(GL_ARRAY_BUFFER, meshVBID_pos);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * posData.size(), posData.data(), GL_STATIC_DRAW);
	CHECK_GL;

	glGenBuffers(1, &meshVBID_uv);
	glBindBuffer(GL_ARRAY_BUFFER, meshVBID_uv);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * uvData.size(), uvData.data(), GL_STATIC_DRAW);
	CHECK_GL;

	// create index buffer object
	glGenBuffers(1, &meshIBID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshIBID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * indexData.size(), indexData.data(), GL_STATIC_DRAW);
	CHECK_GL;

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	CHECK_GL;

	// create vertex array
	glGenVertexArrays(1, &meshVAID);
	glBindVertexArray(meshVAID);
	CHECK_GL;

	GLuint loc_pos = 0;
	GLuint los_uv = 1;

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshIBID);

	glBindBuffer(GL_ARRAY_BUFFER, meshVBID_pos);
	glVertexAttribPointer(loc_pos, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glBindBuffer(GL_ARRAY_BUFFER, meshVBID_uv);
	glVertexAttribPointer(los_uv, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
	CHECK_GL;

	glEnableVertexAttribArray(loc_pos);
	glEnableVertexAttribArray(los_uv);
	CHECK_GL;

	glBindVertexArray(0);

	// glDisableVertexAttribArray(loc_pos);
	// glDisableVertexAttribArray(los_uv);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	CHECK_GL;
}

void GLMeshData::render()
{
	glBindVertexArray(meshVAID);
	CHECK_GL;

	glDrawElements(primitiveType, 3 * numPrimitives, GL_UNSIGNED_INT, (void*)0);
	CHECK_GL;

	glBindVertexArray(0);
	CHECK_GL;
}


#define STB_IMAGE_IMPLEMENTATION

#include <stb/stb_image.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace glm;

int g_width  = 2560/2;
int g_height = 1440/2;

// ogl attributes and variables
GLuint texture_crate   = -1;
GLuint texture_checker = -1;

const int num_ball_textures = 15;
GLuint    texIds[num_ball_textures]; // 15 ball

// camera transformation attributes
glm::mat4 g_view_matrix;
glm::mat4 g_proj_matrix;

glm::vec3 g_cam_position = glm::vec3(50, 20, 50);

float g_cam_horizontal_angle = -(3.0f / 4.0f) * float(M_PI);
float g_cam_vertical_angle   = 0.0f;
float g_cam_fov              = (1.0f / 4.0f) * float(M_PI);

// camera movement/turning attributes
float g_cam_move_speed = 10.0f;
float g_cam_turn_speed = 0.00075f;

float g_last_cursorpos_x = 0.0f;
float g_last_cursorpos_y = 0.0f;


// Vertex shader
const char* vertexShaderSource = R"VERTEX(

#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec2 vertexUV;

// Output data ; will be interpolated for each fragment.
out vec2 UV;

// Values that stay constant for the whole mesh.
uniform mat4 MVP;

void main(){

	// Output position of the vertex, in clip space : MVP * position
	gl_Position =  MVP * vec4(vertexPosition_modelspace,1);
	
	// UV of the vertex. No special space for this one.
	UV = vertexUV;
}


)VERTEX";

// Fragment shader
const char* fragmentShaderSource = R"FRAGMENT(

#version 330 core

// Interpolated values from the vertex shaders
in vec2 UV;

// Ouput data
out vec3 color;

// Values that stay constant for the whole mesh.
uniform sampler2D myTextureSampler;

void main(){

	// Output color = color of the texture at the specified UV
	color = texture( myTextureSampler, UV ).rgb;
}

)FRAGMENT";

GLenum glCheckError_(const char *file, int line)
{
    GLenum errorCode;
    while ((errorCode = glGetError()) != GL_NO_ERROR)
    {
        std::string error;
        switch (errorCode)
        {
            case GL_INVALID_ENUM:                  error = "INVALID_ENUM"; break;
            case GL_INVALID_VALUE:                 error = "INVALID_VALUE"; break;
            case GL_INVALID_OPERATION:             error = "INVALID_OPERATION"; break;
            case GL_STACK_OVERFLOW:                error = "STACK_OVERFLOW"; break;
            case GL_STACK_UNDERFLOW:               error = "STACK_UNDERFLOW"; break;
            case GL_OUT_OF_MEMORY:                 error = "OUT_OF_MEMORY"; break;
            case GL_INVALID_FRAMEBUFFER_OPERATION: error = "INVALID_FRAMEBUFFER_OPERATION"; break;
        }
        std::cout << error << " | " << file << " (" << line << ")" << std::endl;
    }
    return errorCode;
}
#define glCheckError() glCheckError_(__FILE__, __LINE__) 

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
GLuint create_shader_program(const char* vertexSource, const char* fragmentSource)
{
	GLuint vertex_buffer, vertex_shader, fragment_shader, program;
 
	vertex_shader = compileShader(GL_VERTEX_SHADER, vertexSource);
	fragment_shader = compileShader(GL_FRAGMENT_SHADER, fragmentSource);
 
	program = glCreateProgram();
	glAttachShader(program, vertex_shader);
	glAttachShader(program, fragment_shader);
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
    return(program);
}
GLFWwindow* window;
 
void computeMatricesFromInputs()
{

    // glfwGetTime is called only once, the first time this function is called
    static double lastTime = glfwGetTime();

    // Compute time difference between current and last frame
    double currentTime = glfwGetTime();
    float  deltaTime   = float(currentTime - lastTime);

    // Get mouse position
    double x, y;
    glfwGetCursorPos(window, &x, &y);

    float dx = float(x) - g_last_cursorpos_x;
    float dy = float(y) - g_last_cursorpos_y;

    g_last_cursorpos_x = float(x);
    g_last_cursorpos_y = float(y);

    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
    {
        // Compute new orientation
        g_cam_horizontal_angle -= g_cam_turn_speed * dx;
        g_cam_vertical_angle -= g_cam_turn_speed * dy;
    }

    // Direction : Spherical coordinates to Cartesian coordinates conversion
    glm::vec3 direction(
    std::cos(g_cam_vertical_angle) * std::sin(g_cam_horizontal_angle),
    std::sin(g_cam_vertical_angle),
    std::cos(g_cam_vertical_angle) * std::cos(g_cam_horizontal_angle)
    );

    glm::vec3 right = glm::vec3(
    std::sin(g_cam_horizontal_angle - 3.14f / 2.0f),
    0,
    std::cos(g_cam_horizontal_angle - 3.14f / 2.0f)
    );

    glm::vec3 up = glm::cross(right, direction);

    float l_speed = g_cam_move_speed;
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
    {
        l_speed *= 10.0f;
    }

    // Move forward
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        g_cam_position += direction * deltaTime * l_speed;
    }
    // Move backward
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        g_cam_position -= direction * deltaTime * l_speed;
    }
    // Strafe right
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        g_cam_position += right * deltaTime * l_speed;
    }
    // Strafe left
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        g_cam_position -= right * deltaTime * l_speed;
    }

    // Y DOWN
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
    {
        g_cam_position -= up * deltaTime * l_speed;
    }
    // Y UP
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
    {
        g_cam_position += up * deltaTime * l_speed;
    }

    g_proj_matrix = glm::perspective(g_cam_fov, float(g_width) / float(g_height), 0.25f, 4000.0f);
    g_view_matrix = glm::lookAt(g_cam_position, g_cam_position + direction, up);

    lastTime = currentTime;
}

static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);

    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
    {
        glm::vec3 direction(
        std::cos(g_cam_vertical_angle) * std::sin(g_cam_horizontal_angle),
        std::sin(g_cam_vertical_angle),
        std::cos(g_cam_vertical_angle) * std::cos(g_cam_horizontal_angle)
        );

        // createDynamic(physx::PxTransform(physx::PxVec3(g_cam_position.x, g_cam_position.y, g_cam_position.z)), physx::PxSphereGeometry(2),
                    //   physx::PxVec3(direction.x, direction.y, direction.z) * 100.0f);
    }
}
static void error_callback(int error, const char* description)
{
	fprintf(stderr, "Error: %s\n", description);
}
 

static void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    g_width  = width;
    g_height = height;
}

int main(void)
{
	GLuint vertex_buffer, vertex_shader, fragment_shader, program;
	GLint mvp_location, vpos_location, vcol_location;
 
	glfwSetErrorCallback(error_callback);
 
	if (!glfwInit())
    {
        std::cout << "EXIT_FAILURE" << '\n';
        exit(EXIT_FAILURE);
    }
		
    

 
	window = glfwCreateWindow(g_width, g_height, "Simple MeshGeom Example", NULL, NULL);
	if (!window)
	{
        std::cout << "No Window" << '\n';
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
    std::cout << "loge" << '\n';
 
	glfwSetKeyCallback(window, key_callback);
 
	glfwMakeContextCurrent(window);
	gladLoadGL(glfwGetProcAddress);
	glfwSwapInterval(1);
 
	// NOTE: OpenGL error checks have been omitted for brevity

    // set ogl states and defaults
    glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    //glEnable(GL_CULL_FACE);
    //glFrontFace(GL_CW);

    GLMeshData myPlane;
    myPlane.createPlane(0.0f, 128.0f, 2.0f);

    GLMeshData myBox;
    myBox.createBox(4.0f, 4.0f, 4.0f);

    GLMeshData mySphere;
    mySphere.createSphere(2.0f, 32, 32);

    {
        int           imgWidth, imgHeight, numColCh;
        unsigned char *bytes = stbi_load("../res/textures/PresentA_ALB.png", &imgWidth, &imgHeight, &numColCh, 0);

        glGenTextures(1, &texture_crate);
        glBindTexture(GL_TEXTURE_2D, texture_crate);

        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, imgWidth, imgHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, bytes);
        glGenerateMipmap(GL_TEXTURE_2D);

        glBindTexture(GL_TEXTURE_2D, 0);
        stbi_image_free(bytes);
    }
    {
        int           imgWidth, imgHeight, numColCh;
        unsigned char *bytes = stbi_load("../res/textures/NumGrid_ALB.png", &imgWidth, &imgHeight, &numColCh, 0);

        glGenTextures(1, &texture_checker);
        glBindTexture(GL_TEXTURE_2D, texture_checker);

        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, imgWidth, imgHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, bytes);
        glGenerateMipmap(GL_TEXTURE_2D);

        glBindTexture(GL_TEXTURE_2D, 0);
        stbi_image_free(bytes);
    }
    {
        for (int i = 0; i < num_ball_textures; ++i)
        {
            std::stringstream ss;
            ss << std::setfill('0') << std::setw(2) << std::fixed << i + 1;

            std::stringstream baseNameSS;
            baseNameSS << "../res/textures/pool/";
            baseNameSS << "pool_";
            baseNameSS << ss.str();
            baseNameSS << ".ppm";

            int           imgWidth, imgHeight, numColCh;
            unsigned char *bytes = stbi_load(baseNameSS.str().c_str(), &imgWidth, &imgHeight, &numColCh, 0);

            glGenTextures(1, texIds + i);
            glBindTexture(GL_TEXTURE_2D, texIds[i]);

            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, imgWidth, imgHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, bytes);
            glGenerateMipmap(GL_TEXTURE_2D);

            glBindTexture(GL_TEXTURE_2D, 0);
            stbi_image_free(bytes);
        }
    }

    // Create and compile our GLSL program from the shaders
    GLuint programID = create_shader_program(vertexShaderSource, fragmentShaderSource);

    // Get a handle for our "MVP" uniform
    GLuint MatrixID = glGetUniformLocation(programID, "MVP");

    // Get a handle for our "myTextureSampler" uniform
    GLuint TextureID = glGetUniformLocation(programID, "myTextureSampler");


    double lastFPStime  = glfwGetTime();
    int    frameCounter = 0;


    GLMeshData shape;
    shape.createCylinder(3,6,32);

    GLMeshData circleMesh;
    circleMesh.createCircle(1.0f, 32);  // radius = 1.0, 32 segments

    GLMeshData rectangleMesh;
    rectangleMesh.createRectangle(5.0f, 5.0f);  // width = 2.0, height = 1.0

    glm::mat4 OrthoProj = glm::ortho(0.0f, (float)g_width, 0.0f, (float)g_height, 0.1f, 100.0f);

    // mat4x4_ortho(projection, 0.0f, (float)width, (float)height, 0.0f, -1.0f, 1.0f);

    GLuint circleImg;

    {
        int           imgWidth, imgHeight, numColCh;
        unsigned char *bytes = stbi_load("../res/textures/circle.tga", &imgWidth, &imgHeight, &numColCh, 0);

        glGenTextures(1, &circleImg);
        glBindTexture(GL_TEXTURE_2D, circleImg);

        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, imgWidth, imgHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, bytes);
        glGenerateMipmap(GL_TEXTURE_2D);

        glBindTexture(GL_TEXTURE_2D, 0);
        stbi_image_free(bytes);
    }

    do
    {
        double thisFPStime = glfwGetTime();
        frameCounter++;

        if (thisFPStime - lastFPStime >= 1.0)
        {
            lastFPStime = thisFPStime;

            // std::string windowTitle = g_app_title + " (";
            // windowTitle += std::to_string(frameCounter);
            // windowTitle += " fps)";
            // const char *windowCaption = windowTitle.c_str();
            // glfwSetWindowTitle(window, windowCaption);

            frameCounter = 0;
        }

        // stepPhysics();

        glViewport(0, 0, g_width, g_height);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // bind shader
        glUseProgram(programID);

        // compute the MVP matrix from keyboard and mouse input
        // camera
        computeMatricesFromInputs();
#if 0
        // render physx shapes
        {
            const int MAX_NUM_ACTOR_SHAPES = 128;

            physx::PxShape    *shapes[MAX_NUM_ACTOR_SHAPES];
            for (physx::PxU32 i = 0; i < static_cast<physx::PxU32>(physx_actors.size()); i++)
            {
                const physx::PxU32 nbShapes = physx_actors[i].actorPtr->getNbShapes();
                PX_ASSERT(nbShapes <= MAX_NUM_ACTOR_SHAPES);
                physx_actors[i].actorPtr->getShapes(shapes, nbShapes);

                for (physx::PxU32 j = 0; j < nbShapes; j++)
                {
                    const physx::PxMat44          shapePose(physx::PxShapeExt::getGlobalPose(*shapes[j], *physx_actors[i].actorPtr));
                    const physx::PxGeometryHolder h = shapes[j]->getGeometry();

                    // render object
                    glm::mat4 model_matrix = glm::make_mat4(&shapePose.column0.x);
                    glm::mat4 mvp_mat      = g_proj_matrix * g_view_matrix * model_matrix;

                    if (h.getType() == physx::PxGeometryType::eBOX)
                    {
                        glActiveTexture(GL_TEXTURE0);
                        glBindTexture(GL_TEXTURE_2D, texture_crate);
                        glUniform1i(TextureID, 0);

                        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, glm::value_ptr(mvp_mat));
                        myBox.render();
                    } else if (h.getType() == physx::PxGeometryType::eSPHERE)
                    {
                        glActiveTexture(GL_TEXTURE0);
                        glBindTexture(GL_TEXTURE_2D, texIds[physx_actors[i].actorId % 15]);
                        glUniform1i(TextureID, 0);

                        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, glm::value_ptr(mvp_mat));
                        mySphere.render();
                    }
                }
            }
        }
#endif
        // render ground plane
        {
            glm::mat4 model_matrix = glm::mat4(1.0);
            glm::mat4 mvp_mat      = g_proj_matrix * g_view_matrix * model_matrix;

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, texture_checker);
            glUniform1i(TextureID, 0);

            glUniformMatrix4fv(MatrixID, 1, GL_FALSE, glm::value_ptr(mvp_mat));
            myPlane.render();
        }
        {
            glm::mat4 model_matrix = glm::mat4(1.0);
            model_matrix = glm::translate(model_matrix, glm::vec3(1,10,0));
            glm::mat4 mvp_mat      = g_proj_matrix * g_view_matrix * model_matrix;

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, texIds[0]);
            glUniform1i(TextureID, 0);

            glUniformMatrix4fv(MatrixID, 1, GL_FALSE, glm::value_ptr(mvp_mat));
            mySphere.render();
        }
        {

            glm::mat4 model_matrix = glm::mat4(1.0);
            model_matrix = glm::translate(model_matrix, glm::vec3(10,10,-100));
            model_matrix = glm::scale(model_matrix, glm::vec3(2,5,10));
            glm::mat4 mvp_mat      = g_proj_matrix * g_view_matrix * model_matrix;

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, texture_crate);
            glUniform1i(TextureID, 0);

            glUniformMatrix4fv(MatrixID, 1, GL_FALSE, glm::value_ptr(mvp_mat));
            myBox.render();

        }
        {
            glm::mat4 model_matrix = glm::mat4(1.0);
            model_matrix = glm::translate(model_matrix, glm::vec3(10,10,0));
            // model_matrix = glm::scale(model_matrix, glm::vec3(2,5,10));
            glm::mat4 mvp_mat      = g_proj_matrix * g_view_matrix * model_matrix;

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, texture_crate);
            glUniform1i(TextureID, 0);

            glUniformMatrix4fv(MatrixID, 1, GL_FALSE, glm::value_ptr(mvp_mat));
            
            shape.render();

            
        }
        {
            // 2D

            // glActiveTexture(GL_TEXTURE0);
            // glBindTexture(GL_TEXTURE_2D, circleImg);
            // glUniform1i(TextureID, 0);



            // glm::mat4 model_matrix = glm::mat4(1.0);
            // // model_matrix = glm::translate(model_matrix, glm::vec3(-.8,-0.8,0));
            // model_matrix = glm::scale(model_matrix, glm::vec3(.1f));
            // model_matrix *= OrthoProj;
   

            // glUniformMatrix4fv(MatrixID, 1, GL_FALSE, glm::value_ptr(OrthoProj));
            

            // // circleMesh.render();
            // rectangleMesh.render();   
        }

  
        // Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    } while (glfwWindowShouldClose(window) == 0);
 
    return 0;
}