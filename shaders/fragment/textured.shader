#version 150 core

in vec2 texcoord;
flat in uint texIdFrag;

//maximum amount of textures is thus 10
//this should be changed if needed
uniform sampler2D textures;

out vec4 outColor;

void main()
{
    outColor = texture(textures, texcoord);
}