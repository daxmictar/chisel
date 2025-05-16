#include "parser.cc"
#include <optional>
#include <print>

namespace chisel {

std::optional<std::vector<std::string>> c_args_to_vec(std::size_t argc, char* argv[]) {
    if (argc <= 1) {
        return std::nullopt;
    }

    std::vector<std::string> arguments {};
    for (std::size_t idx{ 0 }; idx < argc; ++idx) {
        std::string argument { argv[idx] };
        arguments.push_back(argument);
    }

    return std::make_optional(arguments);
}

constexpr bool is_chisel_file(std::string file_name) {
    return file_name.ends_with(".ch");
}

void run(std::string file_name) {
    auto chisel_file = ChiselFile { file_name };
    if (!chisel_file.is_file_valid() && !chisel_file.is_file_open()) {
        std::println("File: {} is malformed or could not be opened.", chisel_file.file_name());
        std::exit(1);
    }

    auto chisel_file_contents { chisel_file.contents() };
    if (!chisel_file_contents.has_value()) {
        std::println("Could not retrieve contents from file: {}", chisel_file.file_name());
        return;
    }
}

}

int main(int argc, char* argv[]) {
    auto cli_arguments = chisel::c_args_to_vec(argc, argv);
    if (!cli_arguments.has_value()) {
        std::println("Usage:\n\n\tchisel [file]\n\n");
        std::exit(1);
    }

    auto args = cli_arguments.value();
    switch (args.size()) {
    case 2: 
        {
            if (chisel::is_chisel_file(args[1])) {
                chisel::run(args[1]);
            }

        }


    }

    for (auto &argument : cli_arguments.value()) {
        std::println("currently detected argument: {}", argument);
    }

    // for now
    std::exit(0);

    ChiselFile chisel_file {"example.ch"};
    auto file_contents { chisel_file.contents() };
    if (file_contents.has_value()) {
        for (auto s : file_contents.value()) {
            std::println("{}", s);
        }
    }

    return EXIT_SUCCESS;
}
