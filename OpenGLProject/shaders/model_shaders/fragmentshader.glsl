#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 Normal;
in vec4 Position;

uniform vec4 outline_color;
uniform sampler2D texture_diffuse1;

uniform vec3 cameraPos;
uniform samplerCube skybox;

void main()
{
    if(outline_color.w == 0.0f){
        vec3 I = normalize(Position.xyz - cameraPos);
        vec3 R = refract(I, normalize(Normal),1.00f/1.52f);
        vec4 model_color = texture(texture_diffuse1, TexCoords);
        vec4 reflection = vec4(texture(skybox, R).rgb, 1.0);
        //FragColor = model_color;
        FragColor = reflection;
    }
    else{
        FragColor = outline_color;
    }
}