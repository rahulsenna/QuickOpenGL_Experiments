#pragma once

#include <string>
#include <iostream>
#include <sstream>
#include <glad/gl.h>



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