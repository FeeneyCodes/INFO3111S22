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
#include "cVAOManager.h"
#include "cLightManager.h"


void PlaceAModelInARandomLocation(std::string modelName, float minScale, float maxScale);

void CopyLightInformationToShader(cLightManager* pTheLightManager, GLuint shaderID);

//struct sVertex
//{
//    float x, y, z;      // vPos  NOW WITH MORE Z!
//    float r, g, b;      // vCol
//};
// 
// See globalStuff.h for declaration
std::vector< cMesh* > g_vec_pMeshesToDraw;

cMesh* pMeshSphere = NULL;

cLightManager* pTheLightManager = NULL;


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


//    if ( mods == GLFW_MOD_SHIFT )       // ONLY the shift and NO OTHER MODIFIERS
//    if ( ( mods & GLFW_MOD_SHIFT ) == GLFW_MOD_SHIFT )      // The shift and maybe other modifers, too
//        0000111
//        0000001 
//        -------
//        0000001

    const float cameraMovementSpeed = 1.0f;
    const float objectMovementSpeed = 0.5f;
    const float objectRotationSpeed = glm::radians(1.0f);

    // Nudge, nudge, wink, wink - Hey, why not change this value with some other keys...
    //  then maybe you can "select" all the objects?? Huh??? Isn't THAT neat??
    unsigned int objectIDToMove = 2;

    if (key == GLFW_KEY_LEFT)
    {

    }

    if (mods == GLFW_MOD_SHIFT)
    {
        // Move the object
        if (key == GLFW_KEY_A) { ::g_vec_pMeshesToDraw[objectIDToMove]->XYZLocation.x -= objectMovementSpeed; }
        if (key == GLFW_KEY_D) { ::g_vec_pMeshesToDraw[objectIDToMove]->XYZLocation.x += objectMovementSpeed; }
        if (key == GLFW_KEY_W) { ::g_vec_pMeshesToDraw[objectIDToMove]->XYZLocation.z -= objectMovementSpeed; }
        if (key == GLFW_KEY_S) { ::g_vec_pMeshesToDraw[objectIDToMove]->XYZLocation.z += objectMovementSpeed; }
        if (key == GLFW_KEY_Q) { ::g_vec_pMeshesToDraw[objectIDToMove]->XYZLocation.y -= objectMovementSpeed; }
        if (key == GLFW_KEY_E) { ::g_vec_pMeshesToDraw[objectIDToMove]->XYZLocation.y += objectMovementSpeed; }
    }   
    else if (mods == GLFW_MOD_CONTROL)
    {
        // Move the object
        if (key == GLFW_KEY_A) { ::g_vec_pMeshesToDraw[objectIDToMove]->orientationEulerAngle.x -= objectRotationSpeed; }
        if (key == GLFW_KEY_D) { ::g_vec_pMeshesToDraw[objectIDToMove]->orientationEulerAngle.x += objectRotationSpeed; }
        if (key == GLFW_KEY_W) { ::g_vec_pMeshesToDraw[objectIDToMove]->orientationEulerAngle.z -= objectRotationSpeed; }
        if (key == GLFW_KEY_S) { ::g_vec_pMeshesToDraw[objectIDToMove]->orientationEulerAngle.z += objectRotationSpeed; }
        if (key == GLFW_KEY_Q) { ::g_vec_pMeshesToDraw[objectIDToMove]->orientationEulerAngle.y -= objectRotationSpeed; }
        if (key == GLFW_KEY_E) { ::g_vec_pMeshesToDraw[objectIDToMove]->orientationEulerAngle.y += objectRotationSpeed; }
    }
    else if (mods == GLFW_MOD_ALT)
    {
        // Move the object
        if (key == GLFW_KEY_A) { pMeshSphere->XYZLocation.x -= objectMovementSpeed; }
        if (key == GLFW_KEY_D) { pMeshSphere->XYZLocation.x += objectMovementSpeed; }
        if (key == GLFW_KEY_W) { pMeshSphere->XYZLocation.z -= objectMovementSpeed; }
        if (key == GLFW_KEY_S) { pMeshSphere->XYZLocation.z += objectMovementSpeed; }
        if (key == GLFW_KEY_Q) { pMeshSphere->XYZLocation.y -= objectMovementSpeed; }
        if (key == GLFW_KEY_E) { pMeshSphere->XYZLocation.y += objectMovementSpeed; }

        // Linear attenuation
        if (key == GLFW_KEY_1) {
            pTheLightManager->theLights[0].atten.y *= 0.99f;
        }
        if (key == GLFW_KEY_2) {
            pTheLightManager->theLights[0].atten.y *= 1.01f;
        }
        // Quadradic attenuation
        if (key == GLFW_KEY_3) {
            pTheLightManager->theLights[0].atten.z *= 0.99f;
        }
        if (key == GLFW_KEY_4) {
            pTheLightManager->theLights[0].atten.z *= 1.01f;
        }
    }
    else
    {
        // NO modifier keys are pressed

        // When we press #8, a airplane is added to the scene
        if (key == GLFW_KEY_8)
        {
            PlaceAModelInARandomLocation("assets/models/mig29_xyz_n_rgba_uv.ply", 1.0f, 5.0f);
        }

        // Saves the scene to a file
        if ( key == GLFW_KEY_9 && action == GLFW_PRESS )
        {
            SaveSceneToFile("MyAmazingScene.txt");
            std::cout << "Saved info. to file" << std::endl;
        }



        // Camera control
        if ( key == GLFW_KEY_A )
        {
            ::g_cameraEye.x -= cameraMovementSpeed;     // "left"
    //        ::g_vec_pMeshesToDraw[4]->XYZLocation.x -= cameraMovementSpeed;     // "left"
        }
        if ( key == GLFW_KEY_D )
        {
            ::g_cameraEye.x += cameraMovementSpeed;     // "right"
    //        ::g_vec_pMeshesToDraw[4]->XYZLocation.x += cameraMovementSpeed;     // "left"
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
        }        // We AREN'T holding JUST the shift key
    }//if (mods == GLFW_MOD_SHIFT)



}

