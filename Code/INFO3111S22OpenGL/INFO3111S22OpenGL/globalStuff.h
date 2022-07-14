#pragma once

// This is because glad has to be compiled BEFORE glfw

#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <vector>

// Also the mesh stuff
#include "cMesh.h"
#include "cVAOManager.h"

// Anything in here is drawn in the scene
extern std::vector< cMesh* > g_vec_pMeshesToDraw;


bool LoadAllTheModels(std::string sceneFileName,
					  cVAOManager* pVAOManager,
					  unsigned int shaderProgramID,
					  std::string& error);

bool SaveSceneToFile(std::string sceneFileName);


float getRandBetween(float M, float N);