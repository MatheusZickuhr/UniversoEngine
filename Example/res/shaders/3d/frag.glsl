#version 460 core

struct PointLight {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

struct DirectionalLight {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

out vec4 FragColor;

in vec3 vNormal;
in vec3 vAmbient;
in vec3 vDiffuse;
in vec3 vSpecular;
in float vShininess;
in vec2 vTextureCoords;
in float vTextureSlot;
in vec3 vFragPosition;
in vec4 vFragPosLightSpace;

uniform sampler2D textureSlots[32];
uniform vec3 viewPosition;
uniform int numberOfPointLights;
uniform PointLight pointLights[256];
uniform int numberOfDirectionalLights;
uniform DirectionalLight directionalLights[4];
uniform int shadowMapTextureSlot;


vec3 calcPointLight(PointLight light);

vec3 calcDirectionalLight(DirectionalLight light); 

float shadowCalculation(vec4 fragPosLightSpace);

void main() {
    vec3 outputColor = vec3(0.0f);

    for (int i = 0; i < numberOfDirectionalLights; i++) {
        outputColor += calcDirectionalLight(directionalLights[i]);
    }

    for (int i = 0; i < numberOfPointLights; i++) {
        outputColor += calcPointLight(pointLights[i]);
    }
    
    if (vTextureSlot == -1) {
        FragColor = vec4(outputColor, 1.0f);
    } else {
        FragColor = vec4(outputColor, 1.0f) * texture(textureSlots[int(vTextureSlot)], vTextureCoords);
    }
} 

vec3 calcPointLight(PointLight light) {
    // difuse lighting
    vec3 normalizedNormal = normalize(vNormal);
    // I inrverted that expression from the example,
    //and looks more correct (the example was  lightPosition - vFragPosition)
    vec3 lightDirection = normalize(vFragPosition - light.position);

    float diff = max(dot(normalizedNormal, lightDirection), 0.0);
    vec3 diffuse = light.diffuse * (diff * vDiffuse);
    
    // ambient lighting
    float ambientLightStrength = 0.1;

    vec3 ambient = light.ambient * vAmbient;
    
    // specular lighting
    float specularLightStrength = 0.5;

    vec3 viewDir = normalize(viewPosition - vFragPosition);
    // here again on the example was -lightDirection, but Im using lightDirection instead,
    // sience I inverted early
    vec3 reflectDir = reflect(lightDirection, normalizedNormal);

    float spec = pow(max(dot(viewDir, reflectDir), 0.0), vShininess);
    vec3 specular = light.specular * (spec * vSpecular);

    // calculate light attenuation
    float distance = length(light.position - vFragPosition);
    float attenuation = 1.0 / (light.constant + light.linear * distance + 
    		    light.quadratic * (distance * distance)); 

    ambient  *= attenuation; 
    diffuse  *= attenuation;
    specular *= attenuation;   

    // result
    return ambient + diffuse + specular;
}

vec3 calcDirectionalLight(DirectionalLight light) {
    // difuse lighting
    vec3 normalizedNormal = normalize(vNormal);
    // I inrverted that expression from the example,
    //and looks more correct (the example was  lightPosition - vFragPosition)
    vec3 lightDirection = normalize(light.direction);

    float diff = max(dot(normalizedNormal, lightDirection), 0.0);
    vec3 diffuse = light.diffuse * (diff * vDiffuse);
    
    // ambient lighting
    float ambientLightStrength = 0.1;

    vec3 ambient = light.ambient * vAmbient;
    
    // specular lighting
    float specularLightStrength = 0.5;

    vec3 viewDir = normalize(viewPosition - vFragPosition);
    // here again on the example was -lightDirection, but Im using lightDirection instead,
    // sience I inverted early
    vec3 reflectDir = reflect(lightDirection, normalizedNormal);

    float spec = pow(max(dot(viewDir, reflectDir), 0.0), vShininess);
    vec3 specular = light.specular * (spec * vSpecular);

    // calculate shadow
    float shadow = shadowCalculation(vFragPosLightSpace);       

    // result
    return (ambient + (1.0 - shadow) * (diffuse + specular));  
}

float shadowCalculation(vec4 fragPosLightSpace) {
     // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(textureSlots[int(shadowMapTextureSlot)], projCoords.xy).r; 
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // check whether current frag pos is in shadow
    float shadow = currentDepth > closestDepth  ? 1.0 : 0.0;

    return shadow;
}

