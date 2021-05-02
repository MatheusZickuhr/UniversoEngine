#version 460 core

struct PointLight {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
    
    float farPlane;

    int cubeMapSlotIndex;
};

struct DirectionalLight {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    int textureSlotIndex;
    mat4 viewProjection;
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

uniform samplerCube cubeMapSlots[16];
uniform sampler2D textureSlots[16];
uniform vec3 viewPosition;
uniform int numberOfPointLights;
uniform PointLight pointLights[256];
uniform int numberOfDirectionalLights;
uniform DirectionalLight directionalLights[4];


vec3 calcPointLight(PointLight light);

vec3 calcDirectionalLight(DirectionalLight light); 

float shadowCalculationDirectionalLight(DirectionalLight light);

float shadowCalculationPointLight(PointLight pointLight);

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

    // shadow 
    float shadow = shadowCalculationPointLight(light);  

    // result
    return (ambient + (1.0 - shadow) * (diffuse + specular));
}

vec3 calcDirectionalLight(DirectionalLight light) {
    // difuse lighting
    vec3 normalizedNormal = normalize(vNormal);
    // I inrverted that expression from the example,
    //and looks more correct (the example was  lightPosition - vFragPosition)
    vec3 lightDirection = normalize(vec3(0, 0, 0) - light.position);

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
    float shadow = shadowCalculationDirectionalLight(light);       

    // result
    return (ambient + (1.0 - shadow) * (diffuse + specular));  
}

float shadowCalculationDirectionalLight(DirectionalLight light) {
    // hardcoded light pos, change this later
    vec4 fragPosLightSpace = light.viewProjection * vec4(vFragPosition, 1.0);

    vec3 lightDirection = normalize(vFragPosition - light.position);
   
     // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(textureSlots[light.textureSlotIndex], projCoords.xy).r; 
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    //check whether current frag pos is in shadow
    float bias = max(1 * (1.0 - dot(normalize(vNormal), lightDirection)), 0.005);  
    //float bias = max(0.05 * (1.0 - dot(normalize(vNormal), lightDirection)), 0.005);  
    //float bias = 0.005;

    //float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0; 

    // makes shadows smooter
    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(textureSlots[light.textureSlotIndex], 0);
    for (int x = -1; x <= 1; ++x) {
        for (int y = -1; y <= 1; ++y) {
            float pcfDepth = texture(textureSlots[light.textureSlotIndex], projCoords.xy + vec2(x, y) * texelSize).r; 
            shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;        
        }    
    }
    shadow /= 9.0;
    // makes shadows smooter end

    
    if(projCoords.z > 1.0)
        shadow = 0.0;

    return shadow;
}


vec3 sampleOffsetDirections[20] = vec3[]
(
   vec3( 1,  1,  1), vec3( 1, -1,  1), vec3(-1, -1,  1), vec3(-1,  1,  1), 
   vec3( 1,  1, -1), vec3( 1, -1, -1), vec3(-1, -1, -1), vec3(-1,  1, -1),
   vec3( 1,  1,  0), vec3( 1, -1,  0), vec3(-1, -1,  0), vec3(-1,  1,  0),
   vec3( 1,  0,  1), vec3(-1,  0,  1), vec3( 1,  0, -1), vec3(-1,  0, -1),
   vec3( 0,  1,  1), vec3( 0, -1,  1), vec3( 0, -1, -1), vec3( 0,  1, -1)
);  

float shadowCalculationPointLight(PointLight pointLight) {
    // get vector between fragment position and light position
    vec3 fragToLight = vFragPosition - pointLight.position;
    // use the light to fragment vector to sample from the depth map    
    float closestDepth = texture(cubeMapSlots[pointLight.cubeMapSlotIndex], fragToLight).r;
    // it is currently in linear range between [0,1]. Re-transform back to original value
    closestDepth *= pointLight.farPlane;
    // now get current linear depth as the length between the fragment and light position
    float currentDepth = length(fragToLight);
    // now test for shadows
    float shadow = 0.0;
    float bias   = 0.15;
    int samples  = 20;
    float viewDistance = length(viewPosition - vFragPosition);
    float diskRadius = (1.0 + (viewDistance / pointLight.farPlane)) / 25.0;
    for(int i = 0; i < samples; ++i) {
        float closestDepth = texture(cubeMapSlots[pointLight.cubeMapSlotIndex], fragToLight + sampleOffsetDirections[i] * diskRadius).r;
        closestDepth *= pointLight.farPlane;   // undo mapping [0;1]
        if(currentDepth - bias > closestDepth)
            shadow += 1.0;
    }
    shadow /= float(samples); 

    return shadow;
}
