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
    // ���������, ��� �� ������� � ������ �������� �������
    return !str.empty() && std::all_of(str.begin(), str.end(), ::isdigit);
}

void Kitchen::printOrderCard(Order& order) {

    if (order.getStatus() != "������") {
        return;
    }

    // ������� ����� �����
    for (int i = 0; i < cardWidth; ++i) {
        std::cout << "-";
    }
    std::cout << "\n";

    // ���������� ������ ��������
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

    // ������ ����� �����
    for (int i = 0; i < cardWidth; ++i) {
        std::cout << "-";
    }
    std::cout << "\n\n";
}

bool Kitchen::displayOrders(int pageNumber) {
    std::vector<Order>& allOrders = m_orderManager.getOrders();

    // ������� ����� ������, ���������� ������ �� ������ �� �������� "������"
    std::vector<Order*> orders;
    for (auto& order : allOrders) {
        if (order.getStatus() == "������") {
            orders.push_back(&order);
        }
    }

    if (orders.size() == 0) {
        system("cls");
        std::cout << "����� ������� ���.\n\n";
        std::cout << "������� q ��� ������: ";
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

        // �������� ����� ����� ������� ����� ��������
        system("cls");

        // ����� ��������
        for (int i = startIndex; i < endIndex && i < static_cast<int>(orders.size()); ++i) {
            printOrderCard(*orders[i]);
        }


        // ���� �������� ������, �������� �������
        if (endIndex < static_cast<int>(orders.size())) {
            std::cout << "��� " << orders.size() - endIndex << " ������� (N)";
            std::cout << " ->\n";
        }

        // ���� ���� ���������� ��������, �������� ��������� ��� �������� �� ���������� ��������
        if (pageNumber > 1) {
            std::cout << "<- ";
            std::cout << "��� " << (startIndex - perPage + 3) << " ������� (P)\n";
        }

        std::cout << "\n��� ������ ������� q";
        std::cout << "\n������� ID ������ ��� ������� �������������: ";
        std::string input = getUserString();
        // std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if (isValidInt(input)) {
            int orderId = stoi(input);
            // ����� ����� �� ID
            for (auto& order : allOrders) {
                if (order.getId() == orderId) {
                    if (order.getStatus() == "������") {
                        m_workerManager.displayAllWorkers();
                        std::cout << "�������� ����������, �������������� �����: ";
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
                        order.setStatus("�����������");
                        break;
                    }
                    else {
                        std::cout << "����� ��� �����������\n";
                    }
                }
            }
            std::cout << "������ � ����� ������� ���. ���������� ��� ���.\n";

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
                if (!displayOrders(1)) { return false; } // ������� �� ������ ��������
            }
        }
        else if (input == "P" || input == "p") {
            if (pageNumber > 1) {
                if (!displayOrders(pageNumber - 1)) { return false; }
            }
            else {
                if (!displayOrders(totalPages)) { return false; }
                // displayOrders(totalPages); // ������� �� ��������� ��������
            }
        }
        // displayOrders(1);
        if (!displayOrders(1)) { return false; } // ������� �� ������ ��������
    }
}


