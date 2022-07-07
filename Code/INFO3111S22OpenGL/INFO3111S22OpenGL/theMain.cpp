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
    float x, y, z;      // vPos  NOW WITH MORE Z!
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

bool LoadPlyModelFromFile(std::string fileName, sVertex* &pVertices, unsigned int &numberOfVerticesToDraw)
{
    unsigned int numberOfVerticesLoaded = 0;

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

 //   // Allocate the array on the heap...
 //   pVertices = new sVertex[numberOfVerticesLoaded];


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

// The triangles we want to draw are really AFTER the list of vertices.
// Later we will directly load an indexed model, but for now, the code
//  we have only handles as single vertex buffer (no "triangle" or 
//  "element" buffer). But it works, so let's use it, right?

// Here, I'm going to allocate a TEMPORARY array to hold the vertices
//  AND a temporary triangle ("element") array to hold the triangles. 
// Then, I'm going to read through the triangles and load the ACTUAL
//  vertex buffer (array) that the rest of the code will draw.

     //   // Allocate the array on the heap...
    sVertex* pTEMPVerticesInFile = new sVertex[numberOfVerticesLoaded];

    for (unsigned int count = 0; count != numberOfVerticesLoaded; count++)
    {
        // -0.036872 0.127727 0.00440925 // DON'T NEED THIS: 0.850855 0.5 
        float discardThisValue = 0.0f;
        thePlyFile
            >> pTEMPVerticesInFile[count].x
            >> pTEMPVerticesInFile[count].y
            >> pTEMPVerticesInFile[count].z;
//            >> discardThisValue;     // Z
//            >> discardThisValue     // confidence
//            >> discardThisValue;    // intensity

        unsigned int red, green, blue, alpha;
        thePlyFile >> red >> green >> blue >> alpha; 

        pTEMPVerticesInFile[count].r = (float)red/255.0f;
        pTEMPVerticesInFile[count].g = (float)green/255.0f;
        pTEMPVerticesInFile[count].b = (float)blue/255.0f;

    }

    // Now we load the triangle ("element") information
    // (the numbers after the list of vertices)
    // 
    // They look like this: 
    //      3 4812 4936 4891
    // 
    // The "3" indicates it's a triangle 
    // (instead of "1" for a point, "2" for a line, etc.
    // 

    // Allocate a triangle array
    struct sTriangle
    {
        // These are indexes into the vertex array we loaded earlier
        unsigned int vertexIndex0;
        unsigned int vertexIndex1;
        unsigned int vertexIndex2;
        // or this maybe?
        sVertex theVertices[3];
    };

    sTriangle* pTriangleArrayInFile = new sTriangle[numberOfTriangles];

    for (unsigned int count = 0; count != numberOfTriangles; count++)
    {
        // Same as the vertex loading...
        unsigned int discard = 0;   // For the leading "3"
        thePlyFile >> discard;
        
        thePlyFile
            >> pTriangleArrayInFile[count].vertexIndex0
            >> pTriangleArrayInFile[count].vertexIndex1
            >> pTriangleArrayInFile[count].vertexIndex2;
    }

    // Now we can "unroll" or "de-index" (Not sure what you'd 
    //  like to call it) the triangles to get a GIANT list 
    //  of vertices that the code can draw.
    //
    // Remember: this is only because that's the only thing our code
    //  CAN draw at the moment... (we'll change that)

    // The number of vertices TO DRAW is numberOfTriangles * 3
    // (3 vertices per triangle, right?)

    numberOfVerticesToDraw = numberOfTriangles * 3;

    pVertices = new sVertex[ numberOfVerticesToDraw ];

    // We'll start this outer index at the start of the array above...
    unsigned int vertexToDrawIndex = 0;

    for (unsigned int triIndex = 0; triIndex != numberOfTriangles; triIndex++)
    {
        // 3 301 358 300 


        pVertices[ vertexToDrawIndex + 0 ].x = pTEMPVerticesInFile[ pTriangleArrayInFile[triIndex].vertexIndex0 ].x;
        pVertices[ vertexToDrawIndex + 0 ].y = pTEMPVerticesInFile[ pTriangleArrayInFile[triIndex].vertexIndex0 ].y;
        pVertices[ vertexToDrawIndex + 0 ].z = pTEMPVerticesInFile[ pTriangleArrayInFile[triIndex].vertexIndex0 ].z;
        pVertices[vertexToDrawIndex + 0].r = pTEMPVerticesInFile[pTriangleArrayInFile[triIndex].vertexIndex0].r;
        pVertices[vertexToDrawIndex + 0].g = pTEMPVerticesInFile[pTriangleArrayInFile[triIndex].vertexIndex0].g;
        pVertices[vertexToDrawIndex + 0].b = pTEMPVerticesInFile[pTriangleArrayInFile[triIndex].vertexIndex0].b;

        pVertices[ vertexToDrawIndex + 1 ].x = pTEMPVerticesInFile[ pTriangleArrayInFile[triIndex].vertexIndex1 ].x;
        pVertices[ vertexToDrawIndex + 1 ].y = pTEMPVerticesInFile[ pTriangleArrayInFile[triIndex].vertexIndex1 ].y;
        pVertices[ vertexToDrawIndex + 1 ].z = pTEMPVerticesInFile[ pTriangleArrayInFile[triIndex].vertexIndex1 ].z;
        pVertices[vertexToDrawIndex + 1].r = pTEMPVerticesInFile[pTriangleArrayInFile[triIndex].vertexIndex1].r;
        pVertices[vertexToDrawIndex + 1].g = pTEMPVerticesInFile[pTriangleArrayInFile[triIndex].vertexIndex1].g;
        pVertices[vertexToDrawIndex + 1].b = pTEMPVerticesInFile[pTriangleArrayInFile[triIndex].vertexIndex1].b;

        pVertices[ vertexToDrawIndex + 2 ].x = pTEMPVerticesInFile[ pTriangleArrayInFile[triIndex].vertexIndex2 ].x;
        pVertices[ vertexToDrawIndex + 2 ].y = pTEMPVerticesInFile[ pTriangleArrayInFile[triIndex].vertexIndex2 ].y;
        pVertices[ vertexToDrawIndex + 2 ].z = pTEMPVerticesInFile[ pTriangleArrayInFile[triIndex].vertexIndex2 ].z;
        pVertices[vertexToDrawIndex + 2].r = pTEMPVerticesInFile[pTriangleArrayInFile[triIndex].vertexIndex2].r;
        pVertices[vertexToDrawIndex + 2].g = pTEMPVerticesInFile[pTriangleArrayInFile[triIndex].vertexIndex2].g;
        pVertices[vertexToDrawIndex + 2].b = pTEMPVerticesInFile[pTriangleArrayInFile[triIndex].vertexIndex2].b;

        // Next triangle is 3 vertices later in the array
        vertexToDrawIndex += 3;



//        // Here's another way we could have done it...
//        // 3 301 358 300 
//        sVertex& triVertex0 = pVertices[vertexToDrawIndex];
//        unsigned int indexOfVertex0 = pTriangleArrayInFile[triIndex].vertexIndex0;
//        triVertex0.x = pTEMPVerticesInFile[indexOfVertex0].x;
//        triVertex0.y = pTEMPVerticesInFile[indexOfVertex0].y;
//        vertexToDrawIndex++;
//
//        sVertex& triVertex1 = pVertices[vertexToDrawIndex];
//        unsigned int indexOfVertex1 = pTriangleArrayInFile[triIndex].vertexIndex1;
//        triVertex1.x = pTEMPVerticesInFile[indexOfVertex1].x;
//        triVertex1.y = pTEMPVerticesInFile[indexOfVertex1].y;
//        sVertex& triVertex0 = pVertices[vertexToDrawIndex];
//        vertexToDrawIndex++;
//
//        sVertex& triVertex2 = pVertices[vertexToDrawIndex];
//        unsigned int indexOfVertex2 = pTriangleArrayInFile[triIndex].vertexIndex2;
//        triVertex2.x = pTEMPVerticesInFile[indexOfVertex2].x;
//        triVertex2.y = pTEMPVerticesInFile[indexOfVertex2].y;
//        vertexToDrawIndex++;

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
    unsigned int numberOfVerticesToDraw = 0;
//    if ( ! LoadPlyModelFromFile("assets/models/bun_zipper_res2.ply", pVertices, numberOfVerticesToDraw) )
//    if ( ! LoadPlyModelFromFile("assets/models/bun_zipper_res2.xyz.ply", pVertices, numberOfVerticesToDraw) )
//    if ( ! LoadPlyModelFromFile("assets/models/cow_xyz_only.ply", pVertices, numberOfVerticesToDraw) )
    if ( ! LoadPlyModelFromFile("assets/models/cow_xyz_rgba.ply", pVertices, numberOfVerticesToDraw) )
    {
        std::cout << "Oh no! Model didn't load!" << std::endl;
        return -1;
    }

    unsigned int sizeOfVertexArrayInBytes = sizeof(sVertex) * numberOfVerticesToDraw;

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

//    struct sVertex
//    {
//        float x, y, z;      // vPos  NOW WITH MORE Z!
//        float r, g, b;      // vCol
//    };

    vpos_location = glGetAttribLocation(shaderProgramNumber, "vPos");
    vcol_location = glGetAttribLocation(shaderProgramNumber, "vCol");

    glEnableVertexAttribArray(vpos_location);
    glVertexAttribPointer(vpos_location,
                          3,            // 2,   3 because there's also an z
                          GL_FLOAT,
                          GL_FALSE,
                          sizeof(sVertex),      //sizeof(pVertices[0]),         // sizeof(vertices[0]),
                          (void*)offsetof(sVertex, x));    // (void*)0);

    glEnableVertexAttribArray(vcol_location);
    glVertexAttribPointer(vcol_location, 
                          3, 
                          GL_FLOAT, 
                          GL_FALSE,
                          sizeof(sVertex),      //sizeof(pVertices[0]),         // sizeof(vertices[0]),
                          (void*)offsetof(sVertex, r));    //(void*)(sizeof(float) * 2));

    while ( ! glfwWindowShouldClose(window) )
    {
        float ratio;
        int width, height;
//        mat4x4 m, p, mvp;     // linmath.h
        glm::mat4x4 m, p, mvp;

        glfwGetFramebufferSize(window, &width, &height);
        ratio = width / (float)height;

        glViewport(0, 0, width, height);
//        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(0.0f, 0.3f, 1.0f, 1.0f);       // Clear screen to blue colour
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


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


        // This turns this on and off
        glEnable(GL_DEPTH_TEST);

        // Remove "back facing" triangles (just doesn't draw them)
        glCullFace(GL_BACK);


        // Change the polygon mode (i.e. how the triangles are filled in)
        // GL_POINT, GL_LINE, and GL_FILL
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        // GL_LINE_LOOP, GL_POINTS, or GL_TRIANGLES
        glDrawArrays(GL_TRIANGLES, 0, numberOfVerticesToDraw);
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
