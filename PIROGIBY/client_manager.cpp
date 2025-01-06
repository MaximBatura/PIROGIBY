#include "client_manager.h"

void ClientManager::addClient(int id, const std::string& name, const std::string& phoneNumber, double discount) {
    clients.emplace_back(Client{ id, name, phoneNumber, discount });
    system("cls");
    std::cout << "Клиент успешно добавлен!\n";
}

void ClientManager::deleteClient(int id) {
    system("cls");
    int index = findClientById(id);
    if (index != -1) {
        clients.erase(clients.begin() + index);
        std::cout << "Клиент удален.\n";
    }
    else {
        std::cout << "Клиента с таким ID не существует.\n";
    }
}

void ClientManager::blacklistClient(int id) {
    int index = findClientById(id);
    system("cls");
    if (index != -1 && !clients[index].isInBlacklist()) {
        clients[index].addToBlacklist();
        std::cout << "Клиент добавлен в черный список.\n";
    }
    else {
        std::cout << "Клиент уже находится в черном списке или не найден.\n";
    }
}

void ClientManager::unblacklistClient(int id) {
    int index = findClientById(id);
    system("cls");
    if (index != -1 && clients[index].isInBlacklist()) {
        clients[index].removeFromBlacklist();
        std::cout << "Клиент убран из черного списка.\n";
    }
    else {
        std::cout << "Клиент отсутствует в черном списке или не найден.\n";
    }
}

void ClientManager::updateClientInfo(int id, const std::string& newName, const std::string& newPhoneNumber, double newDiscount) {
    int index = findClientById(id);
    system("cls");
    if (index != -1) {
        clients[index].setName(newName);
        clients[index].setPhoneNumber(newPhoneNumber);
        clients[index].setDiscount(newDiscount);
        std::cout << "Информация о клиенте обновлена.\n";
    }
    else {
        std::cout << "Клиента с таким ID не существует.\n";
    }
}

void ClientManager::viewClientInfo(int id) {
    int index = findClientById(id);
    system("cls");
    if (index != -1) {
        printClient(clients[index]);
    }
    else {
        std::cout << "Клиента с таким ID не существует.\n";
    }
}

void ClientManager::listAllClients() {
    system("cls");
    std::cout << "Клиенты:" << std::endl;
    if (clients.empty()) {
        std::cout << "Нет клиентов." << std::endl;
        return;
    }
    for (auto client : clients) { printClient(client); }
}

int ClientManager::findClientById(int id) const {
    for (size_t i = 0; i < clients.size(); ++i) {
        if (clients[i].getId() == id) {
            return static_cast<int>(i);
        }
    }
    return -1;
}

Client* ClientManager::findClientByPhoneNumber(std::string phoneNumber)
{
    for (size_t i = 0; i < clients.size(); ++i) {
        if (clients[i].getPhoneNumber() == phoneNumber) {
            return &clients[i];
        }
    }
    return nullptr;
}

bool ClientManager::canChange() const {
    if (clients.size() == 0) {
        system("cls");
        std::cout << "Действие недоступно. Список клиентов пуст.\n";
        return false;
    }
    else {
        return true;
    }
}

void ClientManager::printClient(Client client) {
    std::cout << "ID: " << client.getId();
    std::cout << ", Имя: " << client.getName();
    std::cout << ", Телефон: " << client.getPhoneNumber();
    std::cout << ", Скидка: " << client.getDiscount();
    std::cout << ", Сумма заказов: " << client.getSum();
    std::cout << ", Сумма ккалорий: " << client.getCalories();
    std::cout << "\n";
}

int clientsMenu(ClientManager& manager) {
    system("cls");
    while (true) {
        std::cout << "--- Меню клиентов ---\n";
        std::cout << "1. Добавить клиента\n";
        std::cout << "2. Удалить информацию о клиенте\n";
        std::cout << "3. Добавить клиента в черный список\n";
        std::cout << "4. Убрать клиента из черного списка\n";
        std::cout << "5. Изменить информацию о клиенте\n";
        std::cout << "6. Посмотреть информацию о клиенте\n";
        std::cout << "7. Просмотр всех клиентов\n";
        std::cout << "8. Вернуться в главное меню\n";
        std::cout << "Выберите пункт: ";

        int choice = getUserChoice(1, 8);
        int id;

        switch (choice) {
        case 1: {
            // Находим минимальный доступный ID
            id = findNextAvailableId(manager.getClients());

            std::cout << "Введите имя клиента: ";
            std::string name = getUserString();

            std::cout << "Введите номер телефона клиента: ";
            std::string phoneNumber = getUserString();

            std::cout << "Введите скидку клиента (%): ";
            int discount = getUserChoice(0, 100);

            manager.addClient(id, name, phoneNumber, discount);
            break;
        }
        case 2: {
            if (!manager.canChange()) { break; }

            std::cout << "Введите ID клиента для удаления: ";
            id = getId(manager.getClients());

            manager.deleteClient(id);
            break;
        }
        case 3: {
            if (!manager.canChange()) { break; }

            std::cout << "Введите ID клиента для добавления в черный список: ";
            id = getId(manager.getClients());
            manager.blacklistClient(id);
            break;
        case 4: {
            if (!manager.canChange()) { break; }

            std::cout << "Введите ID клиента для удаления из черного списка: ";
            id = getId(manager.getClients());

            manager.unblacklistClient(id);
            break;
        }
        case 5: {
            if (!manager.canChange()) { break; }

            std::cout << "Введите ID клиента для изменения информации: ";
            id = getId(manager.getClients());

            std::cout << "Введите новое имя клиента: ";
            std::string newName = getUserString();

            std::cout << "Введите новый номер телефона клиента: ";
            std::string newPhoneNumber = getUserString();

            std::cout << "Введите новую скидку клиента (%): ";
            int newDiscount = getUserChoice(0, 100);

            manager.updateClientInfo(id, newName, newPhoneNumber, newDiscount);
            break;
        }
        case 6: {
            if (!manager.canChange()) { break; }

            std::cout << "Введите ID клиента для просмотра информации: ";
            id = getId(manager.getClients());

            manager.viewClientInfo(id);
            break;
        }
        case 7:
            manager.listAllClients();
            break;
        case 8:
            return 0;
        default:
            std::cout << "Неправильный выбор. Попробуйте снова.\n";
        }
        }
    }
}