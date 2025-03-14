#ifndef SAVE_H
#define SAVE_H

#include "chapters/chapter1.h"
#include "utils.h"
#include <fstream>
#include <iostream>
#include <limits>
#include <map>
#include <string>
#include <unordered_set>

class SaveManager {
public:
  static bool hasItem(const std::string &item) {
    return inventory.find(item) != inventory.end();
  }
  static void checkHealth() {
    if (health <= 0) {
      std::cout << "You have died...\n";
      std::cout << "Press any key to continue...\n";
      getKeyPress();
      newGame();
    }
  }
  inline static int health = 100;
  inline static int stamina = 100;
  inline static float money = 50.0f;
  inline static float accountMoney = 0.0f;
  inline static int difficulty = 2;
  inline static std::string location = "TutorialBunker";
  inline static std::map<std::string, int> inventory;
  inline static std::unordered_set<std::string> visitedAreas;

  static void setHealth(int newHealth) {
    health = newHealth;
    checkHealth();
  }
  static void damage(int amount) { setHealth(health - amount); }
  static void loadGame(const std::string &filename) {
    std::ifstream file(filename);
    if (file) {
      std::getline(file, location);
      file >> health >> stamina;
      file >> difficulty;
      file >> money >> accountMoney;
      file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

      std::string item;
      int quantity;
      while (file >> item >> quantity) {
        inventory[item] = quantity;
      }
      loadVisitedLocations();
    }
  }

  static void saveGame(const std::string &filename) {
    std::ofstream file(filename);
    if (file) {
      file << location << "\n";
      file << health << " " << stamina << "\n";
      file << difficulty << "\n";
      file << money << " " << accountMoney << "\n";
      for (const auto &item : inventory) {
        file << item.first << " " << item.second << "\n";
      }
    }
  }

  static void updateLocation(const std::string &newLocation) {
    location = newLocation;
  }

  static void addItem(const std::string &item, int quantity = 1) {
    inventory[item] += quantity;
  }

  static void removeItem(const std::string &item, int quantity = 1) {
    auto it = inventory.find(item);
    if (it != inventory.end()) {
      it->second -= quantity;
      if (it->second <= 0) {
        inventory.erase(it);
      }
    }
  }

