#include "view.h"
#include "Windows.h"

namespace {
    const std::string Help_message =
    "------------------------------------------------------------------------\n"
    "|dump <filename> - save the universe to a file                         |\n"
    "|tick <n=1> - calculate n (default 1) iterations and print the result  |\n"
    "|exit - finish the game                                                |\n"
    "|help - print information about commands                               |\n"
    "------------------------------------------------------------------------";
}

void Game_view::print_help() const {
    std::cout << Help_message << std::endl;
}

void Game_view::print_err(const std::string& err_message) const {
    std::cout << err_message << std::endl;
}

void Game_view::print_frame(const std::string& name, const Field& field, int glob_iteration, int curr_iteration) const {
    std::cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";
    std::cout << "Name: " + name + ";" << std::endl;
    std::cout << "Total iteration: " << glob_iteration << ";" << std::endl;
    std::cout << "Current iteration: " << curr_iteration << ";" << std::endl;
    
    for (int i = -1; i <= field.get_size(); ++i) {
        for (int j = -1; j <= field.get_size(); ++j) {
            if (i < 0 || i == field.get_size()) {
                std::cout << "--";
            } else if (j < 0) {
                std::cout << "| ";
            } else if (j == field.get_size()) {
                std::cout << " |";
            } else {
                if (field.get_state(i, j)) {
                    std::cout << "* ";
                } else {
                    std::cout << "  ";
                }
            }
        }
        std::cout << std::endl;
    }
    Sleep(500);
}