#include <iostream>
#include <iomanip>
#include <limits>
#include <ctime>

#include "order_manager.h"
#include "worker_manager.h"
#include "menu_manager.h"

// �����������
OrderManager::OrderManager(WorkersManager* workersManager, MenuManager* menuManager, ClientManager* clientManager, IngredientsManager* ingredientsManager)
    : workersManager(workersManager), menuManager(menuManager), clientManager(clientManager), ingredientsManager(ingredientsManager) {}


// ������� ������� ��� ������ �������
std::ostream& operator<<(std::ostream& os, const std::tm& t) {
    char buffer[20];
    strftime(buffer, sizeof(buffer), "%d-%m-%Y %H:%M:%S", &t);
    return os << buffer;
}

void OrderManager::createNewOrder() {

    if (!canChange()) { return; }

    int id = findNextAvailableId(m_orders);

    std::time_t now = std::time(nullptr);
    struct tm timeInfo;
#ifdef _MSC_VER
    errno_t err = localtime_s(&timeInfo, &now);
#else
    localtime_r(&now, &timeInfo);
#endif

    std::cout << "������� ����� �������� ������� (������: 375291111111): ";
    std::string clientPhoneNumber = getUserString();
    Client* clientPtr = clientManager->findClientByPhoneNumber(clientPhoneNumber);
    if (clientPtr == nullptr) {
        std::cout << "������ �� ������ � ���� � ����� ������ ��� �����\n";
        
        // ���������� �������
        // ������� ����������� ��������� ID
        int id = findNextAvailableId(clientManager->getClients());

        std::cout << "������� ��� �������: ";
        std::string name = getUserString();

        std::cout << "������� ������ ������� (%): ";
        int discount = getUserChoice(0, 100);

        clientManager->addClient(id, name, clientPhoneNumber, discount);
        clientPtr = clientManager->findClientByPhoneNumber(clientPhoneNumber);
    }
    else {
        if (clientPtr->isInBlacklist()) {
            system("cls");
            std::cout << "������ � ������ ������. �������� ������ ��������.\n";
            return;
        }
    }

    workersManager->displayAllWorkers();
    std::cout << "������ ������ � ������ " << clientPtr->getName() << ".\n";
    std::cout << "������� id ���������, ���������� �����: ";
    int receivedByIndex = getId(workersManager->getWorkers());
    Worker* receivedBy = workersManager->findWorkerById(receivedByIndex);

    // ����� ������� ����
    std::vector<const MenuItem*> items;

    bool cancel = false;
    while (!cancel) {
        menuManager->viewAllMenuPositions();

        bool choosen = false;
        while (!choosen) {
            std::cout << "������� id ������� ���� ��� 'q' ��� ������: ";
            std::string input;
            std::getline(std::cin, input);

            if (input.length() > MAX_STRING_LENGTH) {
                std::cout << "������������ ����� ������ - " << MAX_STRING_LENGTH << " ��������. ��������� �������.\n";
                continue;
            }

            if (input == "q" or input == "Q") {
                choosen = true;
                cancel = true;
            }
            else if (isNumber(input)) {
                int index = stringToInt(input);
                if (checkId(menuManager->getMenuItems(), index)) {
                    items.push_back(menuManager->findMenuItemById(index));
                }
                else {
                    std::cout << "��� ������ id. ���������� ��� ���.\n";
                    continue;
                }
            }
            else {
                std::cout << "������������ ����. ���������� �����.\n";
                continue;
            }
        }
    }

    /*for (auto item : items) {
        for (auto pair : item->getComposition()) {
            int a = pair.second;
            for (auto ingredient : ingredientsManager->getIngredients()) {
                if (pair.first.get()->getId() == ingredient.getId()) {
                    Ingredient* selectedIng = ingredientsManager->getIngredientPtrById(ingredient.getId());
                    std::vector<Batch>* selectedBatchesPtr = selectedIng->getBatchesPtr();
                    

                    while (a != 0) {
                        if (selectedBatchesPtr->size() == 0) {
                            system("cls");
                            std::cout << "� ��� ������������ ������������ �� ������ ��� ������� ������\n";
                            return;
                        }
                        Batch* btch = findNearestExpirationDate(selectedBatchesPtr);
                        if (btch->getQuantity() <= a) {
                            btch->substractQuantity(btch->getQuantity());
                            a -= btch->getQuantity();
                        }
                        else {
                            btch->substractQuantity(a);
                            a = 0;
                        }
                        ingredient.removeZeroQuantityBatches();
                    }                                    
                    
                    break;
                }
            }
        }
    }*/
    for (auto item : items) {
        for (auto pair : item->getComposition()) {
            int a = pair.second;
            for (auto ingredient : ingredientsManager->getIngredients()) {
                if (pair.first.get()->getId() == ingredient.getId()) {
                    Ingredient* selectedIng = ingredientsManager->getIngredientPtrById(ingredient.getId());
                    std::vector<Batch>* selectedBatchesPtr = selectedIng->getBatchesPtr();

                    //while (a != 0 && !selectedBatchesPtr->empty()) {
                    //    Batch* btch = findNearestExpirationDate(selectedBatchesPtr);

                    //    if (btch->getQuantity() >= a) {
                    //        btch->substractQuantity(a);
                    //        a = 0;
                    //    }
                    //    else {
                    //        a -= btch->getQuantity();  // ��������� ���������� ����������
                    //        btch->substractQuantity(btch->getQuantity());  // ��������� ���������� ������� ������
                    //        selectedIng->removeZeroQuantityBatches();  // ������� �������������� ������
                    //    }
                    //}
                    while (a != 0 && !selectedBatchesPtr->empty()) {
                        Batch* btch = findNearestExpirationDate(selectedBatchesPtr);

                        if (btch->getQuantity() >= a) {
                            btch->substractQuantity(a);
                            a = 0;
                        }
                        else {
                            a -= btch->getQuantity();  // ��������� ���������� ����������
                            btch->substractQuantity(btch->getQuantity());  // ��������� ���������� ������� ������
                            selectedIng->removeZeroQuantityBatches();  // ������� �������������� ������
                        }
                    }

                    if (a != 0) {
                        system("cls");
                        std::cout << "� ��� ������������ ������������ �� ������ ��� ������� ������\n";
                        return;
                    }

                    break;
                }
            }
        }
    }

    // �������� ������ ������
    Order newOrder(id, timeInfo, clientPhoneNumber);
    newOrder.setReceivedBy(receivedBy);
    for (auto item : items) {

        if (item->self_calories == 0) {
            newOrder.addCalories(item->calculateCalories());
            clientPtr->addCalories(item->calculateCalories());
        }
        else {
            newOrder.addCalories(item->self_calories);
            clientPtr->addCalories(item->self_calories);
        }
        newOrder.addItem(item);
        newOrder.addAmountWithoutSale(item->getCost());
        newOrder.addAmountWithSale(item->getCost() - item->getCost() * clientPtr->getDiscount() * 0.01);
        clientPtr->addSum(item->getCost() - item->getCost() * clientPtr->getDiscount() * 0.01);
    }
    newOrder.setDiscount(clientPtr->getDiscount());
    m_orders.push_back(newOrder);
    system("cls");
    receivedBy->addAcceptedOrders(1);
    std::cout << "����� ����� ������ �������!" << std::endl;
}

