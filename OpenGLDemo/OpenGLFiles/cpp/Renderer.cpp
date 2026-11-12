#include "Renderer.h"
#include "GL/glew.h"
#include "glm/glm.hpp"

void Renderer::Initialize(const glm::vec3* verticesData, std::size_t verticesSize, const unsigned int* indices, std::size_t indicesSize)
{
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &VBO); //create 1 buffer
	glBindBuffer(GL_ARRAY_BUFFER, VBO); //bind the buffer to the array buffer, what is assigned to the bind will be drawn
	glBufferData(GL_ARRAY_BUFFER, verticesSize, verticesData, GL_STATIC_DRAW);  //send the vertices data to the buffer

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesSize, indices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0); //enable the vertex position attribute array
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), nullptr);
}

void Renderer::Draw(unsigned int indexCount)
{
	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0); //draw triangles using the 36 indices starting at element 0 of the array
}

void Renderer::Clear()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

Renderer::~Renderer()
{
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
}