#ifdef _WIN32
#include <conio.h> 
#include <windows.h>
#else
#include <termios.h> 
#include <unistd.h>  
#endif
#include "../credits.h"
#include "../save.h"
#include "../utils.h"
#include "chapter1.h"
#include <iostream>
#include <vector>

static bool hasLooted = false;
void emergeFromBunkerHandling();
void vultureSite() {
  int choice = 0; 
  char input;
  SaveManager::loadVisitedLocations();
  
  if (SaveManager::hasVisitedLocation("vultureSite")) {
    std::cout
        << "You find a long-dead man, but his body has already been looted.\n";
    std::cout << "Press enter to leave.\n";
    input = getchar(); 
    if (input == '\n') {
      emergeFromBunkerHandling();
      SaveManager::saveVisitedLocation(
          "vultureSite"); 
    }
    return; 
  }

#ifdef _WIN32
  while (true) {
    system("cls");

    std::cout << "You find a long-dead man. What do you do?\n";
    std::cout << (choice == 0 ? "> Loot his body\n" : "  Loot his body\n");
    std::cout << (choice == 1 ? "> Leave him be\n" : "  Leave him be\n");

    input = _getch(); 

    if (input == 'w' || input == 'W') {
      choice = (choice == 0) ? 1 : 0; 
    } else if (input == 's' || input == 'S') {
      choice = (choice == 1) ? 0 : 1; 
    } else if (input == '\r') {       
      if (choice == 0) {
        std::cout << "You decide to loot the body... you get $100.\n";
        SaveManager::money += 100;
        SaveManager::saveGame("save.txt"); 
        hasLooted = true;                 
        SaveManager::saveVisitedLocation(
            "vultureSite"); 
        std::cout << "You now have " << SaveManager::money << " dollars.\n";
        std::cout << "Press enter to continue... ";
        getKeyPress();
        emergeFromBunkerHandling();
        SaveManager::saveGame("save.txt"); 
        break; 
      } else if (choice == 1) {
        emergeFromBunkerHandling(); 
        SaveManager::saveVisitedLocation(
            "vultureSite");               
        SaveManager::saveGame("save.txt"); 
        break;                             
      }
    }
    Sleep(100); 
  }
#else
  struct termios old_tio, new_tio;

  tcgetattr(STDIN_FILENO, &old_tio);         
  new_tio = old_tio;                          
  new_tio.c_lflag &= ~(ICANON | ECHO);        
  new_tio.c_cc[VMIN] = 1;                     
  new_tio.c_cc[VTIME] = 0;                    
  tcsetattr(STDIN_FILENO, TCSANOW, &new_tio); 

  while (true) {
    system("clear"); 

   
    std::cout << "You find a long-dead man. What do you do?\n";
    std::cout << (choice == 0 ? "> Loot his body\n" : "  Loot his body\n");
    std::cout << (choice == 1 ? "> Leave him be\n" : "  Leave him be\n");

   
    input = getchar();
    if (input == 'w' || input == 'W') {
      choice = (choice == 0) ? 1 : 0; 
    } else if (input == 's' || input == 'S') {
      choice = (choice == 1) ? 0 : 1; 
    } else if (input == '\n') {
      if (choice == 0) {
        std::cout << "You loot the body and find $100.\n";
        SaveManager::money += 100; 
        std::cout << "Your balance is now " << SaveManager::money
                  << " dollars.\n";
        hasLooted = true; 
        SaveManager::saveVisitedLocation(
            "vultureSite"); 
        SaveManager::saveGame("save.txt"); 
        break;                             
      } else if (choice == 1) {
        emergeFromBunkerHandling(); 
                                  
        SaveManager::saveVisitedLocation(
            "vultureSite");                
        SaveManager::saveGame("save.txt"); 
        break; 
      }
    }

    usleep(100000); 
  }

  tcsetattr(STDIN_FILENO, TCSANOW, &old_tio); 
#endif
}
void attemptToLeave() {
  clearConsole();
  std::cout << "As you try to leave the ruins, an unnatural force stops you in "
               "your tracks.\n";
  std::cout << "A deep hum resonates through the air, and a strange static "
               "fills your head.\n";
  std::cout << "Your vision distorts, and no matter how far you walk, you "
               "always end up back where you started.\n\n";

  std::cout
      << "Something is keeping you here. You must find a way to escape...\n";

  
  if (SaveManager::inventory.find("Mysterious Key") !=
      SaveManager::inventory.end()) {
    std::cout << "\nYou feel a strange pull in your pocket. The Mysterious "
                 "Key... could it be the key to your escape?\n";
    std::cout << "You use the key and the force around you begins to fade.\n";
    std::cout << "With the key in hand, you find a hidden path that leads out "
                 "of the ruins.\n";
    std::cout << "You successfully escape from this area.\n";
    std::cout << "Press enter to continue... ";
    getKeyPress();
    
    emergeFromBunkerHandling();
  }

  std::cout << "Press any key to continue...";
  getKeyPress();
  investigateNearbyRuins();
}
void exploreSketchyBuilding() {
  SaveManager::loadVisitedLocations();

  if (SaveManager::hasVisitedLocation("Sketchy Building")) {
    std::cout << "You have already explored the Sketchy Building. Nothing new "
                 "to see here.\n";
    std::cout << "Press any key to continue...\n";
    getKeyPress();           
    investigateNearbyRuins();
    return;                  
  }

  const int numOptions = 2;
  const char *options[numOptions] = {"Look around", "Leave"};
  int selected = 0;

  while (true) {
    clearConsole();
    std::cout << "Inside the Sketchy Building:\n";

    for (int i = 0; i < numOptions; i++) {
      if (i == selected)
        std::cout << "> " << options[i] << " <"
                  << std::endl; 
      else
        std::cout << "  " << options[i] << std::endl;
    }

    char input = getKeyPress();

    if (input == 'w' || input == 'W') {
      selected = (selected - 1 + numOptions) % numOptions; 
    } else if (input == 's' || input == 'S') {
      selected = (selected + 1) % numOptions;    
    } else if (input == '\n' || input == '\r') { 
      break;
    }
  }

  clearConsole();
  if (selected == 0) {
    std::cout
        << "You search the building and find ten dollars on the ground!\n";
    std::this_thread::sleep_for(std::chrono::seconds(2));
    std::cout << "The walls suddenly creak... dust falls from the ceiling.\n";
    std::this_thread::sleep_for(std::chrono::seconds(2));
    std::cout << "The building is collapsing!\n";
    std::cout << "Press any key to continue...";

    getKeyPress(); 
    SaveManager::saveVisitedLocation("Sketchy Building"); 
    investigateNearbyRuins();
  } else {
    std::cout << "You leave the building before anything bad happens.\n";
    std::cout << "Press any key to continue...";
    getKeyPress(); 
    SaveManager::saveVisitedLocation("Sketchy Building"); 
    investigateNearbyRuins();
  }
}

