#pragma once

#include <filesystem>
#include <fstream>
#include <regex>
#include <sstream>
#include <string>
#include <vector>

namespace cpp2web {

class Converter {
public:
    explicit Converter(std::string title = "C++ to Web App");

    void fromSourceText(const std::string& source);

    void addWindow(const std::string& windowName);
    void addButton(const std::string& buttonLabel);
    void addLabel(const std::string& labelText);

    std::string generateHTML() const;
    std::string generateCSS() const;
    std::string generateJS() const;
    bool writeProject(const std::filesystem::path& outputDir) const;

private:
    std::string title_;
    std::vector<std::string> windows_;
    std::vector<std::string> buttons_;
    std::vector<std::string> labels_;
};

}  // namespace cpp2web
