#version 330

// very simple vertex shader that just transforms an object-space
// position into clip space and passes along a texture coordinate

layout (location = 0) in vec4 Position;
layout (location = 1) in vec4 Color;
layout (location = 2) in vec2 UV;

layout (location = 3) in vec4 tintColor;
layout (location = 4) in vec2 uvScale;
layout (location = 5) in vec2 uvOffset;
layout (location = 6) in int textureIndex;
layout (location = 7) in mat4 Model;

out vec2 vUV;
out vec4 vColor;
flat out int vTextureIndex;

uniform mat4 Projection;
uniform mat4 View;
//uniform mat4 Model;
//uniform vec4 tintColor;
//uniform vec2 uvScale;
//uniform vec2 uvOffset;
//uniform int textureIndex;

void main()
{
	vUV = UV * uvScale + uvOffset;
	vColor = Color * tintColor;
	vTextureIndex = textureIndex;
	gl_Position = Projection * View * Model * Position;
}
