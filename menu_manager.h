#ifndef MENU_MANAGER_H
#define MENU_MANAGER_H

#include <string>
#include <vector>

class MenuItem {
private:
    int id;
    std::string name;
    std::vector<std::pair<Ingredient*, double>> composition;

public:
    // Конструктор
    MenuItem(int id, const std::string& name)
        : id(id), name(name) {}

    // Геттеры
    int getId() const { return id; }
    std::string getName() const { return name; }
    std::vector<std::pair<Ingredient*, double>> getComposition() const { return composition; }

    // Сеттеры
    void setName(const std::string& newName) { name = newName; }

    // Методы для управления составом
    void addIngredient(Ingredient* ingredient, double grams);
    void removeIngredient(Ingredient* ingredient);
    void updateIngredient(Ingredient* ingredient, double newGrams);

    // Расчет пищевой ценности
    double calculateProtein();
    double calculateFat();
    double calculateCarbs();
    double calculateCalories();

    // Вывод информации о позиции меню
    void printInfo();
};

class MenuManager {
private:
    std::vector<MenuItem*> menuItems;
    IngredientsManager* ingredientsManager;

public:
    MenuManager(IngredientsManager* manager)
        : ingredientsManager(manager) {}

    // Добавление новой позиции меню
    void addNewPosition();

    // Удаление позиции меню
    void deletePosition();

    // Просмотр всех позиций меню
    void viewAllMenuPositions();
};

#endif // MENU_MANAGER_H
