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

constexpr void print_usage() {
    std::println("Usage:\n\n\tchisel [file]\n\n");
}

constexpr void run(std::string file_name) {
    auto chisel_file = ChiselFile { file_name };
    if (!chisel_file.is_file_valid() && !chisel_file.is_file_open()) {
        std::println("File: {} is malformed or could not be opened.", chisel_file.file_name());
        std::exit(1);
    }

    auto maybe_file_contents { chisel_file.contents() };
    if (!maybe_file_contents.has_value()) {
        std::println("Could not retrieve contents from file: {}", chisel_file.file_name());
        return;
    }

    auto chisel_file_contents { maybe_file_contents.value() };
    std::println("Simulating a run of {} with contents:\n", file_name);
    for (auto line : chisel_file_contents) {
        std::println("{}", line);
    }
    std::println();
}

}

int main(int argc, char* argv[]) {
    auto cli_arguments = chisel::c_args_to_vec(argc, argv);
    if (!cli_arguments.has_value()) {
        chisel::print_usage();
        std::exit(1);
    }

    auto args = cli_arguments.value();
    for (auto argument : args) {
        std::println("currently detected argument: {}", argument);
    }

    switch (args.size()) {
    case 2: 
        {
            if (chisel::is_chisel_file(args[1])) {
                chisel::run(args[1]);
            }

            break;
        }
    default:
        chisel::print_usage();
    }

    return EXIT_SUCCESS;
}
