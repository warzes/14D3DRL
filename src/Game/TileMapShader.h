#pragma once
#if TileMapSimpleTexture
static constexpr const char* simpleTextureVertexShaderSource = R"(
#version 330 core

layout(location = 0) in vec3 aVertexPosition;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTextCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec2 UV;
out vec3 normal;

void main()
{
	vec4 vertPosRelativeToCamera = view * model * vec4(aVertexPosition, 1.0);
	gl_Position =  projection * vertPosRelativeToCamera;
	normal = mat3(transpose(inverse(model))) * aNormal;
	UV = aTextCoord;
}
)";

static constexpr const char* simpleTextureFragmentShaderSource = R"(
#version 330 core

in vec2 UV;
in vec3 normal;

out vec4 FragColor;

uniform vec3 uColor;
uniform sampler2D Texture0;

void main()
{
	//vec3 norm = normalize(normal);	
	FragColor = vec4(texture(Texture0, UV).xyz * uColor, 1.0);
}
)";
#else

#if NewShader
static constexpr const char* vertexShaderSource = R"(
#version 330 core

layout(location = 0) in vec3 aVertexPosition;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTextCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec2 UV;
out vec3 normal;
out float visibility;
out float distance;

//const float density = 0.03;
const float density = 0.3;
const float gradient = 1.5;

void main()
{
	vec4 vertPosRelativeToCamera = view * model * vec4(aVertexPosition, 1.0);
	float dist = length(vertPosRelativeToCamera.xyz);

distance = dist;
	
	visibility = exp(-pow((dist * density), gradient));
	visibility = clamp(visibility, 0.0, 1.0);

	gl_Position =  projection * vertPosRelativeToCamera;
	normal = mat3(transpose(inverse(model))) * aNormal;
	UV = aTextCoord;
}
)";

static constexpr const char* fragmentShaderSource = R"(
#version 330 core

in vec2 UV;
in vec3 normal;
in float visibility;
in float distance;

out vec4 FragColor;

uniform vec3 uColor;
uniform sampler2D Texture0;

void main()
{
	vec3 fogColor = vec3(0.4, 0.5, 0.4);
	vec3 lightDirection = vec3(0.0, 0.8, 0.2);
	vec3 norm = normalize(normal);
	float shadow = dot(norm, lightDirection);
	if(shadow <= 0.0)
	{
		shadow = 0.0;
	}
	vec3 objectColor = texture(Texture0, UV).xyz * uColor;

	FragColor = (vec4(objectColor, 1.0) * (shadow + 0.7)) * 0.7;
	//FragColor = mix(vec4(fogColor, 1.0), FragColor, visibility);

	//float vis = floor(visibility*1000) / 1000;
float vis = distance-shadow;
	FragColor = mix(vec4(fogColor, 1.0), FragColor, vis);


	//FragColor = texture( Texture0, UV );
}
)";
#else
// TODO: проверить нормали. а направление света идет от игрока
static constexpr const char* vertexShaderSource = R"(
#version 330 core

layout(location = 0) in vec3 aVertexPosition;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTextCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec2 UV;
out vec3 normal;
out float visibility;

//const float density = 0.03;
const float density = 0.3;
const float gradient = 1.5;

void main()
{
	vec4 vertPosRelativeToCamera = view * model * vec4(aVertexPosition, 1.0);
	float dist = length(vertPosRelativeToCamera.xyz);
	
	visibility = exp(-pow((dist * density), gradient));
	visibility = clamp(visibility, 0.0, 1.0);

	gl_Position =  projection * vertPosRelativeToCamera;
	normal = mat3(transpose(inverse(model))) * aNormal;
	UV = aTextCoord;
}
)";
static constexpr const char* fragmentShaderSource = R"(
#version 330 core

in vec2 UV;
in vec3 normal;
in float visibility;

out vec4 FragColor;

uniform vec3 uColor;
uniform sampler2D Texture0;

void main()
{
	vec3 fogColor = vec3(0.4, 0.5, 0.4);
	vec3 lightDirection = vec3(0.0, 0.8, 0.2);
	vec3 norm = normalize(normal);
	float shadow = dot(norm, lightDirection);
	if(shadow <= 0.0)
	{
		shadow = 0.0;
	}
	vec3 objectColor = texture(Texture0, UV).xyz * uColor;
	//vec3 objectColor = uColor;

	//FragColor = (vec4(objectColor, 1.0) * (shadow + 0.3)) * 0.7;
	FragColor = (vec4(objectColor, 1.0) * (shadow + 0.7)) * 0.7;
	FragColor = mix(vec4(fogColor, 1.0), FragColor, visibility);

	// HDR tonemapping
	//FragColor.rgb = FragColor.rgb / (FragColor.rgb + vec3(1.0));
	// gamma correct
	//FragColor.rgb = pow(FragColor.rgb, vec3(1.0 / 2.2));

	//FragColor = texture( Texture0, UV );
}
)";
//-----------------------------------------------------------------------------
static constexpr const char* solidFragmentShaderSource = R"(
#version 330 core

in vec2 UV;
in vec3 normal;
in float visibility;

out vec4 FragColor;

uniform vec3 uColor;
uniform sampler2D Texture0;

void main()
{
	vec3 fogColor = vec3(0.4, 0.5, 0.4);
	vec3 lightDirection = vec3(0.0, 0.8, 0.2);
	vec3 norm = normalize(normal);
	float shadow = dot(norm, lightDirection);
	if(shadow <= 0.0)
	{
		shadow = 0.0;
	}
	vec3 objectColor = uColor;

	FragColor = (vec4(objectColor, 1.0) * (shadow + 0.3)) * 0.7;
	FragColor = mix(vec4(fogColor, 1.0), FragColor, visibility);


	vec3 color1 = vec3(0.1, 0.2, 0.3);
	vec3 color2 = vec3(0.8, 0.4, 0.2);

	vec4 color = FragColor;
	//color.rgb = color2;

	if (UV.x < 0.01 || UV.x > 0.99) color.rgb = color1;
	if (UV.y < 0.01 || UV.y > 0.99) color.rgb = color1;
	FragColor = color;
}
)";


static constexpr const char* simpleVertexShaderSource = R"(
#version 330 core

layout(location = 0) in vec3 aVertexPosition;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTextCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec2 UV;
out vec3 normal;

void main()
{
	gl_Position =  projection * view * model * vec4(aVertexPosition, 1.0);
	normal = mat3(transpose(inverse(model))) * aNormal;
	UV = aTextCoord;
}
)";
static constexpr const char* simpleFragmentShaderSource = R"(
#version 330 core

in vec2 UV;
in vec3 normal;
out vec4 FragColor;

uniform vec3 uColor;
uniform sampler2D Texture0;

void main()
{
	vec3 objectColor = texture(Texture0, UV).xyz * uColor;
	FragColor = vec4(objectColor, 1.0);

	//vec3 color1 = vec3(0.1, 0.1, 0.2);
	//vec3 color2 = vec3(0.9, 0.9, 0.8);

	//vec4 color = vec4(color1, 1.0);
	
	//vec3 norm = normalize(normal);
	//if (norm.y < 1.0)
	//{
	//	if (UV.x < 0.01 || UV.x > 0.99) color.rgb = color2;
	//	if (UV.y < 0.01 || UV.y > 0.99) color.rgb = color2;
	//}
	//FragColor = color;

	//vec3 norm = normalize(normal);
	//FragColor = vec4(norm, 1.0);
}
)";
#endif
#endif