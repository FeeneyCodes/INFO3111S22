// This is from here: https://www.glfw.org/docs/latest/quick.html

#include "globalStuff.h"
//#include <glad/glad.h>
//#define GLFW_INCLUDE_NONE
//#include <GLFW/glfw3.h>

//#include "linmath.h"
#include <glm/glm.hpp>
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> 
// glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp> // glm::value_ptr

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <sstream>  // String Stream

#include "cShaderManager.h"

//static const struct
//{
//    float x, y;
//    float r, g, b;
//} vertices[6] =
//{
//    /* triangle #1 */
//    { -0.6f, -0.4f, 1.f, 0.f, 0.f },
//    {  0.6f, -0.4f, 0.f, 1.f, 0.f },
//    {   0.f,  0.6f, 0.f, 0.f, 1.f },
//    /* triangle #2 */
//    { -0.6f + 1.0f, -0.4f, 1.f, 0.f, 0.f },
//    {  0.6f + 1.0f, -0.4f, 0.f, 1.f, 0.f },
//    {  0.0f + 1.0f,  0.6f, 0.f, 0.f, 1.f }
//};

struct sVertex
{
    float x, y;         // vPos
    float r, g, b;      // vCol
};

sVertex vertices[6] =
{
    /* triangle #1 */
    { -0.6f, -0.4f, 0.f, 0.f, 1.f },
    {  0.6f, -0.4f, 0.f, 1.f, 0.f },
    {   0.f,  0.6f, 1.f, 0.f, 0.f },
    /* triangle #2 */
    { -0.6f + 1.0f, -0.4f, 1.f, 0.f, 0.f },
    {  0.6f + 1.0f, -0.4f, 0.f, 1.f, 0.f },
    {  0.0f + 1.0f,  0.6f, 0.f, 0.f, 1.f }
};

//static const char* vertex_shader_text =
//"#version 110\n"
//"uniform mat4 MVP;\n"
//"attribute vec3 vCol;\n"
//"attribute vec2 vPos;\n"
//"varying vec3 color;\n"
//"void main()\n"
//"{\n"
//"    gl_Position = MVP * vec4(vPos, 0.0, 1.0);\n"
//"    color = vCol;\n"
//"}\n";
//
//static const char* fragment_shader_text =
//"#version 110\n"
//"varying vec3 color;\n"
//"void main()\n"
//"{\n"
//"    gl_FragColor = vec4(color, 1.0);\n"
//"}\n";

//// Camera
//float camera_X = 0.0f;
//float camera_Y = 0.0f;
//float camera_Z = 0.0f;
//
//class cVector
//{
//public:
//    float X = 0.0f;
//    float Y = 0.0f; 
//    float Z = 0.0f;
//};
//
//cVector cameraLocation;

glm::vec3 g_cameraEye = glm::vec3(0.0, 0.0, -4.0f);


static void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
    // WASD - movement 
    // A = left
    // D = right
    // W = forward
    // S = backward
    // Q = down
    // E = up

    const float cameraMovementSpeed = 0.1f;

    if ( key == GLFW_KEY_A )
    {
        ::g_cameraEye.x -= cameraMovementSpeed;     // "left"
    }
    if ( key == GLFW_KEY_D )
    {
        ::g_cameraEye.x += cameraMovementSpeed;     // "right"
    }

    if ( key == GLFW_KEY_Q )
    {
        ::g_cameraEye.y -= cameraMovementSpeed;     // "down"
    }
    if ( key == GLFW_KEY_E )
    {
        ::g_cameraEye.y += cameraMovementSpeed;     // "up"
    }

    if ( key == GLFW_KEY_W )
    {
        ::g_cameraEye.z += cameraMovementSpeed;     // "forward"
    }
    if ( key == GLFW_KEY_S )
    {
        ::g_cameraEye.z -= cameraMovementSpeed;     // "backwards"
    }

}