void walkAroundRuins() {
  clearConsole();
  std::cout << "You wander through the ruins, stepping over rubble and "
               "avoiding collapsed structures.\n";
  std::this_thread::sleep_for(std::chrono::seconds(2));
  std::cout << "Suddenly, a group of raiders surrounds you!\n";
  std::this_thread::sleep_for(std::chrono::seconds(2));
  std::cout
      << "They demand your money, and you have no choice but to comply.\n";

  if (SaveManager::money >= 50) {
    SaveManager::money = std::max(0.0f, SaveManager::money - 50);
    std::cout << "The raiders take 50 bucks from you. You now have $"
              << SaveManager::money << ".\n";
  } else {
    std::cout << "You don't have enough money to satisfy them, so they take "
                 "everything you have!\n";
    SaveManager::money = 0;
  }

  std::this_thread::sleep_for(std::chrono::seconds(2));
  std::cout << "They leave you bruised but alive. You should be more careful "
               "next time.\n";

  std::cout << "Press any key to continue...";
  SaveManager::saveGame("save.txt");
  getKeyPress();
  investigateNearbyRuins();
}
void abandonedLaboratory() {
  
  SaveManager::loadVisitedLocations();

  
  if (SaveManager::hasVisitedLocation("Abandoned Laboratory")) {
    std::cout << "You have already visited this laboratory. Nothing new to "
                 "explore here.\n";
    std::cout << "Press any key to return to the ruins...\n";
    getKeyPress();            
    investigateNearbyRuins();
    return;                  
  }

  std::cout << std::endl;
  const int numOptions = 3;
  const char *options[numOptions] = {"Search the desks",
                                     "Inspect the locked safe",
                                     "Look for medical supplies"};
  int selected = 0;
  bool optionSelected[numOptions] = {false, false,
                                     false}; 

  while (true) {
    clearConsole();
    std::cout << "Inside the Abandoned Laboratory:\n";

 
    for (int i = 0; i < numOptions; i++) {
      if (optionSelected[i]) {
        std::cout << "  " << options[i] << " (Already done)"
                  << std::endl; 
      } else {
        if (i == selected) {
          std::cout << "> " << options[i] << " <"
                    << std::endl; 
        } else {
          std::cout << "  " << options[i]
                    << std::endl; 
        }
      }
    }

    char input = getKeyPress();

   
    if (input == 'w' || input == 'W') {
      do {
        selected = (selected - 1 + numOptions) % numOptions;
      } while (optionSelected[selected]);
    } else if (input == 's' || input == 'S') {
      do {
        selected = (selected + 1) % numOptions;
      } while (optionSelected[selected]); 
    } else if (input == '\n' || input == '\r') {
      clearConsole();

   
      if (selected == 0) {
        std::cout << "You rummage through the desks and find some old research "
                     "notes. Some of them mention strange experiments...\n";
      } else if (selected == 1) {
        std::cout << "You force open the locked safe and find $500 inside and "
                     "a strange key!\n";
        SaveManager::money += 500;
        std::cout << "Your money is now: $" << SaveManager::money << "\n";

      
        SaveManager::addItem("Mysterious Key", 1);
        std::cout << "You add the 'Mysterious Key' to your inventory.\n";
        SaveManager::saveGame("save.txt");
      } else if (selected == 2) {
        std::cout << "You search the lab for medical supplies. You find an old "
                     "Health_Potion.\n";
        SaveManager::addItem("Health_Potion");
        SaveManager::saveGame("save.txt");
      }

      optionSelected[selected] = true; 

      
      bool allSelected = true;
      for (int i = 0; i < numOptions; i++) {
        if (!optionSelected[i]) {
          allSelected = false;
          break;
        }
      }

      if (allSelected) {
        break; 
      }

      std::cout << "Press any key to continue...\n";
      getKeyPress(); 
    }
  }


  SaveManager::saveVisitedLocation("Abandoned Laboratory");

  std::cout << "Press any key to return to the ruins...\n";
  getKeyPress(); 

  investigateNearbyRuins();
}

void investigateNearbyRuins() {
  SaveManager::updateLocation("Chapter1NearbyRuins");
  SaveManager::saveGame("save.txt");
  const int numOptions = 4;
  const char *options[numOptions] = {"Explore Sketchy Building", "Go Back",
                                     "Walk Around", "Explore Laboratory"};
  int selected = 0;

  while (true) {
    clearConsole();

    std::cout << "Investigate Nearby Ruins:\n";

    for (int i = 0; i < numOptions; i++) {
      if (i == selected)
        std::cout << "> " << options[i] << " <"
                  << std::endl; 
      else
        std::cout << "  " << options[i] << std::endl;
    }

    char input = getKeyPress();

    if (input == 'w' || input == 'W') {
      selected = (selected - 1 + numOptions) % numOptions; 
    } else if (input == 's' || input == 'S') {
      selected = (selected + 1) % numOptions; 
    } else if (input == '\n' ||
               input ==
                   '\r') { 
      break;               
    }
  }

  
  clearConsole();
  if (selected == 0) {
    exploreSketchyBuilding();
  } else if (selected == 1) {
    attemptToLeave();
  } else if (selected == 2) {
    walkAroundRuins();
  } else if (selected == 3) {
    abandonedLaboratory();
  }
}
void vulturesCirclingScenario() {
  int choice = 0;
  bool selecting = true;

  while (selecting) {
    clearConsole();
    std::cout << "You shield your eyes from the harsh light and look up.\n";
    std::cout << "High above, a group of vultures circle in slow, deliberate "
                 "motions.\n";
    std::cout << "Something-or someone-has caught their interest.\n\n";
    std::cout << "What do you do?\n\n";

    std::string options[] = {"Approach the site", "Stay away"};

    for (int i = 0; i < 2; ++i) {
      if (i == choice)
        std::cout << "> " << options[i] << " <\n"; 
      else
        std::cout << options[i] << "\n";
    }

    char key = getKeyPress();

    if (key == 'w' || key == 'W') {
      choice = (choice == 0) ? 1 : 0; 
    } else if (key == 's' || key == 'S') {
      choice = (choice == 1) ? 0 : 1; 
    } else if (key == 13 || key == '\n') {
      selecting = false; 
    }
  }

  clearConsole();

  if (choice == 0) {
    std::cout << "You move cautiously toward the site, the smell of decay "
                 "growing stronger with each step.\n";
    std::cout << "As you approach, you make out a shape on the ground\n";
    std::cout << "Press enter to continue... ";
    getKeyPress();
    vultureSite();
  } else {
    std::cout << "You decide it's best to stay away. Whatever is attracting "
                 "the vultures, it's none of your concern.\n";
    std::cout
        << "You turn back, keeping your distance from whatever lies ahead.\n";
    std::cout << "Press enter to head back.\n";
    getKeyPress();
    emergeFromBunkerHandling();
  }
}
void surveyLandscape() {
  clearConsole();
  std::cout << "You climb a small hill of debris to get a better look at your "
               "surroundings.\n";
  std::cout << "The landscape stretches before you, barren and lifeless.\n\n";

  std::cout << "In the distance, skeletal remains of buildings stand like "
               "silent sentinels, their glass long shattered.\n";
  std::cout << "Some have collapsed entirely, leaving behind jagged steel and "
               "crumbling concrete.\n";
  std::cout << "Beyond them, you spot a faint plume of smoke rising into the "
               "pale, clouded sky.\n";
  std::cout << "It drifts lazily, a sign that something-or someone-might still "
               "be out there.\n\n";

  std::cout << "The cracked roads below are lined with rusted-out husks of "
               "vehicles, some overturned, others half-buried in dust.\n";
  std::cout
      << "You can make out the remnants of a distant city, barely visible "
         "through the haze, its ruined skyline a ghost of its former self.\n";
  std::cout << "The wind howls through the wreckage, carrying the distant "
               "creaks of metal shifting under its weight.\n\n";

  std::cout << "You take a deep breath. The world is dead... but not empty.\n";
  std::cout << "Press enter when you are done looking.\n";
  getKeyPress();
  emergeFromBunkerHandling();
}
void emergeFromBunkerScenario() {
  clearConsole();
  std::cout << "You step out of the bunker into a desolate, ruined world.\n";
  std::cout << "The ground is cracked, buildings are in ruins, and the air "
               "smells of ash and decay.\n";
  std::cout << "You've survived in the bunker for years, and now you must face "
               "the outside world.\n\n";
}

