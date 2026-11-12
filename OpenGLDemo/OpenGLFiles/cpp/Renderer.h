#pragma once
#include <cstddef>
#include "glm/glm.hpp"

class Renderer
{
	public:
		void Initialize(const glm::vec3* verticesData, std::size_t verticesSize, const unsigned int* indices, std::size_t indicesSize);
		
		void Draw(unsigned int indexCount);

		void Clear();

		~Renderer();

	private:
		unsigned int vao = 0;
		unsigned int VBO = 0;
		unsigned int EBO = 0;
};