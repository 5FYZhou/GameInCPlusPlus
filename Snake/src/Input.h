#pragma once

#include "Constants.h"

enum InputCommand { None, Default, Up, Down, Left, Right, Pause, Exit, ChangeMode, Sprint,
Confirm, Cancel };

class Input {
public:
    Input();
    
    InputCommand ProcessInput();
    bool KeyDown(InputCommand cmd);

    InputCommand GInput();

    bool GetOneKey(InputCommand cmd);

    bool WaitAny();
};