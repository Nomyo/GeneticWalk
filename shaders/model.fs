#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 Normal;
in vec3 pix_pos;

uniform sampler2D texture_diffuse1;
uniform vec3 lightPos;
uniform vec3 lightColor;

void main()
{
    vec4 textureColor = texture(texture_diffuse1, TexCoords);

    // ambient
    float ambientStrength = 0.5;
    vec3 ambient = ambientStrength * lightColor;

    // diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - pix_pos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    vec3 resultColor = (ambient + diffuse) * textureColor.xyz;
    FragColor = vec4(resultColor.x, resultColor.y, resultColor.z, 1.0f);
}
