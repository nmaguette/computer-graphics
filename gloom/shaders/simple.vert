#version 450

in layout(location=1) vec4 position;
in layout(location=4) vec4 colour;
uniform layout(location = 5) mat4x4 transform;
out layout(location = 2) vec4 pos;
out layout(location = 3) vec4 colour2;

void main()
{
    vec4 temp = transform * position;
    gl_Position = temp;
    pos = temp;
    colour2 = colour;
}