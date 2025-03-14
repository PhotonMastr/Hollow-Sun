#include "utils.h"
#include <cstdlib>
#include <iostream>

void credits() {
  clearConsole();
  std::string creditsText[] = {
      "==== CREDITS ====",
      "Game Design, Framework, and Programming: Tristan F, Debargha C, Julian ",
      "ZB",
      "Story and Dialogue: Tristan F, Debargha C",
      "Special Thanks: All the supporters!",
      "Testers: My Friends",
      "Thank you for playing!",
      "Plenty more content coming soon, this is the beginning...",
      "==== END ===="};

  int totalLines = sizeof(creditsText) / sizeof(creditsText[0]);

  clearConsole();

  for (int j = 0; j < totalLines; ++j) {
    std::cout << creditsText[j] << std::endl;
  }

  std::cout << "\nPress any key to exit the credits." << std::endl;
  std::cin.get();
}