void showStartOfChapter1Menu(int &choice) {
  clearConsole();
  std::cout << "==== OPEN WORLD EXPLORATION ====\n\n";
  std::cout << "You have just left the safety of the bunker and are now faced "
               "with the unknown.\n";
  std::cout << "Where do you want to go?\n\n";

  bool hasExploredLab = SaveManager::hasVisitedLocation("Abandoned Laboratory");
  bool hasExploredTempCamp = SaveManager::hasVisitedLocation("temporary camp");
 
  std::string options[] = {"Survey the Landscape",
                           "Look for Signs of Life",
                           "Investigate the Nearby Ruins",
                           "Head Towards the Distant Smoke Plume",
                           "Set Up a Temporary Camp",
                           "View Inventory",
                           "Follow the Road"};

  for (int i = 0; i < 7; ++i) { 
    if (i == choice)
      std::cout << "> " << options[i] << " <\n"; 
    else
      std::cout << options[i] << "\n";
  }

  if (!hasExploredTempCamp) {
    std::cout << "\n(Note: You must setup a temporary camp before you can "
                 "follow the road.)\n";
  }
}

void displaySearchCampMenu(int selected) {
  clearConsole();

  std::cout
      << "\033[1;34mYou are at the camp.\033[0m\n\n"; 
  std::string options[] = {"Loot the empty camp", "Put out the fire", "Leave"};

  for (int i = 0; i < 3; ++i) {
    if (i == selected)
      std::cout << "\033[1;33m> " << options[i]
                << "\033[0m\n";
    else
      std::cout << "  " << options[i] << "\n";
  }
}

void searchCamp() {
  bool looted = SaveManager::hasVisitedLocation("empty camp");
  if (looted) {
    std::cout << "You've already looted this camp.\n";
    std::cout << "Theres nothing left for you here.\n";
    std::cout << "Press any key to continue...\n";
    getKeyPress();
    emergeFromBunkerHandling();
    return;
  }

  int choice = 0; 
  bool selecting = true;

  while (selecting) {
    displaySearchCampMenu(choice);
    char key = getKeyPress();

  
    if (key == 'w' || key == 'W') {
      choice = (choice == 0) ? 2 : choice - 1; 
    } else if (key == 's' || key == 'S') {
      choice = (choice == 2) ? 0 : choice + 1; 
    } else if (key == '\n' || key == '\r') {   
      selecting = false; 
    }
  }

  switch (choice) {
  case 0:
    std::cout << "\nYou start looting the empty camp. You find a bottle of "
                 "Jack Daniels Whisky, and $400, sweet.\n";
    SaveManager::saveVisitedLocation("empty camp");
    SaveManager::addItem("JDWhisky"); 
    SaveManager::money += 400;       
    SaveManager::saveGame("savetxt"); 
    std::cout << "Press enter to continue... ";
    getKeyPress();
    emergeFromBunkerHandling();
    break;
  case 1:
    std::cout << "\nYou approach the fire and decide to put it out. The smoke "
                 "fades as the fire dies down.\n";
    std::cout << "Press enter to continue... ";
    SaveManager::saveVisitedLocation("empty camp");
    SaveManager::saveGame("save.txt");
    getKeyPress();
    emergeFromBunkerHandling();
    break;
  case 2:
    std::cout << "\nYou decide to leave the camp. It's best to keep moving.\n";
    SaveManager::saveVisitedLocation("empty camp");
    SaveManager::saveGame("save.txt");
    std::cout << "Press enter to continue... ";
    getKeyPress();
    emergeFromBunkerHandling();
    break;
  }
}

