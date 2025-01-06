#include <iostream>
#include <iomanip>
#include <limits>
#include <ctime>

#include "order_manager.h"
#include "worker_manager.h"
#include "menu_manager.h"

// Конструктор
OrderManager::OrderManager(WorkersManager* workersManager, MenuManager* menuManager, ClientManager* clientManager, IngredientsManager* ingredientsManager)
    : workersManager(workersManager), menuManager(menuManager), clientManager(clientManager), ingredientsManager(ingredientsManager) {}


// Простая функция для вывода времени
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

    std::cout << "Введите номер телефона клиента (пример: 375291111111): ";
    std::string clientPhoneNumber = getUserString();
    Client* clientPtr = clientManager->findClientByPhoneNumber(clientPhoneNumber);
    if (clientPtr == nullptr) {
        std::cout << "Клиент не найден в базе и будет создан как новый\n";
        
        // Добавление клиента
        // Находим минимальный доступный ID
        int id = findNextAvailableId(clientManager->getClients());

        std::cout << "Введите имя клиента: ";
        std::string name = getUserString();

        std::cout << "Введите скидку клиента (%): ";
        int discount = getUserChoice(0, 100);

        clientManager->addClient(id, name, clientPhoneNumber, discount);
        clientPtr = clientManager->findClientByPhoneNumber(clientPhoneNumber);
    }
    else {
        if (clientPtr->isInBlacklist()) {
            system("cls");
            std::cout << "Клиент в черном списке. Создание заказа отменено.\n";
            return;
        }
    }

    workersManager->displayAllWorkers();
    std::cout << "Выбран клиент с именем " << clientPtr->getName() << ".\n";
    std::cout << "Введите id работника, принявшего заказ: ";
    int receivedByIndex = getId(workersManager->getWorkers());
    Worker* receivedBy = workersManager->findWorkerById(receivedByIndex);

    // Выбор позиций меню
    std::vector<const MenuItem*> items;

    bool cancel = false;
    while (!cancel) {
        menuManager->viewAllMenuPositions();

        bool choosen = false;
        while (!choosen) {
            std::cout << "Введите id позиции меню или 'q' для выхода: ";
            std::string input;
            std::getline(std::cin, input);

            if (input.length() > MAX_STRING_LENGTH) {
                std::cout << "Максимальная длина строки - " << MAX_STRING_LENGTH << " символов. Повторите попытку.\n";
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
                    std::cout << "Нет такого id. Попробуйте еще раз.\n";
                    continue;
                }
            }
            else {
                std::cout << "Некорректный ввод. Попробуйте снова.\n";
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
                            std::cout << "У вас недостаточно ингредиентов на складе для данного заказа\n";
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
                    //        a -= btch->getQuantity();  // Уменьшаем оставшееся количество
                    //        btch->substractQuantity(btch->getQuantity());  // Полностью используем текущую партию
                    //        selectedIng->removeZeroQuantityBatches();  // Удаляем использованную партию
                    //    }
                    //}
                    while (a != 0 && !selectedBatchesPtr->empty()) {
                        Batch* btch = findNearestExpirationDate(selectedBatchesPtr);

                        if (btch->getQuantity() >= a) {
                            btch->substractQuantity(a);
                            a = 0;
                        }
                        else {
                            a -= btch->getQuantity();  // Уменьшаем оставшееся количество
                            btch->substractQuantity(btch->getQuantity());  // Полностью используем текущую партию
                            selectedIng->removeZeroQuantityBatches();  // Удаляем использованную партию
                        }
                    }

                    if (a != 0) {
                        system("cls");
                        std::cout << "У вас недостаточно ингредиентов на складе для данного заказа\n";
                        return;
                    }

                    break;
                }
            }
        }
    }

    // Создание нового заказа
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
    std::cout << "Новый заказ создан успешно!" << std::endl;
}

void OrderManager::deleteOrder() {
    // system("cls");
    if (!canChange()) { return; }
    std::cout << "Введите ID заказа для удаления: ";
    int orderId = getId(m_orders);

    for (auto it = m_orders.begin(); it != m_orders.end(); ++it) {
        if (it->getId() == orderId) {
            m_orders.erase(it);
            std::cout << "Заказ удален." << std::endl;
            return;
        }
    }
}

void OrderManager::updateOrder() {
    if (!canChange()) { return; }
    std::cout << "Введите ID заказа для изменения: ";
    int orderId = getId(m_orders);
    for (auto& order : m_orders) {
        if (order.getId() == orderId) {
            std::cout << "Изменение информации о заказе..." << std::endl;

            std::cout << "Введите имя клиента: ";
            std::string client = getUserString();
            order.setClient(client);

            // Выбор работника, принявшего заказ
            workersManager->displayAllWorkers();
            std::cout << "Введите id работника, принявшего заказ: ";
            int receivedByIndex = getId(workersManager->getWorkers());
            const Worker* receivedBy = workersManager->findWorkerById(receivedByIndex);
            order.setReceivedBy(receivedBy);

            if (order.getStatus() == "Приготовлен") {
                // Выбор работника, приготовившего заказ
                workersManager->displayAllWorkers();
                std::cout << "Введите номер работника, приготовившего заказ:: ";
                int preparedByIndex = getId(workersManager->getWorkers());
                const Worker* preparedBy = workersManager->findWorkerById(preparedByIndex);
                order.setPreparedBy(preparedBy);
            }

            // Выбор позиций меню
            std::vector<const MenuItem*> items;
            bool cancel = false;
            while (!cancel) {
                bool getChoice = false;
                menuManager->viewAllMenuPositions();

                while (!getChoice) {

                    std::cout << "Введите id позиции меню или 'q' для выхода: ";
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
                            std::cout << "Позиции меню с таким ID не найдено. Попробуйте снова.\n";
                            continue;
                        }

                    }
                    else {
                        clearInputBuffer();
                        std::cout << "Некорректный ввод. Попробуйте снова\n";
                        continue;
                    }
                }
            }
            order.setItems(items);
            system("cls");
            std::cout << "Информация о заказе обновлена." << std::endl;
            return;
        }
    }
}

