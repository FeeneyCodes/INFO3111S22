
#include <string>
#include <fstream>
#include "cVAOManager.h"

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

		sModelDrawInfo bunnyDrawInfo;
		pVAOManager->LoadModelIntoVAO(currentModelTypeFileName, bunnyDrawInfo, shaderProgramID);

		numberOfModelTypesLoaded++;

	} while (numberOfModelTypesLoaded < numberOfModelTypesToLoad);



	return true;
}
