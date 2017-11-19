#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 pix_pos;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_diffuse2;

uniform vec2 endzone_coord;
uniform float endzone_radius;

void main()
{
    float nq_endz_x = (pix_pos.x - endzone_coord.x);
    float nq_endz_y = (pix_pos.z - endzone_coord.y);
    float sq_endz_r = endzone_radius * endzone_radius;

    bool in_circle = (nq_endz_x * nq_endz_x + nq_endz_y * nq_endz_y) < sq_endz_r;
    if (in_circle)
      FragColor = texture(texture_diffuse2, TexCoords);
    else
      FragColor = texture(texture_diffuse1, TexCoords);
}