// Function to load the bunny ply format model
// "bunny.ply"
// sVertex* pVertices;
// unsigned int numberOfVerticesLoaded;



int main(void)
{
    GLFWwindow* window;
//    GLuint vertex_buffer_for_cow;          Now handled by the VAOManager
//    GLuint vertex_buffer_for_spider;       Now handled by the VAOManager
    //GLuint vertex_shader;
    //GLuint fragment_shader;
    GLuint shaderProgramNumber = 0;     
//    GLint mvp_location;
//    GLint vpos_location;                  Now handled by the VAOManager
//    GLint vcol_location;                  Now handled by the VAOManager

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
//
//    // TODO: Amazing code to load the bunny from file into this array above
//
//    // START OF: Loading the cow
//    sVertex* pCowVertices = NULL;      // nullptr
//    unsigned int numberOfCowVerticesToDraw = 0;
//    if ( ! LoadPlyModelFromFile("assets/models/cow_xyz_only.ply", pCowVertices, numberOfCowVerticesToDraw) )
//    {
//        std::cout << "Oh no! Model didn't load!" << std::endl;
//        return -1;
//    }
//
//    unsigned int sizeOfCowVertexArrayInBytes = sizeof(sVertex) * numberOfCowVerticesToDraw;
//
//    glGenBuffers(1, &vertex_buffer_for_cow);
//    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_for_cow);
//    glBufferData(GL_ARRAY_BUFFER, sizeOfCowVertexArrayInBytes, pCowVertices, GL_STATIC_DRAW);
////    glBufferData(GL_ARRAY_BUFFER, sizeof(pVertices), pVertices, GL_STATIC_DRAW);
//
//    delete [] pCowVertices;
//    // END OF: Loading the cow
//
////    spider_mastermind.bmd6model.fbx.ascii.xyz.ply
//    // START OF: Loading the spider
//    sVertex* pSpiderVertices = NULL;      // nullptr
//    unsigned int numberOfSpiderVerticesToDraw = 0;
//    if (!LoadPlyModelFromFile("assets/models/spider_mastermind.bmd6model.fbx.ascii.xyz.ply", pSpiderVertices, numberOfSpiderVerticesToDraw))
//    {
//        std::cout << "Oh no! Model didn't load!" << std::endl;
//        return -1;
//    }
//
//    unsigned int sizeOfSpiderVertexArrayInBytes = sizeof(sVertex) * numberOfSpiderVerticesToDraw;
//
//    glGenBuffers(1, &vertex_buffer_for_spider);
//    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_for_spider);
//    glBufferData(GL_ARRAY_BUFFER, sizeOfSpiderVertexArrayInBytes, pSpiderVertices, GL_STATIC_DRAW);
//    //    glBufferData(GL_ARRAY_BUFFER, sizeof(pVertices), pVertices, GL_STATIC_DRAW);
//
//    delete[] pSpiderVertices;
//    // END OF: Loading the spider



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
//
    //fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    //glShaderSource(fragment_shader, 1, &fragment_shader_text, NULL);
    //glCompileShader(fragment_shader);
//
    //program = glCreateProgram();
    //glAttachShader(program, vertex_shader);
    //glAttachShader(program, fragment_shader);
    //glLinkProgram(program);

//    mvp_location = glGetUniformLocation(shaderProgramNumber, "MVP");

//    struct sVertex
//    {
//        float x, y, z;      // vPos  NOW WITH MORE Z!
//        float r, g, b;      // vCol
//    };

//    vpos_location = glGetAttribLocation(shaderProgramNumber, "vPos");
//    vcol_location = glGetAttribLocation(shaderProgramNumber, "vCol");
//
//    glEnableVertexAttribArray(vpos_location);
//    glVertexAttribPointer(vpos_location,
//                          3,            // 2,   3 because there's also an z
//                          GL_FLOAT,
//                          GL_FALSE,
//                          sizeof(sVertex),      //sizeof(pVertices[0]),         // sizeof(vertices[0]),
//                          (void*)offsetof(sVertex, x));    // (void*)0);
//
//    glEnableVertexAttribArray(vcol_location);
//    glVertexAttribPointer(vcol_location,
//                          3,
//                          GL_FLOAT,
//                          GL_FALSE,
//                          sizeof(sVertex),      //sizeof(pVertices[0]),         // sizeof(vertices[0]),
//                          (void*)offsetof(sVertex, r));    //(void*)(sizeof(float) * 2));
//

    // Load some models
    cVAOManager* pVAOManager = new cVAOManager();

    std::string sceneLoadingErrors;
    if ( ! LoadAllTheModels("assets/FeeneyAmazingScene_01.txt", pVAOManager, shaderProgramNumber, sceneLoadingErrors) )
    {
        std::cout << "Something went wrong: " << sceneLoadingErrors << std::endl;
    }
    else
    {
        std::cout << "Scene loaded OK" << std::endl;
    }

//    sModelDrawInfo bunnyDrawInfo;
//    pVAOManager->LoadModelIntoVAO("assets/models/bun_zipper.xyz.ply", bunnyDrawInfo, shaderProgramNumber);
//
//    sModelDrawInfo cowDrawInfo;
//    pVAOManager->LoadModelIntoVAO("assets/models/cow_xyz_only.ply", cowDrawInfo, shaderProgramNumber);
//
//    sModelDrawInfo spiderDrawInfo;
//    pVAOManager->LoadModelIntoVAO("assets/models/spider_mastermind.bmd6model.fbx.ascii.xyz.ply", spiderDrawInfo, shaderProgramNumber);
//
//    sModelDrawInfo seafloorDrawInfo;
//    pVAOManager->LoadModelIntoVAO("assets/models/Seafloor2_xyz.ply", seafloorDrawInfo, shaderProgramNumber);


    // Add these objects to the scene we are going to draw
//   cMesh* pBunny = new cMesh();
//   pBunny->meshFileName = "assets/models/bun_zipper.xyz.ply";
//   ::g_vec_pMeshesToDraw.push_back(pBunny);
//
//   cMesh* pCow = new cMesh();
//   pCow->meshFileName = "assets/models/cow_xyz_only.ply";
//   ::g_vec_pMeshesToDraw.push_back(pCow);

    pMeshSphere = new cMesh();
    pMeshSphere->RGBA = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    pMeshSphere->XYZLocation = glm::vec3(0.0f, 15.0f, 0.0f);
    pMeshSphere->meshFileName = "assets/models/Isoshphere_flat_4div_xyz_n_rgba_uv.ply";
    pMeshSphere->bIsWireframe = true;
    ::g_vec_pMeshesToDraw.push_back(pMeshSphere);


    cMesh* pSpider1 = new cMesh();
    pSpider1->XYZLocation.x = -5.07f;
    pSpider1->orientationEulerAngle.x = glm::radians(-137.5f);
    pSpider1->orientationEulerAngle.z = glm::radians(+8.14f);
    pSpider1->RGBA = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
    pSpider1->meshFileName = "assets/models/mig29_xyz_n_rgba_uv.ply";
    ::g_vec_pMeshesToDraw.push_back(pSpider1);

    cMesh* pSpider2 = new cMesh();
    pSpider2->XYZLocation.x = +5.0f;
    pSpider2->XYZLocation.y = 2.0f;
    pSpider2->RGBA = glm::vec4(0.0f, 1.0f, 1.0f, 1.0f);
    pSpider2->meshFileName = "assets/models/mig29_xyz_n_rgba_uv.ply";
    pSpider2->overallScale = 3.0f;
//    pSpider2->bIsWireframe = true;      // glPolygonMode() will be GL_LINE
    ::g_vec_pMeshesToDraw.push_back(pSpider2);

    cMesh* pCow = new cMesh();
    pCow->XYZLocation.y = -2.0f;
    pCow->orientationEulerAngle.y = glm::radians(-145.0f);
    pCow->overallScale = 0.5f;
    pCow->RGBA = glm::vec4(0.5f, 1.0f, 0.5f, 1.0f);
//    pCow->meshFileName = "assets/models/cow_xyz_only.ply";
    pCow->meshFileName = "assets/models/cow_xyz_n_rgba_uv.ply";
    ::g_vec_pMeshesToDraw.push_back(pCow);

    cMesh* pSeaFloor = new cMesh();
    pSeaFloor->XYZLocation.y = -15.0f;
    // https://www.colorhexa.com/c2b280
    pSeaFloor->RGBA = glm::vec4(76.0f/255.0f, 70.0f/255.0f, 50.0f/255.0f, 1.0f);
    pSeaFloor->meshFileName = "assets/models/Seafloor2_xyz_n_rgba_uv.ply";
    ::g_vec_pMeshesToDraw.push_back(pSeaFloor);

    // Make a bunch of airplanes
    for (unsigned int count = 0; count != 1000; count++)
    {
        PlaceAModelInARandomLocation("assets/models/mig29_xyz_n_rgba_uv.ply", 1.0f, 5.0f);
    }

    //cMesh* pDeLorean = new cMesh();
    //pDeLorean->meshFileName = "assets/models/de--lorean_xyz_n_rgba_uv.ply";
    //pDeLorean->bIsWireframe = true;
    //pDeLorean->orientationEulerAngle.x = glm::radians(-90.0f);
    //pDeLorean->overallScale = 0.5f;
    //pDeLorean->XYZLocation.x = -10.0f;
    //::g_vec_pMeshesToDraw.push_back(pDeLorean);


    pTheLightManager = new cLightManager();

    pTheLightManager->theLights[0].position = glm::vec4(pMeshSphere->XYZLocation, 1.0f);
//    pTheLightManager->theLights[0].position = glm::vec4(0.0f, 15.0f, 0.0f, 1.0f);
    pTheLightManager->theLights[0].diffuse = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

    pTheLightManager->theLights[0].atten.x = 0.0f;  // Constant
    pTheLightManager->theLights[0].atten.y = 0.05f;  // Linear attenuation
    pTheLightManager->theLights[0].atten.z = 0.01f;  // Quadratic attenuation

    pTheLightManager->theLights[0].param2.x = 1.0f;  // Turn the light on



    while ( ! glfwWindowShouldClose(window) )
    {
        float ratio;
        int width, height;
//        mat4x4 m, p, mvp;     // linmath.h

        glm::mat4x4 matModel = glm::mat4(1.0f);
        glm::mat4x4 matProjection = glm::mat4(1.0f);
        glm::mat4x4 matView = glm::mat4(1.0f);

        glm::mat4x4 mvp = glm::mat4(1.0f);

        glfwGetFramebufferSize(window, &width, &height);
        ratio = width / (float)height;

        glViewport(0, 0, width, height);
//        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(0.0f, 0.3f, 3.0f, 1.0f);       // Clear screen to blue colour
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        
//        mat4x4_ortho(p, -ratio, ratio, -1.f, 1.f, 1.f, -1.f);
        matProjection = glm::perspective(0.6f,
                             ratio,
                             0.1f,
                             1000.0f);

//        glm::vec3 cameraEye = glm::vec3(0.0, 0.0, -4.0f);
        glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
        glm::vec3 upVector = glm::vec3(0.0f, 1.0f, 0.0f);

        //        mat4x4_mul(mvp, p, m);
        
        matView = glm::lookAt( ::g_cameraEye,
                               cameraTarget,
                               upVector);


//        glUseProgram(program);
        glUseProgram(shaderProgramNumber);


//        ::g_vec_pMeshesToDraw[4]->XYZLocation.z += 0.01f;

        // Place the light wherever the ball is...
        pTheLightManager->theLights[0].position = glm::vec4(pMeshSphere->XYZLocation, 1.0f);


        // Copy the lighting information to the shader
        CopyLightInformationToShader(pTheLightManager, shaderProgramNumber);



        // Loop through the mesh objects in the scene, drawing each one
        for (unsigned int index = 0; index != ::g_vec_pMeshesToDraw.size(); index++)
        {
            sModelDrawInfo modelDrawingInfo;
            cMesh* pCurrentMesh = ::g_vec_pMeshesToDraw[index];

            //        mat4x4_identity(m);
            matModel = glm::mat4x4(1.0f);

            //        mat4x4_rotate_Z(m, m, (float)glfwGetTime());
            glm::mat4 rotateX = glm::rotate(glm::mat4(1.0f),
                                            pCurrentMesh->orientationEulerAngle.x,
                                            glm::vec3(1.0f, 0.0f, 0.0f));

            glm::mat4 rotateY = glm::rotate(glm::mat4(1.0f),
                                            pCurrentMesh->orientationEulerAngle.y,           //glm::radians(147.0f),  
                                            glm::vec3(0.0f, 1.0f, 0.0f));

            glm::mat4 rotateZ = glm::rotate(glm::mat4(1.0f),
                                            pCurrentMesh->orientationEulerAngle.z,  // (float)glfwGetTime(),           //glm::radians(147.0f),  
                                            glm::vec3(0.0f, 0.0, 1.0f));

            glm::mat4 matTranslate = glm::translate(glm::mat4(1.0f),
                                                    glm::vec3(pCurrentMesh->XYZLocation.x, 
                                                              pCurrentMesh->XYZLocation.y,
                                                              pCurrentMesh->XYZLocation.z));

            float scale = pCurrentMesh->overallScale;
            glm::mat4 matScale = glm::scale(glm::mat4(1.0f),
                                            glm::vec3(scale, scale, scale));


            matModel = matModel * matTranslate;            
            matModel = matModel * rotateX;
            matModel = matModel * rotateY;
            matModel = matModel * rotateZ;
            matModel = matModel * matScale;


//            //mat4x4_mul(mvp, p, m);
//            mvp = matProjection * matView * matModel;
//
//    //        glUniformMatrix4fv(mvp_location, 1, GL_FALSE, (const GLfloat*)mvp);
//            GLint mvp_location = glGetUniformLocation(shaderProgramNumber, "MVP");
//
//            glUniformMatrix4fv(mvp_location, 1, GL_FALSE, glm::value_ptr(mvp));

            //uniform mat4 mModel;
            //uniform mat4 mView;
            //uniform mat4 mProj;

            GLint mModel_location = glGetUniformLocation(shaderProgramNumber, "mModel");
            GLint mView_location = glGetUniformLocation(shaderProgramNumber, "mView");
            GLint mProj_location = glGetUniformLocation(shaderProgramNumber, "mProj");

            glUniformMatrix4fv(mModel_location, 1, GL_FALSE, glm::value_ptr(matModel));
            glUniformMatrix4fv(mView_location, 1, GL_FALSE, glm::value_ptr(matView));
            glUniformMatrix4fv(mProj_location, 1, GL_FALSE, glm::value_ptr(matProjection));

            // Get the objectColourRGBA uniform (register) location inside the shader
            GLint objectColourRGBA_UniLoc = glGetUniformLocation(shaderProgramNumber, "objectColourRGBA");

            glUniform4f( objectColourRGBA_UniLoc, 
                         pCurrentMesh->RGBA.r,
                         pCurrentMesh->RGBA.g, 
                         pCurrentMesh->RGBA.b, 
                         pCurrentMesh->RGBA.a);

            // This turns this on and off
            glEnable(GL_DEPTH_TEST);

            // Remove "back facing" triangles (just doesn't draw them)
            glCullFace(GL_BACK);


            // Change the polygon mode (i.e. how the triangles are filled in)
            // GL_POINT, GL_LINE, and GL_FILL
            if (pCurrentMesh->bIsWireframe)
            {
                // Draw only the lines...
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            }
            else
            {
                // "Fill" in the triangles ("solid" colour)
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            }

            // GL_LINE_LOOP, GL_POINTS, or GL_TRIANGLES
    //        glDrawArrays(GL_TRIANGLES, 0, numberOfCowVerticesToDraw);
    //        glDrawArrays(GL_TRIANGLES, 0, 3);


            if ( pVAOManager->FindDrawInfoByModelName(pCurrentMesh->meshFileName, modelDrawingInfo) )
            {
                // enable VAO(and everything else)
                glBindVertexArray(modelDrawingInfo.VAO_ID);
                glDrawElements(GL_TRIANGLES, modelDrawingInfo.numberOfIndices, GL_UNSIGNED_INT, (void*)0);
                //disable VAO(and everything else)
                glBindVertexArray(0); 			
            }
            
        }//for (unsigned int index = 0



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

    }// while ( ! glfwWindowShouldClose(window) )

    // Save my scene...


    glfwDestroyWindow(window);

    // Clean up
    delete pShaderManager;
    delete pVAOManager;
    delete pTheLightManager;

    glfwTerminate();
    //exit(EXIT_SUCCESS);
    return 0;
}





