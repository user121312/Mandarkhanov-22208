#include "controller.h"
#include <algorithm>
#include <fstream>

namespace {
    const std::string Digits = "012345678";

    int check_num(const std::string& num) {
        if (num.empty() || (num[0] == '0' && num.size() > 1)) {
            throw std::invalid_argument("Number of ticks is zero or absent");
        }

        for (size_t i = 0; i < num.size(); ++i) {
            if (!isdigit(num[i]) && num[i] != '-') {
                throw std::invalid_argument("Number of tikcs has incorrect symbols");
            }
        }

        return stoi(num);
    }

    std::string check_rule(const std::string& rule) {
        if (rule.empty() || !std::is_sorted(rule.begin(), rule.end())) {
            throw std::invalid_argument("Rule is incorrect");
        }

        for (const auto& i: rule) {
            if (Digits.find(i) == Digits.npos) {
                throw std::invalid_argument("Rule is incorrect");
            }
        }

        return rule;
    }

    void print_in_file(std::ofstream& out, const Field& field, const std::string& name, int glob_iteration) {
        out << "Name: " + name + ";" << std::endl;
        out << "Total iteration: " << glob_iteration << ";" << std::endl;

        for (int i = -1; i <= field.get_size(); ++i) {
            for (int j = -1; j <= field.get_size(); ++j) {
                if (i < 0 || i == field.get_size()) {
                    out << "--";
                }
                else if (j < 0) {
                    out << "| ";
                }
                else if (j == field.get_size()) {
                    out << " |";
                }
                else {
                    if (field.get_state(i, j)) {
                        out << "* ";
                    }
                    else {
                        out << "  ";
                    }
                }
            }
            out << std::endl;
        }
    }
}

Game_controller::Game_controller() {
    model_ = std::make_unique<Game_model>();
}

Game_controller::Game_controller(const std::string& inf): Game_controller(inf, "1", "") {}

Game_controller::Game_controller(const std::string& infile, const std::string& ticks, const std::string& outfile) {
    int tick;
    std::string name;
    std::string b_rule;
    std::string s_rule; 
    std::string str;
    std::vector<Point> coords;
    coords.reserve(1000);
    std::ifstream ifile (infile);

    try {
        if (infile.size() < 4 || infile.substr(infile.size() - 5, 5) == ".txt" || !ifile.is_open()) {
            throw std::invalid_argument("You inserted the wrong in file");
        }

        tick = check_num(ticks);
        std::getline(ifile, str);
        if (str != "#Life 1.06") {
            throw std::invalid_argument("You inserted the wrong type of universe");
        }

        getline(ifile, str);
        if (str.size() < 3 || str.substr(0, 3) != "#N ") {
            std::invalid_argument("You inserted the wrong name of universe");
        }
        else {
            name = str.substr(3, str.size() - 3);
        }

        getline(ifile, str);
        if (str.size() < 8 || str.substr(0, 4) != "#R B") {
            throw std::invalid_argument("There are no transition rules");
        }
        else {
            size_t pos = str.find('/');
            if (pos == str.npos || str[pos + 1] != 'S') {
                throw std::invalid_argument("You entered the wrong rules");
            }
            b_rule = check_rule(str.substr(4, pos - 4));
            s_rule = check_rule(str.substr(pos + 2, str.size() - pos - 2));
        }
        
        while (getline(ifile, str)) {
            size_t pos = str.find(' ');
            if (pos == 0 || pos == str.npos) { 
                throw std::invalid_argument("Not enough coordinates");
            }

            try {
                int x = check_num(str.substr(0, pos));
                int y = check_num(str.substr(pos + 1, str.size() - pos - 1));
                coords.push_back({x, y});    
            }
            catch (std::invalid_argument) {
                throw std::invalid_argument("Incorrect coordinates");
            }
        }
        out_file = outfile;
        model_ = std::make_unique<Game_model>(name, b_rule, s_rule, coords, tick);
    }
    catch (std::invalid_argument& err) {
        view_.print_err(err.what());
        std::rethrow_exception(std::current_exception());
    }
}

void Game_controller::play_game() {
    try {
        if (!out_file.empty()) {
            std::ofstream ofile (out_file);
            if (out_file.size() < 4 || 
                out_file.substr(out_file.size() - 5, 5) == ".txt" || 
                !ofile.is_open()) 
            {
                throw std::invalid_argument("You inserted the wrong out file");
            }
            for (int i = 0; i < model_->get_glob_iteration(); ++i) {
                model_->compute_iteration(false);
            }

            print_in_file(
                ofile, model_->get_field(), 
                model_->get_name(), model_->get_glob_iteration()
            );

            return;
        }
        std::string command = "help";

        while (command != "exit") {
            if (command.substr(0, 4) == "tick") {
                int num_tick;
                if (command.size() < 4) {
                    num_tick = 1;
                }
                else {
                    try {
                        num_tick = check_num(command.substr(5, command.size() - 5));
                    } catch (std::invalid_argument) {
                        num_tick = 1;
                    }
                }

                for (int i = 1; i <= num_tick; ++i) {
                    view_.print_frame(model_->get_name(), model_->get_field(), model_->get_glob_iteration() + 1, i);
                    model_->compute_iteration();
                }
            }
            else if (command.substr(0, 4) == "dump") {
                if (command.size() < 9 || command.substr(command.size() - 5, 5) == ".txt") {
                    view_.print_err("You inserted the wrong out file");
                }
                else {
                    std::ofstream ofile(command.substr(5, command.size()- 5));
                    if (ofile.is_open()) {
                        print_in_file(ofile, model_->get_field(), model_->get_name(), model_->get_glob_iteration());
                    }
                }
            }
            else {
                view_.print_help();
            }
            getline(std::cin, command);
        }
    }
    catch (std::invalid_argument& err) {
        view_.print_err(err.what());
        std::rethrow_exception(std::current_exception());
    }
};