void OrderManager::deleteOrder() {
    // system("cls");
    if (!canChange()) { return; }
    std::cout << "������� ID ������ ��� ��������: ";
    int orderId = getId(m_orders);

    for (auto it = m_orders.begin(); it != m_orders.end(); ++it) {
        if (it->getId() == orderId) {
            m_orders.erase(it);
            std::cout << "����� ������." << std::endl;
            return;
        }
    }
}

void OrderManager::updateOrder() {
    if (!canChange()) { return; }
    std::cout << "������� ID ������ ��� ���������: ";
    int orderId = getId(m_orders);
    for (auto& order : m_orders) {
        if (order.getId() == orderId) {
            std::cout << "��������� ���������� � ������..." << std::endl;

            std::cout << "������� ��� �������: ";
            std::string client = getUserString();
            order.setClient(client);

            // ����� ���������, ���������� �����
            workersManager->displayAllWorkers();
            std::cout << "������� id ���������, ���������� �����: ";
            int receivedByIndex = getId(workersManager->getWorkers());
            const Worker* receivedBy = workersManager->findWorkerById(receivedByIndex);
            order.setReceivedBy(receivedBy);

            if (order.getStatus() == "�����������") {
                // ����� ���������, �������������� �����
                workersManager->displayAllWorkers();
                std::cout << "������� ����� ���������, �������������� �����:: ";
                int preparedByIndex = getId(workersManager->getWorkers());
                const Worker* preparedBy = workersManager->findWorkerById(preparedByIndex);
                order.setPreparedBy(preparedBy);
            }

            // ����� ������� ����
            std::vector<const MenuItem*> items;
            bool cancel = false;
            while (!cancel) {
                bool getChoice = false;
                menuManager->viewAllMenuPositions();

                while (!getChoice) {

                    std::cout << "������� id ������� ���� ��� 'q' ��� ������: ";
                    std::string input = getUserString();

                    if (input == "q" or input == "Q") {
                        cancel = true;
                        getChoice = true;
                    }
                    else if (isdigit(input[0])) {
                        int itemIndex = input[0] - '0';

                        if (checkId(menuManager->getMenuItems(), itemIndex)) {
                            items.push_back(menuManager->findMenuItemById(itemIndex));
                        }
                        else {
                            std::cout << "������� ���� � ����� ID �� �������. ���������� �����.\n";
                            continue;
                        }

                    }
                    else {
                        clearInputBuffer();
                        std::cout << "������������ ����. ���������� �����\n";
                        continue;
                    }
                }
            }
            order.setItems(items);
            system("cls");
            std::cout << "���������� � ������ ���������." << std::endl;
            return;
        }
    }
}

