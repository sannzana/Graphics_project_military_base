#version 330 core
out vec4 FragColor;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

struct PointLight {
    vec3 position;
    
    float k_c;  // attenuation factors
    float k_l;  // attenuation factors
    float k_q;  // attenuation factors
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct DirectionLight {
    vec3 direction;
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct SpotLight {
    vec3 position;
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float cos_theta;

    float k_c;  // attenuation factors
    float k_l;  // attenuation factors
    float k_q;  // attenuation factors
};

#define NR_POINT_LIGHTS 24
#define NR_SPOT_LIGHTS 4

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

uniform vec3 viewPos;
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform DirectionLight directionLight;
uniform SpotLight spotLights[NR_SPOT_LIGHTS];
uniform Material material;

// Truck render mode uniforms
uniform int uTruckMode;        // 0=color, 1=texture, 2=blend
uniform vec3 uTruckColor;
uniform float uBlendFactor;

// Ground texture uniforms
uniform int uGroundMode;       // 0=color, 1=texture, 2=blend
uniform vec3 uGroundColor;
uniform float uGroundBlendFactor;
uniform float uTextureTiling;  // Scale factor for texture repetition

// function prototypes
vec3 CalcPointLight(Material material, PointLight light, vec3 N, vec3 fragPos, vec3 V, vec2 texCoords);
vec3 CalcDirectionLight(Material material, DirectionLight light, vec3 N, vec3 V, vec2 texCoords);
vec3 CalcSpotLight(Material material, SpotLight light, vec3 N, vec3 fragPos, vec3 V, vec2 texCoords);

void main()
{
    // properties
    vec3 N = normalize(Normal);
    vec3 V = normalize(viewPos - FragPos);
    
    // Apply texture tiling to texture coordinates
    vec2 tiledTexCoords = TexCoords * uTextureTiling;
    
    vec3 result = vec3(0.0);
    
    // directional light
    result += CalcDirectionLight(material, directionLight, N, V, tiledTexCoords);
    
    // point lights
    for(int i = 0; i < 24; i++)
        result += CalcPointLight(material, pointLights[i], N, FragPos, V, tiledTexCoords);
    
    // spot lights (jeep headlights)
    for(int i = 0; i < NR_SPOT_LIGHTS; i++)
        result += CalcSpotLight(material, spotLights[i], N, FragPos, V, tiledTexCoords);
    
    // Apply truck render mode
    if (uTruckMode == 0) {
        // Color only
        result = uTruckColor;
    }
    else if (uTruckMode == 2) {
        // Blended: 50% texture + 50% color
        vec3 texColor = result;  // This is already lit
        result = mix(texColor, uTruckColor, uBlendFactor);
    }
    // else uTruckMode == 1: keep texture (default, already computed)
    
    // Apply ground render mode (overrides truck mode for ground)
    if (uGroundMode == 0) {
        // Color only
        result = uGroundColor;
    }
    else if (uGroundMode == 2) {
        // Blended: 50% texture + 50% color
        vec3 texColor = result;
        result = mix(texColor, uGroundColor, uGroundBlendFactor);
    }
      
    FragColor = vec4(result, 1.0);
}

// calculates the color when using a point light.
vec3 CalcPointLight(Material material, PointLight light, vec3 N, vec3 fragPos, vec3 V, vec2 texCoords)
{
    vec3 L = normalize(light.position - fragPos);
    vec3 R = reflect(-L, N);
    
    // attenuation
    float d = length(light.position - fragPos);
    float attenuation = 1.0 / (light.k_c + light.k_l * d + light.k_q * (d * d));
    
    vec3 ambient = vec3(texture(material.diffuse, texCoords)) * light.ambient;
    vec3 diffuse = vec3(texture(material.diffuse, texCoords)) * max(dot(N, L), 0.0) * light.diffuse;
    vec3 specular = vec3(texture(material.specular, texCoords)) * pow(max(dot(V, R), 0.0), material.shininess) * light.specular;
    
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    
    return (ambient + diffuse + specular);
}

// calculates the color when using a directional light.
vec3 CalcDirectionLight(Material material, DirectionLight light, vec3 N, vec3 V, vec2 texCoords)
{
    vec3 L = normalize(-light.direction);
    vec3 R = reflect(-L, N);
    
    vec3 ambient = vec3(texture(material.diffuse, texCoords)) * light.ambient;
    vec3 diffuse = vec3(texture(material.diffuse, texCoords)) * max(dot(N, L), 0.0) * light.diffuse;
    vec3 specular = vec3(texture(material.specular, texCoords)) * pow(max(dot(V, R), 0.0), material.shininess) * light.specular;
    
    return (ambient + diffuse + specular);
}

// calculates the color when using a spot light.
vec3 CalcSpotLight(Material material, SpotLight light, vec3 N, vec3 fragPos, vec3 V, vec2 texCoords)
{
    vec3 L = normalize(light.position - fragPos);
    vec3 D = normalize(light.direction);  // IMPORTANT: normalize direction
    vec3 R = reflect(-L, N);
    
    // attenuation
    float d = length(light.position - fragPos);
    float attenuation = 1.0 / (light.k_c + light.k_l * d + light.k_q * (d * d));
    
    // spotlight cone check
    float cos_alpha = dot(L, D);
    float intensity = 0.0;
    
    if(cos_alpha > light.cos_theta)
    {
        // Smooth falloff within the cone
        intensity = cos_alpha;
    }
    
    vec3 ambient = vec3(texture(material.diffuse, texCoords)) * light.ambient;
    vec3 diffuse = vec3(texture(material.diffuse, texCoords)) * max(dot(N, L), 0.0) * light.diffuse;
    vec3 specular = vec3(texture(material.specular, texCoords)) * pow(max(dot(V, R), 0.0), material.shininess) * light.specular;
    
    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
    
    return (ambient + diffuse + specular);
}
