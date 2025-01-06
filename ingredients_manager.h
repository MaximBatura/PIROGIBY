#ifndef INGREDIENTS_MANAGER_H
#define INGREDIENTS_MANAGER_H

#include <string>
#include <vector>

class Ingredient {
public:
    Ingredient(int id, std::string name, double protein, double fat, double carbs, int calories)
        : m_id(id), m_name(name), m_protein(protein), m_fat(fat), m_carbs(carbs), m_calories(calories) {}

    // Геттеры
    int getId() const { return m_id; }
    std::string getName() const { return m_name; }
    double getProtein() const { return m_protein; }
    double getFat() const { return m_fat; }
    double getCarbs() const { return m_carbs; }
    int getCalories() const { return m_calories; }

private:
    int m_id;
    std::string m_name;
    double m_protein;
    double m_fat;
    double m_carbs;
    int m_calories;
};

class IngredientsManager {
public:
    void addIngredient(const Ingredient& ingredient);
    bool removeIngredient(int id);
    bool updateIngredient(int id, const Ingredient& updatedIngredient);
    void displayAllIngredients() const;

private:
    std::vector<Ingredient> ingredients;
};

#endif // INGREDIENTS_MANAGER_H