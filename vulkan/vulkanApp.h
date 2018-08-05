#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW\glfw3.h>
#include <stdexcept>
#include <functional>
#include <cstdlib>
#include <vector>
#include <iostream>
#include <set>
#include <algorithm>
#include <fstream>
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <array>
#include <chrono>

struct QueueFamilyIndices {
	int graphicsFamily = -1;
	int presentFamily = -1;
	bool isComplete() {
		return graphicsFamily >= 0 && presentFamily >= 0;
	}
};
struct SwapchainSupportDetails {
	VkSurfaceCapabilitiesKHR capabilities;
	std::vector<VkSurfaceFormatKHR> formats;
	std::vector<VkPresentModeKHR> presentModes;
};
struct Vertex {
	glm::vec3 pos;
	glm::vec3 color;

	static VkVertexInputBindingDescription getBindingDescription()
	{
		VkVertexInputBindingDescription bindingDescription = {};
		bindingDescription.binding = 0;
		bindingDescription.stride = sizeof(Vertex);
		bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
		return bindingDescription;
	}

	//leírás a shaderek számára paraméterek stb...
	static std::array<VkVertexInputAttributeDescription, 2> getAttributeDescriptions()
	{
		std::array<VkVertexInputAttributeDescription, 2> attributeDescriptions = {};
		attributeDescriptions[0].binding = 0;
		attributeDescriptions[0].location = 0;
		attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
		attributeDescriptions[0].offset = offsetof(Vertex, pos);
		attributeDescriptions[1].binding = 0;
		attributeDescriptions[1].location = 1;
		attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
		attributeDescriptions[1].offset = offsetof(Vertex, color);

		return attributeDescriptions;
	}
};

struct UniformBufferObject {
	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 proj;
};

class App {
public: 
	void Run()
	{
		InitWindow();
		InitVulkan();
		MainLoop();
		CleanUp();
	}

private:

	//index buffer rajzoláshoz
	const std::vector<Vertex> vertices = {
		{ { -1.0f,-1.0f,-1.0f },{ 1.0f,0.0f,1.0f } },//b.a.h
		{ { 1.0f,-1.0f,-1.0f   },{ 1.0f,0.0f,0.0f } },//j.a.h
		{ { 1.0f,1.0f,-1.0f  },{ 1.0f,0.5f,0.5f } }, //j.f.h
		{ {-1.0f,1.0f,-1.0f },{1.0f,0.0f,0.0f}},//b.f.h
		{ {-1.0f,-1.0f,1.0f },{ 0.0f,1.0f,1.0f }},//b.a.e
		{ { 1.0f,-1.0f,1.0f },{ 1.0f,1.0f,0.0f } },//j.a.e
		{ { -1.0f,1.0f,1.0f },{ 0.0f,0.0f,1.0f } },//b.f.e
		{ { 1.0f,1.0f,1.0f },{ 1.0f,0.0f,0.0f } },//j.f.e
	};

