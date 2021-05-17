#version 460 core

#ifndef MAX_TEXTURES
    #define MAX_TEXTURES 16
#endif

#ifndef MAX_CUBE_MAPS
    #define MAX_CUBE_MAPS 16
#endif

#ifndef MAX_POINT_LIGHTS
    #define MAX_POINT_LIGHTS 4
#endif

#ifndef MAX_DIRECTIONAL_LIGHTS
    #define MAX_DIRECTIONAL_LIGHTS 2
#endif

struct PointLight {
	vec4 position;
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
    float constant;
	float linear;
	float quadratic;
	float farPlane;
	int cubeMapSlotIndex;
};

struct DirectionalLight {
	mat4 viewProjection;
	vec4 position;
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	int textureSlotIndex;
};

out vec4 FragColor;

in VsOut {
    vec3 normal;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
    vec2 textureCoords;
    float textureSlotIndex;
    vec3 fragPosition;
} fsIn;


// samplers from 0 to MAX_TEXTURES - 1
layout (binding = 0) uniform sampler2D textureSlots[MAX_TEXTURES];
// samplers from MAX_TEXTURES to MAX_CUBE_MAPS - 1
layout (binding = MAX_TEXTURES) uniform samplerCube cubeMapSlots[MAX_CUBE_MAPS];

layout (std140, binding = 0) uniform camera {
    mat4 cameraViewProjection;
    vec3 cameraPosition;
};

layout (std140, binding = 1) uniform PointLights {
    PointLight pointLights[MAX_POINT_LIGHTS];
    DirectionalLight directionalLights[MAX_DIRECTIONAL_LIGHTS]; 
    int numberOfPointLights;
    int numberOfDirectionalLights;
};


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
    
    if (fsIn.textureSlotIndex == -1) {
        FragColor = vec4(outputColor, 1.0f);
    } else {
        FragColor = vec4(outputColor, 1.0f) * texture(textureSlots[int(fsIn.textureSlotIndex)], fsIn.textureCoords);
    }
} 

vec3 calcPointLight(PointLight light) {
    // difuse lighting
    vec3 normalizedNormal = normalize(fsIn.normal);

    vec3 lightDirection = normalize(fsIn.fragPosition - light.position.xyz);

    float diff = max(dot(normalizedNormal, lightDirection), 0.0);
    vec3 diffuse = light.diffuse.xyz * (diff * fsIn.diffuse);
    
    // ambient lighting
    float ambientLightStrength = 0.1;

    vec3 ambient = light.ambient.xyz * fsIn.ambient;
    
    // specular lighting
    float specularLightStrength = 0.5;

    vec3 viewDir = normalize(cameraPosition - fsIn.fragPosition);

    vec3 reflectDir = reflect(lightDirection, normalizedNormal);

    float spec = pow(max(dot(viewDir, reflectDir), 0.0), fsIn.shininess);
    vec3 specular = light.specular.xyz * (spec * fsIn.specular);

    // calculate light attenuation
    float distance = length(light.position.xyz - fsIn.fragPosition);
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
    vec3 normalizedNormal = normalize(fsIn.normal);

    vec3 lightDirection = normalize(vec3(0, 0, 0) - light.position.xyz);

    float diff = max(dot(normalizedNormal, lightDirection), 0.0);
    vec3 diffuse = light.diffuse.xyz * (diff * fsIn.diffuse);
    
    // ambient lighting
    float ambientLightStrength = 0.1;

    vec3 ambient = light.ambient.xyz * fsIn.ambient;
    
    // specular lighting
    float specularLightStrength = 0.5;

    vec3 viewDir = normalize(cameraPosition - fsIn.fragPosition);

    vec3 reflectDir = reflect(lightDirection, normalizedNormal);

    float spec = pow(max(dot(viewDir, reflectDir), 0.0), fsIn.shininess);
    vec3 specular = light.specular.xyz * (spec * fsIn.specular);

    // calculate shadow
    float shadow = shadowCalculationDirectionalLight(light);       

    // result
    return (ambient + (1.0 - shadow) * (diffuse + specular));  
}

float shadowCalculationDirectionalLight(DirectionalLight light) {
    
    vec4 fragPosLightSpace = light.viewProjection * vec4(fsIn.fragPosition, 1.0);

    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(textureSlots[light.textureSlotIndex], projCoords.xy).r; 
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // calculate bias (based on depth map resolution and slope)
    vec3 normal = normalize(fsIn.normal);
    vec3 lightDir = normalize(light.position.xyz - fsIn.fragPosition);
    float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);
    // check whether current frag pos is in shadow
    // float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;
    // PCF
    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(textureSlots[light.textureSlotIndex], 0);
    for(int x = -1; x <= 1; ++x) {
        for(int y = -1; y <= 1; ++y) {
            float pcfDepth = texture(textureSlots[light.textureSlotIndex], projCoords.xy + vec2(x, y) * texelSize).r; 
            shadow += currentDepth - bias > pcfDepth  ? 1.0 : 0.0;        
        }    
    }
    shadow /= 9.0;
    
    // keep the shadow at 0.0 when outside the far_plane region of the light's frustum.
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
    vec3 fragToLight = fsIn.fragPosition - pointLight.position.xyz;
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
    float viewDistance = length(cameraPosition - fsIn.fragPosition);
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
