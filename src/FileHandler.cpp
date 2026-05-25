#include "FileHandler.h"
#include <fstream>

const std::string FileHandler::filename = "highscore.dat";

void FileHandler::saveHighScore(int score) {
    std::ofstream f(filename);
    if (f.is_open()) f << score;
}

int FileHandler::loadHighScore() {
    std::ifstream f(filename);
    int s = 0;
    if (f.is_open()) f >> s;
    return s;
}