void headTowardsSmokePlume() {
  bool looted = SaveManager::hasVisitedLocation("empty camp");
  if (looted) {
    std::cout << "You've already visited. Nothing more to see.\n";
    std::cout << "Press any key to continue... ";
    getKeyPress();
    emergeFromBunkerHandling();
    return; 
  }
  clearConsole();
  std::cout << "You begin walking towards the distant plume of smoke, the air "
               "growing thicker with each step.\n";
  std::cout << "The source of the smoke is still far off, but you can make out "
               "the faint glow of flames.\n";
  std::cout << "As you approach, the ground becomes rough, and the temperature "
               "rises.\n\n";

  if (SaveManager::difficulty == 3) {
    std::cout << "You reach the source of the smoke and discover a small band "
                 "of survivors gathered around a fire.\n";
    std::cout << "They seem wary but offer to trade supplies with you.\n";
    std::cout << "They give you a 6-pack of Heinekin... thanks i guess. Also "
                 "$500, and a health potion for later. You can buy better beer "
                 "with that $500.\n";
    SaveManager::addItem("Heinekin", 6);
    SaveManager::addItem("Health_Potion");
    SaveManager::money += 500;
    SaveManager::saveVisitedLocation("empty camp");
    SaveManager::saveGame("save.txt");
    std::cout << "Press enter to continue... ";
    getKeyPress();
    emergeFromBunkerHandling();
  } else if (SaveManager::difficulty == 2) {
    std::cout << "There are signs of recent activity, but its unclear who or "
                 "what started this campfire.\n";
    std::cout << "You search the camp for supplies...\n";
    std::cout << "Press enter to continue... ";
    getKeyPress();
    searchCamp();
  } else if (SaveManager::difficulty == 1) {
    std::cout << "The source of the smoke is a trap! A series of explosive "
                 "devices go off as you approach.\n";
    std::cout << "You died. Sometimes investigating isnt the greatest idea, "
                 "you figure.\n";
    std::cout << "Press enter to start new game... ";
    getKeyPress();
    newGame();
  }
}
void goRight() {
  if (SaveManager::difficulty == 1) {
    SaveManager::updateLocation("WastelandCreatureFight");
    SaveManager::saveGame("save.txt");
    const char *options[] = {"Fight", "Run", "Save and Exit"};
    int selection = 0;
    const int numOptions = 3;

    while (true) {
      clearConsole();
      std::cout << "A Wasteland Creature appears!\n";
      std::cout << "What will you do?\n";

      for (int i = 0; i < numOptions; i++) {
        if (i == selection) {
          std::cout << "> " << options[i] << " <\n";
        } else {
          std::cout << "  " << options[i] << "  \n";
        }
      }

      char input = getKeyPress();
      if (input == 'w' || input == 'W') {
        selection = (selection - 1 + numOptions) % numOptions;
      } else if (input == 's' || input == 'S') {
        selection = (selection + 1) % numOptions;
      } else if (input == '\n' || input == ' ') { 
        break;
      }
    }

    clearConsole();
    if (selection == 0) {
      std::cout << "You engage the Wasteland Creature in battle!\n";
      SaveManager::damage(50);
      SaveManager::saveGame("save.txt");
      std::cout << "You take 50 damage!\n";
      std::cout << "Press any key to continue... ";
      getKeyPress();
      postApocalypse();
    } else if (selection == 1) {
      std::cout << "You escape, but it takes effort!\n";
      SaveManager::loseStamina(50);
      SaveManager::saveGame("save.txt");
      std::cout << "You lose 50 stamina.\n";
      std::cout << "Press any key to continue... ";
      getKeyPress();
      postApocalypse();
    } else if (selection == 2) {
      std::cout << "Saving game...\n";
      SaveManager::saveGame("save.txt");
      std::cout << "Game saved. Exiting...\n";
      std::cout << "Press any key to continue... ";
      getKeyPress();
      exit(0);
    }
  } else {
    if (SaveManager::hasVisitedLocation("GoRight")) {
      std::cout << "You already went right!\n";
      std::cout << "Press any key to continue... ";
      getKeyPress();
      postApocalypse();
    } else {
      SaveManager::saveVisitedLocation("GoRight");
      std::cout << "You find a Stamina_Potion on the ground.\n";
      SaveManager::addItem("Stamina_Potion");
      SaveManager::saveGame("save.txt");
      std::cout << "Press any key to continue... ";
      getKeyPress();
      postApocalypse();
    }
  }
}
void chapter1Ending() {
  SaveManager::updateLocation("Chapter1End");
  SaveManager::saveGame("save.txt");
  clearConsole();
  std::cout << "\nAs you walk through the checkpoint, a strange silence "
               "settles around you. The way forward is unclear, but you feel "
               "that something important awaits beyond. For now, the path is "
               "yours to choose.\n";
  std::cout << "More will be revealed soon...\n";
  std::cout << "Thank you for playing Hollow Sun! I appreciate you. Apologies "
               "for lack of content more is coming soon.\n";
  getKeyPress();
  credits();
}
void abandonedMilitaryCheckpoint() {
  bool selecting = true;
  int choice = 0;

  std::cout << "\nYou approach the old military checkpoint. The place is "
               "nearly deserted, but a man sits by a makeshift terminal.\n";
  std::cout << "\"You're carrying something interesting,\" he says, eyeing "
               "your bag. \"I can decrypt those Important_Files for you.\"\n";
  std::cout << "\"In exchange, I'll give you $500 and let you through the "
               "checkpoint. Deal?\"\n";

  while (selecting) {
    clearConsole(); 
    std::cout << "\n==== ABANDONED MILITARY CHECKPOINT ====\n";
    std::cout << (choice == 0 ? "> " : "  ")
              << "Trade Important_Files for $500 and passage\n";
    std::cout << (choice == 1 ? "> " : "  ")
              << "Keep the files and walk away\n";
    std::cout << "\n[W/S to navigate, Enter to choose]\n";

    char key = getKeyPress();
    if (key == 'w' || key == 'W') {
      choice = (choice == 0) ? 1 : 0;
    } else if (key == 's' || key == 'S') {
      choice = (choice == 1) ? 0 : 1;
    } else if (key == '\n' || key == '\r') { 
      if (choice == 0) {                     
        if (SaveManager::hasItem("Important_Files")) {
          SaveManager::money += 500.0f;
          SaveManager::removeItem("Important_Files");
          std::cout << "\n\"Smart choice,\" the man says. He hands you $500 "
                       "and waves you through the checkpoint.\n";
          std::cout << "You pass through, leaving the checkpoint behind.\n";
          SaveManager::saveGame("save.txt");
          getKeyPress();
          chapter1Ending();
        } else {
          std::cout << "\n\"Nice try, but you don't even have the files, I was "
                       "wrong.\" he "
                       "scoffs. \"Come back when you do.\"\n";
        }
      } else {
        std::cout << "\nYou shake your head. \"Suit yourself,\" the man "
                     "mutters as you walk away.\n";
      }
      selecting = false;
    }
  }
}
void findSurvivor() {
  clearConsole();
  std::cout << "You listen carefully, trying to pinpoint where the voice came "
               "from...\n";
  std::cout
      << "The echoes make it hard to tell, but you have a few options.\n\n";

  const std::vector<std::string> options = {
      "Head towards the echoing ruins",
      "Climb onto a rooftop for a better view",
      "Stay where you are and call out again"};

  int choice = 0;
  bool selecting = true;

  while (selecting) {
    clearConsole();
    std::cout << "\n==== TRACKING THE SURVIVOR ====\n\n";

    for (int i = 0; i < options.size(); i++) {
      if (i == choice) {
        std::cout << "> " << options[i] << " <\n";
      } else {
        std::cout << "  " << options[i] << "\n";
      }
    }

    std::cout << "\n[W/S to navigate, Enter to select]\n";
    char key = getKeyPress();

    if (key == 'w' || key == 'W') {
      choice = (choice == 0) ? options.size() - 1 : choice - 1;
    } else if (key == 's' || key == 'S') {
      choice = (choice == options.size() - 1) ? 0 : choice + 1;
    } else if (key == '\n' || key == '\r') { 
      selecting = false;
    }
  }

  clearConsole();
  if (choice == 0) {
    std::cout << "You move through the ruins, carefully stepping over rubble "
                 "and broken glass...\n";
    std::cout << "The voice calls again, closer now. You're getting there.\n";
    std::cout << "Finally, you turn a corner and find the survivor.";
    std::cout << "Press any key to continue... ";
    getKeyPress();
    clearConsole();
    std::cout << "The survivor takes a deep breath, eyes scanning the ruins "
                 "around you.\n";
    std::cout << "\"You wanna know what happened? How things fell apart?\"\n\n";

    std::cout
        << "They sit down on a broken slab of concrete and shake their head.\n";
    std::cout
        << "\"It started slow... real slow. People thought they had time.\"\n";
    std::cout << "\"Governments kept saying things were under control, but we "
                 "all knew better.\"\n";
    std::cout << "\"One day, supply lines broke down. No food. No medicine. No "
                 "power.\"\n";
    std::cout << "They let out a bitter laugh. \"That's when the real chaos "
                 "started.\"\n\n";

    std::cout << "You stay silent, letting them continue.\n";
    std::cout << "\"At first, folks tried to help each other. Neighborhoods "
                 "turned into little\n";
    std::cout << "communities, sharing what they had. But when things got "
                 "bad—when the hunger\n";
    std::cout << "and sickness spread—kindness went out the window.\"\n";
    std::cout << "Their voice hardens. \"People did things... horrible things. "
                 "Things they never\n";
    std::cout << "thought they'd be capable of.\"\n\n";

    std::cout << "The survivor looks at you, their expression unreadable.\n";
    std::cout << "\"I don't know how you're still standing, but if you're "
                 "smart, you won’t trust\n";
    std::cout << "anyone too easily. People will do whatever it takes to "
                 "survive.\"\n";

    std::cout << "\nPress any key to continue...\n";
    getKeyPress();
    clearConsole();
    std::cout << "You decide to head back. What a story.\n";
    std::cout << "Press any key to continue...";
    getKeyPress();
    headingToRuinedCity();
    
  } else if (choice == 1) {
    std::cout << "You find an old fire escape and climb up to a rooftop.\n";
    std::cout << "From here, you see movement in the distance—someone is out "
                 "there.\n";
    std::cout << "Press any key to continue... ";
    getKeyPress();
    findSurvivor();
    
  } else if (choice == 2) {
    std::cout << "You stay put and call out again...\n";
    std::cout
        << "But the response is fainter this time. They're moving away.\n";
    std::cout << "You may need to hurry before you lose them.\n";
    std::cout << "Press any key to continue... ";
    getKeyPress();
    findSurvivor();
  }

  std::cout << "\nPress any key to continue...\n";
  getKeyPress();
}
void yellOut() {
  clearConsole();
  if (SaveManager::difficulty == 1) { 
    std::cout << "Silence.\n";
    std::cout << "Only the wind and distant creaks of collapsing buildings "
                 "answer you.\n";
    std::cout << "It seems like no one is left to respond.\n";
    std::cout << "Press any key to continue... ";
    getKeyPress();
    headingToRuinedCity();

  } else {
    std::cout << "For a moment, there's nothing but the howl of the wind...\n";
    std::cout << "Then—faintly, in the distance—a voice calls back.\n";
    std::cout << "Someone is out there. You need to find them.\n";
    std::cout << "Press any key to continue... ";
    getKeyPress();
    findSurvivor();
  }
}

