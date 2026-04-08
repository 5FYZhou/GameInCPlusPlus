#pragma once

#include <string>
#include <tuple>
#include <unordered_map>
#include <vector>

enum Direction { STOP = 0, UP, DOWN, LEFT, RIGHT };
enum class ShowMode { HALF, FULL };
enum class CodeType { DEFAULT, WALL, SNAKE, FOOD, TEXT };
enum class GameState { GAMEOVER, RUNNING, PAUSED };

struct Vector2 {
    int x, y;
    Vector2(int a = 0, int b = 0) : x(a), y(b) {}
    Vector2 operator+(const Vector2& other) const { return { x + other.x, y + other.y }; }
    Vector2& operator+=(const Vector2& other) { x += other.x; y += other.y; return *this; }
    bool operator==(const Vector2& other) const { return x == other.x && y == other.y; }
};

struct GameData {
    ShowMode mode = ShowMode::FULL;
    std::vector<Vector2> snake;
    Vector2 food;
    Direction dir = Direction::STOP;
    int score = 0;
};

inline constexpr int WIDTH = 20;
inline constexpr int HEIGHT = 20;
inline constexpr int WALL_WIDTH = 1;
inline constexpr int SCENE_WIDTH = WIDTH + WALL_WIDTH * 2;
inline constexpr int SCENE_HEIGHT = HEIGHT + WALL_WIDTH * 2;
inline constexpr int INFOSETOFF = 5;
inline constexpr int DELAY = 200;
inline const std::string SAVE_FILE_NAME = "data/GameData.txt";

inline const std::unordered_map<Direction, Vector2> DIR_MAP = {
    {UP, {0, -1}},
    {DOWN, {0, 1}},
    {LEFT, {-1, 0}},
    {RIGHT, {1, 0}}
};

inline const std::unordered_map<CodeType, std::string> codeMapHalf = {
    {CodeType::DEFAULT, " "},
    {CodeType::WALL, "#"},
    {CodeType::SNAKE, "O"},
    {CodeType::FOOD, "*"}
};

inline const std::unordered_map<CodeType, std::string> codeMapFull = {
    {CodeType::DEFAULT, "  "},
    {CodeType::WALL, "＃"},
    {CodeType::SNAKE, "Ｏ"},
    {CodeType::FOOD, "＊"}
};

inline const std::unordered_map<CodeType, int> colorMap = {
    {CodeType::DEFAULT, 0x00},
    {CodeType::WALL, 0x0f},
    {CodeType::SNAKE, 0x03},
    {CodeType::FOOD, 0x0d},
    {CodeType::TEXT, 0x0f}
};

inline const std::vector<std::tuple<std::string, CodeType, int>> INFO = {
    {"■操作说明：", CodeType::TEXT, 0},
    {"□开始游戏：WASD", CodeType::TEXT, INFOSETOFF * 0.5},
    {"□暂停游戏：空格", CodeType::TEXT, INFOSETOFF * 0.5},
    {"□退出游戏：Esc", CodeType::TEXT, INFOSETOFF * 0.5},
    {"□显示模式切换：回车", CodeType::TEXT, INFOSETOFF * 0.5}
};
