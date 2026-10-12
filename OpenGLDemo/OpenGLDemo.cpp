#include <iostream>

#define GLEW_STATIC

#include <GL/glew.h>
#include <GLFW/glfw3.h>

//Windows prop
const GLint WIDTH = 800, HEIGHT = 600;

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

	float positions[6] = {
		-0.5f, -0.5f,
		 0.5f, -0.5f,
		 0.0f, 0.5f,
	}; //triangle vertex positions, x first y second

	unsigned int vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	unsigned int buffer; //place to store the buffer
	glGenBuffers(1, &buffer); //create 1 buffer
	glBindBuffer(GL_ARRAY_BUFFER, buffer); //bind the buffer to the array buffer, what is assigned to the bind will be drawn
	glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), positions, GL_STATIC_DRAW);  //send the data to the bufferW

	glEnableVertexAttribArray(0); //enable the vertex attribute array
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);

	std::string vertexShader =
		"#version 330 core\n"
		"\n"
		"layout(location = 0) in vec4 position;\n"
		"\n"
		"void main()\n"
		"{\n"
		"	gl_Position = position;\n"
		"}\n";

	std::string fragmentShader =
		"#version 330 core\n"
		"\n"
		"out vec4 color;\n"
		"\n"
		"void main()\n"
		"{\n"
		"	color = vec4(0.5, 0.0, 1.0, 1.0);\n"
		"}\n";

	unsigned int shader = CreateShader(vertexShader, fragmentShader);
	glUseProgram(shader);

	//window main loop
	while (!glfwWindowShouldClose(window)) {

		glClear(GL_COLOR_BUFFER_BIT);

		glDrawArrays(GL_TRIANGLES, 0, 3); //draw 3 vertices starting from index 0

		glfwSwapBuffers(window); 

		glfwPollEvents();
	}


	glfwTerminate();

	return EXIT_SUCCESS;
}

//stride - amount of bytes used to describe all vertex attributes (position, texture coords, normals, etc) floats are 4 bytes
//pointer - at what byte does the desription of the attribute start