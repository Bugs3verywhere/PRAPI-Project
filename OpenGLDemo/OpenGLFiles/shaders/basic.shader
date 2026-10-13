#shader vertex
#version 330 core

layout(location = 0) in vec2 position; //vertices first attribute is position
layout(location = 1) in vec3 vertexColour; //vertices second attribute is colour

out vec3 colour;

void main()
{
    gl_Position = vec4(position, 0.0, 1.0);
    colour = vertexColour; // take vertices colour attribute and assign it to a vec3 variable
}

#shader fragment
#version 330 core

in vec3 colour;
out vec4 color;

void main()
{
    color = vec4(colour, 1.0);
}