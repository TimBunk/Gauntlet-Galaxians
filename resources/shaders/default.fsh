#version 330

// very simple pixel shader that just samples a texture

in vec2 vUV;
in vec4 vColor;
flat in int vTextureIndex;

out vec4 outColour;

uniform sampler2D activeTexture[32];

void main()
{
    outColour = texture2D(activeTexture[vTextureIndex], vUV) * vColor;
    /*if (vTextureIndex == 1) {
      outColour.x = 1.0f;
    }*/
}
