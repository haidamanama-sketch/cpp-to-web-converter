#pragma once

#include <cstdlib>
#include <filesystem>
#include <string>

namespace cpp2web {

enum class Platform {
    Windows,
    Linux,
    macOS,
    Unknown
};

inline Platform currentPlatform() {
#if defined(_WIN32)
    return Platform::Windows;
#elif defined(__APPLE__)
    return Platform::macOS;
#elif defined(__linux__)
    return Platform::Linux;
#else
    return Platform::Unknown;
#endif
}

inline std::string platformName() {
    switch (currentPlatform()) {
        case Platform::Windows: return "Windows";
        case Platform::Linux: return "Linux";
        case Platform::macOS: return "macOS";
        default: return "Unknown";
    }
}

inline std::string openFileCommand(const std::filesystem::path& filePath) {
    const std::string path = filePath.string();
    switch (currentPlatform()) {
        case Platform::Windows:
            return std::string("cmd /c start \"\" \"") + path + "\"";
        case Platform::macOS:
            return std::string("open \"") + path + "\"";
        case Platform::Linux:
            return std::string("xdg-open \"") + path + "\"";
        default:
            return std::string();
    }
}

}  // namespace cpp2web
