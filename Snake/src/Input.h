#pragma once

#include "Constants.h"

enum InputCommand { None, Up, Down, Left, Right, Pause, Exit, ChangeMode };

class Input {
public:
    Input();

    // 处理运行时的输入，返回命令
    InputCommand ProcessRunningInput();

    // 处理暂停时的输入，返回命令
    InputCommand ProcessPausedInput();
};