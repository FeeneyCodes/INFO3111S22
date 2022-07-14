#version 420

in vec3 color;
out vec4 vertexWorldPosition;

out vec4 outputColour;

void main()
{
	// FragColor is depricated
	//gl_FragColor = vec4(color, 1.0);

	outputColour = vec4(color, 1.0);

//	outputColour = vec4( 0.0f, 1.0f, 0.0f, 1.0f );
}