	//vertex buffer rajzoláshoz
	const std::vector<Vertex> vertices2
	{
		{{-1.0f,-1.0f,-1.0f}, { 0.583f,  0.771f,  0.014f}},// triangle 1 : begin
	{ { -1.0f,-1.0f, 1.0f },{ 0.609f,  0.115f,  0.436f }},
	{ { -1.0f, 1.0f, 1.0f }, { 0.327f,  0.483f,  0.844f }} ,// triangle 1 : end
	{ { 1.0f, 1.0f,-1.0f },{ 0.822f,  0.569f,  0.201f} } , // triangle 2 : begin
	{ { -1.0f,-1.0f,-1.0f },{ 0.435f,  0.602f,  0.223f } } ,
	{ { -1.0f, 1.0f,-1.0f },{ 0.310f,  0.747f,  0.185f } } ,// triangle 2 : end
	{ { 1.0f,-1.0f, 1.0f },{ 0.597f,  0.770f,  0.761f } } ,
	{ { -1.0f,-1.0f,-1.0f },{ 0.559f,  0.436f,  0.730f } } ,
	{ { 1.0f,-1.0f,-1.0f },{ 0.359f,  0.583f,  0.152f } },
	{ { 1.0f, 1.0f,-1.0f },{ 0.483f,  0.596f,  0.789f }} ,
	{ { 1.0f,-1.0f,-1.0f },{ 0.559f,  0.861f,  0.639f } } ,
	{ { -1.0f,-1.0f,-1.0f },{ 0.195f,  0.548f,  0.859f } } ,
	{ { -1.0f,-1.0f,-1.0f },{ 0.014f,  0.184f,  0.576f} } ,
	{ { -1.0f, 1.0f, 1.0f }, { 0.771f,  0.328f,  0.970f } } ,
	{ { -1.0f, 1.0f,-1.0f},{ 0.406f,  0.615f,  0.116f } } ,
	{ { 1.0f,-1.0f, 1.0f },{ 0.676f,  0.977f,  0.133f } } ,
	{ { -1.0f,-1.0f, 1.0f },{ 0.971f,  0.572f,  0.833f } } ,
	{ { -1.0f,-1.0f,-1.0f },{ 0.140f,  0.616f,  0.489f } } ,
	{ { -1.0f, 1.0f, 1.0f },{ 0.997f,  0.513f,  0.064f } } ,
	{ { -1.0f,-1.0f, 1.0f },{ 0.945f,  0.719f,  0.592f } } ,
	{ { 1.0f,-1.0f, 1.0f },{ 0.543f,  0.021f,  0.978f } } ,
	{ { 1.0f, 1.0f, 1.0f },{ 0.279f,  0.317f,  0.505f } } ,
	{ { 1.0f,-1.0f,-1.0f },{ 0.167f,  0.620f,  0.077f } } ,
	{ { 1.0f, 1.0f,-1.0f },{ 0.347f,  0.857f,  0.137f } } ,
	{ { 1.0f,-1.0f,-1.0f },{ 0.055f,  0.953f,  0.042f } } ,
	{ { 1.0f, 1.0f, 1.0f },{ 0.714f,  0.505f,  0.345f } } ,
	{ { 1.0f,-1.0f, 1.0f },{ 0.783f,  0.290f,  0.734f } } ,
	{ { 1.0f, 1.0f, 1.0f },{ 0.722f,  0.645f,  0.174f } } ,
	{ { 1.0f, 1.0f,-1.0f },{ 0.302f,  0.455f,  0.848f } } ,
	{ { -1.0f, 1.0f,-1.0f },{ 0.225f,  0.587f,  0.040f } } ,
	{ {  1.0f, 1.0f, 1.0f },{ 0.517f,  0.713f,  0.338f } } ,
	{ { -1.0f, 1.0f,-1.0f },{ 0.053f,  0.959f,  0.120f } } ,
	{ { -1.0f, 1.0f, 1.0f },{ 0.393f,  0.621f,  0.362f } } ,
	{ { 1.0f, 1.0f, 1.0f },{ 0.673f,  0.211f,  0.457f } } ,
	{ { -1.0f, 1.0f, 1.0f },{ 0.820f,  0.883f,  0.371f } } ,
	{ { 1.0f,-1.0f, 1.0f },{ 0.982f,  0.099f,  0.879f } }
	}; 
	//összekötött csúcsok sorrendje
	const std::vector<uint16_t> indices = {
		0,1,2,2,3,0, //hátsó oldal
		0,4,5,5,1,0, //alsó oldal
		0,3,6,6,4,0, // bal oldal
		3,6,7,7,2,3, // felsõ oldal
		1,2,7,7,5,1,  // jobb oldal
		5,7,6,6,4,5   // elülsõ oldal
	};

