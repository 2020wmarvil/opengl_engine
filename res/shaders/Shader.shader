#shader vertex
#version 330 core

layout(location = 0) in vec4 a_Pos;
layout(location = 1) in vec3 a_Color;
layout(location = 2) in vec2 a_TexCoord;

out vec3 v_Color;
out vec2 v_TexCoord;

uniform float u_Offset;
uniform mat4 u_MVP;

void main() {
	gl_Position = u_MVP * a_Pos;

	v_Color = a_Color;
	v_TexCoord = a_TexCoord;
};

#shader fragment
#version 330 core

in vec3 v_Color;
in vec2 v_TexCoord;

layout(location = 0) out vec4 color;

uniform sampler2D u_Texture1;
uniform sampler2D u_Texture2;

void main() {
//	vec4 texColor = texture(u_Texture1, v_TexCoord);
	vec4 texColor = mix(texture(u_Texture1, v_TexCoord), texture(u_Texture2, v_TexCoord), 0.3);

//	color = texColor;						// display just a texture
//	color = vec4(v_Color, 1.0f);			// display just a color
	color = texColor * vec4(v_Color, 1.0f); // display both for shits and gigs
}