void crumblingSkyscraper() {
  clearConsole();
  if (SaveManager::hasVisitedLocation("crumblingSkyscraper")) {
    std::cout << "You've already visited the skyscraper, sorry.\n";
    std::cout << "Press any key to continue... ";
    getKeyPress();
    headingToRuinedCity();
  } else {
    SaveManager::saveVisitedLocation("crumblingSkyscraper");
    std::cout << "You step into the remains of a once-great skyscraper, its "
                 "skeletal frame groaning in the wind.\n";
    std::cout << "Dust and rubble crunch beneath your feet as you cautiously "
                 "make your way inside.\n\n";

    if (SaveManager::difficulty == 1) { 
      std::cout << "The building creaks ominously, a chilling reminder of its "
                   "fragile state.\n";
      std::cout << "As you climb the shattered stairs, the floor suddenly "
                   "gives way!\n";
      std::cout << "You barely manage to grab onto a twisted railing, but not "
                   "before taking some serious damage.\n";
      std::cout << "(-40 HP)\n";
      SaveManager::damage(40);
      std::cout << "You decide to turn back before the entire structure "
                   "collapses on you.\n";
    } else if (SaveManager::difficulty == 2) { 
      std::cout << "Navigating the ruins, you find an old office space filled "
                   "with broken desks and shattered monitors.\n";
      std::cout << "You rummage through the debris and uncover a locked metal "
                   "case.\n";
      std::cout << "Without a key, it's impossible to open it here. Maybe "
                   "someone in a trading hub could help?\n";
      SaveManager::addItem("Locked_Case");
      std::cout << "(You found a 'Locked_Case'!)\n";
    } else { 
      std::cout << "The skyscraper still holds secrets from the past.\n";
      std::cout
          << "You find an executive's abandoned office, untouched by time.\n";
      std::cout << "Inside a locked drawer, you discover an old stash of "
                   "emergency funds!\n";
      std::cout << "(+$1000)\n";
      SaveManager::money += 1000.0f;
    }

    std::cout << "\nPress any key to continue... ";
    SaveManager::saveGame("save.txt");
    getKeyPress();
    headingToRuinedCity(); 
  }
}
void spiritsMenu() {
  int choice = 0;
  bool inMenu = true;

 
  float prices[5] = {50.0f, 40.0f, 45.0f, 30.0f,
                     60.0f}; 
                             
  std::string options[6] = {
      "1. Jack Daniels Whisky", "2. Smirnoff", "3. Bacardi", "4. Malibu",
      "5. 1800 Tequila",        "6. Go back"};

  
  std::string saveNames[5] = {"JDWhisky", "Smirnoff", "Bacardi", "Malibu",
                              "1800Tequila"};

  while (inMenu) {
    clearConsole();
    std::cout << "\n==== Spirits Selection ====\n";

   
    for (int i = 0; i < 6; i++) {
      if (i == choice) {
        std::cout << "> " << options[i] << " - $" << prices[i]
                  << " <\n"; 
      } else {
        if (i < 5) {
          std::cout << options[i] << " - $" << prices[i]
                    << "\n"; 
        } else {
          std::cout << options[i] << "\n"; 
        }
      }
    }

    std::cout << "\nCurrent money: $" << SaveManager::money
              << "\n"; 

    char key = getKeyPress(); 

    if (key == 'w' || key == 'W') {
      choice = (choice == 0) ? 5 : choice - 1; 
    } else if (key == 's' || key == 'S') {
      choice = (choice == 5) ? 0 : choice + 1; 
    } else if (key == 10 || key == 13 || key == '\r' || key == '\n') { 
      if (choice == 5) {
        SaveManager::saveGame(
            "save.txt"); 
        return;          
      }

   
      if (SaveManager::money >= prices[choice]) {
        std::cout << "You bought " << options[choice].substr(3)
                  << "!\n";                  
        SaveManager::money -= prices[choice]; 
        SaveManager::money = std::max(
            SaveManager::money, 0.0f); 

       
        SaveManager::addItem(saveNames[choice]);

      
        SaveManager::saveGame("save.txt");
      } else {
        std::cout << "You don't have enough money to buy this item.\n";
      }
      std::cout << "\nPress any key to continue... ";
      getKeyPress();
    }
  }
}

