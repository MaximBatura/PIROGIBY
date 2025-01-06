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
        if (pair.first->getUnit() == "грамм") {
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
        if (pair.first->getUnit() == "грамм") {
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
        if (pair.first->getUnit() == "грамм") {
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
        if (pair.first->getUnit() == "грамм") {
            totalCalories += pair.first->getCalories() * pair.second / 100.0;
        }
        else {
            totalCalories += pair.first->getCalories() * pair.second;
        }
        
    }
    return totalCalories;
}

// Функция для поиска ингредиента в составе
bool MenuItem::isIngredientInComposition(const Ingredient& ingredient){
    for (const auto& pair : composition) {
        if ((*pair.first).getId() == ingredient.getId()) { // Сравниваем сами объекты Ingredient
            return true;
        }
    }
    return false;
}


void MenuManager::addNewPosition() {

    // Находим минимальный доступный ID
    int itemID = findNextAvailableId(menuItems);

    std::cout << "Введите название позиции: ";
    std::string itemName = getUserString();

    MenuItem* newItem = new MenuItem(itemID, itemName);

    if (ingredientsManager->getIngredients().size() == 0) {
        std::cout << "У вас нет ни одного ингредиента в базе. Будет добавлена не составная позиция меню.\n";

        std::cout << "Введите сколько грамм белка в данной позиции, если они есть: ";
        newItem->self_protein = getUserChoice(0, 100);

        std::cout << "Введите сколько грамм жиров в данной позиции, если они есть: ";
        newItem->self_fat = getUserChoice(0, 100);

        std::cout << "Введите сколько грамм углеводов в данной позиции, если они есть: ";
        newItem->self_carbs = getUserChoice(0, 100);

        std::cout << "Введите сколько ккалорий в данной позиции, если они есть: ";
        newItem->self_calories = getUserChoice(0, 100);
    }
    else {

        bool cancel = false;
        std::vector<int> added_ids = {};
        while (!cancel) {
            ingredientsManager->displayAllIngredients(added_ids);

            bool choosen = false;
            while (!choosen) {
                std::cout << "Введите id ингредиента или 'q' для выхода: ";
                std::string input;
                std::getline(std::cin, input);

                if (input.length() > MAX_STRING_LENGTH) {
                    std::cout << "Максимальная длина строки - " << MAX_STRING_LENGTH << " символов. Повторите попытку.\n";
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
                            std::cout << "Этот ингредиент уже добавлен\n";
                            continue;
                        }
                        else {
                            if (selectedIngredient.getUnit() == "грамм") {
                                std::cout << "Введите количество грамм: ";
                            }
                            else {
                                std::cout << "Введите количество штук: ";
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
                        std::cout << "Нет такого id. Попробуйте еще раз.\n";
                        continue;
                    }
                }
                else {
                    std::cout << "Некорректный ввод. Попробуйте снова.\n";
                    continue;
                }
            }
        }
    }

    std::cout << "Введите цену позиции: ";
    int itemCost = getUserChoice(0, 1000000);
    newItem->setCost(itemCost);

    menuItems.push_back(newItem);
    system("cls");
    std::cout << "Позиция успешно добавлена!\n";
}

void MenuManager::deletePosition() {
    if (menuItems.empty()) {
        std::cout << "Меню пусто.\n";
        return;
    }

    std::cout << "Выберите позицию для удаления:\n";
    for (size_t i = 0; i < menuItems.size(); ++i) {
        std::cout << menuItems[i]->getId() << ". " << menuItems[i]->getName() << "\n";
    }
    
    int id_to_delete = getId(menuItems);
    MenuItem* menuItem_to_delete = findMenuItemById(id_to_delete);
    // Теперь находим итератор на элемент, соответствующий этому указателю
    auto it = std::find(menuItems.begin(), menuItems.end(), menuItem_to_delete);
    menuItems.erase(it);

    system("cls");
    std::cout << "Позиция удалена.\n";
}


void MenuManager::viewAllMenuPositions() {
    system("cls");
    if (menuItems.empty()) {
        std::cout << "Меню пусто.\n";
        return;
    }

    for (size_t i = 0; i < menuItems.size(); ++i) {
        if (menuItems[i]->self_protein == 0) {
            std::cout << "ID: " << menuItems[i]->getId() << ", Название: " << menuItems[i]->getName() << ", Белки: " << menuItems[i]->calculateProtein() << ", Жиры: " << menuItems[i]->calculateFat() << ", Углеводы: " << menuItems[i]->calculateCarbs() << ", Ккалории: " << menuItems[i]->calculateCalories() << ", Цена: " << menuItems[i]->getCost() << "\n";
        }
        else {
            std::cout << "ID: " << menuItems[i]->getId() << ", Название: " << menuItems[i]->getName() << ", Белки: " << menuItems[i]->self_protein << ", Жиры: " << menuItems[i]->self_fat << ", Углеводы: " << menuItems[i]->self_carbs << ", Ккалории: " << menuItems[i]->self_calories<< ", Цена: " << menuItems[i]->getCost() << "\n";
        }
    }
}

MenuItem* MenuManager::findMenuItemById(int id) const {
    for (auto& menuItem : menuItems) {
        if (menuItem->getId() == id) {
            return menuItem;
        }
    }

    // Если элемент не найден, возвращаем nullptr
    return nullptr;
}


void MenuMenu(MenuManager& manager) {
    system("cls");
    while (true) {
        std::cout << "--- Меню: ---\n";
        std::cout << "1. Добавить позицию\n";
        std::cout << "2. Удалить позицию\n";
        std::cout << "3. Посмотреть все позиции\n";
        std::cout << "4. Вернуться в главное меню\n";
        std::cout << "Выберите пункт: ";
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
            std::cerr << "Неверный выбор.\n";
            break;
        }
    }
}