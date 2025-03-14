#include "utils.h"
#include <cstdlib>
#include <iostream>
#ifdef _WIN32
#include <conio.h>
#else
#include <termios.h>
#include <unistd.h>
#endif
#include <fstream>
#include <string>
int getDifficulty() {
  std::ifstream saveFile("save.txt");
  std::string line;

  if (saveFile.is_open()) {
    std::getline(saveFile, line);
    std::getline(saveFile, line);
    return std::stoi(line);
  }

  return 2;
}
std::string readFirstLine() {
  std::ifstream file("save.txt");
  std::string line;

  if (file.is_open()) {
    std::getline(file, line);
    file.close();
  } else {
    std::cerr << "Error: Could not open the file!" << std::endl;
  }

  return line;
}
void clearConsole() {
  std::cout << std::flush;
#ifdef _WIN32
  system("cls");
#else
  system("clear");
#endif
}
void flushInput() {
#ifndef _WIN32
  tcflush(STDIN_FILENO, TCIFLUSH);
#endif
}
char getKeyPress() {
#ifdef _WIN32
  return _getch();
#else
  struct termios oldt, newt;
  char ch;
  tcgetattr(STDIN_FILENO, &oldt);
  newt = oldt;
  newt.c_lflag &= ~(ICANON | ECHO);
  tcsetattr(STDIN_FILENO, TCSANOW, &newt);
  ch = getchar();
  tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
  return ch;
#endif
}
