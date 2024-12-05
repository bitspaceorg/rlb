#version 330

in vec2 fragTexCoord;
in vec4 fragColor;

out vec4 finalColor;

uniform sampler2D texture0;
uniform vec3 lightDir;
uniform float ambientStrength;

void main() {
    vec4 texColor = texture(texture0, fragTexCoord);
    float aoFactor = max(dot(normalize(lightDir), vec3(0.0, 1.0, 0.0)), 0.0);
    vec3 ambient = texColor.rgb * ambientStrength * aoFactor;
    finalColor = vec4(ambient, texColor.a);
}
