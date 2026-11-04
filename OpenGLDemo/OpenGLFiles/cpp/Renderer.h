#pragma once
#include <cstddef>

class Renderer
{
	public:
		void Initialize(const float* verticesData, std::size_t verticesSize, const unsigned int* indices, std::size_t indicesSize);
		
		void Draw(unsigned int indexCount);

		void Clear();

		~Renderer();

	private:
		unsigned int vao = 0;
		unsigned int vertexBuffer = 0;
		unsigned int elementBuffer = 0;
};