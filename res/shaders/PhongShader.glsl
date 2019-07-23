#shader vertex
#version 330 core
  
layout(location = 0) in vec4 a_Pos;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec3 a_Color;
layout(location = 3) in vec2 a_TexCoord;

out vec3 v_Normal;
out vec3 v_Color;
out vec2 v_TexCoord;

out vec3 v_FragPos;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Proj;
uniform mat4 u_ModelView;
uniform mat4 u_ViewProj;
uniform mat4 u_MVP;

uniform vec3 u_LightPos;

void main() {
	gl_Position = u_MVP * a_Pos;

	v_Normal = mat3(transpose(inverse(u_ModelView))) * a_Normal;
	v_Color = a_Color;
	v_TexCoord = a_TexCoord;

	v_FragPos = vec3(u_ModelView * a_Pos);
};

#shader fragment
#version 330 core

struct Material {
    vec3 ambient, diffuse, specular;
    float shininess;
}; 

struct Light {
    vec3 ambient, diffuse, specular;
    vec3 position;
};

in vec3 v_Normal;
in vec3 v_Color;
in vec2 v_TexCoord;

in vec3 v_FragPos;

layout(location = 0) out vec4 color;

uniform vec3 u_ObjectColor;
uniform mat4 u_View;

uniform Material material;
uniform Light light;

uniform sampler2D u_Texture1;
uniform sampler2D u_Texture2;

void main() {
	vec4 texColor = mix(texture(u_Texture1, v_TexCoord), texture(u_Texture2, v_TexCoord), 0.3);

	// calculate ambient light
	vec3 ambientLight = light.ambient * material.ambient;

	// calculate diffuse light
	vec3 normal = normalize(v_Normal);
	vec3 lightPos = vec3(u_View * vec4(light.position, 1.0));
	vec3 lightDir = normalize(lightPos - v_FragPos);
	float diffuse = max(dot(normal, lightDir), 0.0f);
	vec3 diffuseLight = light.diffuse * (diffuse * material.diffuse);

	// calculate specular light
	vec3 cameraDir = normalize(-v_FragPos);
	vec3 reflectDir = reflect(-lightDir, normal);
	float specular = pow(max(dot(cameraDir, reflectDir), 0.0f), material.shininess);
	vec3 specularLight = light.specular * (specular * material.specular);

	vec3 resultantColor = (ambientLight + diffuseLight + specularLight); //* u_ObjectColor;

//	color = texColor * vec4(v_Color, 1.0f) * vec4(resultantColor, 1.0f);
//	color = texColor * vec4(resultantColor, 1.0f);
	color = vec4(resultantColor, 1.0f);
}
