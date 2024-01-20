#pragma once
#include "../util/util.h"
#include <string>
#include <vector>

class Game_model {
private:
    int global_iteration_;
    std::string name_;
    const std::vector<bool> b_rule_;
    const std::vector<bool> s_rule_;
    Field curr_field_;
    Field old_field_;
public:
    Game_model();
    Game_model(const std::string& name, const std::string& brule, const std::string& srule, const std::vector<Point>& coords, int tick);

    int get_glob_iteration () const;
    void compute_iteration(bool update_glob_it = true);
    const std::string& get_name() const;
    const Field& get_field() const;
};