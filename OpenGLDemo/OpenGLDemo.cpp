#include <iostream>
#include <fstream> //reads files
#include <string>
#include <sstream>
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

struct ShaderProgramSource
{
	std::string VertexSource;
	std::string FragmentSource;
};

const GLint WIDTH = 800, HEIGHT = 600;

static ShaderProgramSource ParseShader(const std::string& filepath)
{
	std::ifstream stream(filepath); //input file path 

	enum class ShaderType
	{
		NONE = -1,
		VERTEX = 0,
		FRAGMENT = 1
	}; //defining the different shader types

	std::string line;
	std::stringstream ss[2];
	ShaderType type = ShaderType::NONE;
	while (getline(stream, line))
	{
		if (line.find("#shader") != std::string::npos)
		{
			if (line.find("vertex") != std::string::npos)
				type = ShaderType::VERTEX; //tells us we are dealing with a vertex shader
			else if (line.find("fragment") != std::string::npos)
				type = ShaderType::FRAGMENT; //same as above but for fragmen
		}
		else
		{
			ss[(int)type] << line << '\n';
		}
	}

	return { ss[0].str(), ss[1].str() };
}

static unsigned int CompileShader(unsigned int type, const std::string& source)
{
	unsigned int id = glCreateShader(type);
	const char* src = source.c_str();
	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);

	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE) {
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char* message = (char*)alloca(length * sizeof(char));
		glGetShaderInfoLog(id, length, &length, message);
		std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << "shader!" << std::endl;
		std::cout << message << std::endl;
		glDeleteShader(id);
		return 0;
	}


	return id;
}

static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
	unsigned int program = glCreateProgram();
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);
	glValidateProgram(program);

	glDeleteShader(vs);
	glDeleteShader(fs);

	return program;
}

int main() {
	glfwInit();

	// GLFW version range
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // CORE uses the new opengl functions while COMPAR uses older opengl functions
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // Enables forwards compatibility
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE); // Resisable window true/false

	// Window settings
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Test Test", nullptr, nullptr);

	// In case GLFW fails
	if (nullptr == window) {
		std::cout << "GLFW Failure" << std::endl;
		glfwTerminate();
		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);

	int screenWidth, screenHeight;
	glfwGetFramebufferSize(window, &screenWidth, &screenHeight);

	glewExperimental = GL_TRUE; 

	if (GLEW_OK != glewInit()) {
		std::cout << "Failure GLEW" << std::endl;
		return EXIT_FAILURE;
	}

	// Initial coordinates setting
	glViewport(0, 0, screenWidth, screenHeight);

	float verticesData[] = {
		-0.5f, -0.5f,	1.0f, 0.0f, 0.0f,
		 0.5f, -0.5f,	0.0f, 1.0f, 0.0f,
		 0.0f, 0.5f,	0.0f, 0.0f, 1.0f
	}; //triangle vertex positions and colour data, x first y second then rbg data

	unsigned int vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	unsigned int buffer; //place to store the buffer
	glGenBuffers(1, &buffer); //create 1 buffer
	glBindBuffer(GL_ARRAY_BUFFER, buffer); //bind the buffer to the array buffer, what is assigned to the bind will be drawn
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesData), verticesData, GL_STATIC_DRAW);  //send the vertices data to the buffer

	glEnableVertexAttribArray(0); //enable the vertex position attribute array
	glEnableVertexAttribArray(1); //enable the colour array
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, 0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void*)(2 * sizeof(float)));

	// glm::vec4 vec(1.0f, 0.0f, 0.0f, 1.0f); //put in our inital vertex position as a vector
	// glm::mat4 trans = glm::mat4(1.0f); //define a matrix to hold a transformation of 1
	// trans = glm::translate(trans, glm::vec3(1.0f, 1.0f, 0.0f)); //translate the matrix by 1 in the x and y direction
	// vec = trans * vec; //perform the transformation
	// std::cout << vec.x << vec.y << vec.z << std::endl; // output results of transformation, should be 2 1 0

	ShaderProgramSource source = ParseShader("OpenGLFiles/shaders/basic.shader"); //tells the shader command what shader file to use
	//std::cout << source.VertexSource << std::endl;
	//std::cout << source.FragmentSource << std::endl;

	glm::mat4 trans = glm::mat4(1.0f);


	unsigned int shader = CreateShader(source.VertexSource, source.FragmentSource);
	glUseProgram(shader);


	//window main loop
	while (!glfwWindowShouldClose(window)) {

		glClear(GL_COLOR_BUFFER_BIT);

		glDrawArrays(GL_TRIANGLES, 0, 3); //draw 3 vertices starting from index 0

		trans = glm::mat4(1.0f);
		trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0.0, 1.0, 0.0));
		unsigned int transformLoc = glGetUniformLocation(shader, "transform");
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));

		glfwSwapBuffers(window); 

		glfwPollEvents();
	}

	glDeleteProgram(shader);
	glfwTerminate();
	return EXIT_SUCCESS;
}

//stride - amount of bytes used to describe all vertex attributes (position, texture coords, normals, etc) floats are 4 bytes
//pointer - at what byte does the desription of the attribute start