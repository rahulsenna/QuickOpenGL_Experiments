#pragma once

#include <glad/gl.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include "linmath.h"

#include <cmath>
#include <string>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace glm;
#include <iostream>
#include <vector>

#define M_PI 3.141592653589793238462

#include "shader.cpp"

#include "UploadImage.cpp"

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
    void createQuad();
    void createCircle(float radius, uint32_t segments);

	void render();
    void renderQuad();
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

void GLMeshData::createQuad()
{
#if  0
    numPrimitives = 2;

    // Vertices
    posData = {
        0.0f,  0.0f, 0,
        1.0f,  0.0f, 0,
        1.0f,  1.0f, 0,
        0.0f,  1.0f, 0,
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

#else
    float quadVertices[] = {
        // positions   // texture coords
         0.0f,  1.0f,  0.0f, 1.0f,
         1.0f,  1.0f,  1.0f, 1.0f,
         0.0f,  0.0f,  0.0f, 0.0f,
         1.0f,  0.0f,  1.0f, 0.0f
    };

    glGenVertexArrays(1, &meshVAID);
    glGenBuffers(1, &meshVBID_pos);
    glBindVertexArray(meshVAID);
    glBindBuffer(GL_ARRAY_BUFFER, meshVBID_pos);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

#endif  //0
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

void GLMeshData::renderQuad()
{
	glBindVertexArray(meshVAID);
	CHECK_GL;

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	CHECK_GL;

	glBindVertexArray(0);
	CHECK_GL;
}


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
out vec4 color;

// Values that stay constant for the whole mesh.
uniform sampler2D myTextureSampler;

void main(){

	// Output color = color of the texture at the specified UV
    vec4 c = texture( myTextureSampler, UV );
	color = vec4(c.rgb,c.a);
}

)FRAGMENT";



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


GLuint MatrixID;

glm::mat4 OrthoProjection = glm::ortho(0.0f, (float)g_width, (float)g_height, 0.0f, -1.0f, 1.0f);

glm::mat4 GetQuadMatrix(float x, float y, float width, float height)
{
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(x, y, 0.0f));
    model = glm::scale(model, glm::vec3(width, height, 1.0f));

    return OrthoProjection*model;
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
 
	glfwSetKeyCallback(window, key_callback);
 
	glfwMakeContextCurrent(window);
	gladLoadGL(glfwGetProcAddress);
	glfwSwapInterval(1);
 
	// NOTE: OpenGL error checks have been omitted for brevity

    // set ogl states and defaults
    glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    uint32_t programID = create_shader_program(vertexShaderSource, fragmentShaderSource);

    //glEnable(GL_CULL_FACE);
    //glFrontFace(GL_CW);

    GLMeshData myPlane;
    myPlane.createPlane(0.0f, 128.0f, 2.0f);

    GLMeshData myBox;
    myBox.createBox(4.0f, 4.0f, 4.0f);

    GLMeshData mySphere;
    mySphere.createSphere(2.0f, 32, 32);
    texture_crate   = UploadImage("../res/textures/PresentA_ALB.png");
    texture_checker = UploadImage("../res/textures/NumGrid_ALB.png", true); 
    {
        for (int i = 0; i < num_ball_textures; ++i)
        {
            auto num = std::string(2 - std::to_string(i + 1).length(), '0') + std::to_string(i + 1);
            std::string src = "../res/textures/pool/pool_" + num + ".ppm";
            texIds[i] = UploadImage(src.c_str());
        }
    }

    // Create and compile our GLSL program from the shaders
    

    // Get a handle for our "MVP" uniform
    MatrixID = glGetUniformLocation(programID, "MVP");

    // Get a handle for our "myTextureSampler" uniform
    GLuint TextureID = glGetUniformLocation(programID, "myTextureSampler");


    double lastFPStime  = glfwGetTime();
    int    frameCounter = 0;


    GLMeshData shape;
    shape.createCylinder(3,6,32);

    GLMeshData circleMesh;
    circleMesh.createCircle(1.0f, 32);  // radius = 1.0, 32 segments

    GLMeshData rectangleMesh;
    rectangleMesh.createQuad();  // width = 2.0, height = 1.0

    // mat4x4_ortho(projection, 0.0f, (float)width, (float)height, 0.0f, -1.0f, 1.0f);

    // GLuint circleImg = UploadImage("../res/textures/red-circle.png");
    // GLuint circleImg = UploadImage("../res/textures/circle.png");
    GLuint circleImg = UploadImage("../res/textures/shoot2.png");
    GLuint rectImg = UploadImage("../res/textures/rect_round_corner2.png");
    GLuint exclaimImg = UploadImage("../res/textures/Exclamation_Mark.png");
    
    float aspect = (float)g_height/(float)g_width;
    float w = g_width  * 0.01f * aspect;
    float h = g_height * 0.01f;

    auto circleMat = GetQuadMatrix(g_width - 20.f*w , g_height - 20.*h, w*20.f,h*20.f);
    auto rectMat = GetQuadMatrix(w*1.f,h*50.f, w*20.f,h*5.f);
    auto exclaimMark = GetQuadMatrix(w*1.f,h*20.f, w*5.f,h*5.f);

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

            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, circleImg);
            glUniform1i(TextureID, 0);
            glUniformMatrix4fv(MatrixID, 1, GL_FALSE, glm::value_ptr(circleMat));
            rectangleMesh.renderQuad();
            // myPlane.render();

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, rectImg);
            glUniform1i(TextureID, 0);
            glUniformMatrix4fv(MatrixID, 1, GL_FALSE, glm::value_ptr(rectMat));
            rectangleMesh.renderQuad();


            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, exclaimImg);
            glUniform1i(TextureID, 0);
            glUniformMatrix4fv(MatrixID, 1, GL_FALSE, glm::value_ptr(exclaimMark));
            rectangleMesh.renderQuad();

            

            glDisable(GL_BLEND);

        }

  
        // Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    } while (glfwWindowShouldClose(window) == 0);
 
    return 0;
}