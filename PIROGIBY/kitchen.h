#ifndef KITCHEN_H
#define KITCHEN_H

#include <string>
#include <vector>
#include <ctime>

class Kitchen {
public:
    Kitchen(OrderManager& orderManager, WorkersManager& workerManager)
        : m_orderManager(orderManager), m_workerManager(workerManager) {}

    void printOrderCard(Order& order);
    bool displayOrders(int pageNumber);
    bool isValidInt(const std::string& str);

    // Размер карточки
    int cardWidth = 40;
    int cardHeight = 10;
    int maxCardsPerRow = 3;  // Максимальное количество карточек в строке

    int perPage = 3;
private:
    OrderManager& m_orderManager;
    WorkersManager& m_workerManager;

    // std::vector<Order> orders = m_orderManager.getOrders();
    std::time_t now = std::time(nullptr);
    struct tm timeInfo;
#ifdef _MSC_VER
    errno_t err = localtime_s(&timeInfo, &now);
#else
    localtime_r(&now, &timeInfo);
#endif

};

#endif // KITCHEN_H