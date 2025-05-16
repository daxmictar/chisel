#include <cstdlib>
#include <fstream>
#include <optional>
#include <print>
#include <string>
#include <vector>

class ChiselFile {
    std::size_t line_number_;
    std::ifstream current_file_;
    std::string file_name_;

public:
    ChiselFile(std::string file_name) noexcept {
        // check if the file is a valid chisel file:
        // - .ch extension
        if (!file_name.ends_with(".ch")) {
            std::println("file: {}, is not a chisel file", file_name);
            return;
        } else {
            std::println("ends with .ch");
        }

        current_file_ = std::ifstream { file_name };
        if (!current_file_.is_open()) {
            current_file_.close();
            return;
        } else {
            std::println("file: {}, is open", file_name);
        }

        this->file_name_ = file_name;
        this->line_number_ = 0;
    }

    ~ChiselFile() {
        current_file_.close();
        std::println("closing file by calling ~ChiselFile()");
    }

    constexpr std::string file_name() {
        return this->file_name_;
    }

    constexpr bool is_file_open() {
        return !this->current_file_.is_open();
    }

    constexpr bool is_file_valid() {
        return !this->current_file_.bad();
    }

    std::optional<std::vector<std::string>> contents() {
        if (!this->current_file_.is_open()) {
            return std::nullopt;
        }

        std::vector<std::string> file_contents {};
        std::string string_buffer;
        while (std::getline(this->current_file_, string_buffer)) {
            file_contents.push_back(string_buffer);
            ++this->line_number_;
        }

        return std::make_optional(file_contents);
    }
};

class Parser {
    ChiselFile* current_file;
};
