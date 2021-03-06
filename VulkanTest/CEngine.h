#include "BUILD_ORDER.h"
#pragma once

class CEngine
{
public:
	void Run();

private:
	void InitWindow();
	void InitVulkan();
	void MainLoop();
	void Cleanup();
	void CreateInstance();
	void PrintExtensions();
	bool CheckValidationLayerSupport();
	void SetupDebugCallback();
	std::vector<const char*> GetRequiredExtensions();
	void PickPhysicalDevice();
	bool IsDeviceSuitable(VkPhysicalDevice device);
	int RateDeviceSuitability(VkPhysicalDevice device);
	SQueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device);
	void CreateLogicalDevice();
	void CreateSurface();
	bool CheckDeviceExtensionSupport(VkPhysicalDevice device);
	SSwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice device);

	// Debugging callback
	static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(
		VkDebugReportFlagsEXT flags,
		VkDebugReportObjectTypeEXT objType,
		uint64_t obj,
		size_t location,
		int32_t code,
		const char* layerPrefix,
		const char* msg,
		void* userData
	);
	VkResult CreateDebugReportCallbackEXT(
		const VkDebugReportCallbackCreateInfoEXT* pCreateInfo,
		const VkAllocationCallbacks* pAllocator, 
		VkDebugReportCallbackEXT* pCallback
	);
	void DestroyDebugReportCallbackEXT(VkDebugReportCallbackEXT pCallback,
		const VkAllocationCallbacks* pAllocator
	);

#ifdef NDEBUG
	const bool enableValidationLayers = false;
#else
	const bool enableValidationLayers = true;
#endif // NDEBUG

	const int WIDTH = 800;
	const int HIGHT = 600;

	const std::vector<const char*> validationLayers = {
		"VK_LAYER_LUNARG_standard_validation"
	};

	const std::vector<const char*> deviceExtensions = {
		VK_KHR_SWAPCHAIN_EXTENSION_NAME
	};

	VkDebugReportCallbackEXT callback;
	VkSurfaceKHR surface;
	VkPhysicalDevice physicalDevice = VK_NULL_HANDLE; // Handle for physical device
	VkDevice device; // Handle for logical device
	VkQueue graphicsQueue;
	VkQueue presentQueue;

	GLFWwindow* window;
	VkInstance instance;
};