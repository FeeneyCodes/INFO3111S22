
#include <string>
#include <fstream>
#include "cVAOManager.h"
#include "cMesh.h"

extern std::vector< cMesh* > g_vec_pMeshesToDraw;


bool LoadAllTheModels(std::string sceneFileName, 
					  cVAOManager* pVAOManager,
					  unsigned int shaderProgramID,
					  std::string &error)
{
	// Open the file that lists the models I'd like to load into the VAO
	std::ifstream sceneFile(sceneFileName.c_str());
	
	if ( ! sceneFile.is_open() )
	{
		error = "Can't load file " + sceneFileName;
		return false;
	}

	// File is open at this point
	
	// How many types of models am I going to load? 
	// 1st number in the file is this number
	unsigned int numberOfModelTypesToLoad = 0;
	sceneFile >> numberOfModelTypesToLoad;

	unsigned int numberOfModelTypesLoaded = 0;
	do
	{
		std::string currentModelTypeFileName;
		sceneFile >> currentModelTypeFileName;

		// assets/models/bun_zipper.xyz_n_rgba_uv.ply

		sModelDrawInfo bunnyDrawInfo;
		pVAOManager->LoadModelIntoVAO(currentModelTypeFileName, bunnyDrawInfo, shaderProgramID);

		numberOfModelTypesLoaded++;

	} while (numberOfModelTypesLoaded < numberOfModelTypesToLoad);



	return true;
}

//https://stackoverflow.com/questions/686353/random-float-number-generation
float getRandBetween(float M, float N)
{
	return M + (rand() / (RAND_MAX / (N - M)));
}

bool SaveSceneToFile(std::string sceneFileName)
{
	std::ofstream saveFile(sceneFileName.c_str());

	// TODO: Make sure the file openned...

	for (unsigned int index = 0; index != ::g_vec_pMeshesToDraw.size(); index++)
	{
		cMesh* pCurrentMesh = ::g_vec_pMeshesToDraw[index];

		saveFile << pCurrentMesh->meshFileName << std::endl;
		saveFile
			<< pCurrentMesh->XYZLocation.x << " "
			<< pCurrentMesh->XYZLocation.y << " "
			<< pCurrentMesh->XYZLocation.z << std::endl;

		
		// TODO: and the rest of the data...
	
	}// for (unsigned int index...

	saveFile.close();

	return true;
}
 