#include "warehouse.h"

void warehouseMenu(IngredientsManager& ingredientsManager) {
    system("cls");
    while (true) {
        std::cout << "--- Меню учета ингредиентов ---\n";
        std::cout << "1. Создать поставку\n";
        std::cout << "2. Продукты, у которых скоро закончится срок годности\n";
        std::cout << "3. Остатки на складе\n";
        std::cout << "4. Вернуться в главное меню\n";
        std::cout << "Выберите пункт: ";

        int grams;
        int pricePerUnit;
        int choice = getUserChoice(1, 4);
        switch (choice) {
        case 1: {
            if (!ingredientsManager.canChange()) { return; }
            ingredientsManager.displayAllIngredients();
            std::cout << "Введите id ингредиента, чтобы добавить его на склад: ";
            int id = getId(ingredientsManager.getIngredients());
            Ingredient* ingr = ingredientsManager.getIngredientPtrById(id);

            if (ingr->getUnit() == "грамм") {
                std::cout << "Введите количество (грамм) ингредиента для добавления на склад: ";
                grams = getUserChoice(0, 1000000);

                std::cout << "Введите цену за единицу (за грамм): ";
                pricePerUnit = getUserChoice(0, 1000000);
            }
            else {
                std::cout << "Введите количество (штук) ингредиента для добавления на склад: ";
                grams = getUserChoice(0, 1000000);

                std::cout << "Введите цену за единицу (за штуку): ";
                pricePerUnit = getUserChoice(0, 1000000);
            }

            std::cout << "Введите срок годности ингредиента (ДД.ММ.ГГГГ): ";
            std::string expirationDate = getUserString(10);
            while (true) {
                if (isValidDate(expirationDate)) {
                    break;
                }
                std::cout << "Попробуйте еще раз (ДД.ММ.ГГГГ, например 05.01.2025): ";
                expirationDate = getUserString(10);
            }

            Batch newBatch(expirationDate, grams, pricePerUnit);
            ingr->addBatch(newBatch);
               
            system("cls");
            std::cout << "Поставка оформлена\n";
            break;
        }
        case 2: {
            system("cls");
            for (auto ingredient : ingredientsManager.getIngredients()) {
                for (auto batch : ingredient.getBatches()) {
                    if (isDateMoreThanAWeekAway(batch.getExpirationDate())) {
                        if (ingredient.getUnit() == "грамм") {
                            std::cout << "У продукта " << ingredient.getName() << " " << batch.getQuantity() << " грамм скоро (" << batch.getExpirationDate() << ") испортятся.\n";
                        }
                        else {
                            std::cout << "У продукта " << ingredient.getName() << " " << batch.getQuantity() << " штук скоро (" << batch.getExpirationDate() << ") испортятся.\n";
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
                    if (ingredient.getUnit() == "грамм") {
                        std::cout << ingredient.getName() << ": " << stock << " грамм\n";
                    }
                    else {
                        std::cout << ingredient.getName() << ": " << stock << " штук\n";
                    }
                }
            }
            break;
        }
        case 4:
            return;
        default:
            std::cout << "Неверный выбор. Попробуйте снова." << std::endl;
        }
    }
}