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
    // �����������
    MenuItem(int id, const std::string& name)
        : id(id), name(name) {}

    // �������
    int getId() const { return id; }
    std::string getName() const { return name; }
    std::vector<std::pair<Ingredient*, double>> getComposition() const { return composition; }

    // �������
    void setName(const std::string& newName) { name = newName; }

    // ������ ��� ���������� ��������
    void addIngredient(Ingredient* ingredient, double grams);
    void removeIngredient(Ingredient* ingredient);
    void updateIngredient(Ingredient* ingredient, double newGrams);

    // ������ ������� ��������
    double calculateProtein();
    double calculateFat();
    double calculateCarbs();
    double calculateCalories();

    // ����� ���������� � ������� ����
    void printInfo();
};

class MenuManager {
private:
    std::vector<MenuItem*> menuItems;
    IngredientsManager* ingredientsManager;

public:
    MenuManager(IngredientsManager* manager)
        : ingredientsManager(manager) {}

    // ���������� ����� ������� ����
    void addNewPosition();

    // �������� ������� ����
    void deletePosition();

    // �������� ���� ������� ����
    void viewAllMenuPositions();
};

#endif // MENU_MANAGER_H