void OrderManager::findOrderByAttribute() {
    if (!canChange()) { return; }
    std::cout << "Поиск заказа по атрибуту..." << std::endl;

    // Поиск по различным параметрам:
    // ID, клиенту, работникам, позиции меню

    std::cout << "Выберите атрибут поиска (1-id, 2-клиент, 3-работник, 4-позиция меню): ";
    int atr_id = getUserChoice(1, 4);

    switch (atr_id) {
    case 1:
    {
        // Поиск по ID

        std::cout << "Введите id: ";
        int search_id = getId(m_orders);
        // Очистка буфера ввода после ввода числа
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        for (const auto& order : m_orders) {
            if (order.getId() == search_id) {
                system("cls");
                std::cout << "Заказ найден:" << std::endl;
                printOrder(order);
                return;
            }
        }
        system("cls");
        std::cout << "Заказ не найден." << std::endl;
        break;
    }
    case 2: // Поиск по Клиенту
    {
        std::cout << "Введите имя клиента: ";
        std::string client = getUserString();

        for (const auto& order : m_orders) {
            if (order.getClient() == client) {
                system("cls");
                std::cout << "Заказ найден:" << std::endl;
                printOrder(order);
                return;
            }
        }
        system("cls");
        std::cout << "Заказ не найден." << std::endl;
        break;
    }
    case 3: // Поиск по Работнику, принявшему или приготовившему заказ
    {
        std::cout << "Введите имя работника: ";
        std::string worker = getUserString();

        for (const auto& order : m_orders) {
            if (order.getReceivedBy()->getName() == worker or order.getPreparedBy()->getName() == worker) {
                system("cls");
                std::cout << "Заказ найден:" << std::endl;
                printOrder(order);
                return;
            }
        }
        system("cls");
        std::cout << "Заказ не найден." << std::endl;
        break;
    }
    case 4: // Поиск по Позиции меню
    {
        std::cout << "Введите наименование позиции: ";
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
                std::cout << "Заказ найден:" << std::endl;
                printOrder(order);
                return;
            }
        }
        break;
    }
    default:
        std::cerr << "Неверный тип атрибута!" << std::endl;
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
    std::cout << "Заказ не найден." << std::endl;
}

void OrderManager::viewOrdersHistory() {
    system("cls");
    std::cout << "История заказов:" << std::endl;
    if (m_orders.empty()) {
        std::cout << "Нет активных заказов." << std::endl;
        return;
    }
    for (auto order : m_orders) { printOrder(order); }
}

void OrderManager::printOrder(Order order) const {
    std::cout << "ID: " << order.getId();
    std::cout << ", Статус: " << order.getStatus();
    std::cout << ", Клиент: " << order.getClient();
    std::cout << ", Сумма заказа до скидки: " << order.getAmountWithoutSale();
    std::cout << ", Скидка: " << order.getDiscount();
    std::cout << ", Сумма заказа после скидки: " << order.getAmountWithSale();
    std::cout << ", Принял заказ: " << order.getReceivedBy()->getName();
    std::cout << ", Время принятия: " << order.getReceivedTime();

    if (order.getStatus() == "Приготовлен") {
        std::cout << ", Приготовил: " << order.getPreparedBy()->getName()
            << ", Время приготовления: " << order.getCookedTime();
    }
    std::cout << ", Сумма ккалорий: " << order.getCalories();
    std::cout << ", Позиции меню: ";
    for (auto item : order.getItems()) {
        std::cout << item->getName() << ", ";
    }
    std::cout << "\n";
}

bool OrderManager::canChange()
{
    if (menuManager->getMenuSize() == 0) {
        system("cls");
        std::cout << "Действие недоступно, так как у вас нет ни одной позиции в меню.\n";
        return false;
    }
    else if (workersManager->getWorkersSize() == 0) {
        system("cls");
        std::cout << "Действие недоступно, так как у вас нет ни одного работника.\n";
        return false;
    }
    return true;
}

int ordersMenu(OrderManager& manager) {
    system("cls");
    while (true) {
        std::cout << "--- Меню управления заказами ---\n";
        std::cout << "1. Создать новый заказ\n";
        std::cout << "2. Удалить созданный заказ\n";
        std::cout << "3. Изменить информацию заказа\n";
        std::cout << "4. Найти заказ по атрибуту\n";
        std::cout << "5. Посмотреть историю заказов\n";
        std::cout << "6. Вернуться в главное меню\n";

        std::cout << "Выберите пункт: ";
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
            std::cout << "Неверный ввод. Попробуйте еще раз." << std::endl;
            break;
        }
    }

    return 0;
}

