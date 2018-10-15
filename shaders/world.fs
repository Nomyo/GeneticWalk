#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 pix_pos;
in vec3 Normal;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_diffuse2;

uniform vec2 endzone_coord;
uniform float endzone_radius;

uniform vec3 lightPos;
uniform vec3 lightColor;

void main()
{
    float nq_endz_x = (pix_pos.x - endzone_coord.x);
    float nq_endz_y = (pix_pos.z - endzone_coord.y);
    float sq_endz_r = endzone_radius * endzone_radius;

    vec4 textureColor;
    bool in_circle = (nq_endz_x * nq_endz_x + nq_endz_y * nq_endz_y) < sq_endz_r;
    if (in_circle)
      textureColor = texture(texture_diffuse2, TexCoords);
    else
      textureColor = texture(texture_diffuse1, TexCoords);

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
