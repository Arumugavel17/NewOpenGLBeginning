	#version 330 core

in vec3 Normal;
in vec3 FragPos;
in vec2 TextCoord;
out vec4 FragColor;

struct SpotLight {
    float innerCuttOf;
    float outerCuttOf;
    
    float constant;
    float linear;
    float quadratic;

    vec3 position;
    vec3 forward;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight{
    float constant;
    float linear;
    float quadratic;
    
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct DirectionLight{
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};


struct Material {
    sampler2D diffuseMap;
    sampler2D specularMap;
    float shininess;
};

uniform SpotLight SpotLight_instance;
uniform DirectionLight DirectionLight_instance;
uniform PointLight PointLight_instance;
uniform Material material;
uniform vec3 ViewPos;

vec3 direction_light(DirectionLight LightSource){

    // ambient
    vec3 temp = texture(material.diffuseMap,TextCoord).xyz;
    vec3 ambient = LightSource.ambient * temp;
    
    // diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = -normalize(LightSource.direction);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = LightSource.diffuse * (diff * temp);
    
    // specular
    vec3 viewDir = normalize(ViewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = LightSource.specular * (spec * texture(material.specularMap,TextCoord).xyz);

    vec3 result = ambient + diffuse + specular;
    return result * 2;
}

vec3 point_light(PointLight LightSource){
    
    // ambient
    vec3 temp = texture(material.diffuseMap,TextCoord).xyz;
    vec3 ambient = LightSource.ambient * temp;
    
    // diffuse
    vec3 diffuse = vec3(0.0f),specular = vec3(0.0f);
    
    float distance = length(LightSource.position - FragPos);
    float attenuation = 1.0 / (LightSource.constant + (LightSource.linear * distance) + LightSource.quadratic * (distance*distance)); 
    
    vec3 lightDir = normalize(LightSource.position - FragPos);
        
    vec3 norm = normalize(Normal);
    float diff = max(dot(norm, lightDir), 0.0);
    diffuse = LightSource.diffuse * (diff * temp);
    
    vec3 viewDir = normalize(ViewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    specular = LightSource.specular * (spec * texture(material.specularMap,TextCoord).xyz);
    
    diffuse *= attenuation;
    specular *= attenuation;

    ambient *= attenuation;
    
    vec3 result = ambient + diffuse + specular;
    return result * 2;
}

vec3 spot_light(SpotLight LightSource){
    float epsilon = LightSource.innerCuttOf - LightSource.outerCuttOf;
    // ambient
    vec3 temp = texture(material.diffuseMap,TextCoord).xyz;
    vec3 ambient = LightSource.ambient * temp;
    
    // diffuse
    vec3 diffuse = vec3(0.0f),specular = vec3(0.0f);
    
    float distance = length(LightSource.position - FragPos);
    float attenuation = 1.0 / (LightSource.constant + (LightSource.linear * distance) + LightSource.quadratic * (distance*distance)); 
    
    vec3 lightDir = normalize(LightSource.position - FragPos);
    float theta = dot(-lightDir,normalize(LightSource.forward));

    if(theta > LightSource.outerCuttOf){
        float intensity = clamp((theta - LightSource.outerCuttOf) / epsilon, 0.0f, 1.0f);
        
        vec3 norm = normalize(Normal);
        float diff = max(dot(norm, lightDir), 0.0);
        diffuse = LightSource.diffuse * (diff * temp);
        
        vec3 viewDir = normalize(ViewPos - FragPos);
        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
        specular = LightSource.specular * (spec * texture(material.specularMap,TextCoord).xyz);
        
        diffuse *= intensity;
        specular *= intensity;
        diffuse *= attenuation;
        specular *= attenuation;
    }

    ambient *= attenuation;
    
    vec3 result = ambient + diffuse + specular;
    return result * 10;
}
void main()
{
    vec3 result = vec3(0.0);

    // Adding the result of the spot_light function call
    // result += spot_light(SpotLight_instance);
    result += direction_light(DirectionLight_instance);
    result += point_light(PointLight_instance);

    FragColor = vec4(result, 1.0);
}