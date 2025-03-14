#include "chapters/chapter1.h"
#include "game.h"
#include "save.h"
#include "utils.h"
#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <limits>
#include <thread>
void mainMenu();

void continueGame() {
  std::ifstream saveFile("save.txt");

  if (!saveFile.is_open()) {
    std::cout << "No save file found. Creating new game...\n";
    std::this_thread::sleep_for(std::chrono::seconds(3));

    std::ofstream newSaveFile("save.txt");
    if (newSaveFile.is_open()) {
      newSaveFile << "TutorialBunker\n";
      newSaveFile << "2\n";
      newSaveFile << "0\n";
      newSaveFile << "0\n";
      newSaveFile.close();
    } else {
      std::cout << "Failed to create new save file.\n";
      return;
    }

    SaveManager::inventory.clear();

    newGame();
    return;
  }

  SaveManager::loadGame("save.txt");

  std::string location = readFirstLine();

  if (location == "TutorialBunker") {
    newGame();
  } else if (location == "EmergeFromBunker") {
    emergeFromBunkerHandling();
  } else if (location == "Chapter1NearbyRuins") {
    investigateNearbyRuins();
  } else if (location == "TheRoad") {
    followTheRoad();
  } else if (location == "FourWayCrossroad") {
    postApocalypse();
  } else if (location == "WastelandCreatureFight") {
    goRight();
  } else if (location == "RuinedCity") {
    headingToRuinedCity();
  } else if (location == "Chapter1End") {
    std::cout << "You've finished the game (for now), you can either reopen "
                 "and start a new one, or close it.\n";
    std::cout << "Press any key to continue... ";
    getKeyPress();
  }
}
void displayDifficultyMenu(int selected) {
  clearConsole();

  std::cout << "\033[1;34mSelect Difficulty:\033[0m\n\n";
  std::string options[] = {"Hard", "Medium", "Easy"};

  for (int i = 0; i < 3; ++i) {
    if (i == selected)
      std::cout << "\033[1;33m> " << options[i] << "\033[0m\n";
    else
      std::cout << "  " << options[i] << "\n";
  }
}

void settings() {
  SaveManager::loadGame("save.txt");
  int choice = SaveManager::difficulty - 1;
  bool selecting = true;

  while (selecting) {
    displayDifficultyMenu(choice);
    char key = getKeyPress();

    if (key == 'w' || key == 'W')
      choice = (choice == 0) ? 2 : choice - 1;
    else if (key == 's' || key == 'S')
      choice = (choice == 2) ? 0 : choice + 1;
    else if (key == '\n' || key == '\r')
      selecting = false;
  }

  SaveManager::difficulty = (choice == 0) ? 1 : (choice == 1) ? 2 : 3;

  SaveManager::saveGame("save.txt");

  std::cout << "\nDifficulty set to: "
            << (SaveManager::difficulty == 1
                    ? "Hard"
                    : (SaveManager::difficulty == 2 ? "Medium" : "Easy"))
            << "\n";

  std::cout << "Press any key to return to the main menu...";
  getKeyPress();

  mainMenu();
}

void displayMenu(int selected) {
  clearConsole();
  SaveManager::loadGame("save.txt");
  std::cout << "\033[1;31mH   O    L    L   O    W      S    U    N\033[0m\n\n";

  std::string options[] = {"Continue Game", "New Game", "Settings", "Bank",
                           "Exit Game"};

  for (int i = 0; i < 5; ++i) {
    if (i == selected)
      std::cout << "\033[1;33m> " << options[i] << "\033[0m\n";
    else
      std::cout << "  " << options[i] << "\n";
  }
}
void mainMenu() {
  int choice = 0;
  bool selecting = true;

  while (selecting) {
    displayMenu(choice);
    char key = getKeyPress();

    if (key == 'w' || key == 'W')
      choice = (choice == 0) ? 4 : choice - 1;
    else if (key == 's' || key == 'S')
      choice = (choice == 4) ? 0 : choice + 1;

    if (key == '\r' || key == '\n') {
      selecting = false;
    }
  }

  switch (choice) {
  case 0:
    continueGame();
    break;
  case 1:
    newGame();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    break;
  case 2:
    settings();
    break;
  case 3:
    bankMenu();
    break;
  case 4:
    exit(0);
    break;
  }
}

int main() {
  mainMenu();
  SaveManager::loadGame("save.txt");
  return 0;
}
