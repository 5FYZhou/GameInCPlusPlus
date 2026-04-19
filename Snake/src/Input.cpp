#include "Input.h"
#include <conio.h>
#include <windows.h>

Input::Input() {
    // 构造函数，如果需要初始化可以在这里
}

bool Input::GetOneKey(InputCommand cmd) {
    int ch = _getch();
    switch (cmd) {
        case Pause:
            if (ch == 32) return true; // Space
            break;
        case ChangeMode:
            if (ch == 13) return true; // Enter
            break;
        case Confirm:
            if (ch == 'Y' || ch == 'y') return true;
            break;
        default:
            break;
    }
    return false;
}

InputCommand Input::ProcessInput() {
    InputCommand cmd = None;
    while (_kbhit()) {
        int ch = _getch();
        if (ch == 0 || ch == 224) {
            // 方向键、功能键等特殊键返回两字节序列
            int special = _getch();
            switch (special) {
            case 72: cmd = Up; break;    // ↑
            case 80: cmd = Down; break;  // ↓
            case 75: cmd = Left; break;  // ←
            case 77: cmd = Right; break; // →
            default: cmd = Default; break;
            }
            continue;
        }

        switch (ch) {
        case 'w': case 'W': cmd = Up; break;
        case 's': case 'S': cmd = Down; break;
        case 'a': case 'A': cmd = Left; break;
        case 'd': case 'D': cmd = Right; break;
        case 'y': case 'Y': cmd = Confirm; break;
        // Ctrl + WASD 可能产生控制字符
        case 23: cmd = Up; break;    // Ctrl+W
        case 19: cmd = Down; break;  // Ctrl+S
        case 1:  cmd = Left; break;  // Ctrl+A
        case 4:  cmd = Right; break; // Ctrl+D
        // ESC
        case 27: cmd = Exit; break;
        // Space
        case 32: cmd = Pause; break;
        // Enter
        case 13: cmd = ChangeMode; break;
        default: cmd = Default; break;
        }
    }
    return cmd;  // 返回处理后的命令
}

bool Input::KeyDown(InputCommand cmd){
    switch (cmd)
    {
    case Sprint:
        return (GetAsyncKeyState(VK_MENU) & 0x8000);
    default:
        break;
    }
    return false;
}

InputCommand Input::GInput(){
    // 先处理退出 / 功能键（优先级高）
    if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) return Exit;

    static SHORT lastSpaceState = 0;
    SHORT now = GetAsyncKeyState(VK_SPACE);
    // 检测“刚按下”
    if ((now & 0x8000) && !(lastSpaceState & 0x8000)) {
        lastSpaceState = now;
        return Pause;
    }
    lastSpaceState = now;

    
    if (GetAsyncKeyState(VK_RETURN) & 0x8000) return ChangeMode;
    if((GetAsyncKeyState('Y') & 0x8000)) return Confirm;

    // 方向键（支持 WASD + 方向键）
    if ((GetAsyncKeyState('W') & 0x8000) || (GetAsyncKeyState(VK_UP) & 0x8000)) {
        return Up;
    }
    if ((GetAsyncKeyState('S') & 0x8000) || (GetAsyncKeyState(VK_DOWN) & 0x8000)) {
        return Down;
    }
    if ((GetAsyncKeyState('A') & 0x8000) || (GetAsyncKeyState(VK_LEFT) & 0x8000)) {
        return Left;
    }
    if ((GetAsyncKeyState('D') & 0x8000) || (GetAsyncKeyState(VK_RIGHT) & 0x8000)) {
        return Right;
    }

    if (_kbhit()) {
        int ch = _getch();
        if (ch != 32 && ch != 13 && ch != 27 && ch != 'Y' && ch != 'y') {
            return Default;
        }
    }
    return None;
}

bool Input::WaitAny(){
    if(_getch()) return true;
    return false;
}