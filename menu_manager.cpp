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
    std::cout << "��������: " << name << "\n";
    std::cout << "������:\n";
    for (auto& pair : composition) {
        std::cout << "- " << pair.first->getName() << ": " << pair.second << " �\n";
    }
    std::cout << "�����: " << calculateProtein() << " �\n";
    std::cout << "����: " << calculateFat() << " �\n";
    std::cout << "��������: " << calculateCarbs() << " �\n";
    std::cout << "�������: " << calculateCalories() << " ����\n";
}


void MenuManager::addNewPosition() {
    std::string itemName;
    std::cout << "������� �������� �������: ";
    std::cin >> itemName;

    MenuItem* newItem = new MenuItem(menuItems.size() + 1, itemName);

    bool done = false;
    while (!done) {
        std::cout << "�������� ���������� ��� ���������� (��� 'q' ��� ������):\n";
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
                std::cout << "������� ���������� �����: ";
                std::cin >> grams;

                newItem->addIngredient(&selectedIngredient, grams);
            }
            else {
                std::cerr << "�������� �����.\n";
            }
        }
    }

    menuItems.push_back(newItem);
    std::cout << "������� ������� ���������!\n";
}

void MenuManager::deletePosition() {
    if (menuItems.empty()) {
        std::cout << "���� �����.\n";
        return;
    }

    std::cout << "�������� ������� ��� ��������:\n";
    for (size_t i = 0; i < menuItems.size(); ++i) {
        std::cout << i + 1 << ". " << menuItems[i]->getName() << "\n";
    }

    size_t choice;
    std::cin >> choice;
    if (choice > 0 && choice <= menuItems.size()) {
        delete menuItems[choice - 1];
        menuItems.erase(menuItems.begin() + choice - 1);
        std::cout << "������� �������.\n";
    }
    else {
        std::cerr << "�������� �����.\n";
    }
}


void MenuManager::viewAllMenuPositions() {
    if (menuItems.empty()) {
        std::cout << "���� �����.\n";
        return;
    }

    for (size_t i = 0; i < menuItems.size(); ++i) {
        std::cout << "ID: " << menuItems[i]->getId() << ", ��������: " << menuItems[i]->getName() << ", �����: " << menuItems[i]->calculateProtein() << ", ����: " << menuItems[i]->calculateFat() << ", ��������: " << menuItems[i]->calculateCarbs() << ", ��������: " << menuItems[i]->calculateCalories() << "\n";
    }
}

void MenuMenu(MenuManager& manager) {
    while (true) {
        std::cout << "--- ����: ---\n";
        std::cout << "1. �������� �������\n";
        std::cout << "2. ������� �������\n";
        std::cout << "3. ���������� ��� �������\n";
        std::cout << "4. ��������� � ������� ����\n";

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
            std::cerr << "�������� �����.\n";
            break;
        }
    }
}