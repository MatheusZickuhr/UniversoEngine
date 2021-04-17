#version 460 core
out vec4 FragColor;

in vec3 vColor;
in vec3 vNormal;
in vec2 vTextureCoords;
in float vTextureSlot;
in vec3 vFragPosition;

uniform sampler2D textureSlots[32];
uniform vec3 lightColor;
uniform vec3 lightPosition;
uniform vec3 viewPosition;

void main() {
    // difuse lighting
    vec3 normalizedNormal = normalize(vNormal);
    // I inrverted that expression from the example,
    //and looks more correct (the example was  lightPosition - vFragPosition)
    vec3 lightDirection = normalize(vFragPosition - lightPosition);

    float diff = max(dot(normalizedNormal, lightDirection), 0.0);
    vec3 diffuse = diff * lightColor;
    
    // ambient lighting
    float ambientLightStrength = 0.1;

    vec3 ambient = ambientLightStrength * lightColor;
    
    // specular lighting
    float specularLightStrength = 0.5;

    vec3 viewDir = normalize(viewPosition - vFragPosition);
    // here again on the example was -lightDirection, but Im using lightDirection instead,
    // sience I inverted early
    vec3 reflectDir = reflect(lightDirection, normalizedNormal);

    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularLightStrength * spec * lightColor;  

    // result
    vec3 result = (ambient + diffuse + specular) * vColor;

    if (vTextureSlot == -1) {
        FragColor = vec4(result, 1.0f);
    } else {
        FragColor = vec4(result, 1.0f) * texture(textureSlots[int(vTextureSlot)], vTextureCoords);
    }
} 
