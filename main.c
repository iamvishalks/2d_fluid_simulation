

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
    "#version 33 core\n"
    "\n"
    "void main()\n"
    "{\n"
    "\n"
    "}\n";

const char *frag_shader_source =
    "#version 33 core\n"
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
        fprintf(stderr, "%. *s\n", message_size, message);
        return false;
    
    }

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

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    return 0;
}
