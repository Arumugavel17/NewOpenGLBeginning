#version 330 core
out vec4 FragColor;

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
    vec3 viewPos;
} fs_in;

uniform sampler2D floorTexture;
uniform vec3 lightPos;

//struct DirectionLight{
//    vec3 direction;
//    vec3 ambient;
//    vec3 diffuse;
//    vec3 specular;
//};
//
//struct Material {
//    sampler2D diffuseMap;
//    sampler2D specularMap;
//    float shininess;
//};
//
//uniform DirectionLight DirectionLight_instance;
//uniform Material material;

void main()
{           
    vec3 color = texture(floorTexture, fs_in.TexCoords).rgb;
    
    // ambient
    vec3 ambient = 0.05 * color;

    // diffuse
    vec3 lightDir = normalize(lightPos - fs_in.FragPos);
    vec3 normal = normalize(fs_in.Normal);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * color;
    // specular
    vec3 viewDir = normalize(fs_in.viewPos - fs_in.FragPos);

    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = 0.0;
    
    vec3 halfwayDir = normalize(lightDir + viewDir);  
    spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);
    
    vec3 specular = vec3(0.3) * spec; // assuming bright white light color
    FragColor = vec4( ambient + diffuse + specular , 1.0);
    //FragColor = vec4(vec3(1.0f) * dot(normal, halfwayDir) , 1.0);
}