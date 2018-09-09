#include "game.hpp"
#include "engine.hpp"

void game_loop() {
    Engine engine = Engine(800, 600, "Control_and_Defeat");
    while (!engine.stop()) {

        engine.finish_frame();
    }
}
