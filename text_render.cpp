#include <glad/gl.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include "linmath.h"


#define STB_TRUETYPE_IMPLEMENTATION
#include <stb/stb_truetype.h>

#include "stdlib.h"
#include "stdio.h"
#include <iostream>


// Font atlas struct
typedef struct
{
    GLuint texture;
    stbtt_bakedchar cdata[96]; // ASCII 32..126 is 95 glyphs
    int w, h;
} FontAtlas;

FontAtlas font;
GLuint shaderProgram;
GLuint VAO, VBO;
GLint projection_loc, vpos_location, vcol_location;
GLint textUniformLocation;
// Vertex shader
const char* vertexShaderSource = R"VERTEX(
    #version 330 core
    layout (location = 0) in vec4 vertex; // <vec2 pos, vec2 tex>
    out vec2 TexCoords;
    uniform mat4 projection;
    void main() {
        gl_Position = projection * vec4(vertex.xy, 0.0, 1.0);
        TexCoords = vertex.zw;
    }
)VERTEX";

// Fragment shader
const char* fragmentShaderSource = R"FRAGMENT(
    #version 330 core
    in vec2 TexCoords;
    out vec4 color;
    uniform sampler2D text;
    uniform vec3 textColor;
    void main() {
        vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, TexCoords).r);
        color = vec4(textColor, 1.0) * sampled;
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


// Load font and create texture atlas
void load_font(const char* filename, float font_height)
{
    std::cout << "Loading font: " << filename << std::endl;
    unsigned char* ttf_buffer =(unsigned char *) malloc(1 << 20);
    unsigned char* temp_bitmap =(unsigned char *) malloc(512 * 512);

    fread(ttf_buffer, 1, 1 << 20, fopen(filename, "rb"));
    font.w = 512;
    font.h = 512;
    stbtt_BakeFontBitmap(ttf_buffer, 0, font_height, temp_bitmap, font.w, font.h, 32, 96, font.cdata);

    glGenTextures(1, &font.texture);
    glBindTexture(GL_TEXTURE_2D, font.texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, font.w, font.h, 0, GL_RED, GL_UNSIGNED_BYTE, temp_bitmap);
    // glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, font.w, font.h, 0, GL_RED, GL_UNSIGNED_BYTE, temp_bitmap); // GL_R8 for GL ES
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    std::cout << "Font texture created with ID: " << font.texture << std::endl;

    free(ttf_buffer);
    free(temp_bitmap);    
}
void init_render_data()
{
    shaderProgram = create_shader_program(vertexShaderSource, fragmentShaderSource);

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    projection_loc = glGetUniformLocation(shaderProgram, "projection");
    GLint textUniformLocation = glGetUniformLocation(shaderProgram, "text");
    glCheckError();
    
    std::cout << "Shader program created with ID: " << shaderProgram << std::endl;
}

#include <string>
void render_text(std::string textStr, float x, float y, float scale, float r, float g, float b)
{
    // std::cout << "Rendering text: " << text << " at (" << x << ", " << y << ")" << std::endl;
    glUseProgram(shaderProgram);
    glUniform3f(glGetUniformLocation(shaderProgram, "textColor"), r, g, b);

    glBindVertexArray(VAO);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBindTexture(GL_TEXTURE_2D, font.texture);

    GLuint indices[] = {0, 1, 2, 0, 2, 3};

    const char *text = textStr.c_str();

    while (*text) {
        if (*text >= 32 && *text < 128)
        {
            stbtt_aligned_quad q;
            stbtt_GetBakedQuad(font.cdata, font.w, font.h, *text - 32, &x, &y, &q, 1);

            float vertices[4][4] = {
                { q.x0 * scale, q.y0 * scale, q.s0, q.t0 },
                { q.x0 * scale, q.y1 * scale, q.s0, q.t1 },
                { q.x1 * scale, q.y1 * scale, q.s1, q.t1 },
                { q.x1 * scale, q.y0 * scale, q.s1, q.t0 }
            };

            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, indices);

            glBindBuffer(GL_ARRAY_BUFFER, 0);
        }
        ++text;
    }
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

// FIXED PIPELINE
void my_stbtt_print(float x, float y, char *text)
{
   // assume orthographic projection with units = screen pixels, origin at top left
   glEnable(GL_BLEND);
   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   glEnable(GL_TEXTURE_2D);
   glBindTexture(GL_TEXTURE_2D, font.texture);
   glBegin(GL_QUADS);
   while (*text) {
      if (*text >= 32 && *text < 128) {
         stbtt_aligned_quad q;
         stbtt_GetBakedQuad(font.cdata, 512,512, *text-32, &x,&y,&q,1);//1=opengl & d3d10+,0=d3d9
         glTexCoord2f(q.s0,q.t0); glVertex2f(q.x0,q.y0);
         glTexCoord2f(q.s1,q.t0); glVertex2f(q.x1,q.y0);
         glTexCoord2f(q.s1,q.t1); glVertex2f(q.x1,q.y1);
         glTexCoord2f(q.s0,q.t1); glVertex2f(q.x0,q.y1);
      }
      ++text;
   }
   glEnd();
}


static void error_callback(int error, const char* description)
{
	fprintf(stderr, "Error: %s\n", description);
}
 
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GLFW_TRUE);
}
 #define CORNFLOWER_BLUE 100 / 255.f, 149 / 255.f, 237 / 255.f, 1

// In your main function or rendering loop
#include <chrono>
int main()
{
    // Initialize GLFW and OpenGL context
    // ...
    GLFWwindow* window;
 
	glfwSetErrorCallback(error_callback);
 
	if (!glfwInit())
		exit(EXIT_FAILURE);
 
    // // Request core profile explicitly
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
 
	window = glfwCreateWindow(640, 480, "Simple example", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
 
	glfwSetKeyCallback(window, key_callback);
 
	glfwMakeContextCurrent(window);
	gladLoadGL(glfwGetProcAddress);
	glfwSwapInterval(0); //0 == Vsync Off, 1 == On
 
    init_render_data();
	
 

    load_font("../res/digital_7_mono.ttf", 20.0f);
	float ratio;
    int width, height;
    mat4x4 projection;

    glClearColor(CORNFLOWER_BLUE);    

    auto lastFrameTime = std::chrono::high_resolution_clock::now();

    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);
        auto currentFrameTime = std::chrono::high_resolution_clock::now();
        std::chrono::duration<float, std::milli> duration = currentFrameTime - lastFrameTime;

        lastFrameTime = currentFrameTime;

        render_text("FPS: " + std::to_string(1000.f/duration.count()), 10.0f, 32.0f, .7f, 1.0f, 1.0f, 1.0f);

        glfwGetFramebufferSize(window, &width, &height);
        ratio = width / (float) height;

        glViewport(0, 0, width, height);

        mat4x4_ortho(projection, 0.0f, (float)width, (float)height, 0.0f, -1.0f, 1.0f);
        
        glUseProgram(shaderProgram);
		glUniformMatrix4fv(projection_loc, 1, GL_FALSE, (const GLfloat*) projection);

        render_text("Hello, Sailor!", 0.0f, 132.0f, 1.0f, 1.0f, 0.0f, 0.0f);
        // my_stbtt_print(10,10,"Hello");

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Cleanup
    // ...

    return 0;
}