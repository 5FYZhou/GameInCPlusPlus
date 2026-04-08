#include <windows.h>
#include "Game.h"

int main() {
    SetConsoleOutputCP(CP_UTF8);
    Game game;
    game.Run();
    return 0;
}
