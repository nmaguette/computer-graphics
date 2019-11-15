#version 450

in layout(location=2) vec4 pos;
in layout(location=3) vec4 colour;
out vec4 color;

void main()
{
    color = colour;

}