#include <GL/glew.h>
#include <GL/glut.h>
#include "Shader.hpp"

namespace
{
	std::string readFile(std::string path)
	{
		using namespace std;

		string contents;
		ifstream fin(path, ios::in | ios::binary);
		if(!fin.good())
			throw runtime_error("Could not open shader file: " + path);

		fin.seekg(0, ios::end); 		//go to end
		contents.resize(fin.tellg());	//make contents big enough to hold the file
		fin.seekg(0, ios::beg);			//to back to the beginning
		fin.read(&contents[0], contents.size()); //read all contents
		fin.close();					//close file descriptor

		return contents;
	}


	std::string getCode(std::string path)
	{
		auto contents = readFile(path); //reads shader file

		#ifdef GL_ES_VERSION_2_0
			std::string header = "#version 100\n"
				"#define GLES2\n";
		#else
			std::string header = "#version 120\n";
		#endif

		return header + contents;
	}
	


	void setCode(GLuint handle, const std::string &code)
	{
		const GLchar* source[1] = { code.c_str() };
		glShaderSource(handle, 1, source, NULL);
		glCompileShader(handle);
		GLint compile_ok = GL_FALSE;
		glGetShaderiv(handle,GL_COMPILE_STATUS,&compile_ok);

		if(compile_ok == GL_FALSE)
			throw std::runtime_error("Shader failed to compile");
	}
}



template<typename T>
std::shared_ptr<cs5400::Shader<T>> make_shader(std::string path)
{
	auto code = getCode(path);
	auto shader = std::make_shared<cs5400::Shader<T>>();
	setCode(shader->getHandle(),code);
	return shader;
}



std::shared_ptr<cs5400::VertexShader> cs5400::make_vertexShader(std::string path)
{
	return make_shader<detail::VertexShaderTag>(path);
}



std::shared_ptr<cs5400::FragmentShader> cs5400::make_fragmentShader(std::string path)
{
	return make_shader<detail::FragmentShaderTag>(path);
}