void PlaceAModelInARandomLocation(std::string modelName, float minScale, float maxScale)
{
    cMesh* pDeLorean = new cMesh();
    pDeLorean->meshFileName = modelName;// "assets/models/mig29_xyz_n_rgba_uv.ply";
//    pDeLorean->bIsWireframe = true;
    pDeLorean->orientationEulerAngle.x = getRandBetween(-3.14159f, 3.14159f);
    pDeLorean->orientationEulerAngle.y = getRandBetween(-3.14159f, 3.14159f);
    pDeLorean->orientationEulerAngle.z = getRandBetween(-3.14159f, 3.14159f);
    // Maybe random colours, too
    pDeLorean->RGBA.r = getRandBetween(0.0f, 1.0f);
    pDeLorean->RGBA.g = getRandBetween(0.0f, 1.0f);
    pDeLorean->RGBA.b = getRandBetween(0.0f, 1.0f);
    pDeLorean->overallScale = getRandBetween(minScale, maxScale);
    pDeLorean->XYZLocation.x = getRandBetween(-100.0f, 100.0f);
    pDeLorean->XYZLocation.y = getRandBetween(-100.0f, 100.0f);
    pDeLorean->XYZLocation.z = getRandBetween(-100.0f, 100.0f);
    ::g_vec_pMeshesToDraw.push_back(pDeLorean);
    return;
}