  static void liquorDrunk() {
    srand(time(nullptr));
    int scenario = rand() % 5;

    std::cout << "\nYou feel the burning sensation of liquor coursing through "
                 "your veins...\n";

    switch (scenario) {
    case 0:
      std::cout << "You completely black out and wake up feeling stupid. Your "
                   "wallet feels significantly lighter... (-$110)\n";
      SaveManager::money = std::max(0.0f, SaveManager::money - 110);
      SaveManager::damage(50);
      SaveManager::saveGame("save.txt");
      break;
    case 1:
      std::cout << "You start a fight and get absolutely wrecked. You lose "
                   "50hp and wake up with a broken rib.\n";
      SaveManager::damage(50);
      SaveManager::saveGame("save.txt");
      break;
    case 2:
      std::cout << "You drunkenly gamble away a ton of money... (-$100)\n";
      SaveManager::money -= 100.0f;
      SaveManager::saveGame("save.txt");
      break;
    case 3:
      std::cout << "You pass out in an alley and wake up feeling awful. Your "
                   "health and stamina take a huge hit.\n";
      SaveManager::damage(70);
      SaveManager::loseStamina(50);
      SaveManager::saveGame("save.txt");
      break;
    case 4:
      std::cout << "You attempt to show off while drunk and end up breaking "
                   "your leg. Your movement is severely impaired.\n";
      SaveManager::damage(80);
      SaveManager::saveGame("save.txt");
      getKeyPress();
      break;
    }

    std::cout << "\nPress any key to continue...\n";
    getKeyPress();
  }
  static void beerDrunk() {
    srand(time(nullptr));
    int scenario = rand() % 5;

    std::cout << "\nYou start feeling a bit drunk from the beer...\n";

    switch (scenario) {
    case 0:
      std::cout << "You black out and wake up in an unfamiliar place!\n";
      break;
    case 1:
      std::cout << "You get into a drunken fight with a random stranger. Try "
                   "not to be so silly. You lost 20hp.\n";
      SaveManager::damage(20);
      SaveManager::saveGame("save.txt");
      break;
    case 2:
      std::cout
          << "You reach into your pocket and find some extra money! (+$100)\n";
      SaveManager::money += 100.0f;
      SaveManager::saveGame("save.txt");
      break;
    case 3:
      std::cout << "You pass out and lose track of time, losing stamina and "
                   "health as a result...\n";
      SaveManager::loseStamina(20);
      SaveManager::damage(30);
      SaveManager::saveGame("save.txt");
      break;
    case 4:
      std::cout << "You trip and seriously injure yourself as a result of your "
                   "drunkenness. You "
                   "lose a lot of health.";
      SaveManager::damage(20);
      SaveManager::saveGame("save.txt");
      getKeyPress();
      break;
    }

    std::cout << "\nPress any key to continue...\n";
    getKeyPress();
  }
  static void displayInventory() {
    clearConsole();
    if (inventory.empty()) {
      std::cout << "\nInventory:\n - Empty\n";
      std::cout << "\nPress any key to return..." << std::endl;
      getKeyPress();
      return;
    }
    int choice = 0;
    bool selecting = true;
    while (selecting) {
      clearConsole();
      std::cout << "\n==== INVENTORY ====\n\n";
      int index = 0;
      for (const auto &item : inventory) {
        if (index == choice) {
          std::cout << "> " << item.first << " x" << item.second << " <\n";
        } else {
          std::cout << "  " << item.first << " x" << item.second << "\n";
        }
        index++;
      }
      std::cout << "\n[W/S to navigate, Enter to use, Esc to exit]\n";
      char key = getKeyPress();
      if (key == 'w' || key == 'W') {
        choice = (choice == 0) ? inventory.size() - 1 : choice - 1;
      } else if (key == 's' || key == 'S') {
        choice = (choice == inventory.size() - 1) ? 0 : choice + 1;
      } else if (key == 10 || key == 13 || key == '\r' || key == '\n') {
        auto it = std::next(inventory.begin(), choice);
        useItem(it->first);
        if (inventory.empty()) {
          return;
        }
      } else if (key == 27) {
        selecting = false;
      }
    }
  }
  static void gainStamina(int amount) { setStamina(stamina + amount); }
  static void loseStamina(int amount) { setStamina(stamina - amount); }
  static void checkStamina() {
    if (stamina <= 0) {
      std::cout << "You're too exhausted to continue...\n";
      std::cout << "You collapse from exhaustion and are forced to rest.\n";
      std::cout << "Press any key to continue...\n";
      getKeyPress();

      setStamina(50);

      std::cout << "You wake up hours later, feeling slightly recovered.\n";
    }
  }
  static void setStamina(int newStamina) {
    stamina = newStamina;
    checkStamina();
  }
  static void heal(int amount) { setHealth(health + amount); }
  static void useItem(const std::string &item) {
    if (inventory[item] > 0) {
      if (item == "Health_Potion") {
        std::cout << "You drank a health potion and gained health.";
        heal(80);
        std::cout << "Press any key to continue... ";
        getKeyPress();

      } else if (item == "Heinekin") {
        beerDrunk();
      } else if (item == "Stamina_Potion") {
        std::cout << "You drank a stamina potion and gained stamina.";
        std::cout << "Press any key to continue... ";
        getKeyPress();
        gainStamina(80);
      } else if (item == "JDWhisky") {
        liquorDrunk();
      } else if (item == "PBRBeer") {
        beerDrunk();
      } else if (item == "Corona") {
        beerDrunk();
      } else if (item == "Budweiser") {
        beerDrunk();
      } else if (item == "CoorsLight") {
        beerDrunk();
      } else if (item == "Smirnoff") {
        liquorDrunk();
      } else if (item == "1800Tequila") {
        liquorDrunk();
      } else if (item == "Bacardi") {
        liquorDrunk();
      } else if (item == "Malibu") {
        liquorDrunk();
      } else if (item == "Locked_Case_Key") {
        std::cout << "You opened the locked case with the key.\n";
        std::cout << "Inside, you find $750, A bottle of Smirnoff, a note, and "
                     "a Health_Potion\n";
        SaveManager::money += 750;
        SaveManager::addItem("Smirnoff");
        SaveManager::addItem("Strange_Note");
        SaveManager::addItem("Health_Potion");
        SaveManager::saveGame("save.txt");
        std::cout << "Press any key to continue... ";
        getKeyPress();
      } else if (item == "Locked_Case") {
        if (SaveManager::hasItem("Locked_Case_Key")) {
          std::cout << "You have the key for this case, try using the key.\n";
          std::cout << "Press any key to continue... ";
          getKeyPress();
        } else {
          std::cout
              << "You dont have a key for this, how do you expect to use it?\n";
          std::cout << "Press any key to continue... ";
          getKeyPress();
        }
      } else if (item == "Strange_Note") {
        std::cout << "\"If you're reading this... I hope you made it farther "
                     "than we did.\"\n";
        std::cout << "\"The world didn’t end in fire or disease. It ended in "
                     "hunger. In desperation.\"\n";
        std::cout << "\"At first, we thought we could stay up here, safe above "
                     "the streets, waiting for things to get better.\"\n";
        std::cout << "\"But people don’t wait when they’re starving.\"\n";
        std::cout
            << "\"The city turned on itself. The ones who had food became "
               "targets. The ones who had nothing became hunters.\"\n";
        std::cout << "\"We barricaded the doors, rationed what we had… but it "
                     "wasn’t enough. Someone always finds a way in.\"\n";
        std::cout << "\"Your last hope is this Secret_Case.\"\n";
        std::cout << "\"Be careful who you trust. Hunger changes people.\"\n";
        std::cout << "- Officer J. Reynolds\n";
        std::cout << "Press any key to continue... ";
        getKeyPress();
      } else if (item == "Important_Files") {
          std::cout << "You need to bring these to someone, these are useless!\n";
          std::cout << "Press any key to continue...";
          getKeyPress();
          addItem("Important_Files");
          saveGame("save.txt");
      } else if (item == "Encrypted_Data_Chip") {
          std::cout << "You need to decrypt this somehow...\n";
          std::cout << "Press any key to continue... ";
          getKeyPress();
          addItem("Encrypted_Data_Chip");
          saveGame("save.txt");
      }

      inventory[item]--;

      if (inventory[item] <= 0) {
        inventory.erase(item);
      }
    }

    saveGame("save.txt");
    getKeyPress();
  }
  static void clearLocationCheckFile() {
    std::ofstream file("locationcheck.txt", std::ios::trunc);
    if (file.is_open()) {
      file.close();
    }
    std::ifstream fileCheck("locationcheck.txt");

    if (fileCheck.is_open()) {
      fileCheck.close();
    }
  }

  static void loadVisitedLocations() {
    std::ifstream file("locationcheck.txt");
    if (file.is_open()) {
      std::string area;
      while (std::getline(file, area)) {
        if (!area.empty()) {
          visitedAreas.insert(area);
        }
      }
      file.close();
    }
  }

  static void saveVisitedLocation(const std::string &area) {
    if (area.empty())
      return;

    visitedAreas.insert(area);
    std::ofstream file("locationcheck.txt", std::ios::app);
    if (file.is_open()) {
      file << area << std::endl;
      file.close();
    }
  }

  static bool hasVisitedLocation(const std::string &area) {
    return visitedAreas.find(area) != visitedAreas.end();
  }
};

#endif // SAVE_H
