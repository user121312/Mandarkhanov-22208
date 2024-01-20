#pragma once
#include "../util/util.h"
#include <iostream>
#include <string>

class Game_view {
public:
    void print_err(const std::string& err_message) const;
    void print_help() const;
    void print_frame(const std::string& name, const Field& field, int glob_iteration, int curr_iteration) const;
};