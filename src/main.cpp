#include "ast_printer.hpp"
#include "lexer.hpp"
#include "parser.hpp"
#include <fstream>
#include <iostream>
#include <sstream>

void print_usage(const char* prog) {
    std::cerr << "Usage: " << prog << " [options] <file.tig>\n";
    std::cerr << "Options:\n";
    std::cerr << "  --lex     Print tokens only\n";
    std::cerr << "  --parse   Parse and report errors (default)\n";
    std::cerr << "  --ast     Print the AST\n";
}

std::string read_file(const std::string& path) {
    std::ifstream file(path);
    if (!file) {
        std::cerr << "Error: cannot open file: " << path << "\n";
        exit(1);
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

void run_lexer(const std::string& source) {
    tiger::Lexer lexer(source);
    while (true) {
        tiger::Token tok = lexer.next_token();
        std::cout << tok << "\n";
        if (tok.type == tiger::TokenType::END_OF_FILE) break;
    }

    if (lexer.has_errors()) {
        std::cerr << "\nLexer errors:\n";
        for (const auto& err : lexer.errors()) {
            std::cerr << "  " << err << "\n";
        }
    }
}

void run_parser(const std::string& source, bool print_ast) {
    tiger::Lexer lexer(source);
    tiger::Parser parser(lexer);

    auto program = parser.parse();

    if (lexer.has_errors()) {
        std::cerr << "Lexer errors:\n";
        for (const auto& err : lexer.errors()) {
            std::cerr << "  " << err << "\n";
        }
    }

    if (parser.has_errors()) {
        std::cerr << "Parser errors:\n";
        for (const auto& err : parser.errors()) {
            std::cerr << "  " << err << "\n";
        }
    }

    if (!lexer.has_errors() && !parser.has_errors()) {
        if (print_ast) {
            tiger::AstPrinter printer(std::cout);
            printer.print(*program);
        } else {
            std::cout << "Parsing successful!\n";
        }
    }
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        print_usage(argv[0]);
        return 1;
    }

    enum class Mode { LEX, PARSE, AST };
    Mode mode = Mode::PARSE;
    std::string filename;

    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        if (arg == "--lex") {
            mode = Mode::LEX;
        } else if (arg == "--parse") {
            mode = Mode::PARSE;
        } else if (arg == "--ast") {
            mode = Mode::AST;
        } else if (arg == "--help" || arg == "-h") {
            print_usage(argv[0]);
            return 0;
        } else {
            filename = arg;
        }
    }

    if (filename.empty()) {
        print_usage(argv[0]);
        return 1;
    }

    std::string source = read_file(filename);

    switch (mode) {
        case Mode::LEX:
            run_lexer(source);
            break;
        case Mode::PARSE:
            run_parser(source, false);
            break;
        case Mode::AST:
            run_parser(source, true);
            break;
    }

    return 0;
}
