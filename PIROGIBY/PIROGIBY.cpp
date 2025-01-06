#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <stdexcept>

#include "warehouse.h"

#include "other.cpp"

#include "ingredients_manager.h"
#include "ingredients_manager.cpp"

#include "menu_manager.h"
#include "menu_manager.cpp"

#include "client_manager.h"
#include "client_manager.cpp"

#include "worker_manager.h"
#include "worker_manager.cpp"

#include "order_manager.h"
#include "order_manager.cpp"

#include "kitchen.h"
#include "kitchen.cpp"


#include "warehouse.cpp"

IngredientsManager ingredientsManager;
MenuManager menuManager(&ingredientsManager);
ClientManager clientManager;
WorkersManager workerManager;
OrderManager ordersManager(&workerManager, &menuManager, &clientManager, &ingredientsManager);
Kitchen kitchen(ordersManager, workerManager);


int main() {
    setlocale(LC_ALL, "RU");

    while (true) {
        system("cls");
        std::cout << "--- Главное меню ---" << std::endl;
        std::cout << "1. Справочник ингредиентов" << std::endl;
        std::cout << "2. Меню пироговой" << std::endl;
        std::cout << "3. Клиенты" << std::endl;
        std::cout << "4. Персонал" << std::endl;
        std::cout << "5. Заказы" << std::endl;
        std::cout << "6. Кухня" << std::endl;
        std::cout << "7. Склад" << std::endl;
        std::cout << "8. Выход" << std::endl;
        std::cout << "Выберите пункт: ";

        int choice = getUserChoice(1, 8);

        switch (choice) {
        case 1:
            ingredientsMenu(ingredientsManager);
            break;
        case 2:
            MenuMenu(menuManager);
            break;
        case 3:
            clientsMenu(clientManager);

            break;
        case 4:
            workerMenu(&workerManager);
            break;
        case 5:
            ordersMenu(ordersManager);
            break;
        case 6:
            kitchenMenu(kitchen);
            break;
        case 7:
            warehouseMenu(ingredientsManager);
            break;
        case 8:
            std::cout << "Завершение программы..." << std::endl;
            return 1;
        default:
            std::cout << "Неверный выбор. Попробуйте снова." << std::endl;
        }
    }

    return 0;
}


