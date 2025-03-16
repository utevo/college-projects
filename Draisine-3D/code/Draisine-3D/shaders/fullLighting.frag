#version 330 core

out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;
in vec4 FragPosLightSpace;

uniform vec3 LIGHT_COLOR;  
uniform vec3 LIGHT_POS; 
uniform vec3 CAMERA_POS;

uniform sampler2D shadowMap;

uniform float AMBIENT_LIGHT_STRENGHT;
uniform float DIFFUSE_LIGHT_STRENGHT;
uniform float SPECULAR_LIGHT_STRENGHT;

uniform sampler2D TEXTURE;

float ShadowCalculation(vec4 fragPosLightSpace)
{
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadowMap, projCoords.xy).r; 
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
   
    float bias = 0.002;

    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r; 
            shadow += currentDepth - bias > pcfDepth  ? 1.0 : 0.0;        
        }    
    }
    shadow /= 9.0;
    
    // keep the shadow at 0.0 when outside the far_plane region of the light's frustum.
    if(projCoords.z > 1.0)
        shadow = 0.0;
        
    return shadow;
}

void main() {
    // ambient light
    vec3 ambient = AMBIENT_LIGHT_STRENGHT * LIGHT_COLOR;

    // diffuse light
	vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(LIGHT_POS - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = DIFFUSE_LIGHT_STRENGHT * diff * LIGHT_COLOR;

    // specular ligth
    vec3 viewDir = normalize(CAMERA_POS - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 64);
    vec3 specular = SPECULAR_LIGHT_STRENGHT * spec * LIGHT_COLOR; 

	// calculate shadow
    float shadow = ShadowCalculation(FragPosLightSpace);       

    vec3 light = ambient + (1.0 - shadow) * (diffuse + specular);
	vec4 colorBeforeLight = texture(TEXTURE, TexCoord) * vec4(1.0, 1.0, 1.0, 1.0) ;
	FragColor = vec4(light, 1.0) * colorBeforeLight;
}