void beerMenu() {
  int choice = 0;
  bool inMenu = true;

  
  float prices[5] = {5.0f, 3.0f, 4.5f, 3.5f,
                     4.0f}; 
                          
  std::string options[6] = {"1. Heineken",  "2. Pabst Blue Ribbon (PBR)",
                            "3. Corona",    "4. Coors Light",
                            "5. Budweiser", "6. Go back"};

 
  std::string saveNames[5] = {"Heineken", "PBRBeer", "Corona", "CoorsLight",
                              "Budweiser"};

  while (inMenu) {
    clearConsole();
    std::cout << "\n==== Beer Selection ====\n";

    
    for (int i = 0; i < 6; i++) {
      if (i == choice) {
        std::cout << "> " << options[i] << " - $" << prices[i]
                  << " <\n"; 
      } else {
        if (i < 5) {
          std::cout << options[i] << " - $" << prices[i]
                    << "\n"; 
        } else {
          std::cout << options[i] << "\n"; 
        }
      }
    }

    std::cout << "\nCurrent money: $" << SaveManager::money
              << "\n"; 

    char key = getKeyPress(); 

    if (key == 'w' || key == 'W') {
      choice = (choice == 0) ? 5 : choice - 1; 
    } else if (key == 's' || key == 'S') {
      choice = (choice == 5) ? 0 : choice + 1; 
    } else if (key == 10 || key == 13 || key == '\r' || key == '\n') { 
      if (choice == 5) {
        SaveManager::saveGame(
            "save.txt"); 
        return;          
      }

      
      if (SaveManager::money >= prices[choice]) {
        std::cout << "You bought " << options[choice].substr(3)
                  << "!\n";                   
        SaveManager::money -= prices[choice]; 
        SaveManager::money = std::max(
            SaveManager::money, 0.0f); 

        SaveManager::addItem(saveNames[choice]);

       
        SaveManager::saveGame("save.txt");
      } else {
        std::cout << "You don't have enough money to buy this item.\n";
      }
      std::cout << "\nPress any key to continue... ";
      getKeyPress();
    }
  }
}
void openLiquorStoreMenu() {
  clearConsole();
  int choice = 0;
  bool inMenu = true;

  while (inMenu) {
    std::cout << "\n==== Welcome to the Ashenfall Liquor Store ====\n";
    std::cout << "Please choose a category to browse:\n";

   
    std::string options[3] = {"1. Beer", "2. Spirits", "3. Leave the store"};

    for (int i = 0; i < 3; i++) {
      if (i == choice) {
        std::cout << "> " << options[i]
                  << " <\n"; 
      } else {
        std::cout << options[i] << "\n";
      }
    }

    char key = getKeyPress(); 

    if (key == 'w' || key == 'W') {
      choice = (choice == 0) ? 2 : choice - 1;
    } else if (key == 's' || key == 'S') {
      choice = (choice == 2) ? 0 : choice + 1;
    } else if (key == 10 || key == 13 || key == '\r' || key == '\n') {
      if (choice == 0) {
        beerMenu();
      } else if (choice == 1) {
        spiritsMenu();
      } else if (choice == 2) {
        inMenu = false; 
        headingToRuinedCity();
      }
    }

    clearConsole();
  }
}

