#version 420

uniform mat4 MVP;

uniform vec4 objectColourRGBA;

in vec3 vPos;	// vec2 = x,y  vec3 = x,y,z
in vec3 vCol;

out vec3 color;

void main()
{
    vec3 vFinalPosition = vPos;

//    vFinalPosition.x -= 1.75f;

    gl_Position = MVP * vec4(vFinalPosition, 1.0);

//    color = vCol;
    color = objectColourRGBA.rgb;
}
