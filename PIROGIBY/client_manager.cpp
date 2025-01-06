#include "client_manager.h"

void ClientManager::addClient(int id, const std::string& name, const std::string& phoneNumber, double discount) {
    clients.emplace_back(Client{ id, name, phoneNumber, discount });
    system("cls");
    std::cout << "������ ������� ��������!\n";
}

void ClientManager::deleteClient(int id) {
    system("cls");
    int index = findClientById(id);
    if (index != -1) {
        clients.erase(clients.begin() + index);
        std::cout << "������ ������.\n";
    }
    else {
        std::cout << "������� � ����� ID �� ����������.\n";
    }
}

void ClientManager::blacklistClient(int id) {
    int index = findClientById(id);
    system("cls");
    if (index != -1 && !clients[index].isInBlacklist()) {
        clients[index].addToBlacklist();
        std::cout << "������ �������� � ������ ������.\n";
    }
    else {
        std::cout << "������ ��� ��������� � ������ ������ ��� �� ������.\n";
    }
}

void ClientManager::unblacklistClient(int id) {
    int index = findClientById(id);
    system("cls");
    if (index != -1 && clients[index].isInBlacklist()) {
        clients[index].removeFromBlacklist();
        std::cout << "������ ����� �� ������� ������.\n";
    }
    else {
        std::cout << "������ ����������� � ������ ������ ��� �� ������.\n";
    }
}

void ClientManager::updateClientInfo(int id, const std::string& newName, const std::string& newPhoneNumber, double newDiscount) {
    int index = findClientById(id);
    system("cls");
    if (index != -1) {
        clients[index].setName(newName);
        clients[index].setPhoneNumber(newPhoneNumber);
        clients[index].setDiscount(newDiscount);
        std::cout << "���������� � ������� ���������.\n";
    }
    else {
        std::cout << "������� � ����� ID �� ����������.\n";
    }
}

void ClientManager::viewClientInfo(int id) {
    int index = findClientById(id);
    system("cls");
    if (index != -1) {
        printClient(clients[index]);
    }
    else {
        std::cout << "������� � ����� ID �� ����������.\n";
    }
}

void ClientManager::listAllClients() {
    system("cls");
    std::cout << "�������:" << std::endl;
    if (clients.empty()) {
        std::cout << "��� ��������." << std::endl;
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
        std::cout << "�������� ����������. ������ �������� ����.\n";
        return false;
    }
    else {
        return true;
    }
}

void ClientManager::printClient(Client client) {
    std::cout << "ID: " << client.getId();
    std::cout << ", ���: " << client.getName();
    std::cout << ", �������: " << client.getPhoneNumber();
    std::cout << ", ������: " << client.getDiscount();
    std::cout << ", ����� �������: " << client.getSum();
    std::cout << ", ����� ��������: " << client.getCalories();
    std::cout << "\n";
}

int clientsMenu(ClientManager& manager) {
    system("cls");
    while (true) {
        std::cout << "--- ���� �������� ---\n";
        std::cout << "1. �������� �������\n";
        std::cout << "2. ������� ���������� � �������\n";
        std::cout << "3. �������� ������� � ������ ������\n";
        std::cout << "4. ������ ������� �� ������� ������\n";
        std::cout << "5. �������� ���������� � �������\n";
        std::cout << "6. ���������� ���������� � �������\n";
        std::cout << "7. �������� ���� ��������\n";
        std::cout << "8. ��������� � ������� ����\n";
        std::cout << "�������� �����: ";

        int choice = getUserChoice(1, 8);
        int id;

        switch (choice) {
        case 1: {
            // ������� ����������� ��������� ID
            id = findNextAvailableId(manager.getClients());

            std::cout << "������� ��� �������: ";
            std::string name = getUserString();

            std::cout << "������� ����� �������� �������: ";
            std::string phoneNumber = getUserString();

            std::cout << "������� ������ ������� (%): ";
            int discount = getUserChoice(0, 100);

            manager.addClient(id, name, phoneNumber, discount);
            break;
        }
        case 2: {
            if (!manager.canChange()) { break; }

            std::cout << "������� ID ������� ��� ��������: ";
            id = getId(manager.getClients());

            manager.deleteClient(id);
            break;
        }
        case 3: {
            if (!manager.canChange()) { break; }

            std::cout << "������� ID ������� ��� ���������� � ������ ������: ";
            id = getId(manager.getClients());
            manager.blacklistClient(id);
            break;
        case 4: {
            if (!manager.canChange()) { break; }

            std::cout << "������� ID ������� ��� �������� �� ������� ������: ";
            id = getId(manager.getClients());

            manager.unblacklistClient(id);
            break;
        }
        case 5: {
            if (!manager.canChange()) { break; }

            std::cout << "������� ID ������� ��� ��������� ����������: ";
            id = getId(manager.getClients());

            std::cout << "������� ����� ��� �������: ";
            std::string newName = getUserString();

            std::cout << "������� ����� ����� �������� �������: ";
            std::string newPhoneNumber = getUserString();

            std::cout << "������� ����� ������ ������� (%): ";
            int newDiscount = getUserChoice(0, 100);

            manager.updateClientInfo(id, newName, newPhoneNumber, newDiscount);
            break;
        }
        case 6: {
            if (!manager.canChange()) { break; }

            std::cout << "������� ID ������� ��� ��������� ����������: ";
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
            std::cout << "������������ �����. ���������� �����.\n";
        }
        }
    }
}