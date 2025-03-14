#include "game.h"
#include "chapters/chapter1.h"
#include "save.h"
#include "utils.h"
#include <iomanip>
#include <iostream>
bool codeFound = false;
int searchMenu();
void mainMenu();
void displayBankInfo() {
  SaveManager::loadGame("save.txt");
  std::cout << std::fixed << std::setprecision(2);

  std::cout << "Bank Account Balance: $" << SaveManager::accountMoney
            << std::endl;
  std::cout << "Wallet Balance: $" << SaveManager::money << std::endl;
}

void depositMoney() {
  SaveManager::loadGame("save.txt");
  float depositAmount;
  std::cout << "Enter amount to deposit: $";
  std::cin >> depositAmount;

  if (depositAmount <= 0) {
    std::cout << "Invalid amount. Please enter a positive value." << std::endl;
    return;
  }

  if (depositAmount <= SaveManager::money) {
    SaveManager::money -= depositAmount;
    SaveManager::accountMoney += depositAmount;
    std::cout << "Successfully deposited $" << depositAmount
              << " into your bank account!" << std::endl;

    SaveManager::saveGame("save.txt");
  } else {
    std::cout
        << "You don't have enough money in your wallet to deposit this amount."
        << std::endl;
  }
}

void withdrawMoney() {
  SaveManager::loadGame("save.txt");
  float withdrawAmount;
  std::cout << "Enter amount to withdraw: $";
  std::cin >> withdrawAmount;

  if (withdrawAmount <= 0) {
    std::cout << "Invalid amount. Please enter a positive value." << std::endl;
    return;
  }

  if (withdrawAmount <= SaveManager::accountMoney) {
    SaveManager::money += withdrawAmount;
    SaveManager::accountMoney -= withdrawAmount;
    std::cout << "Successfully withdrew $" << withdrawAmount
              << " from your bank account!" << std::endl;

    SaveManager::saveGame("save.txt");
  } else {
    std::cout << "You don't have enough money in your bank account to withdraw "
                 "this amount."
              << std::endl;
  }
}

void bankMenu() {
  SaveManager::loadGame("save.txt");
  bool exitBank = false;

  while (!exitBank) {
    clearConsole();
    std::cout << "\n==== Welcome to Your Bank ====" << std::endl;
    displayBankInfo();

    std::cout << "\nSelect an option:" << std::endl;
    std::cout << "1. Deposit Money" << std::endl;
    std::cout << "2. Withdraw Money" << std::endl;
    std::cout << "3. Exit to Main Menu" << std::endl;

    int choice;
    std::cin >> choice;

    // Check for invalid input
    if (std::cin.fail()) {
      std::cin.clear(); // Clear the error state
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      std::cout << "Invalid choice. Please press enter to try again.\n";
      std::cin.get();
      continue;
    }

    switch (choice) {
    case 1:
      depositMoney();
      break;
    case 2:
      withdrawMoney();
      break;
    case 3:
      exitBank = true;
      std::cout << "Exiting to Main Menu..." << std::endl;
      mainMenu();
      break;
    default:
      std::cout << "Invalid choice. Please press enter to try again."
                << std::endl;
      std::cin.get();
    }

    SaveManager::saveGame("save.txt");
  }
}
void endTutorial() {
  int choice = 0;
  bool selecting = true;
  std::string options[] = {"Leave the bunker", "Stay in the bunker"};

  while (selecting) {
    clearConsole();

    std::cout << "The breaker has been flipped. The door is now safe to open!"
              << std::endl;

    for (int i = 0; i < 2; ++i) {
      if (i == choice)
        std::cout << "\033[1;33m> " << options[i] << "\033[0m\n";
      else
        std::cout << "  " << options[i] << "\n";
    }

    char key = getKeyPress();
    if (key == 'w' || key == 'W')
      choice = (choice == 0) ? 1 : choice - 1;
    else if (key == 's' || key == 'S')
      choice = (choice == 1) ? 0 : choice + 1;
    else if (key == 13 || key == '\n')
      selecting = false;
  }

  clearConsole();
  if (choice == 0) {
    std::cout << "You leave the bunker and step out into the open world...\n";
    SaveManager::updateLocation("EmergeFromBunker");
    emergeFromBunkerHandling();
  } else {
    std::cout << "You stay in the bunker, unsure of what to do next...\n";
    std::this_thread::sleep_for(std::chrono::seconds(3));
    newGame();
  }
}