void CopyLightInformationToShader(cLightManager* pTheLightManager, GLuint shaderID)
{
    GLint position_UniLoc = glGetUniformLocation(shaderID, "theLights[0].position");
    glUniform4f(position_UniLoc,
                pTheLightManager->theLights[0].position.x,
                pTheLightManager->theLights[0].position.y,
                pTheLightManager->theLights[0].position.z,
                pTheLightManager->theLights[0].position.w);

    GLint diffuse_UniLoc = glGetUniformLocation(shaderID, "theLights[0].diffuse");
    glUniform4f(diffuse_UniLoc,
                pTheLightManager->theLights[0].diffuse.r,
                pTheLightManager->theLights[0].diffuse.g,
                pTheLightManager->theLights[0].diffuse.b,
                pTheLightManager->theLights[0].diffuse.a);

    GLint specular_UniLoc = glGetUniformLocation(shaderID, "theLights[0].specular");
    glUniform4f(specular_UniLoc,
                pTheLightManager->theLights[0].specular.r,
                pTheLightManager->theLights[0].specular.g,
                pTheLightManager->theLights[0].specular.b,
                pTheLightManager->theLights[0].specular.a);

    GLint atten_UniLoc = glGetUniformLocation(shaderID, "theLights[0].atten");
    glUniform4f(atten_UniLoc,
                pTheLightManager->theLights[0].atten.r,
                pTheLightManager->theLights[0].atten.g,
                pTheLightManager->theLights[0].atten.b,
                pTheLightManager->theLights[0].atten.a);

    GLint direction_UniLoc = glGetUniformLocation(shaderID, "theLights[0].direction");
    glUniform4f(direction_UniLoc,
                pTheLightManager->theLights[0].direction.r,
                pTheLightManager->theLights[0].direction.g,
                pTheLightManager->theLights[0].direction.b,
                pTheLightManager->theLights[0].direction.a);

    GLint param1_UniLoc = glGetUniformLocation(shaderID, "theLights[0].param1");
    glUniform4f(param1_UniLoc,
                pTheLightManager->theLights[0].param1.x,
                pTheLightManager->theLights[0].param1.y,
                pTheLightManager->theLights[0].param1.z,
                pTheLightManager->theLights[0].param1.w);

    GLint param2_UniLoc = glGetUniformLocation(shaderID, "theLights[0].param2");
    glUniform4f(param2_UniLoc,
                pTheLightManager->theLights[0].param2.x,
                pTheLightManager->theLights[0].param2.y,
                pTheLightManager->theLights[0].param2.z,
                pTheLightManager->theLights[0].param2.w);

    return;
}






