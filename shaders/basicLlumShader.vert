#version 330 core

in vec3 vertex;
in vec3 normal;

in vec3 matamb;
in vec3 matdiff;
in vec3 matspec;
in float matshin;

uniform mat3 normalMatrix;
uniform mat4 proj;
uniform mat4 view;
uniform mat4 TG;

out vec3 fcolor;
out vec4 vertexSCO;
out vec3 _matamb;
out vec3 _matdiff;
out vec3 _matspec;
out float _matshine;
out vec3 _normal;

void main()
{	
    vertexSCO = view*TG * vec4(vertex, 1.0f);
    _normal = normalize(normalMatrix * normal);
    _matamb = matamb;
    _matdiff = matdiff;
    _matspec = matspec;
    _matshine = matshin;
    fcolor = matdiff;
    gl_Position = proj * view * TG * vec4(vertex, 1.0f);
}
