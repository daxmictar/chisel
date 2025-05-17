#include <cstdlib>
#include <exception>
#include <fstream>
#include <optional>
#include <print>
#include <string>
#include <vector>

namespace chisel {

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
    ChiselFile* current_file_;
    std::vector<std::string> current_file_contents_;

public:
    Parser(std::string file_name) {
        ChiselFile* chisel_file = new ChiselFile(file_name);
        if (chisel_file->is_file_open() && chisel_file->is_file_valid()) {
            this->current_file_ = chisel_file;
        }
    }

    ~Parser() {
        delete current_file_;
    }

    constexpr bool open(std::string file_name) {
        auto chisel_file = new ChiselFile(file_name);
        if (!chisel_file->is_file_open() && !chisel_file->is_file_valid()) {
            delete chisel_file;
            return false;
        }

        this->current_file_ = chisel_file;

        // check if the file contents are valid
        auto maybe_chisel_file_contents { chisel_file->contents() };
        if (!maybe_chisel_file_contents.has_value()) {
            delete chisel_file;
            return false;
        }

        this->current_file_contents_ = maybe_chisel_file_contents.value();

        return true;
    }

    constexpr std::size_t file_size() {
        return this->current_file_contents_.size();
    }

    /// Advance the position of the parser by `offset`.
    constexpr std::size_t advance(std::size_t offset) {
        auto size { this->file_size() };
        if (size + offset > size) {
            return 0;
        }

        return size + offset;
    }
};

}
