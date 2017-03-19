#version 430 core

in vec3 position;
in vec3 color;
// in GLfloat now;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

out vec3 vColor;

void main()
{
    vColor = color;
    gl_Position = proj * view * model * vec4(position.x, position.y, position.z, 1.0);
}
