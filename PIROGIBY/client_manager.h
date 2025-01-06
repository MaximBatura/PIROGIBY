#ifndef CLIENT_MANAGER_H
#define CLIENT_MANAGER_H

#include <string>
#include <vector>
#include "order_manager.h"

class Client : public BaseClass {
private:
    std::string name;
    std::string phoneNumber;
    int discount;
    bool isBlacklisted = false;
    int sum = 0;
    int sumOfCalories = 0;

public:
    // Конструктор
    Client(int id, const std::string& name, const std::string& phoneNumber, double discount)
        : BaseClass(id), name(name), phoneNumber(phoneNumber), discount(discount) {}

    // Геттеры и сеттеры
    std::string getName() const { return name; }
    void setName(const std::string& newName) { name = newName; }

    std::string getPhoneNumber() const { return phoneNumber; }
    void setPhoneNumber(const std::string& newPhoneNumber) { phoneNumber = newPhoneNumber; }

    double getDiscount() const { return discount; }
    void setDiscount(double newDiscount) { discount = newDiscount; }

    bool isInBlacklist() const { return isBlacklisted; }
    void addToBlacklist() { isBlacklisted = true; }
    void removeFromBlacklist() { isBlacklisted = false; }

    void addSum(int addingPart) { sum += addingPart; }
    void addCalories(int addingPart) { sumOfCalories += addingPart; }

    int getSum() { return sum; }
    int getCalories() { return sumOfCalories; }

    friend std::ostream& operator<<(std::ostream& os, const Client& client);
};

class ClientManager {
private:
    std::vector<Client> clients;

public:
    void addClient(int id, const std::string& name, const std::string& phoneNumber, double discount = 0.0);
    void deleteClient(int id);
    void blacklistClient(int id);
    void unblacklistClient(int id);
    void updateClientInfo(int id, const std::string& newName, const std::string& newPhoneNumber, double newDiscount);
    void viewClientInfo(int id);
    void listAllClients();
    bool canChange() const;
    void printClient(Client client);

    int findClientById(int id) const;
    Client* findClientByPhoneNumber(std::string phoneNumber);

    std::vector<Client> getClients() { return clients; }
};

#endif // CLIENT_MANAGER_H
