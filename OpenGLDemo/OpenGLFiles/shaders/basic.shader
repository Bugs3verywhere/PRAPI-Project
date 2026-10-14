#shader vertex
#version 330 core

layout(location = 0) in vec3 position; //vertices first attribute is position
layout(location = 1) in vec3 vertexColour; //vertices second attribute is colour

out vec3 colour;
uniform mat4 transform;
uniform mat4 view; 
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * transform * vec4(position, 1.0); //applies the transformation of all of our matrices to the position of the objects vertices
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