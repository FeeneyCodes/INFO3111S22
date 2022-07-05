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
#include <fstream>

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

//sVertex vertices[6] =
//{
//    /* triangle #1 */
//    // -0.09 to 0.06 in the x axis
//    { -0.6f, -0.4f, 0.0f, 0.0f, 1.0f },
//    {  0.6f, -0.4f, 0.0f, 1.0f, 0.0f },
//    {   0.f,  0.6f, 1.0f, 0.0f, 0.0f },
//    /* triangle #2 */
//    {  0.9f, -0.4f, 0.0f, 1.0f, 0.0f },
//    { -0.3f, -0.4f, 0.0f, 1.0f, 0.0f },
//    {  0.2f,  0.6f, 0.0f, 1.0f, 1.0f }
//};
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

// Function to load the bunny ply format model
// "bunny.ply"
// sVertex* pVertices;
// unsigned int numberOfVerticesLoaded;

bool LoadPlyModelFromFile(std::string fileName, sVertex* &pVertices, unsigned int &numberOfVerticesLoaded)
{
    std::ifstream thePlyFile(fileName.c_str());
    if ( ! thePlyFile.is_open() )
    {
        return false;
    }

    std::string token;
    while (thePlyFile >> token)
    {
        if (token == "vertex")
        {
            break;
        }
    }
    // The next thing is the number of vertices.
    thePlyFile >> numberOfVerticesLoaded;

    // Allocate the array on the heap...
    pVertices = new sVertex[numberOfVerticesLoaded];


    while (thePlyFile >> token)
    {
        if (token == "face")
        {
            break;
        }
    }
    // Next is the number of "Faces" (aka "triangles")
    unsigned int numberOfTriangles = 0;
    thePlyFile >> numberOfTriangles;


    while (thePlyFile >> token)
    {
        if (token == "end_header")
        {
            break;
        }
    }

    // This particular ply file (the O.G. bunny one has this header:
    //   ply
    //   format ascii 1.0
    //   comment zipper output
    //   element vertex 1889
    //   property float x
    //   property float y
    //   property float z
    //   property float confidence
    //   property float intensity
    //   element face 3851
    //   property list uchar int vertex_indices
    //   end_header
    for (unsigned int count = 0; count != numberOfVerticesLoaded; count++)
    {
        // -0.036872 0.127727 0.00440925 0.850855 0.5 
        float discardThisValue = 0.0f;
        thePlyFile
            >> pVertices[count].x
            >> pVertices[count].y
            >> discardThisValue     // Z
            >> discardThisValue     // confidence
            >> discardThisValue;    // intensity

        pVertices[count].r = 1.0f;
        pVertices[count].g = 1.0f;
        pVertices[count].b = 1.0f;

    }


//    // Amazing code here
//    numberOfVerticesLoaded = 6;
//    // Allocate the array on the heap...
//    pVertices = new sVertex[numberOfVerticesLoaded];
//
//    /* triangle #1 */
//    // -0.09 to 0.06 in the x axis
//    pVertices[0] = { -0.6f, -0.4f, 0.0f, 0.0f, 1.0f };
//    pVertices[1] = {  0.6f, -0.4f, 0.0f, 1.0f, 0.0f };
//    pVertices[2] = {   0.f,  0.6f, 1.0f, 0.0f, 0.0f };
//    /* triangle #2 */
//    pVertices[3] = {  0.9f, -0.4f, 0.0f, 1.0f, 0.0f };
//    pVertices[4] = { -0.3f, -0.4f, 0.0f, 1.0f, 0.0f };
//    pVertices[5] = {  0.2f,  0.6f, 0.0f, 1.0f, 1.0f };

    thePlyFile.close();

    return true;
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

//    sVertex pVertices[1000000];
//    sVertex* pVertices = new sVertex[1000000];

    // TODO: Amazing code to load the bunny from file into this array above

    sVertex* pVertices = NULL;      // nullptr
    unsigned int numberOfVerticesLoaded = 0;
    if ( ! LoadPlyModelFromFile("assets/models/bun_zipper_res2.ply", pVertices, numberOfVerticesLoaded) )
    {
        std::cout << "Oh no! Model didn't load!" << std::endl;
        return -1;
    }

    unsigned int sizeOfVertexArrayInBytes = sizeof(sVertex) * numberOfVerticesLoaded;

    glGenBuffers(1, &vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeOfVertexArrayInBytes, pVertices, GL_STATIC_DRAW);
//    glBufferData(GL_ARRAY_BUFFER, sizeof(pVertices), pVertices, GL_STATIC_DRAW);

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
    glVertexAttribPointer(vpos_location, 
                          2, 
                          GL_FLOAT, 
                          GL_FALSE,
                          sizeof(pVertices[0]),         // sizeof(vertices[0]),
                          (void*)0);                         

    glEnableVertexAttribArray(vcol_location);
    glVertexAttribPointer(vcol_location, 
                          3, 
                          GL_FLOAT, 
                          GL_FALSE,
                          sizeof(pVertices[0]),         // sizeof(vertices[0]),
                          (void*)(sizeof(float) * 2));

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
        glDrawArrays(GL_TRIANGLES, 0, numberOfVerticesLoaded);
//        glDrawArrays(GL_TRIANGLES, 0, 6);

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
