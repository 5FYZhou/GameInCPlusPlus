#pragma once

#include "Constants.h"
#include <fstream>

class SaveSystem {
public:
    static bool Save(const GameData& data,
                     const std::string& filePath = SAVE_FILE_NAME) {
        std::ofstream out(filePath);
        if (!out.is_open()) {
            return false;
        }

        out << static_cast<int>(data.mode) << "\n";
        out << data.snake.size() << "\n";
        for (const auto& v : data.snake) {
            out << v.x << " " << v.y << "\n";
        }
        out << data.food.x << " " << data.food.y << "\n";
        out << static_cast<int>(data.dir) << "\n";
        out << data.score << "\n";
        return true;
    }

    static bool Load(GameData& data,
                     const std::string& filePath = SAVE_FILE_NAME) {
        std::ifstream in(filePath);
        if (!in.is_open()) {
            return false;
        }

        int modeInt;
        if (!(in >> modeInt)) {
            return false;
        }
        data.mode = static_cast<ShowMode>(modeInt);

        int snakeSize;
        if (!(in >> snakeSize)) {
            return false;
        }
        data.snake.clear();
        for (int i = 0; i < snakeSize; i++) {
            int x, y;
            if (!(in >> x >> y)) {
                return false;
            }
            data.snake.emplace_back(x, y);
        }

        if (!(in >> data.food.x >> data.food.y)) {
            return false;
        }

        int dirInt;
        if (!(in >> dirInt)) {
            return false;
        }
        data.dir = static_cast<Direction>(dirInt);

        if (!(in >> data.score)) {
            return false;
        }

        return true;
    }

    static bool HasSaveData(const std::string& filePath = SAVE_FILE_NAME) {
        std::ifstream in(filePath);
        return in && in.peek() != std::ifstream::traits_type::eof();
    }
};
