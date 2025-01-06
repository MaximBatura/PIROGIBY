#include "worker_manager.h"
#include "worker_manager.h"
#include "worker_manager.h"
#include "worker_manager.h"

void WorkersManager::addWorker() {
    int id = findNextAvailableId(workers);

    std::cout << "Введите имя работника: ";
    std::string name = getUserString();

    std::cout << "Введите должность работника: ";
    std::string position = getUserString();

    // Динамическое создание нового объекта Worker
    Worker* newWorker = new Worker(id, name, position);
    workers.push_back(newWorker);
    system("cls");

    std::cout << "Информация о работнике добавлена.\n";
}

void WorkersManager::deleteWorker() {
    if (!canChange()) { return; }
    // bool found = false;

    std::cout << "Введите ID работника, которого хотите удалить: ";
    int id_to_delete = getId(workers);
    Worker* worker_to_delete = findWorkerById(id_to_delete);
    // Теперь находим итератор на элемент, соответствующий этому указателю
    auto it = std::find(workers.begin(), workers.end(), worker_to_delete);
    workers.erase(it);

    system("cls");
    std::cout << "Работник удален.\n";


    /*for (auto it = workers.begin(); it != workers.end(); ++it) {
        if ((*it)->getId() == id_to_delete) {
            workers.erase(it);
            found = true;
            break;
        }
    }*/
    /*system("cls");
    if (found) {
        std::cout << "Работник удален.\n";
    }
    else {
        std::cout << "Работника с таким ID не найдено.\n";
    }*/
}

void WorkersManager::updateWorker() {
    if (!canChange()) { return; }

    std::cout << "Введите ID работника, информацию о котором хотите изменить: ";
    int id_to_update = getId(workers);

    for (auto& worker : workers) {
        if (worker->getId() == id_to_update) {
            std::cout << "Введите новое имя работника: ";
            std::string new_name = getUserString();

            std::cout << "Введите новую должность работника: ";
            std::string new_position = getUserString();

            worker->setName(new_name);
            worker->setPosition(new_position);
            system("cls");
            std::cout << "Информация о работнике обновлена.\n";
            return;
        }
    }

    std::cout << "Работника с таким ID не найдено.\n";
}

void WorkersManager::findWorkerByAttribute() {
    if (!canChange()) { return; }
    
    std::cout << "Введите значение атрибута для поиска (имя или должность): ";
    std::string attribute_value = getUserString();
    system("cls");
    bool found = false;
    for (const auto& worker : workers) {
        if (worker->getName().find(attribute_value) != std::string::npos ||
            worker->getPosition().find(attribute_value) != std::string::npos) {
            std::cout << "ID: " << worker->getId()
                << ", Имя: " << worker->getName()
                << ", Должность: " << worker->getPosition() << '\n';
            found = true;
        }
    }

    if (!found) {
        std::cout << "Работники с указанным значением атрибута не найдены.\n";
    }
}

Worker* WorkersManager::findWorkerById(int id) {
    bool found = false;
    for (const auto& worker : workers) {
        if (worker->getId() == id) {
            return worker;
            found = true;
        }
    }

    if (!found) {
        std::cout << "Работники с указанным значением атрибута не найдены.\n";
    }
}


void WorkersManager::displayAllWorkers() {
    system("cls");
    if (workers.empty()) {
        std::cout << "Список работников пуст.\n";
    }
    else {
        for (auto worker : workers) {
            std::cout << "ID: " << worker->getId();
            std::cout << ", Имя: " << worker->getName();
            std::cout << ", Должность: " << worker->getPosition();
            std::cout << ", Количество принятых заказов: " << worker->getAcceptedOrders();
            std::cout << ", Количество приготовленых заказов: " << worker->getCookedOrders();
            std::cout << ", График работы: ";
            std::vector<int> workingHoursPerWeek = worker->getWorkingHours();
            for (int i = 0; i < workingHoursPerWeek.size(); i++) {
                std::cout << i << ": " << workingHoursPerWeek[i] << ", ";
            }
            std::cout << "\n";
        }
    }
}

bool WorkersManager::canChange() const
{
    if (workers.size() == 0) {
        system("cls");
        std::cout << "Действие недоступно. Список работников пуст.\n";
        return false;
    }
    else {
        return true;
    }
}

void WorkersManager::showCurrentSchedule()
{
    system("cls");
    std::vector<std::string> days = { "Понедельник", "Вторник", "Среда", "Четверг", "Пятница", "Суббота", "Воскресенье" };

    for (int i = 0; i < days.size(); i++) {
        std::cout << days[i] << ": ";
        for (auto worker : workers) {
            if ((worker->getWorkingHours())[i] > 0) {
                std::cout << worker->getName() << "(" << worker->getId() << "): " << worker->getWorkingHours()[i] << " часов, ";
            }
        }
    }
    std::cout << "\n";
    return;
}

void WorkersManager::createNewSchedule()
{
    if (!canChange()) { return; }

    displayAllWorkers();
    std::cout << "\nВведите ID работника, которому хотите установить график работы: ";
    int workerId = getId(workers);

    for (auto& worker : workers) {
        if (worker->getId() == workerId) {
            std::cout << "Работник найден\n";
            for (int i = 0; i < 7; i++) { worker->setWorkingHours(i, 0); } // Очистка старого графика

            // Заполнение нового графика
            for (int i = 0; i < 7; ++i) {
                std::cout << "Введите количество часов работы для дня " << i + 1 << ": ";
                int hours = getUserChoice(0, 12);
                worker->setWorkingHours(i, hours);
            }
            system("cls");
            std::cout << "Новый график работы успешно создан!" << std::endl;
            return;
        }
    }

    std::cout << "Работник с таким ID не найден." << std::endl;

}

int workerMenu(WorkersManager* manager) {
    system("cls");
    int choice;

    while (true) {
        std::cout << "--- Меню работников ---\n";
        std::cout << "1. Добавить информацию о работнике\n";
        std::cout << "2. Удалить информацию о работнике\n";
        std::cout << "3. Изменить информацию о работнике\n";
        std::cout << "4. Найти работника по атрибуту\n";
        std::cout << "5. Просмотреть всех работников\n";
        std::cout << "6. Текущий график работы\n";
        std::cout << "7. Создать новый график работы\n";
        std::cout << "8. Вернуться в главное меню\n";
        std::cout << "Выберите пункт: ";
        choice = getUserChoice(1, 8);

        switch (choice) {
        case 1:
            manager->addWorker();
            break;
        case 2:
            manager->deleteWorker();
            break;
        case 3:
            manager->updateWorker();
            break;
        case 4:
            manager->findWorkerByAttribute();
            break;
        case 5:
            manager->displayAllWorkers();
            break;
        case 6:
            manager->showCurrentSchedule();
            break;
        case 7:
            manager->createNewSchedule();
            break;
        case 8:
            return 0;
        default:
            std::cout << "Неверный ввод. Попробуйте еще раз.\n";
            break;
        }
    }

    return 0;

}