void ashenfallLiquorStore() {
  clearConsole();
  std::cout << "From the outside, the liquor store looks like every other ruin "
               "in the city—windows shattered, sign barely hanging on.\n";
  std::cout << "You step inside, expecting decay and emptiness...\n";
  std::cout << "But to your surprise, the interior is clean. The shelves are "
               "stocked. The air smells of aged wood and faint whiskey.\n";
  std::cout << "Behind the counter stands a man, casually polishing a glass as "
               "if none of this was unusual.\n";
  std::cout << "He looks up, nodding slightly. \"Didn't think anyone was still "
               "out there looking for a drink.\"\n";
  std::cout << "You glance around, wondering how this place is so "
               "well-maintained while everything else has crumbled.\n";

  std::cout << "\nPress any key to continue... ";
  getKeyPress();

  openLiquorStoreMenu(); 
}
void exploreAbandonedMall() {
  if (SaveManager::hasVisitedLocation("Abandoned mall")) {
    std::cout << "You've already explored the abandoned mall.\n";
    std::cout << "Press any key to continue... ";
    getKeyPress();
    headingToRuinedCity();
  } else {
    SaveManager::saveVisitedLocation("Abandoned mall");
    std::vector<std::string> shops = {
        "Empty Clothing Store", "Empty Electronics Store", "Empty Food Court",
        "Luxury Jewelry Store", "Security Office"};
    int choice = 0;
    bool exploring = true;

    while (exploring) {
      clearConsole();
      std::cout << "\n==== ABANDONED MALL ====\n\n";
      for (int i = 0; i < shops.size(); i++) {
        if (i == choice) {
          std::cout << "> " << shops[i] << " <\n";
        } else {
          std::cout << "  " << shops[i] << "\n";
        }
      }
      std::cout << "\n[W/S to navigate, Enter to search, Esc to exit]\n";

      char key = getKeyPress();
      if (key == 'w' || key == 'W') {
        choice = (choice == 0) ? shops.size() - 1 : choice - 1;
      } else if (key == 's' || key == 'S') {
        choice = (choice == shops.size() - 1) ? 0 : choice + 1;
      } else if (key == 10 || key == 13 || key == '\r' ||
                 key == '\n') { 
        clearConsole();
        std::cout << "\nSearching " << shops[choice] << "...\n";

        if (choice < 3) {
          std::cout
              << "You find nothing of value. The store has been completely "
                 "looted.\n";
        } else if (choice == 3) {
          std::cout
              << "You break open a small safe in the back and find $500!\n";
          SaveManager::money += 500.0f;
        } else if (choice == 4) {
          std::cout << "You find a key for a Locked_Case. This might be useful "
                       "if you have the case.\n";
          SaveManager::addItem("Locked_Case_Key");
        }
        SaveManager::saveGame("save.txt");
        std::cout << "\nPress any key to continue...\n";
        getKeyPress();
      } else if (key == 27) { 
        exploring = false;
        headingToRuinedCity();
      }
    }
  }
}
void headingToRuinedCity() {
  SaveManager::updateLocation("RuinedCity");
  const std::vector<std::string> options = {
      "Investigate a crumbling skyscraper",
      "Yell out to find other people",
      "Explore the old mall",
      "Explore the run-down liquor store",
      "Go to the abandoned military checkpoint",
      "View Inventory", 
      "Save and exit",
      "Go Back (The man at the checkpoint might want something there...)" 
  };

  int choice = 0;
  bool selecting = true;

  while (selecting) {
      clearConsole();
      std::cout << "\n==== RUINED CITY OPTIONS ====\n\n";

      for (int i = 0; i < options.size(); i++) {
          if (i == choice) {
              std::cout << "> " << options[i] << " <\n";
          } else {
              std::cout << "  " << options[i] << "\n";
          }
      }

      std::cout << "\n[W/S to navigate, Enter to select]\n";
      char key = getKeyPress();

      if (key == 'w' || key == 'W') {
          choice = (choice == 0) ? options.size() - 1 : choice - 1;
      } else if (key == 's' || key == 'S') {
          choice = (choice == options.size() - 1) ? 0 : choice + 1;
      } else if (key == '\n' || key == '\r') { 
          selecting = false;
      }
  }

  clearConsole();
  if (choice == 0) {
      crumblingSkyscraper();
  } else if (choice == 1) {
      yellOut();
  } else if (choice == 2) {
      exploreAbandonedMall();
  } else if (choice == 3) {
      ashenfallLiquorStore();
  } else if (choice == 4) {
      abandonedMilitaryCheckpoint();
  } else if (choice == 5) {
      std::cout << "\n==== YOUR INVENTORY ====\n";
      SaveManager::displayInventory(); 
                                  
      std::cout << "\nPress any key to continue...";
      getKeyPress();
      headingToRuinedCity(); 
  } else if (choice == 6) {
      std::cout << "Saving game...\n";
      SaveManager::saveGame("save.txt");
      std::cout << "Game saved. Exiting...\n";
      exit(0);
  } else if (choice == 7) { 
      postApocalypse();
  }
}
void abandonedComputerLab() {
  if (SaveManager::hasVisitedLocation("abandonedComputerLab")) {
    std::cout << "You already visited the abandoned computer lab.\n";
    std::cout << "Press any key to continue... ";
    getKeyPress();
    postApocalypse();
  } else {
    if (SaveManager::hasItem("Encrypted_Data_Chip")) {
      SaveManager::saveVisitedLocation("abandonedComputerLab");
      std::cout
          << "\nYou approach the abandoned computer lab. The terminal is "
             "old, "
             "but the security system appears to be intact. You insert the "
             "Encrypted "
             "Data Chip into the computer's slot...\n";
      std::cout << "The screen flickers, and after a tense moment, the system "
                   "unlocks. "
                   "A set of important files appears on the screen.\n";
      std::cout
          << "These files seem to be worth a lot. You could probably trade "
             "them for some "
             "good money somewhere nearby...\n";
      std::cout << "Press any key to continue...\n";
      getKeyPress();

      SaveManager::removeItem("Encrypted_Data_Chip");
      SaveManager::addItem("Important_Files");
      SaveManager::saveGame("save.txt");
      postApocalypse();

    } else {
      std::cout
          << "\nYou don't have the Encrypted Data Chip. You cannot unlock "
             "the computer.\n";
      std::cout << "You turn back and return to the post-apocalyptic world.\n";
      std::cout << "Press any key to continue... ";
      getKeyPress();
      postApocalypse(); 
    }
  }
}
void postApocalypse() {
  SaveManager::updateLocation("FourWayCrossroad");
  SaveManager::saveGame("save.txt");

  const char *options[] = {"Go straight", "Go right", "Go left", "Go back (you might need something to go left)"};
  int selection = 0;
  const int numOptions = 4;

  while (true) {
    clearConsole();
    std::cout << "You reach a four-way street\n";
    std::cout << "What will you do?\n";

    for (int i = 0; i < numOptions; i++) {
      if (i == selection) {
        std::cout << "> " << options[i] << " <\n";
      } else {
        std::cout << "  " << options[i] << "  \n";
      }
    }

    char input = getKeyPress();
    if (input == 'w' || input == 'W') {
      selection = (selection - 1 + numOptions) % numOptions;
    } else if (input == 's' || input == 'S') {
      selection = (selection + 1) % numOptions;
    } else if (input == '\n' || input == '\r') {
      break;
    }
  }


  if (selection == 0) {
    headingToRuinedCity();
  } else if (selection == 1) {
    goRight();
  } else if (selection == 2) {
    abandonedComputerLab();
  } else if (selection == 3) {
    emergeFromBunkerHandling();
  }
}
void displayBunkerMenu(int selectedOption) {
  std::string options[] = {"Enter Bunker", "Check Inventory", "Exit"};

  clearConsole();

  std::cout << "Hidden Bunker Menu\n";
  std::cout << "Use 'w' to go up and 's' to go down.\n\n";

  for (int i = 0; i < 3; i++) {
    if (i == selectedOption) {
      std::cout << "> " << options[i] << " <"
                << std::endl; 
    } else {
      std::cout << options[i] << std::endl;
    }
  }
}
void insideBunkerMenu(int &selectedOption) {
  std::string bunkerOptions[] = {"Look for Treasures", "Exit Bunker"};

  while (true) {
    clearConsole(); 
    std::cout << "Inside the Bunker\n";
    std::cout << "Use 'w' to go up and 's' to go down.\n\n";

    for (int i = 0; i < 2; i++) {
      if (i == selectedOption) {
        std::cout << "> " << bunkerOptions[i] << " <" << std::endl;
      } else {
        std::cout << bunkerOptions[i] << std::endl;
      }
    }

    char input = getKeyPress(); 

    if (input == 'w' || input == 'W') {
      if (selectedOption > 0) {
        selectedOption--;
      }
    } else if (input == 's' || input == 'S') {
      if (selectedOption < 1) {
        selectedOption++;
      }
    } else if (input == 13 || input == '\n') {
      if (selectedOption == 0) {
        std::cout << "You look for treasures in the bunker... You found a "
                     "hidden stash of potions and alcohol...\n";
        SaveManager::addItem("PBRBeer", 15);
        SaveManager::addItem("Stamina_Potion", 5);
        SaveManager::addItem("Health_Potion", 2);
        SaveManager::saveGame("save.txt");
        std::cout << "Press any key to continue... ";
        getKeyPress();  
        postApocalypse();
        break;
      } else if (selectedOption == 1) {
        std::cout << "You exit the bunker.\n";
        postApocalypse(); 
        break;
      }
    }
  }
}
void hiddenBunkerMenu() {
  clearConsole();
  if (SaveManager::hasVisitedLocation("HiddenBunker")) {

  } else {
    SaveManager::saveVisitedLocation("HiddenBunker");
  }
  int selectedOption = 0;
  bool selecting = true;

  while (selecting) {
    clearConsole();
    displayBunkerMenu(
        selectedOption);

    char input = getKeyPress(); 

    if (input == 'w' || input == 'W') {
      selectedOption =
          (selectedOption == 0) ? 2 : selectedOption - 1; 
    } else if (input == 's' || input == 'S') {
      selectedOption = (selectedOption == 2) ? 0 : selectedOption + 1;
    } else if (input == '\n' || input == '\r') { 
      switch (selectedOption) {
      case 0:
        std::cout << "You enter the bunker.\n";
        insideBunkerMenu(selectedOption);
        break;
      case 1:
        std::cout << "You check your inventory.\n";
        SaveManager::displayInventory();
        hiddenBunkerMenu();
        break;
      case 2:
        std::cout << "Exiting...\n";
        selecting = false; 
        break;
      }
    }
  }
}
void followTheRoad() {
  clearConsole();
  SaveManager::updateLocation("TheRoad");
  SaveManager::saveGame("save.txt");
  std::cout << "==== FOLLOWING THE ROAD ====\n\n";
  std::cout << "You decide to follow the cracked and worn road, unsure of what "
               "lies ahead...\n";
  std::cout << "After a long walk, you notice a figure sitting by a ruined gas "
               "station.\n";
  std::cout << "A hooded stranger motions for you to come closer.\n\n";

  std::cout << "\"You look like someone searching for answers... or "
               "supplies,\" the stranger says.\n";
  std::cout << "\"I have something valuable for you, if you're willing to "
               "trade. Or "
               "perhaps, you'd rather hear about a hidden bunker?\"\n\n";

  std::string options[] = {"Trade supplies with the stranger",
                           "Ask about the hidden bunker",
                           "Ignore them and keep walking",
                           "Threaten them and try to steal their belongings"};

  int choice = 0;
  bool selecting = true;

  while (selecting) {
    clearConsole();
    std::cout << "==== FOLLOWING THE ROAD ====\n\n";
    std::cout
        << "\"I have something valuable for you, if you're willing to trade. "
           "Or perhaps, you'd rather hear about a hidden bunker?\"\n\n";

   
    for (int i = 0; i < 4; ++i) {
      if (i == choice)
        std::cout << "> " << options[i] << " <\n";
      else
        std::cout << "  " << options[i] << "\n";
    }

    char key = getKeyPress(); 
    if (key == 'w' || key == 'W') {
      choice = (choice == 0) ? 3 : choice - 1; 
    } else if (key == 's' || key == 'S') {
      choice = (choice == 3) ? 0 : choice + 1;
    } else if (key == 13 || key == '\n') {
      selecting = false; 
    }
  }

  
  if (choice == 0) {
    std::cout << "\nYou trade some of your supplies for a rare item: a heavily "
                 "encrypted data chip.\n";
    SaveManager::addItem("Encrypted_Data_Chip");
    SaveManager::saveGame("save.txt");
    std::cout << "Press any key to continue... ";
    getKeyPress();
    postApocalypse();
  } else if (choice == 1) {
    SaveManager::loadVisitedLocations();
    if (SaveManager::hasVisitedLocation("HiddenBunker")) {
      std::cout << "You've already visited the hidden bunker.\n";
      std::cout << "Press any key to continue.... ";
      getKeyPress();
      followTheRoad();
    } else {
      std::cout << "\nThe stranger smirks. \"There's an old bunker, untouched "
                   "by raiders. Coordinates? Thatll cost you.\"\n";
      std::cout << "You reluctantly hand over some money ($25), and he "
                   "provides rough directions.\n";
      SaveManager::money = std::max(0.0f, SaveManager::money - 25);
      SaveManager::saveGame("save.txt");
      std::cout << "Press any key to continue... ";
      getKeyPress();
      hiddenBunkerMenu();
    }
  } else if (choice == 2) {
    std::cout
        << "\nYou decide not to trust them and keep walking down the road.\n";
    std::cout << "Press any key to continue... ";
    getKeyPress();
    postApocalypse();
  } else if (choice == 3) {
    std::cout << "\nYou draw your weapon, but the stranger is faster.\n";
    std::cout << "Before you react, a concealed turret activates, forcing you "
                 "to flee!\n";
    std::cout << "(You barely escape but lose some health.)\n";
    SaveManager::damage(20);
    SaveManager::saveGame("save.txt");
    std::cout << "Press any key to continue... ";
    getKeyPress();
    postApocalypse();
  }
}

