#version 450 core

in vec2 fragTexCoord;
in vec3 fragColor;

out vec4 color;

uniform float currentTime;

uniform sampler2D tex;

void main (void)
{
    color = texture(tex, fragTexCoord);


   //  color = vec4(colorTemp, 1.0f);
}