void handleHardModeBreaker(bool &hasKey, bool &hasCode) {
  int choice = 0;
  bool selecting = true;
  std::string options[] = {"Flip the breaker", "Enter code"};

  while (selecting) {
    clearConsole();
    std::cout << "You check the toolbox and find the breaker key!\n";

    if (!hasCode) {
      std::cout << "You still need the code to open the breaker panel. Try "
                   "searching again.\n";
    }

    if (!hasCode) {
      options[0] = "Can't flip breaker yet (No code)";
    } else {
      options[0] = "Flip the breaker";
    }

    for (int i = 0; i < 2; ++i) {
      if (i == choice)
        std::cout << "\033[1;33m> " << options[i] << "\033[0m\n";
      else
        std::cout << "  " << options[i] << "\n";
    }

    char key = getKeyPress();
    flushInput();

    if (key == 'w' || key == 'W')
      choice = (choice == 0) ? 1 : choice - 1;
    else if (key == 's' || key == 'S')
      choice = (choice == 1) ? 0 : choice + 1;
    else if (key == 13 || key == '\n') // Enter key
      selecting = false;
  }

  if (choice == 0) {
    if (!hasCode) {
      std::cout << "You can't flip the breaker yet, you need the code!\n";
      std::this_thread::sleep_for(std::chrono::seconds(2));
      handleHardModeBreaker(hasKey, hasCode);
    } else {
      std::cout << "You flip the breaker and unlock the door!\n";
      clearConsole();
      endTutorial();
    }
  } else if (choice == 1) {
    if (!hasCode) {
      std::string enteredCode;
      std::cout << "Enter the code: ";
      std::cin >> enteredCode;

      if (enteredCode == "5442") {
        std::cout << "Correct code! You unlock the breaker panel.\n";
        hasCode = true;
        codeFound = true;
        std::this_thread::sleep_for(std::chrono::seconds(2));
        handleHardModeBreaker(hasKey, hasCode);
      } else {
        std::cout << "Incorrect code. Try again.\n";
        std::this_thread::sleep_for(std::chrono::seconds(2));
        handleHardModeBreaker(hasKey, hasCode);
      }
    } else {
      std::cout << "You have already entered the code and unlocked the breaker "
                   "panel.\n";
    }
  }
}

void handleNormalEasyModeBreaker(bool &hasKey) {
  int choice = 0;
  bool selecting = true;
  std::string options[] = {"Flip the breaker"};

  while (selecting) {
    clearConsole();
    std::cout << "You check the toolbox and find the breaker key!\n";

    for (int i = 0; i < 1; ++i) {
      if (i == choice)
        std::cout << "\033[1;33m> " << options[i] << "\033[0m\n";
      else
        std::cout << "  " << options[i] << "\n";
    }

    char key = getKeyPress();
    flushInput();

    if (key == 'w' || key == 'W')
      choice = (choice == 0) ? 0 : choice - 1;
    else if (key == 's' || key == 'S')
      choice = (choice == 0) ? 0 : choice + 1;
    else if (key == 13 || key == '\n')
      selecting = false;
  }

  if (choice == 0) {
    std::cout << "You flip the breaker and unlock the door!\n";
    clearConsole();
    endTutorial();
  }
}

int searchMenu() {
  int choice = 0;
  bool selecting = true;
  std::string options[] = {"Look under the bed", "Check the toolbox",
                           "Search the locker"};

  while (selecting) {
    clearConsole();
    std::cout << "You wake up in your fallout bunker. The door is electrified. "
                 "To leave, you must flip the breaker, but you need the "
                 "breaker key first.\n\n";

    for (int i = 0; i < 3; ++i) {
      if (i == choice)
        std::cout << "\033[1;33m> " << options[i] << "\033[0m\n";
      else
        std::cout << "  " << options[i] << "\n";
    }

    char key = getKeyPress();

    if (key == 'w' || key == 'W')
      choice = (choice == 0) ? 2 : choice - 1;
    else if (key == 's' || key == 'S')
      choice = (choice == 2) ? 0 : choice + 1;
    else if (key == 13 || key == '\n')
      selecting = false;
  }
  return choice;
}

bool handleSearch(int choice, bool &codeFound) {
  if (choice == 0) {
    std::cout << "You look under the bed and find nothing but dust and an old "
                 "sock.\n";
    return false;
  } else if (choice == 1) {
    if (SaveManager::difficulty == 1 && !codeFound) {
      std::cout << "You find a hidden compartment with a code: 5442.\n";
      std::this_thread::sleep_for(std::chrono::seconds(7));
    } else {
      std::cout << "You find the breaker key!\n";
    }
    return true;
  } else {
    std::cout << "You search the locker and find an old gas mask, but no "
                 "breaker key.\n";
    return false;
  }
}

void newGame() {
  SaveManager::clearLocationCheckFile();
  SaveManager::updateLocation("TutorialBunker");
  SaveManager::money = 50;
  SaveManager::accountMoney = 0;
  SaveManager::health = 100;
  SaveManager::stamina = 100;
  SaveManager::inventory.clear();
  SaveManager::saveGame("save.txt");
  bool keyFound = false;

  while (!keyFound) {
    int choice = searchMenu();
    keyFound = handleSearch(choice, codeFound);

    if (!keyFound) {
      std::cout << "You haven't found the key yet. Try again.\n";
      std::cout << "Press Enter to continue...\n";
      std::cin.get();
    }
  }

  if (SaveManager::difficulty == 1) {
    handleHardModeBreaker(keyFound, codeFound);
  } else {
    handleNormalEasyModeBreaker(keyFound);
  }
}
