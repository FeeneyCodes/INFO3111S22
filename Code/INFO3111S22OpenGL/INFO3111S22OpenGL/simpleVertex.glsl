#version 420

uniform mat4 MVP;

in vec2 vPos;
in vec3 vCol;

out vec3 color;

void main()
{
    vec2 vFinalPosition = vPos;

//    vFinalPosition.x -= 1.75f;

    gl_Position = MVP * vec4(vFinalPosition, 0.0, 1.0);

    color = vCol;
}
