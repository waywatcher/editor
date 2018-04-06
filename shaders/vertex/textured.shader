#version 150 core

in vec2 pos;
in vec2 tex;
in uint texId;

uniform mat3 modelView;

out vec2 texcoord;
flat out uint texIdFrag;

void main()
{
	gl_Position = vec4(modelView * vec3(pos, 1), 1);
	texcoord = tex;
	texIdFrag = texId;
}
