#ifndef WORKER_MANAGER_H
#define WORKER_MANAGER_H

#include <string>
#include <vector>

class Worker : public BaseClass {
private:
    std::string name;
    std::string position;
    std::vector<int> workingHoursPerWeek = { 0, 0, 0, 0, 0, 0, 0 }; // Вектор для хранения количества часов работы за каждый день недели
    int acceptedOrders = 0;
    int cookedOrders = 0;

public:
    // Конструктор по умолчанию
    Worker() : BaseClass(-1), name(""), position("") {}

    // Конструктор с параметрами
    Worker(int id, const std::string& name, const std::string& position)
        : BaseClass(id), name(name), position(position) {}

    // Геттеры и сеттеры для name и position
    void setName(const std::string& new_name) { name = new_name; }
    std::string getName() const { return name; }

    void setPosition(const std::string& new_position) { position = new_position; }
    std::string getPosition() const { return position; }

    void setWorkingHours(int day, int hours) { if (day >= 0 && day <= 6) { workingHoursPerWeek[day] = hours; } }
    std::vector<int> getWorkingHours(){ return workingHoursPerWeek; }

    int getAcceptedOrders() { return acceptedOrders; }
    void addAcceptedOrders(int addQuantity) { acceptedOrders += addQuantity; }

    int getCookedOrders() { return cookedOrders; }
    void addCookedOrders(int addQuantity) { cookedOrders += addQuantity; }
};

class WorkersManager {
private:
    std::vector<Worker*> workers;

public:
    // Объявления методов
    void addWorker();
    void deleteWorker();
    void updateWorker();
    void findWorkerByAttribute();
    Worker* findWorkerById(int id);
    void displayAllWorkers();
    std::vector<Worker*> getWorkers() { return workers; }
    int getWorkersSize() { return workers.size(); }
    bool canChange() const;

    void showCurrentSchedule();
    void createNewSchedule();
};

#endif // WORKER_MANAGER_H