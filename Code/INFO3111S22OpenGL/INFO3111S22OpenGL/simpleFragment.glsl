#version 420

//in vec3 color;
//in vec4 vertexWorldPosition;

in vec4 fColour;
in vec4 fNormal;
in vec4 fUV_x2;
in vec4 vertexWorldPosition;


out vec4 outputColour;

void main()
{
	// FragColor is depricated
	//gl_FragColor = vec4(color, 1.0);

	outputColour = fColour;
	
	outputColour.rgb = fNormal.xyz;

//	outputColour = vec4( 0.0f, 1.0f, 0.0f, 1.0f );
}
