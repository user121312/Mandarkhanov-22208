#include <iostream>
#include <vector>
#include <string>
#include "controller/controller.h"
#include "model/model.h"
#include "view/view.h"

std::vector<std::string> pars_args(int argc, char** argv) {
    std::vector<std::string> args(argc);
    for (int i = 0; i < argc; ++i) {
        args[i] = argv[i];
    }
    return args;
}

int main(int argc, char** argv) {
    std::unique_ptr<Game_controller> life;
    std::vector<std::string> args = pars_args(argc, argv);
    try {
        if (argc == 1) { // defolt mode
            life = std::make_unique<Game_controller>();
        }
        
        else if (argc == 2) { // online mode
            life = std::make_unique<Game_controller>(args[1]);
        }

        else if (argc == 6) { // оffline mode
            int i_num = 1, o_num = 1;
            for (int i = 0; i < argc; ++i) {
                if (args[i] == "-o") {
                    o_num += i;
                }
                else if (args[i] == "-i") {
                    i_num += i;
                }
            }
            if (i_num > 2 && o_num > 2) {
                life = std::make_unique<Game_controller>(args[1], args[i_num], args[o_num]);
            }
            else {
                std::cout << "You inserted the wrong keys";
            }
        }
        else {
            std::cout << "You inserted the wrong keys";
            return 1;
        }
    }
    catch (std::invalid_argument) {
        return 1;
    }
    life->play_game();
    return 0;
}