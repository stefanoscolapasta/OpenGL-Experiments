//core stands for not letting us use any kind of deprecated functions, would result in shader compilation error
// "a" "b" will be concatenated directly, nice C++ :3
//the location is the location of the specified attribute in the glVertexAttribPointer(...) function of OpenGL
//why vec4 if it is a 2d vec? Well the output needs to be a vec4 (remember homogeneus coordinates in image space after and for perspective projection?)
//by doing 'in vec4' OpenGL automatically convert/cast our vec2 to vec4, no need for swizzling in the shader :3
//it will do something like --> (position.xy, 0.0, 1.0) yeah last coord needs to be 1, remember it otherwise it will break the perspective divide lol

#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texCoord;

out vec2 v_TexCoord; //v stands for 'varying'

uniform mat4 u_MVP;

void main()
{
   gl_Position = u_MVP * position;
   v_TexCoord = texCoord;
};

//--------------------------------------------
#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec2 v_TexCoord;

uniform vec4 u_Color;
uniform sampler2D u_Texture; //A sampler2D is used to do lookup in a standard texture image;

void main()
{
	vec4 texColor = texture(u_Texture, v_TexCoord); //this does the sampling from the texture coordinate onto the actual texture and retrieves the color (per fragment)
	color = texColor;
};