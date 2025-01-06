#ifndef INGREDIENTS_MANAGER_H
#define INGREDIENTS_MANAGER_H

#include <string>
#include <vector>

class Ingredient : public BaseClass {
public:
    Ingredient(int id, const std::string& name, std::string unit, int protein, int fat,
        int carbohydrates, int calories)
        : BaseClass(id), m_name(name), m_unit(unit), m_protein(protein), m_fat(fat),
        m_carbohydrates(carbohydrates), m_calories(calories) {}

    std::string getName() const { return m_name; }
    int getProtein() const { return m_protein; }
    int getFat() const { return m_fat; }
    int getCarbs() const { return m_carbohydrates; }
    int getCalories() const { return m_calories; }

    std::string getUnit() { return m_unit; }
    void setUnit(std::string newValue) { m_unit = newValue; }

    std::vector<Batch> getBatches() const { return batches; }
    std::vector<Batch>* getBatchesPtr() { return &batches; }
    void addBatch(Batch newBatch) { batches.push_back(newBatch); }

    void removeZeroQuantityBatches();

    // Определение оператора '<'
    bool operator<(const Ingredient& other) const {
        return id < other.id;
    }

private:
    std::string m_name;
    int m_protein;
    int m_fat;
    int m_carbohydrates;
    int m_calories;
    std::string m_unit;
    std::vector<Batch> batches;
};

class IngredientsManager {
public:
    void addIngredient(const Ingredient& ingredient);
    bool removeIngredient(int id);
    bool updateIngredient(int id, const Ingredient& updatedIngredient);
    void displayAllIngredients(std::vector<int> without_ids = {}) const;
    bool canChange() const;

    std::vector<Ingredient> getIngredients() const { return ingredients; }
    Ingredient getIngredientById(int id) const;
    Ingredient* getIngredientPtrById(int id);

private:
    std::vector<Ingredient> ingredients;
};

#endif // INGREDIENTS_MANAGER_H