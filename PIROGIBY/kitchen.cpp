#include "kitchen.h"
#include <iostream>
#include <vector>
#include <ctime>
#include <iomanip>
#include <string>

void kitchenMenu(Kitchen kitchen) {
    while (true) {
        system("cls");

        if (!kitchen.displayOrders(1)) { return; }
        // if (!kitchen.processInput()) { return; }

    }
}

bool Kitchen::isValidInt(const std::string& str) {
    // Проверяем, все ли символы в строке являются цифрами
    return !str.empty() && std::all_of(str.begin(), str.end(), ::isdigit);
}

void Kitchen::printOrderCard(Order& order) {

    if (order.getStatus() != "Принят") {
        return;
    }

    // Верхняя часть рамки
    for (int i = 0; i < cardWidth; ++i) {
        std::cout << "-";
    }
    std::cout << "\n";

    // Информация внутри карточки
    std::cout << " ID: " << order.getId() << "\n";

    char timeBuffer[20];
    strftime(timeBuffer, sizeof(timeBuffer), "%Y-%m-%d %H:%M:%S", &order.getReceivedTime());
    std::cout << " Accepted Time: " << timeBuffer << "\n";

    if (!order.getItems().empty()) {
        std::cout << " Items:" << "\n";
        for (auto item : order.getItems()) {
            std::cout << "   - " << item->getName() << "\n";
        }
    }
    else {
        std::cout << " No Items" << "\n";
    }

    // Нижняя часть рамки
    for (int i = 0; i < cardWidth; ++i) {
        std::cout << "-";
    }
    std::cout << "\n\n";
}

bool Kitchen::displayOrders(int pageNumber) {
    std::vector<Order>& allOrders = m_orderManager.getOrders();

    // Создаем новый вектор, содержащий ссылки на заказы со статусом "Принят"
    std::vector<Order*> orders;
    for (auto& order : allOrders) {
        if (order.getStatus() == "Принят") {
            orders.push_back(&order);
        }
    }

    if (orders.size() == 0) {
        system("cls");
        std::cout << "Новых заказов нет.\n\n";
        std::cout << "Введите q для выхода: ";
        std::string input;
        std::getline(std::cin, input);
        if (input == "q") {
            return false;
        }
    }
    else {
        const int totalPages = (orders.size() + perPage - 1) / perPage;
        int startIndex = (pageNumber - 1) * perPage;
        int endIndex = std::min(startIndex + perPage, static_cast<int>(orders.size()));

        // Очистить экран перед выводом новой страницы
        system("cls");

        // Вывод карточек
        for (int i = startIndex; i < endIndex && i < static_cast<int>(orders.size()); ++i) {
            printOrderCard(*orders[i]);
        }


        // Если остались заказы, добавить стрелку
        if (endIndex < static_cast<int>(orders.size())) {
            std::cout << "Еще " << orders.size() - endIndex << " заказов (N)";
            std::cout << " ->\n";
        }

        // Если есть предыдущие страницы, добавить стрелочку для возврата на предыдущую страницу
        if (pageNumber > 1) {
            std::cout << "<- ";
            std::cout << "Еще " << (startIndex - perPage + 3) << " заказов (P)\n";
        }

        std::cout << "\nДля выхода введите q";
        std::cout << "\nВведите ID заказа для отметки приготовления: ";
        std::string input = getUserString();
        // std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if (isValidInt(input)) {
            int orderId = stoi(input);
            // Найти заказ по ID
            for (auto& order : allOrders) {
                if (order.getId() == orderId) {
                    if (order.getStatus() == "Принят") {
                        m_workerManager.displayAllWorkers();
                        std::cout << "Выберите сотрудника, приготовившего заказ: ";
                        int choice = getId(m_workerManager.getWorkers());
                        Worker* cookedWorker = m_workerManager.findWorkerById(choice);
                        order.setPreparedBy(cookedWorker);
                        cookedWorker->addCookedOrders(1);

                        std::time_t now = std::time(nullptr);
                        struct tm timeInfo;
#ifdef _MSC_VER
                        errno_t err = localtime_s(&timeInfo, &now);
#else
                        localtime_r(&now, &timeInfo);
#endif
                        order.setCookedTime(timeInfo);
                        order.setStatus("Приготовлен");
                        break;
                    }
                    else {
                        std::cout << "Заказ уже приготовлен\n";
                    }
                }
            }
            std::cout << "Заказа с таким номером нет. Попробуйте еще раз.\n";

        }
        else if (input == "Q" || input == "q") {
            return false;
        }
        else if (input == "N" || input == "n") {
            if (pageNumber < totalPages) {
                if (!displayOrders(pageNumber + 1)) { return false; }
                // displayOrders(pageNumber + 1);
            }
            else {
                if (!displayOrders(1)) { return false; } // Переход на первую страницу
            }
        }
        else if (input == "P" || input == "p") {
            if (pageNumber > 1) {
                if (!displayOrders(pageNumber - 1)) { return false; }
            }
            else {
                if (!displayOrders(totalPages)) { return false; }
                // displayOrders(totalPages); // Переход на последнюю страницу
            }
        }
        // displayOrders(1);
        if (!displayOrders(1)) { return false; } // Переход на первую страницу
    }
}


