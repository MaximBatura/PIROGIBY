#include "menu_manager.h"

void MenuItem::addIngredient(Ingredient* ingredient, double grams) {
    for (auto& pair : composition) {
        if (pair.first == ingredient) {
            pair.second += grams;
            return;
        }
    }
    composition.emplace_back(std::make_pair(ingredient, grams));
}

void MenuItem::removeIngredient(Ingredient* ingredient) {
    auto it = std::find_if(composition.begin(), composition.end(),
        [&](const std::pair<Ingredient*, double>& p) { return p.first == ingredient; });
    if (it != composition.end()) {
        composition.erase(it);
    }
}

void MenuItem::updateIngredient(Ingredient* ingredient, double newGrams) {
    for (auto& pair : composition) {
        if (pair.first == ingredient) {
            pair.second = newGrams;
            break;
        }
    }
}

double MenuItem::calculateProtein() {
    double totalProtein = 0.0;
    for (auto& pair : composition) {
        std::cout << pair.first->getProtein();
        std::cout << pair.second / 100.0;
        totalProtein += pair.first->getProtein() * pair.second / 100.0;
    }
    return totalProtein;
}

double MenuItem::calculateFat() {
    double totalFat = 0.0;
    for (auto& pair : composition) {
        totalFat += pair.first->getFat() * pair.second / 100.0;
    }
    return totalFat;
}

double MenuItem::calculateCarbs() {
    double totalCarbs = 0.0;
    for (auto& pair : composition) {
        totalCarbs += pair.first->getCarbs() * pair.second / 100.0;
    }
    return totalCarbs;
}

double MenuItem::calculateCalories() {
    double totalCalories = 0.0;
    for (auto& pair : composition) {
        totalCalories += pair.first->getCalories() * pair.second / 100.0;
    }
    return totalCalories;
}

void MenuItem::printInfo() {
    std::cout << "ID: " << id << "\n";
    std::cout << "Название: " << name << "\n";
    std::cout << "Состав:\n";
    for (auto& pair : composition) {
        std::cout << "- " << pair.first->getName() << ": " << pair.second << " г\n";
    }
    std::cout << "Белки: " << calculateProtein() << " г\n";
    std::cout << "Жиры: " << calculateFat() << " г\n";
    std::cout << "Углеводы: " << calculateCarbs() << " г\n";
    std::cout << "Калории: " << calculateCalories() << " ккал\n";
}


void MenuManager::addNewPosition() {
    std::string itemName;
    std::cout << "Введите название позиции: ";
    std::cin >> itemName;

    MenuItem* newItem = new MenuItem(menuItems.size() + 1, itemName);

    bool done = false;
    while (!done) {
        std::cout << "Выберите ингредиент для добавления (или 'q' для выхода):\n";
        ingredientsManager->displayAllIngredients();

        char choice;
        std::cin >> choice;
        if (choice == 'q') {
            done = true;
        }
        else {
            size_t index = choice - '1';
            if (index >= 0 && index < ingredientsManager->getIngredients().size()) {
                Ingredient selectedIngredient = ingredientsManager->getIngredients()[index];

                double grams;
                std::cout << "Введите количество грамм: ";
                std::cin >> grams;

                newItem->addIngredient(&selectedIngredient, grams);
            }
            else {
                std::cerr << "Неверный выбор.\n";
            }
        }
    }

    menuItems.push_back(newItem);
    std::cout << "Позиция успешно добавлена!\n";
}

void MenuManager::deletePosition() {
    if (menuItems.empty()) {
        std::cout << "Меню пусто.\n";
        return;
    }

    std::cout << "Выберите позицию для удаления:\n";
    for (size_t i = 0; i < menuItems.size(); ++i) {
        std::cout << i + 1 << ". " << menuItems[i]->getName() << "\n";
    }

    size_t choice;
    std::cin >> choice;
    if (choice > 0 && choice <= menuItems.size()) {
        delete menuItems[choice - 1];
        menuItems.erase(menuItems.begin() + choice - 1);
        std::cout << "Позиция удалена.\n";
    }
    else {
        std::cerr << "Неверный выбор.\n";
    }
}


void MenuManager::viewAllMenuPositions() {
    if (menuItems.empty()) {
        std::cout << "Меню пусто.\n";
        return;
    }

    for (size_t i = 0; i < menuItems.size(); ++i) {
        std::cout << "ID: " << menuItems[i]->getId() << ", Название: " << menuItems[i]->getName() << ", Белки: " << menuItems[i]->calculateProtein() << ", Жиры: " << menuItems[i]->calculateFat() << ", Углеводы: " << menuItems[i]->calculateCarbs() << ", Ккалории: " << menuItems[i]->calculateCalories() << "\n";
    }
}

void MenuMenu(MenuManager& manager) {
    while (true) {
        std::cout << "--- Меню: ---\n";
        std::cout << "1. Добавить позицию\n";
        std::cout << "2. Удалить позицию\n";
        std::cout << "3. Посмотреть все позиции\n";
        std::cout << "4. Вернуться в главное меню\n";

        int choice;
        std::cin >> choice;

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