int main(void)
{
    GLFWwindow* window;
    GLuint vertex_buffer;
    //GLuint vertex_shader;
    //GLuint fragment_shader;
    GLuint shaderProgramNumber = 0;     
    GLint mvp_location, vpos_location, vcol_location;

    glfwSetErrorCallback(error_callback);

    if ( ! glfwInit() )
    {
        //exit(EXIT_FAILURE);
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    window = glfwCreateWindow(640, 480, "Simple example", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwSetKeyCallback(window, key_callback);

    glfwMakeContextCurrent(window);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    glfwSwapInterval(1);

    // NOTE: OpenGL error checks have been omitted for brevity

    glGenBuffers(1, &vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Use the shader manager thingy...
    cShaderManager* pShaderManager = new cShaderManager();
//    cShaderManager shaderManager;

    cShaderManager::cShader vertShader;
    vertShader.fileName = "simpleVertex.glsl";

    cShaderManager::cShader fragShader;
    fragShader.fileName = "simpleFragment.glsl";

    if ( ! pShaderManager->createProgramFromFile("basicShader", vertShader, fragShader) )
    {
        std::cout << "Oh no, there was an error!" << std::endl;
        std::cout << pShaderManager->getLastError() << std::endl;
        // exit
        return -1;
    }

    // Compiled and linked the shader program OK, so remember which program is was
    shaderProgramNumber = pShaderManager->getIDFromFriendlyName("basicShader");

    //vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    //glShaderSource(vertex_shader, 1, &vertex_shader_text, NULL);
    //glCompileShader(vertex_shader);

    //fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    //glShaderSource(fragment_shader, 1, &fragment_shader_text, NULL);
    //glCompileShader(fragment_shader);

    //program = glCreateProgram();
    //glAttachShader(program, vertex_shader);
    //glAttachShader(program, fragment_shader);
    //glLinkProgram(program);

    mvp_location = glGetUniformLocation(shaderProgramNumber, "MVP");
    vpos_location = glGetAttribLocation(shaderProgramNumber, "vPos");
    vcol_location = glGetAttribLocation(shaderProgramNumber, "vCol");

    glEnableVertexAttribArray(vpos_location);
    glVertexAttribPointer(vpos_location, 2, GL_FLOAT, GL_FALSE,
        sizeof(vertices[0]), (void*)0);

    glEnableVertexAttribArray(vcol_location);
    glVertexAttribPointer(vcol_location, 3, GL_FLOAT, GL_FALSE,
        sizeof(vertices[0]), (void*)(sizeof(float) * 2));

    while ( ! glfwWindowShouldClose(window) )
    {
        float ratio;
        int width, height;
//        mat4x4 m, p, mvp;     // linmath.h
        glm::mat4x4 m, p, mvp;

        glfwGetFramebufferSize(window, &width, &height);
        ratio = width / (float)height;

        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT);

//        mat4x4_identity(m);
        m = glm::mat4x4(1.0f);

//        mat4x4_rotate_Z(m, m, (float)glfwGetTime());
        glm::mat4 rotateZ = glm::rotate(glm::mat4(1.0f),
                                        (float)glfwGetTime(),
                                        glm::vec3(0.0f, 0.0, 1.0f));     

        
//        mat4x4_ortho(p, -ratio, ratio, -1.f, 1.f, 1.f, -1.f);
        p = glm::perspective(0.6f,
                             ratio,
                             0.1f,
                             1000.0f);

//        mat4x4_mul(mvp, p, m);
        glm::mat4x4 v = glm::mat4(1.0f);

//        glm::vec3 cameraEye = glm::vec3(0.0, 0.0, -4.0f);
        glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
        glm::vec3 upVector = glm::vec3(0.0f, 1.0f, 0.0f);

        v = glm::lookAt(
            ::g_cameraEye,
            cameraTarget,
            upVector);

        //mat4x4_mul(mvp, p, m);
        mvp = p * v * m;

//        glUseProgram(program);
        glUseProgram(shaderProgramNumber);

//        glUniformMatrix4fv(mvp_location, 1, GL_FALSE, (const GLfloat*)mvp);
        glUniformMatrix4fv(mvp_location, 1, GL_FALSE, glm::value_ptr(mvp));

        // GL_LINE_LOOP, GL_POINTS, or GL_TRIANGLES
        glDrawArrays(GL_TRIANGLES, 0, 6);

        glfwSwapBuffers(window);
        glfwPollEvents();

        // Change the title of the window to show the camera location
        std::stringstream ssTitle;
        ssTitle << "Camera (x,y,z): "               // just like cout or fstream, etc.
            << ::g_cameraEye.x << ", "
            << ::g_cameraEye.y << ", "
            << ::g_cameraEye.z;
//        glfwSetWindowTitle(window, "Hello!");
        glfwSetWindowTitle( window, ssTitle.str().c_str() );

    }

    glfwDestroyWindow(window);

    // Clean up
    delete pShaderManager;

    glfwTerminate();
    //exit(EXIT_SUCCESS);
    return 0;
}
