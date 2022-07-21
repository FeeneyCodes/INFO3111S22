#version 420

//uniform mat4 MVP;
uniform mat4 mModel;
uniform mat4 mView;
uniform mat4 mProj;

uniform vec4 objectColourRGBA;		
// All registers are floats, so pass this as a float or int
// (i.e. there really isn't a "boolean")
// 0 = false, non 1 is true
uniform bool bUseModelFileColours;

//in vec3 vPos;	// vec2 = x,y  vec3 = x,y,z
//in vec3 vCol;

// This mirrors the sVert_n_rgba_uv struct in the C++ side
in vec4 vPosition;
in vec4 vRGBA; 
in vec4 vNormal;
in vec4 vUV_x2;

// To the pixel fragment shader
out vec4 fColour;
out vec4 fNormal;
out vec4 fUV_x2;
out vec4 vertexWorldPosition;
out mat4 matModelInvTrans;		// Used for lighting

void main()
{
    vec3 vFinalPosition = vPosition.xyz;

//    vFinalPosition.x -= 1.75f;

// Original line from the C++ side: 
// mvp = matProjection * matView * matModel;

	mat4 mMVP = mProj * mView * mModel;

//    gl_Position = MVP * vec4(vFinalPosition, 1.0);
    gl_Position = mMVP * vec4(vFinalPosition, 1.0);

	vertexWorldPosition = mModel * vec4(vFinalPosition, 1.0);

	// Also rotate the normal with the model matrix
	// The inverse transpose of the model matrix removes
	//	any tranlation (movement) and scaling transformations
	// (leaving only rotation)
	mat4 matModelInvTrans = inverse(transpose(mModel));
    fNormal = matModelInvTrans * vec4(normalize(vNormal.xyz), 1.0);
	
   
    // These are just being passed unchanged
	if ( bUseModelFileColours )
	{
		fColour = vRGBA;
	}
	else
	{
		fColour = objectColourRGBA;
	}
    
    fUV_x2 = vUV_x2;
	
}
