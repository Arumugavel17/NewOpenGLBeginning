#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
uniform vec4 outline_color;
uniform sampler2D texture_diffuse1;

void main()
{
    if(outline_color.w == 0.0f){
        FragColor = texture(texture_diffuse1, TexCoords);
    }
    else{
        FragColor = outline_color;
    }
}