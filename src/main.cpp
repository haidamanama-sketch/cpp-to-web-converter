#include "cpp2web/converter.hpp"
#include "cpp2web/platform.hpp"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

namespace {

std::string readFileIfExists(const std::filesystem::path& path) {
    std::ifstream input(path);
    if (!input.is_open()) {
        return {};
    }
    return std::string((std::istreambuf_iterator<char>(input)), std::istreambuf_iterator<char>());
}

}  // namespace

int main(int argc, char** argv) {
    std::filesystem::path outputDir = "./output";
    std::filesystem::path inputFile;
    bool openBrowser = false;

    for (int i = 1; i < argc; ++i) {
        const std::string arg = argv[i];
        if (arg == "--output" && i + 1 < argc) {
            outputDir = argv[++i];
        } else if (arg == "--input" && i + 1 < argc) {
            inputFile = argv[++i];
        } else if (arg == "--open") {
            openBrowser = true;
        } else if (arg == "--help" || arg == "-h") {
            std::cout << "Usage: cpp2web_app [--input input.cpp] [--output output_dir] [--open]\n";
            return 0;
        }
    }

    cpp2web::Converter converter("C++ to Web App");

    if (!inputFile.empty()) {
        const std::string source = readFileIfExists(inputFile);
        if (source.empty()) {
            std::cerr << "Input file not found or empty: " << inputFile << "\n";
            return 1;
        }
        converter.fromSourceText(source);
    } else {
        converter.addWindow("Main");
        converter.addButton("Click Me");
        converter.addLabel("C++ generated HTML / JavaScript / CSS");
    }

    if (!converter.writeProject(outputDir)) {
        std::cerr << "Failed to export project to " << outputDir << "\n";
        return 1;
    }

    std::cout << "Generated project at: " << std::filesystem::absolute(outputDir) << "\n";
    std::cout << "Platform: " << cpp2web::platformName() << "\n";

    if (openBrowser) {
        const auto command = cpp2web::openFileCommand(outputDir / "index.html");
        if (!command.empty()) {
            std::system(command.c_str());
        }
    }

    return 0;
}
