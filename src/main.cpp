#include "cpp2web/converter.hpp"
#include "cpp2web/platform.hpp"

#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <iterator>
#include <string>

namespace {
std::string readFile(const std::filesystem::path& path) {
    std::ifstream input(path);
    if (!input) return {};
    return {std::istreambuf_iterator<char>(input), std::istreambuf_iterator<char>()};
}
void printHelp() {
    std::cout << "CPP2WEB - convert a small C++ UI DSL into a static web app\n\n"
              << "Usage: cpp2web_app [--input app.cpp] [--output directory] [--open]\n"
              << "Supported calls: .window(\"Main\"), .text(\"...\"),\n"
              << "  .input(\"id\", \"placeholder\"), .button(\"label\", \"action\")\n";
}
}

int main(int argc, char** argv) {
    std::filesystem::path outputDir = "./output";
    std::filesystem::path inputFile;
    bool openBrowser = false;
    for (int i = 1; i < argc; ++i) {
        const std::string arg = argv[i];
        if (arg == "--output" && i + 1 < argc) outputDir = argv[++i];
        else if (arg == "--input" && i + 1 < argc) inputFile = argv[++i];
        else if (arg == "--open") openBrowser = true;
        else if (arg == "--help" || arg == "-h") { printHelp(); return 0; }
        else { std::cerr << "Unknown or incomplete option: " << arg << "\n"; printHelp(); return 2; }
    }

    cpp2web::Converter converter("CPP2WEB application");
    if (!inputFile.empty()) {
        const auto source = readFile(inputFile);
        if (source.empty()) { std::cerr << "Cannot read input file: " << inputFile << "\n"; return 1; }
        converter.fromSourceText(source);
    } else {
        converter.addWindow("Welcome");
        converter.addLabel("A useful, dependency-free web UI generated from C++.");
        converter.addInput("name", "Type your name");
        converter.addButton("Submit", "Hello from CPP2WEB");
    }
    if (!converter.writeProject(outputDir)) { std::cerr << "Failed to write " << outputDir << "\n"; return 1; }
    std::cout << "Generated " << std::filesystem::absolute(outputDir) << " on " << cpp2web::platformName() << "\n";
    if (openBrowser) {
        const auto command = cpp2web::openFileCommand(outputDir / "index.html");
        if (!command.empty()) std::system(command.c_str());
    }
    return 0;
}
