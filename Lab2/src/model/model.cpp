#include "model.h"
#include "../util/util.h"
#include <iostream>

namespace {
    std::vector<bool> rule_to_bool(const std::string& rule) {
        std::vector<bool> new_rule (9, false);

        for (const auto& i: rule) {
            new_rule[i - '0'] = true;
        }

        return std::move(new_rule);
    }

    int neighbors(const Field& field, int size, int row, int column) {
        int count = 0;
        int maxind = size - 1;

        for (int i = -1; i < 2; ++i) {
            for (int j = -1; j < 2; ++j) {
                if (i != 0 || j != 0) {
                    if (field.get_state(i + row, j + column)) {
                        ++count;
                    }
                }
            }
        }

        return count;
    }
}

Game_model::Game_model(): Game_model(
    Default_name, 
    Default_b_rule, 
    Default_s_rule, 
    Default_frame,
    0
) {}

Game_model::Game_model(
    const std::string& name,
    const std::string& brule,
    const std::string& srule, 
    const std::vector<Point>& coords,
    int tick
): 
    name_(name),
    b_rule_(rule_to_bool(brule)),
    s_rule_(rule_to_bool(srule)),
    curr_field_(coords),
    global_iteration_(tick)
{};

const Field& Game_model::get_field() const { 
    return curr_field_;
}

const std::string& Game_model::get_name() const {
    return name_;
}

int Game_model::get_glob_iteration() const {
    return global_iteration_;
}

void Game_model::compute_iteration(bool update_glob_it) {
    std::swap(curr_field_, old_field_);
    
    for (int i = 0; i < old_field_.get_size(); ++i) {
        for (int j = 0; j < old_field_.get_size(); ++j) {
            curr_field_.set_state(i, j, false);
            int count_neigh = neighbors(old_field_, old_field_.get_size(), i, j);
            if (old_field_.get_state(i, j) && s_rule_[count_neigh]) {
                curr_field_.set_state(i, j, true);
            }
            else if (!old_field_.get_state(i, j) && b_rule_[count_neigh]) {
                curr_field_.set_state(i, j, true);
            }
        }
    }
    if (update_glob_it) {
        ++global_iteration_;
    }
}