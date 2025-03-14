#ifndef UTILS_H
#define UTILS_H
#include <thread>
#include <chrono>
void clearConsole(); 
void newGame();
char getKeyPress();
void flushInput();
std::string readFirstLine();
int getDifficulty();
#endif