void OrderManager::findOrderByAttribute() {
    if (!canChange()) { return; }
    std::cout << "����� ������ �� ��������..." << std::endl;

    // ����� �� ��������� ����������:
    // ID, �������, ����������, ������� ����

    std::cout << "�������� ������� ������ (1-id, 2-������, 3-��������, 4-������� ����): ";
    int atr_id = getUserChoice(1, 4);

    switch (atr_id) {
    case 1:
    {
        // ����� �� ID

        std::cout << "������� id: ";
        int search_id = getId(m_orders);
        // ������� ������ ����� ����� ����� �����
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        for (const auto& order : m_orders) {
            if (order.getId() == search_id) {
                system("cls");
                std::cout << "����� ������:" << std::endl;
                printOrder(order);
                return;
            }
        }
        system("cls");
        std::cout << "����� �� ������." << std::endl;
        break;
    }
    case 2: // ����� �� �������
    {
        std::cout << "������� ��� �������: ";
        std::string client = getUserString();

        for (const auto& order : m_orders) {
            if (order.getClient() == client) {
                system("cls");
                std::cout << "����� ������:" << std::endl;
                printOrder(order);
                return;
            }
        }
        system("cls");
        std::cout << "����� �� ������." << std::endl;
        break;
    }
    case 3: // ����� �� ���������, ���������� ��� �������������� �����
    {
        std::cout << "������� ��� ���������: ";
        std::string worker = getUserString();

        for (const auto& order : m_orders) {
            if (order.getReceivedBy()->getName() == worker or order.getPreparedBy()->getName() == worker) {
                system("cls");
                std::cout << "����� ������:" << std::endl;
                printOrder(order);
                return;
            }
        }
        system("cls");
        std::cout << "����� �� ������." << std::endl;
        break;
    }
    case 4: // ����� �� ������� ����
    {
        std::cout << "������� ������������ �������: ";
        std::string menuPosition = getUserString();

        for (const auto& order : m_orders) {
            bool found = false;
            for (const auto& item : order.getItems()) {
                if (item->getName() == menuPosition) {
                    found = true;
                    break;
                }
            }
            if (found) {
                system("cls");
                std::cout << "����� ������:" << std::endl;
                printOrder(order);
                return;
            }
        }
        break;
    }
    default:
        std::cerr << "�������� ��� ��������!" << std::endl;
        break;
    }
}

Order OrderManager::findOrderById(int id)
{
    for (auto order : m_orders) {
        if (order.getId() == id) {
            return order;
        }
    }
    std::cout << "����� �� ������." << std::endl;
}

void OrderManager::viewOrdersHistory() {
    system("cls");
    std::cout << "������� �������:" << std::endl;
    if (m_orders.empty()) {
        std::cout << "��� �������� �������." << std::endl;
        return;
    }
    for (auto order : m_orders) { printOrder(order); }
}

void OrderManager::printOrder(Order order) const {
    std::cout << "ID: " << order.getId();
    std::cout << ", ������: " << order.getStatus();
    std::cout << ", ������: " << order.getClient();
    std::cout << ", ����� ������ �� ������: " << order.getAmountWithoutSale();
    std::cout << ", ������: " << order.getDiscount();
    std::cout << ", ����� ������ ����� ������: " << order.getAmountWithSale();
    std::cout << ", ������ �����: " << order.getReceivedBy()->getName();
    std::cout << ", ����� ��������: " << order.getReceivedTime();

    if (order.getStatus() == "�����������") {
        std::cout << ", ����������: " << order.getPreparedBy()->getName()
            << ", ����� �������������: " << order.getCookedTime();
    }
    std::cout << ", ����� ��������: " << order.getCalories();
    std::cout << ", ������� ����: ";
    for (auto item : order.getItems()) {
        std::cout << item->getName() << ", ";
    }
    std::cout << "\n";
}

bool OrderManager::canChange()
{
    if (menuManager->getMenuSize() == 0) {
        system("cls");
        std::cout << "�������� ����������, ��� ��� � ��� ��� �� ����� ������� � ����.\n";
        return false;
    }
    else if (workersManager->getWorkersSize() == 0) {
        system("cls");
        std::cout << "�������� ����������, ��� ��� � ��� ��� �� ������ ���������.\n";
        return false;
    }
    return true;
}

int ordersMenu(OrderManager& manager) {
    system("cls");
    while (true) {
        std::cout << "--- ���� ���������� �������� ---\n";
        std::cout << "1. ������� ����� �����\n";
        std::cout << "2. ������� ��������� �����\n";
        std::cout << "3. �������� ���������� ������\n";
        std::cout << "4. ����� ����� �� ��������\n";
        std::cout << "5. ���������� ������� �������\n";
        std::cout << "6. ��������� � ������� ����\n";

        std::cout << "�������� �����: ";
        int choice = getUserChoice(1, 6);

        switch (choice) {
        case 1:
            manager.createNewOrder();
            break;
        case 2:
            manager.deleteOrder();
            break;
        case 3:
            manager.updateOrder();
            break;
        case 4:
            manager.findOrderByAttribute();
            break;
        case 5:
            manager.viewOrdersHistory();
            break;
        case 6:
            return 0;
        default:
            std::cout << "�������� ����. ���������� ��� ���." << std::endl;
            break;
        }
    }

    return 0;
}

