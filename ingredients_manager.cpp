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

void IngredientsManager::displayAllIngredients() const {
    for (const auto& ingredient : ingredients) {
        std::cout << "ID: " << ingredient.getId()
            << ", Наименование: " << ingredient.getName()
            << ", Белки: " << ingredient.getProtein()
            << ", Жиры: " << ingredient.getFat()
            << ", Углеводы: " << ingredient.getCarbs()
            << ", Калории: " << ingredient.getCalories() << "\n";
    }
}
