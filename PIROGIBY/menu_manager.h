#ifndef MENU_MANAGER_H
#define MENU_MANAGER_H

#include <string>
#include <vector>

class MenuItem : public BaseClass {
private:
    std::string name;
    std::vector<std::pair<std::shared_ptr<Ingredient>, double>> composition;
    int cost;
    // int operator->() { return id; }

public:
    // Конструктор
    MenuItem(int id, const std::string& name)
        : BaseClass(id), name(name), cost(0) {}

    // Геттеры
    std::string getName() const { return name; }
    std::vector<std::pair<std::shared_ptr<Ingredient>, double>> getComposition() const { return composition; }
    int getCost() const { return cost; }

    // Сеттеры
    void setName(const std::string& newName) { name = newName; }
    void setCost(int newCost) { cost = newCost; }

    // Методы для управления составом
    void addIngredient(std::shared_ptr<Ingredient> ingredient, double grams);
    void removeIngredient(const std::shared_ptr<Ingredient>& ingredient);
    void updateIngredient(std::shared_ptr<Ingredient> ingredient, double newGrams);
    bool isIngredientInComposition(const Ingredient& ingredient);

    // Расчет пищевой ценности
    double calculateProtein() const;
    double calculateFat() const;
    double calculateCarbs() const;
    double calculateCalories() const;

    int self_protein = 0;
    int self_fat = 0;
    int self_carbs = 0;
    int self_calories = 0;
};

class MenuManager {
private:
    std::vector<MenuItem*> menuItems;
    IngredientsManager* ingredientsManager;

public:
    MenuManager(IngredientsManager* manager)
        : ingredientsManager(manager) {}

    int getMenuSize() { return menuItems.size(); }

    // Добавление новой позиции меню
    void addNewPosition();

    // Удаление позиции меню
    void deletePosition();

    // Просмотр всех позиций меню
    void viewAllMenuPositions();
    std::vector<MenuItem*> getMenuItems() { return menuItems; }

    // Поиск позиции меню по id
    MenuItem* findMenuItemById(int id) const;
};

#endif // MENU_MANAGER_H
