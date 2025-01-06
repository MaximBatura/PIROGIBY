#include "menu_manager.h"

void MenuItem::addIngredient(std::shared_ptr<Ingredient> ingredient, double grams) {
    for (auto pair : composition) {
        if (pair.first == ingredient) {
            pair.second += grams;
            return;
        }
    }
    composition.emplace_back(std::make_pair(ingredient, grams));
}

void MenuItem::removeIngredient(const std::shared_ptr<Ingredient>& ingredient) {
    auto it = std::find_if(composition.begin(), composition.end(),
        [&](const std::pair<std::shared_ptr<Ingredient>, double>& p) { return p.first == ingredient; });
    if (it != composition.end()) {
        composition.erase(it);
    }
}

void MenuItem::updateIngredient(std::shared_ptr<Ingredient> ingredient, double newGrams) {
    for (auto& pair : composition) {
        if (pair.first.get() == ingredient.get()) {
            pair.second = newGrams;
            break;
        }
    }
}

double MenuItem::calculateProtein() const {
    double totalProtein = 0.0;
    for (const auto& pair : composition) {
        if (pair.first->getUnit() == "     ") {
            totalProtein += pair.first->getProtein() * pair.second / 100.0;
        }
        else {
            totalProtein += pair.first->getProtein() * pair.second;
        }
        
    }
    return totalProtein;
}

double MenuItem::calculateFat() const {
    double totalFat = 0.0;
    for (auto& pair : composition) {
        if (pair.first->getUnit() == "     ") {
            totalFat += pair.first->getFat() * pair.second / 100.0;
        }
        else {
            totalFat += pair.first->getFat() * pair.second;
        }
        
    }
    return totalFat;
}

double MenuItem::calculateCarbs() const {
    double totalCarbs = 0.0;
    for (auto& pair : composition) {
        if (pair.first->getUnit() == "     ") {
            totalCarbs += pair.first->getCarbs() * pair.second / 100.0;
        }
        else {
            totalCarbs += pair.first->getCarbs() * pair.second;
        }
        
    }
    return totalCarbs;
}

double MenuItem::calculateCalories() const {
    double totalCalories = 0.0;
    for (auto& pair : composition) {
        if (pair.first->getUnit() == "     ") {
            totalCalories += pair.first->getCalories() * pair.second / 100.0;
        }
        else {
            totalCalories += pair.first->getCalories() * pair.second;
        }
        
    }
    return totalCalories;
}

//                                         
bool MenuItem::isIngredientInComposition(const Ingredient& ingredient){
    for (const auto& pair : composition) {
        if ((*pair.first).getId() == ingredient.getId()) { //                         Ingredient
            return true;
        }
    }
    return false;
}


