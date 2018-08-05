#include "vulkanApp.h"
#include <GLFW\glfw3.h>
#include <glm\vec4.hpp>
#include <glm\mat4x4.hpp>
#include <iostream>

int main()
{
	App vulkan;

	try {
		vulkan.Run();
	}
	catch (const std::runtime_error & e)
	{
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}
	
	return EXIT_SUCCESS;
}