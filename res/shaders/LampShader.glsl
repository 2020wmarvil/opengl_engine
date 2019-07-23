#shader vertex
#version 330 core

layout(location = 0) in vec4 a_Pos;

uniform mat4 u_MVP;

void main() {
    gl_Position = u_MVP * a_Pos;
}

#shader fragment
#version 330 core

out vec4 color;

uniform vec3 u_LampColor;

void main() {
    color = vec4(u_LampColor, 1.0f);
}
