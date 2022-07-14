#version 420

//uniform mat4 MVP;
uniform mat4 mModel;
uniform mat4 mView;
uniform mat4 mProj;

uniform vec4 objectColourRGBA;		

in vec3 vPos;	// vec2 = x,y  vec3 = x,y,z
in vec3 vCol;

out vec3 color;

void main()
{
    vec3 vFinalPosition = vPos;

//    vFinalPosition.x -= 1.75f;

// Original line from the C++ side: 
// mvp = matProjection * matView * matModel;

	mat4 mMVP = mProj * mView * mModel;

//    gl_Position = MVP * vec4(vFinalPosition, 1.0);
    gl_Position = mMVP * vec4(vFinalPosition, 1.0);

//    color = vCol;

   color = objectColourRGBA.rgb;
	
}
