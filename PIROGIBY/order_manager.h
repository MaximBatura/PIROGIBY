#ifndef ORDER_MANAGER_H
#define ORDER_MANAGER_H

#include <string>
#include <vector>
#include <ctime>
#include "worker_manager.h"

class ClientManager;

class Order : public BaseClass {
public:
    // Конструктор
    Order(int id, const std::tm& receivedTime, const std::string& client)
        : BaseClass(id), m_client(client), m_receivedTime(receivedTime) {}

    // Геттеры и сеттеры
    const std::tm& getReceivedTime() const { return m_receivedTime; }
    void setReceivedTime(const std::tm& time) { m_receivedTime = time; }
    const std::string& getClient() const { return m_client; }
    void setClient(const std::string& client) { m_client = client; }
    const Worker* getReceivedBy() const { return m_receivedBy; }
    void setReceivedBy(const Worker* worker) { m_receivedBy = worker; }
    const Worker* getPreparedBy() const { return m_preparedBy; }
    void setPreparedBy(const Worker* worker) { m_preparedBy = worker; }
    const std::vector<const MenuItem*>& getItems() const { return m_items; }
    void addItem(const MenuItem* item) { m_items.push_back(item); }
    void setItems(const std::vector<const MenuItem*> items) { m_items = items; }
    std::string getStatus() { return status; }
    void setStatus(std::string newStatus) { status = newStatus; }
    std::tm getCookedTime() const { return m_CookedTime; }
    void setCookedTime(std::tm cookedTime) { m_CookedTime = cookedTime; }

    int getAmountWithoutSale() { return amountWithoutSale; }
    void addAmountWithoutSale(int addingPart) { amountWithoutSale += addingPart; }

    int getAmountWithSale() { return amountWithSale; }
    void addAmountWithSale(int addingPart) { amountWithSale += addingPart; }

    int getDiscount() { return m_discount; }
    void setDiscount(int discount) { m_discount = discount; }

    void addCalories(int addingCalories) { calories += addingCalories; }
    int getCalories() const { return calories; }

private:
    std::string m_client;
    const Worker* m_receivedBy;
    std::tm m_receivedTime;
    const Worker* m_preparedBy;
    std::tm m_CookedTime;
    std::vector<const MenuItem*> m_items;
    int calories = 0;
    int amountWithoutSale = 0;
    int m_discount;
    int amountWithSale = 0;
    std::string status = "Принят";
};


class OrderManager {
public:
    // Конструктор
    OrderManager(WorkersManager* workersManager, MenuManager* menuManager, ClientManager* clientManager, IngredientsManager* ingredientsManager);

    // Геттер
    std::vector<Order>& getOrders() { return m_orders; }

    // Методы для работы с заказами
    void createNewOrder();
    void deleteOrder();
    void updateOrder();
    void findOrderByAttribute();
    Order findOrderById(int id);
    void viewOrdersHistory();
    void printOrder(Order order) const;
    bool canChange();

private:
    std::vector<Order> m_orders;
    WorkersManager* workersManager;
    MenuManager* menuManager;
    ClientManager* clientManager;
    IngredientsManager* ingredientsManager;

};

#endif // ORDER_MANAGER_H
