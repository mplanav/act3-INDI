#version 330 core

in vec3 fcolor;
out vec3 fragColor;

in vec4 vertexSCO;
in vec3 normal;

in vec3 _matamb;
in vec3 _matdiff;
in vec3 _matspec;
in float _matshine;
in vec3 _normal;

uniform vec3 llumAmbient;
uniform vec3 llumFocus;
uniform vec3 posFocus;

uniform vec3 faro1Pos;
uniform vec3 faro1Col;
uniform vec3 faro2Pos;
uniform vec3 faro2Col;

uniform vec3 torxesPos[6];  // Coordenadas de las antorchas
uniform vec3 torxesCol[6];  // Colores de las antorchas

vec3 ambient()
{
    return llumAmbient * _matamb;
}

vec3 difus(vec3 normSCO, vec3 L, vec3 colFocus)
{
    vec3 colRes = vec3(0);
    if(dot(L, normSCO) > 0)
        colRes = colFocus * _matdiff * dot(L, normSCO);
    return colRes;
}

vec3 especular(vec3 normSCO, vec3 L, vec4 vertSCO, vec3 colFocus)
{
    vec3 colRes = vec3(0);
    if((dot(normSCO, L) < 0) || (_matshine == 0))
        return colRes;

    vec3 R = reflect(-L, normSCO);
    vec3 V = normalize(-vertSCO.xyz);

    if(dot(R, V) < 0) return colRes;

    float shine = pow(max(0.0, dot(R, V)), _matshine);
    return(_matspec * colFocus * shine);
}

void main()
{
    // llumSol
    vec3 normSCO = normalize(_normal);
    vec3 L = normalize(posFocus - vec3(vertexSCO));
    vec3 _llumAmbient = ambient();
    vec3 difusLight = difus(normSCO, L, llumFocus);
    vec3 specLight = especular(normSCO, L, vertexSCO, llumFocus);

    // faro1
    vec3 L1 = normalize(faro1Pos - vec3(vertexSCO));
    vec3 difusLightF1 = difus(normSCO, L1, faro1Col);
    vec3 specLightF1 = especular(normSCO, L1, vertexSCO, faro1Col);

    // faro2
    vec3 L2 = normalize(faro2Pos - vec3(vertexSCO));
    vec3 difusLightF2 = difus(normSCO, L2, faro2Col);
    vec3 specLightF2 = especular(normSCO, L2, vertexSCO, faro2Col);

    // Atenuación para faro1
    vec3 dirF1 = faro1Pos - faro2Pos;  // Diferencia de posiciones
    float At1 = pow(dot(dirF1, -L1), 4.0);
    difusLightF1 *= At1;
    specLightF1 *= At1;

    // Atenuación para faro2
    vec3 dirF2 = faro2Pos - faro1Pos;  // Diferencia de posiciones
    float At2 = pow(dot(dirF2, -L2), 4.0);
    difusLightF2 *= At2;
    specLightF2 *= At2;
    
    // Iluminación de las antorchas
    vec3 torchesLight = vec3(0.0);
    for(int i = 0; i < 6; ++i)
    {
        vec3 Lt = normalize(torxesPos[i] - vec3(vertexSCO));
        float d = length(torxesPos[i] - vec3(vertexSCO));
        float attenuation = (d < 3.0) ? 1.0 : exp(-(d - 3.0));
        vec3 difusLightT = difus(normSCO, Lt, torxesCol[i]) * attenuation;
        vec3 specLightT = especular(normSCO, Lt, vertexSCO, torxesCol[i]) * attenuation;
        torchesLight += difusLightT + specLightT;
    }

    fragColor = _llumAmbient + difusLight + specLight + difusLightF1 + difusLightF2 + specLightF1 + specLightF2 + torchesLight;
}
