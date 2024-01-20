#pragma once
#include "../model/model.h"
#include "../view/view.h"
#include "../util/util.h"
#include <string>
#include <memory>

class Game_controller{
private:
    std::unique_ptr<Game_model> model_;
    Game_view view_;
    std::string out_file;
public:
    Game_controller();
    Game_controller(const std::string& infile);
    Game_controller(const std::string& infile, const std::string& ticks, const std::string& outfile);
    void play_game();
};