void setupTemporaryCamp() {
  clearConsole();
  if (SaveManager::hasVisitedLocation("temporary camp")) {
    std::cout << "You already set up a camp... can't do it again.\n";
    std::cout << "Press any key to continue... ";
    getKeyPress();
    emergeFromBunkerHandling();
  } else {
    std::cout << "\nYou decide to set up a temporary camp...\n";

    int healthGained = 30;
    int staminaGained = 50;
    SaveManager::saveVisitedLocation("temporary camp");
    SaveManager::health += healthGained;
    SaveManager::stamina += healthGained;
    SaveManager::saveGame("save.txt");

    std::cout << "\nYou rest for the night.\n";
    std::cout << " - Health restored: +" << healthGained
              << " (Current: " << SaveManager::health << ")\n";
    std::cout << " - Stamina restored: +" << staminaGained
              << " (Current: " << SaveManager::stamina << ")\n";
    std::cout << "Press any key to continue... ";
    getKeyPress();
    if (SaveManager::difficulty == 1) {
      std::cout << "\nYou were attacked by raiders in the night! You barely "
                   "escape but lose 20 HP.\n";
      SaveManager::damage(20);
      SaveManager::saveGame("save.txt");
      std::cout << "Press any key to continue... ";
      getKeyPress();
      emergeFromBunkerHandling();
    } else if (SaveManager::difficulty == 2) {
      std::cout << "\nYou find some supplies near your campsite. +1 "
                   "Health_Potion added to inventory.\n";
      SaveManager::addItem("Health_Potion", 1);
      SaveManager::saveGame("save.txt");
      std::cout << "Press any key to continue... ";
      getKeyPress();
      emergeFromBunkerHandling();
    } else if (SaveManager::difficulty == 3) {
      std::cout
          << "\nA heavy storm disrupts your sleep. Stamina reduced by 20.\n";
      SaveManager::loseStamina(20);
      SaveManager::saveGame("save.txt");
      std::cout << "Press any key to continue... ";
      getKeyPress();
      emergeFromBunkerHandling();
    }
  }
}
void emergeFromBunkerHandling() {
  SaveManager::loadVisitedLocations();
  SaveManager::updateLocation("EmergeFromBunker");
  SaveManager::saveGame("save.txt");

  int choice = 0;
  bool selecting = true;

  emergeFromBunkerScenario();

  while (selecting) {
    showStartOfChapter1Menu(choice);
    char key = getKeyPress();

    
    if (key == 'w' || key == 'W') {
      choice = (choice == 0) ? 6 : choice - 1; 
    } else if (key == 's' || key == 'S') {
      choice = (choice == 6) ? 0 : choice + 1; 
    } else if (key == 13 || key == '\n') {
      selecting = false; 
    }
  }


  if (choice == 0) {
    surveyLandscape();
  } else if (choice == 1) {
    vulturesCirclingScenario();
  } else if (choice == 2) {
    if (SaveManager::hasVisitedLocation("Abandoned Laboratory")) {
      emergeFromBunkerHandling(); 
    } else {
      investigateNearbyRuins();
    }
  } else if (choice == 3) {
    headTowardsSmokePlume();
  } else if (choice == 4) {
    setupTemporaryCamp();
  } else if (choice == 5) {
    SaveManager::displayInventory(); 
    emergeFromBunkerHandling();
  } else if (choice == 6) {
    if (SaveManager::hasVisitedLocation("temporary camp")) {
      followTheRoad();
    } else {
      emergeFromBunkerHandling(); 
    }
  }
}
