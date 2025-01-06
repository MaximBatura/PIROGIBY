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
        std::cout << "Ингредиентов больше нет\n";
        return;
    }
    for (const auto& ingredient : ingredients) {
        auto it = std::find(without_ids.begin(), without_ids.end(), ingredient.getId());

        // Проверяем, нашли ли мы число
        if (it != without_ids.end()) {
            continue;
        }
        else {
            std::cout << "ID: " << ingredient.getId()
                << ", Наименование: " << ingredient.getName()
                << ", Белки: " << ingredient.getProtein()
                << ", Жиры: " << ingredient.getFat()
                << ", Углеводы: " << ingredient.getCarbs()
                << ", Калории: " << ingredient.getCalories() << "\n";
        } 
    }
}

bool IngredientsManager::canChange() const
{
    if (ingredients.size() == 0) {
        system("cls");
        std::cout << "У вас нет ни одного ингредиента в базе.\n";
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
    // Создаем временный вектор для хранения непустых партий
    std::vector<Batch> nonEmptyBatches;

    // Копируем все непустые партии в новый вектор
    for (auto& batch : batches) {
        if (batch.getQuantity() > 0) {
            nonEmptyBatches.push_back(batch);
        }
    }

    // Заменяем старый список новым списком без пустых партий
    batches.swap(nonEmptyBatches);
}

void ingredientsMenu(IngredientsManager& manager) {
    system("cls");
    while (true) {
        std::cout << "--- Справочник ингредиентов ---\n";
        std::cout << "1. Добавить ингредиент\n";
        std::cout << "2. Удалить ингредиент\n";
        std::cout << "3. Изменить ингредиент\n";
        std::cout << "4. Просмотреть все ингредиенты\n";
        std::cout << "5. Вернуться в главное меню\n";
        std::cout << "Выберите пункт: ";

        int choice = getUserChoice(0, 5);

        switch (choice) {
        case 1: {
            // Находим минимальный доступный ID
            int id = findNextAvailableId(manager.getIngredients());

            std::cout << "Укажите единицы измерения (грамм - 1, штук - 2): ";
            int choice = getUserChoice(1, 2);
            std::string unit;
            if (choice == 1) {
                unit = "грамм";
            }
            else {
                unit = "штук";
            }

            std::cout << "Введите наименование ингредиента: ";
            std::string name = getUserString();
            std::cout << "Введите содержание белков на 100 г / штуку продукта: ";
            int protein = getUserChoice(0, 100);
            std::cout << "Введите содержание жиров на 100 г / штуку продукта: ";
            int fat = getUserChoice(0, 100);
            std::cout << "Введите содержание углеводов на 100 г / штуку продукта: ";
            int carbs = getUserChoice(0, 100);
            std::cout << "Введите количество калорий на 100 г / штуку продукта: ";
            int calories = getUserChoice(0, 100000);

            Ingredient newIngredient(id, name, unit, protein, fat, carbs, calories);
            manager.addIngredient(newIngredient);
            system("cls");
            std::cout << "Ингредиент успешно добавлен!" << std::endl;
            break;
        }
        case 2: {
            if (!manager.canChange()) { break; }
            std::cout << "Введите ID ингредиента для удаления: ";
            int id = getId(manager.getIngredients());

            system("cls");
            if (manager.removeIngredient(id)) {
                std::cout << "Ингредиент удален!" << std::endl;
            }
            else {
                std::cout << "Ингредиент с таким ID не найден!" << std::endl;
            }
            break;
        }
        case 3: {
            if (!manager.canChange()) { break; }

            std::cout << "Введите ID ингредиента для изменения: ";
            int id = getId(manager.getIngredients());

            std::cout << "Введите новое наименование ингредиента: ";
            std::string name = getUserString();
            std::cout << "Укажите единицы измерения (грамм - 1, штук - 2): ";
            int choice = getUserChoice(1, 2);
            std::string unit;
            if (choice == 1) {
                unit = "грамм";
            }
            else {
                unit = "штук";
            }
            std::cout << "Введите новое содержание белков на 100 г / штуку продукта: ";
            int protein = getUserChoice(0, 100);
            std::cout << "Введите новое содержание жиров на 100 г / штуку продукта: ";
            int fat = getUserChoice(0, 100);
            std::cout << "Введите новое содержание углеводов на 100 г / штуку продукта: ";
            int carbs = getUserChoice(0, 100);
            std::cout << "Введите новое количество калорий на 100 г / штуку продукта: ";
            int calories = getUserChoice(0, 100000);

            Ingredient updatedIngredient(id, unit, name, protein, fat, carbs, calories);

            system("cls");
            if (manager.updateIngredient(id, updatedIngredient)) {
                std::cout << "Ингредиент успешно изменен!" << std::endl;
            }
            else {
                std::cout << "Ингредиент с таким ID не найден!" << std::endl;
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
            std::cout << "Неверный выбор. Попробуйте снова." << std::endl;
        }
    }
}