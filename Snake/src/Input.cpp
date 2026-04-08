#include "Input.h"
#include <conio.h>

Input::Input() {
    // 构造函数，如果需要初始化可以在这里
}

InputCommand Input::ProcessRunningInput() {
    if (_kbhit()) {
        switch (_getch()) {
        case 'w':
            return Up;
        case 's':
            return Down;
        case 'a':
            return Left;
        case 'd':
            return Right;
        case 27:  // ESC
            return Exit;
        case 32:  // Space
            return Pause;
        case 13:  // Enter
            return ChangeMode;
        default:
            return None;
        }
    }
    return None;
}

InputCommand Input::ProcessPausedInput() {
    if (_kbhit()) {
        switch (_getch()) {
        case 27:  // ESC
            return Exit;
        case 13:  // Enter
            return ChangeMode;
        default:
            return None;  // 继续游戏
        }
    }
    return Pause;  // 保持暂停状态
}