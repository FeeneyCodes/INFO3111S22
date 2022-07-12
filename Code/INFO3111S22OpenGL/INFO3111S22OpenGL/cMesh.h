#pragma once

#include <string>
#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

// This represents a 3D model we'd like to draw
class cMesh
{
public:
	cMesh()
	{
		this->RGBA = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
		this->XYZLocation = glm::vec3(0.0f, 0.0f, 0.0f);
		this->orientationEulerAngle = glm::vec3(0.0f, 0.0f, 0.0f);
		this->overallScale = 1.0f;
		this->bIsWireframe = false;
	}
	std::string meshFileName;
	// These are from 0.0 to 1.0f (1.0f = white, 0.0f = black)
	glm::vec4 RGBA;		// Red, Green, Blue, Alpha
	glm::vec3 XYZLocation;
	glm::vec3 orientationEulerAngle;
	float overallScale;		
	// 
	bool bIsWireframe;
};