// ************************************************
// Code we aren't using anymore

/*

struct sPLYFileStuff
{
    std::string fileName;
    sVertex* pVertices;
    unsigned int numberOfVerticesToDraw;
};
bool LoadPlyModelFromFile(sPLYFileStuff& theDrawStuff);
bool LoadPlyModelFromFile(std::string fileName, sVertex*& pVertices, unsigned int& numberOfVerticesToDraw)
{
    unsigned int numberOfVerticesLoaded = 0;

    std::ifstream thePlyFile(fileName.c_str());
    if (!thePlyFile.is_open())
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

          //      unsigned int red, green, blue, alpha;
          //      thePlyFile >> red >> green >> blue >> alpha; 

          //      pTEMPVerticesInFile[count].r = (float)red/255.0f;
          //      pTEMPVerticesInFile[count].g = (float)green/255.0f;
          //      pTEMPVerticesInFile[count].b = (float)blue/255.0f;

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

    pVertices = new sVertex[numberOfVerticesToDraw];

    // We'll start this outer index at the start of the array above...
    unsigned int vertexToDrawIndex = 0;

    for (unsigned int triIndex = 0; triIndex != numberOfTriangles; triIndex++)
    {
        // 3 301 358 300 


        pVertices[vertexToDrawIndex + 0].x = pTEMPVerticesInFile[pTriangleArrayInFile[triIndex].vertexIndex0].x;
        pVertices[vertexToDrawIndex + 0].y = pTEMPVerticesInFile[pTriangleArrayInFile[triIndex].vertexIndex0].y;
        pVertices[vertexToDrawIndex + 0].z = pTEMPVerticesInFile[pTriangleArrayInFile[triIndex].vertexIndex0].z;
        pVertices[vertexToDrawIndex + 0].r = 1.0f;
        pVertices[vertexToDrawIndex + 0].g = 1.0f;
        pVertices[vertexToDrawIndex + 0].b = 1.0f;

        pVertices[vertexToDrawIndex + 1].x = pTEMPVerticesInFile[pTriangleArrayInFile[triIndex].vertexIndex1].x;
        pVertices[vertexToDrawIndex + 1].y = pTEMPVerticesInFile[pTriangleArrayInFile[triIndex].vertexIndex1].y;
        pVertices[vertexToDrawIndex + 1].z = pTEMPVerticesInFile[pTriangleArrayInFile[triIndex].vertexIndex1].z;
        pVertices[vertexToDrawIndex + 1].r = 1.0f;
        pVertices[vertexToDrawIndex + 1].g = 1.0f;
        pVertices[vertexToDrawIndex + 1].b = 1.0f;

        pVertices[vertexToDrawIndex + 2].x = pTEMPVerticesInFile[pTriangleArrayInFile[triIndex].vertexIndex2].x;
        pVertices[vertexToDrawIndex + 2].y = pTEMPVerticesInFile[pTriangleArrayInFile[triIndex].vertexIndex2].y;
        pVertices[vertexToDrawIndex + 2].z = pTEMPVerticesInFile[pTriangleArrayInFile[triIndex].vertexIndex2].z;
        pVertices[vertexToDrawIndex + 2].r = 1.0f;
        pVertices[vertexToDrawIndex + 2].g = 1.0f;
        pVertices[vertexToDrawIndex + 2].b = 1.0f;
//
        // Next triangle is 3 vertices later in the array
        vertexToDrawIndex += 3;
//
//
//
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
//
    }
//
//
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
//
//    thePlyFile.close();
//
//    return true;
//}
//bool LoadPlyModelFromFile(std::string fileName, sVertex*& pVertices, unsigned int& numberOfVerticesToDraw)
//{
//    unsigned int numberOfVerticesLoaded = 0;
//
//    std::ifstream thePlyFile(fileName.c_str());
//    if (!thePlyFile.is_open())
//    {
//        return false;
//    }
//
//    std::string token;
//    while (thePlyFile >> token)
//    {
//        if (token == "vertex")
//        {
//            break;
//        }
//    }
//    // The next thing is the number of vertices.
//    thePlyFile >> numberOfVerticesLoaded;
//
//    //   // Allocate the array on the heap...
//    //   pVertices = new sVertex[numberOfVerticesLoaded];
//
//
//    while (thePlyFile >> token)
//    {
//        if (token == "face")
//        {
//            break;
//        }
//    }
//    // Next is the number of "Faces" (aka "triangles")
//    unsigned int numberOfTriangles = 0;
//    thePlyFile >> numberOfTriangles;
//
//
//    while (thePlyFile >> token)
//    {
//        if (token == "end_header")
//        {
//            break;
//        }
//    }
//
//    // This particular ply file (the O.G. bunny one has this header:
//    //   ply
//    //   format ascii 1.0
//    //   comment zipper output
//    //   element vertex 1889
//    //   property float x
//    //   property float y
//    //   property float z
//    //   property float confidence
//    //   property float intensity
//    //   element face 3851
//    //   property list uchar int vertex_indices
//    //   end_header
//
//// The triangles we want to draw are really AFTER the list of vertices.
//// Later we will directly load an indexed model, but for now, the code
////  we have only handles as single vertex buffer (no "triangle" or 
////  "element" buffer). But it works, so let's use it, right?
//
//// Here, I'm going to allocate a TEMPORARY array to hold the vertices
////  AND a temporary triangle ("element") array to hold the triangles. 
//// Then, I'm going to read through the triangles and load the ACTUAL
////  vertex buffer (array) that the rest of the code will draw.
//
//     //   // Allocate the array on the heap...
//    sVertex* pTEMPVerticesInFile = new sVertex[numberOfVerticesLoaded];
//
//    for (unsigned int count = 0; count != numberOfVerticesLoaded; count++)
//    {
//        // -0.036872 0.127727 0.00440925 // DON'T NEED THIS: 0.850855 0.5 
//        float discardThisValue = 0.0f;
//        thePlyFile
//            >> pTEMPVerticesInFile[count].x
//            >> pTEMPVerticesInFile[count].y
//            >> pTEMPVerticesInFile[count].z;
//        //            >> discardThisValue;     // Z
//        //            >> discardThisValue     // confidence
//        //            >> discardThisValue;    // intensity
//
//          //      unsigned int red, green, blue, alpha;
//          //      thePlyFile >> red >> green >> blue >> alpha; 
//
//          //      pTEMPVerticesInFile[count].r = (float)red/255.0f;
//          //      pTEMPVerticesInFile[count].g = (float)green/255.0f;
//          //      pTEMPVerticesInFile[count].b = (float)blue/255.0f;
//
//    }
//
//    // Now we load the triangle ("element") information
//    // (the numbers after the list of vertices)
//    // 
//    // They look like this: 
//    //      3 4812 4936 4891
//    // 
//    // The "3" indicates it's a triangle 
//    // (instead of "1" for a point, "2" for a line, etc.
//    // 
//
//    // Allocate a triangle array
//    struct sTriangle
//    {
//        // These are indexes into the vertex array we loaded earlier
//        unsigned int vertexIndex0;
//        unsigned int vertexIndex1;
//        unsigned int vertexIndex2;
//        // or this maybe?
//        sVertex theVertices[3];
//    };
//
//    sTriangle* pTriangleArrayInFile = new sTriangle[numberOfTriangles];
//
//    for (unsigned int count = 0; count != numberOfTriangles; count++)
//    {
//        // Same as the vertex loading...
//        unsigned int discard = 0;   // For the leading "3"
//        thePlyFile >> discard;
//
//        thePlyFile
//            >> pTriangleArrayInFile[count].vertexIndex0
//            >> pTriangleArrayInFile[count].vertexIndex1
//            >> pTriangleArrayInFile[count].vertexIndex2;
//    }
//
//    // Now we can "unroll" or "de-index" (Not sure what you'd 
//    //  like to call it) the triangles to get a GIANT list 
//    //  of vertices that the code can draw.
//    //
//    // Remember: this is only because that's the only thing our code
//    //  CAN draw at the moment... (we'll change that)
//
//    // The number of vertices TO DRAW is numberOfTriangles * 3
//    // (3 vertices per triangle, right?)
//
//    numberOfVerticesToDraw = numberOfTriangles * 3;
//
//    pVertices = new sVertex[numberOfVerticesToDraw];
//
//    // We'll start this outer index at the start of the array above...
//    unsigned int vertexToDrawIndex = 0;
//
//    for (unsigned int triIndex = 0; triIndex != numberOfTriangles; triIndex++)
//    {
//        // 3 301 358 300 
//
//
//        pVertices[vertexToDrawIndex + 0].x = pTEMPVerticesInFile[pTriangleArrayInFile[triIndex].vertexIndex0].x;
//        pVertices[vertexToDrawIndex + 0].y = pTEMPVerticesInFile[pTriangleArrayInFile[triIndex].vertexIndex0].y;
//        pVertices[vertexToDrawIndex + 0].z = pTEMPVerticesInFile[pTriangleArrayInFile[triIndex].vertexIndex0].z;
//        pVertices[vertexToDrawIndex + 0].r = 1.0f;
//        pVertices[vertexToDrawIndex + 0].g = 1.0f;
//        pVertices[vertexToDrawIndex + 0].b = 1.0f;
//
//        pVertices[vertexToDrawIndex + 1].x = pTEMPVerticesInFile[pTriangleArrayInFile[triIndex].vertexIndex1].x;
//        pVertices[vertexToDrawIndex + 1].y = pTEMPVerticesInFile[pTriangleArrayInFile[triIndex].vertexIndex1].y;
//        pVertices[vertexToDrawIndex + 1].z = pTEMPVerticesInFile[pTriangleArrayInFile[triIndex].vertexIndex1].z;
//        pVertices[vertexToDrawIndex + 1].r = 1.0f;
//        pVertices[vertexToDrawIndex + 1].g = 1.0f;
//        pVertices[vertexToDrawIndex + 1].b = 1.0f;
//
//        pVertices[vertexToDrawIndex + 2].x = pTEMPVerticesInFile[pTriangleArrayInFile[triIndex].vertexIndex2].x;
//        pVertices[vertexToDrawIndex + 2].y = pTEMPVerticesInFile[pTriangleArrayInFile[triIndex].vertexIndex2].y;
//        pVertices[vertexToDrawIndex + 2].z = pTEMPVerticesInFile[pTriangleArrayInFile[triIndex].vertexIndex2].z;
//        pVertices[vertexToDrawIndex + 2].r = 1.0f;
//        pVertices[vertexToDrawIndex + 2].g = 1.0f;
//        pVertices[vertexToDrawIndex + 2].b = 1.0f;
//
//        // Next triangle is 3 vertices later in the array
//        vertexToDrawIndex += 3;
//
//
//
//        //        // Here's another way we could have done it...
//        //        // 3 301 358 300 
//        //        sVertex& triVertex0 = pVertices[vertexToDrawIndex];
//        //        unsigned int indexOfVertex0 = pTriangleArrayInFile[triIndex].vertexIndex0;
//        //        triVertex0.x = pTEMPVerticesInFile[indexOfVertex0].x;
//        //        triVertex0.y = pTEMPVerticesInFile[indexOfVertex0].y;
//        //        vertexToDrawIndex++;
//        //
//        //        sVertex& triVertex1 = pVertices[vertexToDrawIndex];
//        //        unsigned int indexOfVertex1 = pTriangleArrayInFile[triIndex].vertexIndex1;
//        //        triVertex1.x = pTEMPVerticesInFile[indexOfVertex1].x;
//        //        triVertex1.y = pTEMPVerticesInFile[indexOfVertex1].y;
//        //        sVertex& triVertex0 = pVertices[vertexToDrawIndex];
//        //        vertexToDrawIndex++;
//        //
//        //        sVertex& triVertex2 = pVertices[vertexToDrawIndex];
//        //        unsigned int indexOfVertex2 = pTriangleArrayInFile[triIndex].vertexIndex2;
//        //        triVertex2.x = pTEMPVerticesInFile[indexOfVertex2].x;
//        //        triVertex2.y = pTEMPVerticesInFile[indexOfVertex2].y;
//        //        vertexToDrawIndex++;
//
//    }
//
//
//    //    // Amazing code here
//    //    numberOfVerticesLoaded = 6;
//    //    // Allocate the array on the heap...
//    //    pVertices = new sVertex[numberOfVerticesLoaded];
//    //
//    //    /* triangle #1 */
//    //    // -0.09 to 0.06 in the x axis
//    //    pVertices[0] = { -0.6f, -0.4f, 0.0f, 0.0f, 1.0f };
//    //    pVertices[1] = {  0.6f, -0.4f, 0.0f, 1.0f, 0.0f };
//    //    pVertices[2] = {   0.f,  0.6f, 1.0f, 0.0f, 0.0f };
//    //    /* triangle #2 */
//    //    pVertices[3] = {  0.9f, -0.4f, 0.0f, 1.0f, 0.0f };
//    //    pVertices[4] = { -0.3f, -0.4f, 0.0f, 1.0f, 0.0f };
//    //    pVertices[5] = {  0.2f,  0.6f, 0.0f, 1.0f, 1.0f };
//
//    thePlyFile.close();
//
//    return true;
//}
//
//*/
