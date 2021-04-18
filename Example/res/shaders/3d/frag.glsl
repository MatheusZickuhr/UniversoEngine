#version 460 core
out vec4 FragColor;

in vec3 vColor;
in vec3 vNormal;
in vec2 vTextureCoords;
in float vTextureSlot;
in vec3 vFragPosition;

uniform sampler2D textureSlots[32];
uniform vec3 viewPosition;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

uniform Material material;

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

uniform int numberOfPointLights;
uniform PointLight pointLights[4];

uniform int numberOfDirectionalLights;
uniform DirectionalLight directionalLights[4];

vec3 calcPointLight(PointLight light);

vec3 calcDirectionalLight(DirectionalLight light); 

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
    vec3 diffuse = light.diffuse * (diff * material.diffuse);
    
    // ambient lighting
    float ambientLightStrength = 0.1;

    vec3 ambient = light.ambient * material.ambient;
    
    // specular lighting
    float specularLightStrength = 0.5;

    vec3 viewDir = normalize(viewPosition - vFragPosition);
    // here again on the example was -lightDirection, but Im using lightDirection instead,
    // sience I inverted early
    vec3 reflectDir = reflect(lightDirection, normalizedNormal);

    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * (spec * material.specular);

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
    vec3 diffuse = light.diffuse * (diff * material.diffuse);
    
    // ambient lighting
    float ambientLightStrength = 0.1;

    vec3 ambient = light.ambient * material.ambient;
    
    // specular lighting
    float specularLightStrength = 0.5;

    vec3 viewDir = normalize(viewPosition - vFragPosition);
    // here again on the example was -lightDirection, but Im using lightDirection instead,
    // sience I inverted early
    vec3 reflectDir = reflect(lightDirection, normalizedNormal);

    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * (spec * material.specular);

    // result
    return ambient + diffuse + specular;
}