	GLFWwindow * window;
	VkInstance instance;
	VkDebugReportCallbackEXT callback;
	VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
	VkDevice device;
	VkQueue graphicsQueue;
	VkQueue presentQueue;
	VkSurfaceKHR surface;
	VkSwapchainKHR swapChain;
	std::vector<VkImage> swapChainImages;
	VkFormat swapChainImageFormat;
	VkExtent2D swapChainExtent;
	std::vector<VkImageView> swapChainImageViews;
	VkShaderModule vertexShaderModule;
	VkShaderModule fragmentShaderModule;
	VkRenderPass renderPass;
	VkDescriptorSetLayout descriptorSetLayout;
	VkDescriptorPool descriptorPool;
	VkDescriptorSet descriptorSet;
	VkPipelineLayout pipelineLayout;
	VkPipeline graphicsPipeline;
	std::vector<VkFramebuffer> swapChainFramebuffers;
	VkCommandPool commandPool;
	std::vector<VkCommandBuffer> commandBuffers;
	VkBuffer vertexBuffer;
	VkDeviceMemory vertexBufferMemory;
	VkBuffer indexBuffer;
	VkDeviceMemory indexBufferMemory;
	VkBuffer uniformBuffer;
	VkDeviceMemory uniformBufferMemory;

	//renderelésre kész jelzése
	VkSemaphore imageAvailableSemaphore;
	//megjelenítsére kész jelzése
	VkSemaphore renderFinishedSemaphore;
	const int WIDTH = 800;
	const int HEIGHT = 600;
	const std::vector<const char*> validationLayers = {
		"VK_LAYER_LUNARG_standard_validation"
	};
	const std::vector<const char*> deviceExtensions = {
		VK_KHR_SWAPCHAIN_EXTENSION_NAME
	};	

	
#ifdef NDEBUG
	const bool enableValidationLayers = false;
#else
	const bool enableValidationLayers = true;
#endif // NDEBUG

	static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT objtype, uint64_t obj, size_t location,
														int32_t code, const char* layerPrefix, const char* msg, void* userData);
	void InitWindow();
	void InitVulkan();
	void CreateInstance();
	bool CheckValidationLayerSupport();
	bool CheckDeviceExtensionSupport(VkPhysicalDevice device);
	std::vector<const char*> getRequiredExtensions();
	void setupDebugCallback();
	VkResult CreateDebugReportCallbackEXT(VkInstance instance, const VkDebugReportCallbackCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugReportCallbackEXT* pCallback);
	void DestroyDebugReportCallbackEXT(VkInstance instance, VkDebugReportCallbackEXT callback, const VkAllocationCallbacks* pAllocator);
	void PickPhysicalDevice();
	bool isDeviceSuitable(VkPhysicalDevice device);
	QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
	void CreateLogicalDevice();
	void CreateSurface();
	void CreateSwapChain();
	void CreateImageViews();
	void CreateRenderPass();
	void CreateGraphicsPipeline();
	void CreateFramebuffers();
	void CreateCommandPool();
	void CreateCommandBuffers();
	void CreateBuffer(VkDeviceSize size,VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer & buffer, VkDeviceMemory& bufferMemory);
	void CreateVertexBuffer();
	void CreateIndexBuffer();
	void CreateUniformBuffer();
	void UpdateUniformBuffer();
	void CreateDescriptorSetLayout();
	void CreateDescriptorPool();
	void CreateDescriptorSet();
	void CopyBuffer(VkBuffer srcBuffer,VkBuffer dstBuffer,VkDeviceSize size);
	void DrawFrames();
	void CreateSemaphores();
	void ReCreateSwapChain();
	void CleanUpSwapChain();
	uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
	static void onWindowResized(GLFWwindow* window, int width, int height);
	static std::vector<char> ReadShader(const std::string & filename);
	VkShaderModule createShaderModule(const std::vector<char> & code);
	SwapchainSupportDetails querySwapChainSupport(VkPhysicalDevice device);
	VkSurfaceFormatKHR ChoosSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
	VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR> availablePresentModes);
	VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
	void MainLoop();
	void CleanUp();

};