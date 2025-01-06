#include "ingredients_manager.h"

void IngredientsManager::addIngredient(const Ingredient& ingredient) {
    ingredients.push_back(ingredient);
}

bool IngredientsManager::removeIngredient(int id) {
    auto it = std::find_if(ingredients.begin(), ingredients.end(),
        [id](const Ingredient& ing) { return ing.getId() == id; });

    if (it != ingredients.end()) {
        ingredients.erase(it);
        return true;
    }
    return false;
}

bool IngredientsManager::updateIngredient(int id, const Ingredient& updatedIngredient) {
    auto it = std::find_if(ingredients.begin(), ingredients.end(),
        [id](const Ingredient& ing) { return ing.getId() == id; });

    if (it != ingredients.end()) {
        *it = updatedIngredient;
        return true;
    }
    return false;
}

void IngredientsManager::displayAllIngredients(std::vector<int> without_ids) const {
    if (ingredients.size() != 0 and ingredients.size() == without_ids.size()) {
        std::cout << "������������ ������ ���\n";
        return;
    }
    for (const auto& ingredient : ingredients) {
        auto it = std::find(without_ids.begin(), without_ids.end(), ingredient.getId());

        // ���������, ����� �� �� �����
        if (it != without_ids.end()) {
            continue;
        }
        else {
            std::cout << "ID: " << ingredient.getId()
                << ", ������������: " << ingredient.getName()
                << ", �����: " << ingredient.getProtein()
                << ", ����: " << ingredient.getFat()
                << ", ��������: " << ingredient.getCarbs()
                << ", �������: " << ingredient.getCalories() << "\n";
        } 
    }
}

bool IngredientsManager::canChange() const
{
    if (ingredients.size() == 0) {
        system("cls");
        std::cout << "� ��� ��� �� ������ ����������� � ����.\n";
        return false;
    }
}

Ingredient IngredientsManager::getIngredientById(int id) const
{
    for (auto ingredient : ingredients) {
        if (ingredient.getId() == id) {
            return ingredient;
        }
    }
}

Ingredient* IngredientsManager::getIngredientPtrById(int id)
{
    for (auto& ingredient : ingredients) {
        if (ingredient.getId() == id) {
            return &ingredient;
        }
    }
}

void Ingredient::removeZeroQuantityBatches() {
    // ������� ��������� ������ ��� �������� �������� ������
    std::vector<Batch> nonEmptyBatches;

    // �������� ��� �������� ������ � ����� ������
    for (auto& batch : batches) {
        if (batch.getQuantity() > 0) {
            nonEmptyBatches.push_back(batch);
        }
    }

    // �������� ������ ������ ����� ������� ��� ������ ������
    batches.swap(nonEmptyBatches);
}

void ingredientsMenu(IngredientsManager& manager) {
    system("cls");
    while (true) {
        std::cout << "--- ���������� ������������ ---\n";
        std::cout << "1. �������� ����������\n";
        std::cout << "2. ������� ����������\n";
        std::cout << "3. �������� ����������\n";
        std::cout << "4. ����������� ��� �����������\n";
        std::cout << "5. ��������� � ������� ����\n";
        std::cout << "�������� �����: ";

        int choice = getUserChoice(0, 5);

        switch (choice) {
        case 1: {
            // ������� ����������� ��������� ID
            int id = findNextAvailableId(manager.getIngredients());

            std::cout << "������� ������� ��������� (����� - 1, ���� - 2): ";
            int choice = getUserChoice(1, 2);
            std::string unit;
            if (choice == 1) {
                unit = "�����";
            }
            else {
                unit = "����";
            }

            std::cout << "������� ������������ �����������: ";
            std::string name = getUserString();
            std::cout << "������� ���������� ������ �� 100 � / ����� ��������: ";
            int protein = getUserChoice(0, 100);
            std::cout << "������� ���������� ����� �� 100 � / ����� ��������: ";
            int fat = getUserChoice(0, 100);
            std::cout << "������� ���������� ��������� �� 100 � / ����� ��������: ";
            int carbs = getUserChoice(0, 100);
            std::cout << "������� ���������� ������� �� 100 � / ����� ��������: ";
            int calories = getUserChoice(0, 100000);

            Ingredient newIngredient(id, name, unit, protein, fat, carbs, calories);
            manager.addIngredient(newIngredient);
            system("cls");
            std::cout << "���������� ������� ��������!" << std::endl;
            break;
        }
        case 2: {
            if (!manager.canChange()) { break; }
            std::cout << "������� ID ����������� ��� ��������: ";
            int id = getId(manager.getIngredients());

            system("cls");
            if (manager.removeIngredient(id)) {
                std::cout << "���������� ������!" << std::endl;
            }
            else {
                std::cout << "���������� � ����� ID �� ������!" << std::endl;
            }
            break;
        }
        case 3: {
            if (!manager.canChange()) { break; }

            std::cout << "������� ID ����������� ��� ���������: ";
            int id = getId(manager.getIngredients());

            std::cout << "������� ����� ������������ �����������: ";
            std::string name = getUserString();
            std::cout << "������� ������� ��������� (����� - 1, ���� - 2): ";
            int choice = getUserChoice(1, 2);
            std::string unit;
            if (choice == 1) {
                unit = "�����";
            }
            else {
                unit = "����";
            }
            std::cout << "������� ����� ���������� ������ �� 100 � / ����� ��������: ";
            int protein = getUserChoice(0, 100);
            std::cout << "������� ����� ���������� ����� �� 100 � / ����� ��������: ";
            int fat = getUserChoice(0, 100);
            std::cout << "������� ����� ���������� ��������� �� 100 � / ����� ��������: ";
            int carbs = getUserChoice(0, 100);
            std::cout << "������� ����� ���������� ������� �� 100 � / ����� ��������: ";
            int calories = getUserChoice(0, 100000);

            Ingredient updatedIngredient(id, unit, name, protein, fat, carbs, calories);

            system("cls");
            if (manager.updateIngredient(id, updatedIngredient)) {
                std::cout << "���������� ������� �������!" << std::endl;
            }
            else {
                std::cout << "���������� � ����� ID �� ������!" << std::endl;
            }
            break;
        }
        case 4:
            if (!manager.canChange()) { break; }
            system("cls");
            manager.displayAllIngredients();
            break;
        case 5:
            return;
        default:
            std::cout << "�������� �����. ���������� �����." << std::endl;
        }
    }
}