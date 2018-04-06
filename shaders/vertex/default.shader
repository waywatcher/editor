#version 150 core

in vec2 pos;
in vec2 tex;
in mat2 modelView;

void main()
{
    gl_Position = vec4(pos, 0.0, 1.0);
}
