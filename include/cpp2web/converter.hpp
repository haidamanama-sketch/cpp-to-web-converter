#pragma once

#include <filesystem>
#include <string>
#include <vector>

namespace cpp2web {

struct Input {
    std::string id;
    std::string placeholder;
};

struct Button {
    std::string label;
    std::string action;
};

class Converter {
public:
    explicit Converter(std::string title = "C++ to Web App");

    // Parses the supported, deliberately small UI DSL from C++ source text.
    void fromSourceText(const std::string& source);

    void addWindow(const std::string& windowName);
    void addButton(const std::string& buttonLabel, const std::string& action = {});
    void addLabel(const std::string& labelText);
    void addInput(const std::string& id, const std::string& placeholder = {});

    std::string generateHTML() const;
    std::string generateCSS() const;
    std::string generateJS() const;
    bool writeProject(const std::filesystem::path& outputDir) const;

private:
    std::string title_;
    std::vector<std::string> windows_;
    std::vector<Button> buttons_;
    std::vector<std::string> labels_;
    std::vector<Input> inputs_;
};

}  // namespace cpp2web
