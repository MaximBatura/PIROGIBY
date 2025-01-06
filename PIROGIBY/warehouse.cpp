#include "warehouse.h"

void warehouseMenu(IngredientsManager& ingredientsManager) {
    system("cls");
    while (true) {
        std::cout << "--- ���� ����� ������������ ---\n";
        std::cout << "1. ������� ��������\n";
        std::cout << "2. ��������, � ������� ����� ���������� ���� ��������\n";
        std::cout << "3. ������� �� ������\n";
        std::cout << "4. ��������� � ������� ����\n";
        std::cout << "�������� �����: ";

        int grams;
        int pricePerUnit;
        int choice = getUserChoice(1, 4);
        switch (choice) {
        case 1: {
            if (!ingredientsManager.canChange()) { return; }
            ingredientsManager.displayAllIngredients();
            std::cout << "������� id �����������, ����� �������� ��� �� �����: ";
            int id = getId(ingredientsManager.getIngredients());
            Ingredient* ingr = ingredientsManager.getIngredientPtrById(id);

            if (ingr->getUnit() == "�����") {
                std::cout << "������� ���������� (�����) ����������� ��� ���������� �� �����: ";
                grams = getUserChoice(0, 1000000);

                std::cout << "������� ���� �� ������� (�� �����): ";
                pricePerUnit = getUserChoice(0, 1000000);
            }
            else {
                std::cout << "������� ���������� (����) ����������� ��� ���������� �� �����: ";
                grams = getUserChoice(0, 1000000);

                std::cout << "������� ���� �� ������� (�� �����): ";
                pricePerUnit = getUserChoice(0, 1000000);
            }

            std::cout << "������� ���� �������� ����������� (��.��.����): ";
            std::string expirationDate = getUserString(10);
            while (true) {
                if (isValidDate(expirationDate)) {
                    break;
                }
                std::cout << "���������� ��� ��� (��.��.����, �������� 05.01.2025): ";
                expirationDate = getUserString(10);
            }

            Batch newBatch(expirationDate, grams, pricePerUnit);
            ingr->addBatch(newBatch);
               
            system("cls");
            std::cout << "�������� ���������\n";
            break;
        }
        case 2: {
            system("cls");
            for (auto ingredient : ingredientsManager.getIngredients()) {
                for (auto batch : ingredient.getBatches()) {
                    if (isDateMoreThanAWeekAway(batch.getExpirationDate())) {
                        if (ingredient.getUnit() == "�����") {
                            std::cout << "� �������� " << ingredient.getName() << " " << batch.getQuantity() << " ����� ����� (" << batch.getExpirationDate() << ") ����������.\n";
                        }
                        else {
                            std::cout << "� �������� " << ingredient.getName() << " " << batch.getQuantity() << " ���� ����� (" << batch.getExpirationDate() << ") ����������.\n";
                        }
                        
                    }
                }
            }
            break;
        }
        case 3: {
            system("cls");
            int stock;
            for (auto ingredient : ingredientsManager.getIngredients()) {
                stock = 0;
                for (auto batch : ingredient.getBatches()) {
                    stock += batch.getQuantity();
                }
                if (stock != 0) {
                    if (ingredient.getUnit() == "�����") {
                        std::cout << ingredient.getName() << ": " << stock << " �����\n";
                    }
                    else {
                        std::cout << ingredient.getName() << ": " << stock << " ����\n";
                    }
                }
            }
            break;
        }
        case 4:
            return;
        default:
            std::cout << "�������� �����. ���������� �����." << std::endl;
        }
    }
}