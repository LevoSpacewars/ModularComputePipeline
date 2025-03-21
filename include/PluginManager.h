#include <iostream>
#include <filesystem>
#include <vector>
#include <dlfcn.h>
#include <unordered_map>
#include <cassert>

//! Define a function pointer for the init function

/// @brief Class that dynamically loads plugins (in the form of shared libraries) at runtime.
class pluginManager
{
public:
    /// @brief Initializer
    pluginManager() = default;

    /// @brief With input plugin_path, loads and return void* plugin
    /// @param std::string plugin_path
    /// @note file pathed must be .so (shared library).
    /// @note shared libraries must be recompiled on each device.
    /// @return void *
    /// @see description for return of dlopen() https://man7.org/linux/man-pages/man3/dlopen.3.html

    /// @throws dlerror() if dlopen(plugin_path, RTLD_LAZY) returns a null pointer

    void *loadPlugin(const std::string plugin_path)
    {

        const char *path = plugin_path.c_str(); // Convert std::string to const char*

        void *plugin = dlopen(path, RTLD_LAZY); // dlopen takes in const char * and __mode
        if (!plugin)
        {
            std::cerr << "Failed to load plugin: " << dlerror() << std::endl;
        }
        return plugin;
    };

    /// @brief With input Directory path, loads all plugins (in the form of shared libraries) as void* and returns a hashmap
    /// @param std::string dirPath containing set of shared libraries
    /// @return std::unordered_map<std::string, void *>
    /// @note hash keys are shared library file names devoid of the file extension.
    /// @note see loadPlugin method for more information on loaded plugins.

    std::unordered_map<std::string, void *> autoLoadDIR(std::string dirPath)
    {

        std::filesystem::directory_iterator files(dirPath);

        std::unordered_map<std::string, void *> sharedLibs;
        // Iterate over files in the directory
        for (auto file : files)
        {
            if (file.path().extension() == ".so")
            {

                // Open the shared library
                std::string file_name = file.path().filename();
                std::string raw_path = file.path();
                file_name = file_name.substr(0, file_name.length() - 3); // cut off file extension

                sharedLibs[file_name] = loadPlugin(raw_path);
            }
        }
        return sharedLibs;
    }
};
