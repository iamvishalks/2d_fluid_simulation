

#define _XOPEN_SOURCE 500

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <unistd.h>

#define GLEW_STATIC
#include <GL/glew.h>

#define GL_GLEXT_PROTOTYPES
#include <GLFW/glfw3.h>

//#include "./la.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

const char *vert_shader_source =
    "#version 330 core\n"
    "\n"
    "void main()\n"
    "{\n"
    "\n"
    "}\n";

const char *frag_shader_source =
    "#version 330 core\n"
    "\n"
    "void main()\n"
    "{\n"
    "\n"
    "}\n";

const char *shader_type_as_cstr(unsigned int shader)
{
    switch (shader)
    {
    case GL_VERTEX_SHADER:
        return "GL_VERTEX_SHADER";
    case GL_FRAGMENT_SHADER:
        return "GL_FRAGMENT_SHADER";

    default:
        return "(Unknown)";
    }
}

bool compile_shader_source(const char *source, unsigned int shader_type, unsigned int *shader)
{
    *shader = glCreateShader(shader_type);
    glShaderSource(*shader, 1, &source, NULL);
    glCompileShader(*shader);

    int compiled = 0;
    glGetShaderiv(*shader, GL_COMPILE_STATUS, &compiled);

    if (!compiled)
    {
        char message[1024];
        int message_size = 0;

        glGetShaderInfoLog(*shader, sizeof(message), &message_size, message);
        fprintf(stderr, "ERROR: could not compile %s\n", shader_type_as_cstr(shader_type));
        fprintf(stderr, "%.*s\n", message_size, message);
        return false;
    }

    return true;
}

bool link_program(unsigned int vert_shader, unsigned int frag_shader, unsigned int *program)
{
    *program = glCreateProgram();

    glAttachShader(*program, vert_shader);
    glAttachShader(*program, frag_shader);
    glLinkProgram(*program);

    int linked = 0;
    glGetProgramiv(*program, GL_LINK_STATUS, &linked);
    if (!linked)
    {
        int message_size = 0;
        char message[1024];

        glGetProgramInfoLog(*program, sizeof(message), &message_size, message);
        fprintf(stderr, "Program Linking: %.*s\n", message_size, message);
    }

    glDeleteShader(vert_shader);
    glDeleteShader(frag_shader);

    return program;
}

void MessageCallback(GLenum source,
                     GLenum type,
                     GLuint id,
                     GLenum severity,
                     GLsizei length,
                     const GLchar *message,
                     const void *userParam)
{
    (void)source;
    (void)id;
    (void)length;
    (void)userParam;
    fprintf(stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
            (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""),
            type, severity, message);
}

void window_size_callback(GLFWwindow *window, int width, int height)
{
    (void)window;
    glViewport(width / 2 - SCREEN_WIDTH / 2, height / 2 - SCREEN_HEIGHT / 2, SCREEN_WIDTH, SCREEN_HEIGHT);
}

void init_shaders()
{
    unsigned int vert_shader = 0;
    if (!compile_shader_source(vert_shader_source, GL_VERTEX_SHADER, &vert_shader))
    {
        exit(1);
    }

    unsigned int frag_shader = 0;
    if (!compile_shader_source(frag_shader_source, GL_FRAGMENT_SHADER, &frag_shader))
    {
        exit(1);
    }

    unsigned int program = 0;
    if (!link_program(vert_shader, frag_shader, &program))
    {
        exit(1);
    }

    glUseProgram(program);
}

typedef enum
{
    POSTITON_ATTRIB = 0,
    COLOR_ATTRIB,
    COUNT_ATTRIBS
} Attribs;

typedef struct
{
    float x;
    float y;
    float r;
    float g;
    float b;

} Vert;

#define VERTS_CAPACITY 1024
Vert verts[VERTS_CAPACITY];

unsigned long verts_count = 0;

unsigned int vao = 0;
unsigned int vbo = 0;

void init_buffers()
{
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(POSTITON_ATTRIB);

    glVertexAttribPointer(POSTITON_ATTRIB, 2, GL_FLOAT, GL_FALSE, sizeof(Vert), (void *)0);
    glVertexAttribPointer(COLOR_ATTRIB, 3, GL_FLOAT, GL_FALSE, sizeof(Vert), (void *)(sizeof(float) * 2));
}

int main()
{
    if (!glfwInit())
    {
        fprintf(stderr, "ERROR: could not initialize GLFW\n");
        exit(1);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *const window = glfwCreateWindow(
        SCREEN_WIDTH,
        SCREEN_HEIGHT,
        "2D Fluid Simulation",
        NULL,
        NULL);

    if (window == NULL)
    {
        fprintf(stderr, "ERROR: could not create a window.\n");
        glfwTerminate();
        exit(1);
    }
    // test
    glfwMakeContextCurrent(window);

    if (GLEW_OK != glewInit())
    {
        fprintf(stderr, "Could not init GLEW\n");
        exit(1);
    }

    if (!GLEW_EXT_draw_instanced)
    {
        fprintf(stderr, "Support for EXT_draw_instanced is required!\n");
        exit(1);
    }

    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(MessageCallback, 0);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glfwSetFramebufferSizeCallback(window, window_size_callback);

    // time = glfwGetTime();

    init_shaders();
    init_buffers();

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    return 0;
}
