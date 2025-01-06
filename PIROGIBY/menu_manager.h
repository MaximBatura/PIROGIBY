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
    // �����������
    MenuItem(int id, const std::string& name)
        : BaseClass(id), name(name), cost(0) {}

    // �������
    std::string getName() const { return name; }
    std::vector<std::pair<std::shared_ptr<Ingredient>, double>> getComposition() const { return composition; }
    int getCost() const { return cost; }

    // �������
    void setName(const std::string& newName) { name = newName; }
    void setCost(int newCost) { cost = newCost; }

    // ������ ��� ���������� ��������
    void addIngredient(std::shared_ptr<Ingredient> ingredient, double grams);
    void removeIngredient(const std::shared_ptr<Ingredient>& ingredient);
    void updateIngredient(std::shared_ptr<Ingredient> ingredient, double newGrams);
    bool isIngredientInComposition(const Ingredient& ingredient);

    // ������ ������� ��������
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

    // ���������� ����� ������� ����
    void addNewPosition();

    // �������� ������� ����
    void deletePosition();

    // �������� ���� ������� ����
    void viewAllMenuPositions();
    std::vector<MenuItem*> getMenuItems() { return menuItems; }

    // ����� ������� ���� �� id
    MenuItem* findMenuItemById(int id) const;
};

#endif // MENU_MANAGER_H
