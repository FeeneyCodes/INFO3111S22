#ifndef _cVAOManager_HG_
#define _cVAOManager_HG_

// Will load the models and place them 
// into the vertex and index buffers to be drawn

#include <string>
#include <map>
#include <vector>

// The vertex structure, as it is in the SHADER (on the GPU)
// This is also called the 'vertex layout'. 
// This means that if the shader vertex layout changed, this
//  vertex structure will also need to change (usually). 
struct sVert
{
	float x, y, z;		// added "z"
	float r, g, b;
};


// This represents a single mesh (3D object) structure in 'indexed' format. 
// Its layout is set up to match how the GPU sees the mesh, rather
//  than how the file was, etc. 
// It's also storing the infomation that we need to tell the GPU
//  which model we want to draw. 
struct sModelDrawInfo
{
	sModelDrawInfo(); 

	std::string meshName;

	unsigned int VAO_ID;

	unsigned int VertexBufferID;
	unsigned int VertexBuffer_Start_Index;
	unsigned int numberOfVertices;

	unsigned int IndexBufferID;
	unsigned int IndexBuffer_Start_Index;
	unsigned int numberOfIndices;
	unsigned int numberOfTriangles;

	// The "local" (i.e. "CPU side" temporary array)
	sVert* pVertices;	//  = 0;
	// The index buffer (CPU side)
	unsigned int* pIndices;
};


class cVAOManager
{
public:

	bool LoadModelIntoVAO(std::string fileName, 
						  sModelDrawInfo &drawInfo, 
						  unsigned int shaderProgramID);

	bool LoadModelIntoVAO(std::string fileName, 
						  sModelDrawInfo &drawInfo, 
						  unsigned int shaderProgramID,
						  float objectColourRed, float objectColourBlue, float ObjectColourGreen);

	// An alternative where I don't care about the loading info
	bool LoadModelIntoVAO(std::string fileName, unsigned int shaderProgramID);

	// Another variation that loads a bunch of models at once
	bool LoadModelsIntoVAO(std::vector<std::string> vecFileNames, 
						   std::vector<sModelDrawInfo> &vecModelDrawInfo,
						   unsigned int shaderProgramID);

	// We don't want to return an int, likely
	bool FindDrawInfoByModelName(std::string filename,
								 sModelDrawInfo &drawInfo);

	std::string getLastError(bool bAndClear = true);

	// You could pass "assets/models/"
	void SetModelDirectorPrefix(std::string filePathPrefix);

private:

	std::map< std::string /*model name*/,
		      sModelDrawInfo /* info needed to draw*/ >
		m_map_ModelName_to_VAOID;

	// Loads the ply model file into a temporary array
	bool m_LoadTheModel( std::string fileName, 
						 sModelDrawInfo &drawInfo);

	std::string m_lastErrorString;
	void m_AppendTextToLastError(std::string text, bool addNewLineBefore = true);
};

#endif	// _cVAOManager_HG_
