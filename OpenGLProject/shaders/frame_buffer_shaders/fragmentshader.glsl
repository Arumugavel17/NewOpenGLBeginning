#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D hello;
uniform int shadow = 0;

void main() {    
        if(shadow == 0){
            FragColor = texture(hello,TexCoord);
        }else if (shadow == 1){
            float depth = texture(hello,TexCoord).r;
            FragColor = vec4(vec3(depth),10f);
            FragColor = texture(hello,TexCoord);
        }
}