void MenuManager::addNewPosition() {

    //                               ID
    int itemID = findNextAvailableId(menuItems);

    std::cout << "                        : ";
    std::string itemName = getUserString();

    MenuItem* newItem = new MenuItem(itemID, itemName);

    if (ingredientsManager->getIngredients().size() == 0) {
        std::cout << "                                      .                                          .\n";

        std::cout << "                                            ,              : ";
        newItem->self_protein = getUserChoice(0, 100);

        std::cout << "                                            ,              : ";
        newItem->self_fat = getUserChoice(0, 100);

        std::cout << "                                                ,              : ";
        newItem->self_carbs = getUserChoice(0, 100);

        std::cout << "                                         ,              : ";
        newItem->self_calories = getUserChoice(0, 100);
    }
    else {

        bool cancel = false;
        std::vector<int> added_ids = {};
        while (!cancel) {
            ingredientsManager->displayAllIngredients(added_ids);

            bool choosen = false;
            while (!choosen) {
                std::cout << "        id                 'q'           : ";
                std::string input;
                std::getline(std::cin, input);

                if (input.length() > MAX_STRING_LENGTH) {
                    std::cout << "                          - " << MAX_STRING_LENGTH << "         .                  .\n";
                    continue;
                }
                
                if (input == "q" or input == "Q") {
                    choosen = true;
                    cancel = true;
                }
                else if (isNumber(input)) {
                    int index = stringToInt(input);
                    if (checkId(ingredientsManager->getIngredients(), index)) {
                        Ingredient selectedIngredient = ingredientsManager->getIngredientById(index);
                        if (newItem->isIngredientInComposition(selectedIngredient)) {
                            std::cout << "                            \n";
                            continue;
                        }
                        else {
                            if (selectedIngredient.getUnit() == "     ") {
                                std::cout << "                        : ";
                            }
                            else {
                                std::cout << "                       : ";
                            }
                            int grams;
                            std::cin >> grams;
                            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

                            std::shared_ptr<Ingredient> sharedPtr = std::make_shared<Ingredient>(selectedIngredient);
                            newItem->addIngredient(sharedPtr, grams);
                            added_ids.push_back(selectedIngredient.getId());
                            choosen = true;
                        }
                    }
                    else {
                        std::cout << "           id.                   .\n";
                        continue;
                    }
                }
                else {
                    std::cout << "                 .                 .\n";
                    continue;
                }
            }
        }
    }

    std::cout << "                    : ";
    int itemCost = getUserChoice(0, 1000000);
    newItem->setCost(itemCost);

    menuItems.push_back(newItem);
    system("cls");
    std::cout << "                         !\n";
}

void MenuManager::deletePosition() {
    if (menuItems.empty()) {
        std::cout << "          .\n";
        return;
    }

    std::cout << "                             :\n";
    for (size_t i = 0; i < menuItems.size(); ++i) {
        std::cout << menuItems[i]->getId() << ". " << menuItems[i]->getName() << "\n";
    }
    
    int id_to_delete = getId(menuItems);
    MenuItem* menuItem_to_delete = findMenuItemById(id_to_delete);
    //                                   ,                                
    auto it = std::find(menuItems.begin(), menuItems.end(), menuItem_to_delete);
    menuItems.erase(it);

    system("cls");
    std::cout << "               .\n";
}


void MenuManager::viewAllMenuPositions() {
    system("cls");
    if (menuItems.empty()) {
        std::cout << "          .\n";
        return;
    }

    for (size_t i = 0; i < menuItems.size(); ++i) {
        if (menuItems[i]->self_protein == 0) {
            std::cout << "ID: " << menuItems[i]->getId() << ",         : " << menuItems[i]->getName() << ",      : " << menuItems[i]->calculateProtein() << ",     : " << menuItems[i]->calculateFat() << ",         : " << menuItems[i]->calculateCarbs() << ",         : " << menuItems[i]->calculateCalories() << ",     : " << menuItems[i]->getCost() << "\n";
        }
        else {
            std::cout << "ID: " << menuItems[i]->getId() << ",         : " << menuItems[i]->getName() << ",      : " << menuItems[i]->self_protein << ",     : " << menuItems[i]->self_fat << ",         : " << menuItems[i]->self_carbs << ",         : " << menuItems[i]->self_calories<< ",     : " << menuItems[i]->getCost() << "\n";
        }
    }
}

MenuItem* MenuManager::findMenuItemById(int id) const {
    for (auto& menuItem : menuItems) {
        if (menuItem->getId() == id) {
            return menuItem;
        }
    }

    //                       ,            nullptr
    return nullptr;
}


void MenuMenu(MenuManager& manager) {
    system("cls");
    while (true) {
        std::cout << "---     : ---\n";
        std::cout << "1.                 \n";
        std::cout << "2.                \n";
        std::cout << "3.                       \n";
        std::cout << "4.                         \n";
        std::cout << "              : ";
        int choice = getUserChoice(1, 4);

        switch (choice) {
        case 1:
            manager.addNewPosition();
            break;
        case 2:
            manager.deletePosition();
            break;
        case 3:
            manager.viewAllMenuPositions();
            break;
        case 4:
            return;
        default:
            std::cerr << "              .\n";
            break;
        }
    }
}