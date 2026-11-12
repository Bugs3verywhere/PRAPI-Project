#shader vertex
#version 330 core

layout(location = 0) in vec3 position; //vertices first attribute is position

uniform mat4 transform;
uniform mat4 view; 
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * transform * vec4(position, 1.0); //applies the transformation of all of our matrices to the position of the objects vertices
}

#shader fragment
#version 330 core

uniform vec4 planetColour;
out vec4 colour;

void main()
{   
    colour = planetColour;
} 