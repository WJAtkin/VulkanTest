#include "BUILD_ORDER.h"

void CEngine::Run()
{
	InitWindow();
	InitVulkan();
	MainLoop();
	Cleanup();
}

void CEngine::InitWindow()
{
	glfwInit();
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	window = glfwCreateWindow(WIDTH, HIGHT, "Vulkan Engine Test", nullptr, nullptr);
}

void CEngine::InitVulkan()
{
	CreateInstance();
	SetupDebugCallback();
}

void CEngine::MainLoop()
{
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
	}
}

void CEngine::Cleanup()
{
	DestroyDebugReportCallbackEXT(callback, nullptr);
	vkDestroyInstance(instance, nullptr);

	glfwDestroyWindow(window);
	glfwTerminate();
}

void CEngine::CreateInstance()
{
	if (enableValidationLayers && !CheckValidationLayerSupport())
	{
		throw std::runtime_error("Validation layers requested, but not available!");
	}

	VkApplicationInfo appInfo = {};
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = "Vulkan Engine Test";
	appInfo.applicationVersion = VK_MAKE_VERSION(1, 1, 0);
	appInfo.pEngineName = "No Engine";
	appInfo.engineVersion = VK_MAKE_VERSION(1, 1, 0);
	appInfo.apiVersion = VK_API_VERSION_1_1;

	unsigned int glfwExtensionCount = 0;
	const char** glfwExtensions;
	glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

	VkInstanceCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	createInfo.pApplicationInfo = &appInfo;
	createInfo.enabledLayerCount = 0;
	auto extensions = GetRequiredExtensions();
	createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
	createInfo.ppEnabledExtensionNames = extensions.data();
	if (enableValidationLayers)
	{
		createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
		createInfo.ppEnabledLayerNames = validationLayers.data();
	}
	else
	{
		createInfo.enabledLayerCount = 0;
	}

	VkResult result = vkCreateInstance(&createInfo, nullptr, &instance);
	if (result != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to create instance!");
	}

	PrintExtensions();
}

void CEngine::PrintExtensions()
{
	// Gets the supported extensions and prints to console
	uint32_t extensionCount = 0;
	vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
	std::vector<VkExtensionProperties> extensions(extensionCount);
	vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());
	CLogger::LogMessage("Available extensions:");

	for (const auto& extension : extensions)
	{
		std::string msg = "        ";
		msg += extension.extensionName;
		CLogger::LogMessage(msg);
	}
}

bool CEngine::CheckValidationLayerSupport()
{
	uint32_t layerCount;
	vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

	std::vector<VkLayerProperties> availableLayers(layerCount);
	vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

	for (const char* layerName : validationLayers)
	{
		bool layerFound = false;

		for (const auto& layerProperties : availableLayers)
		{
			if (strcmp(layerName, layerProperties.layerName) == 0)
			{
				layerFound = true;
				break;
			}
		}

		if (!layerFound)
			return false;
	}

	return true;
}

std::vector<const char*> CEngine::GetRequiredExtensions()
{
	std::vector<const char*> extensions;

	unsigned int glfwExtensionCount = 0;
	const char** glfwExtensions;
	glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

	for (unsigned int i = 0; i < glfwExtensionCount; i++)
	{
		extensions.push_back(glfwExtensions[i]);
	}

	if (enableValidationLayers)
	{
		extensions.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
	}

	return extensions;
}

// Debugging callback
VKAPI_ATTR VkBool32 VKAPI_CALL CEngine::DebugCallback(VkDebugReportFlagsEXT flags,
	VkDebugReportObjectTypeEXT objType, uint64_t obj, size_t location, int32_t code,
	const char* layerPrefix, const char* msg, void* userData)
{
	std::string sMsg = "Validation layer: ";
	sMsg += msg;
	CLogger::LogMessage(sMsg);

	return VK_FALSE;
}

void CEngine::SetupDebugCallback()
{
	if (!enableValidationLayers) 
		return;

	VkDebugReportCallbackCreateInfoEXT createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT;
	createInfo.flags = VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT;
	createInfo.pfnCallback = DebugCallback;

	// Passes the debugging callback to vulkan
	if (CreateDebugReportCallbackEXT(&createInfo, nullptr, &callback) != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to set up debug callback!");
	}
}

VkResult CEngine::CreateDebugReportCallbackEXT(
	const VkDebugReportCallbackCreateInfoEXT* pCreateInfo,
	const VkAllocationCallbacks* pAllocator,
	VkDebugReportCallbackEXT* pCallback)
{
	auto func = (PFN_vkCreateDebugReportCallbackEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugReportCallbackEXT");
	if (func != nullptr)
		return func(instance, pCreateInfo, pAllocator, pCallback);
	else
		return VK_ERROR_EXTENSION_NOT_PRESENT;
}

void CEngine::DestroyDebugReportCallbackEXT(
	VkDebugReportCallbackEXT pCallback,
	const VkAllocationCallbacks* pAllocator)
{
	auto func = (PFN_vkDestroyDebugReportCallbackEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugReportCallbackEXT");
	if (func != nullptr)
	{
		func(instance, callback, pAllocator);
	}
}