#version 330 core

in vec2 texCoord;
flat in int texIndex;

out vec4 FragColor;

uniform sampler2D blockTextures[4];
  
void main()
{
    float outline = 0.03;

    if (texCoord.x < outline || texCoord.x > 1.0 - outline || texCoord.y < outline || texCoord.y > 1.0 - outline) 
        FragColor = vec4(0.0, 0.0, 0.0, 1.0);
    else
        FragColor = texture(blockTextures[texIndex], texCoord);
}