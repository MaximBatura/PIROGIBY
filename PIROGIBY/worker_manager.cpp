#include "worker_manager.h"
#include "worker_manager.h"
#include "worker_manager.h"
#include "worker_manager.h"

void WorkersManager::addWorker() {
    int id = findNextAvailableId(workers);

    std::cout << "������� ��� ���������: ";
    std::string name = getUserString();

    std::cout << "������� ��������� ���������: ";
    std::string position = getUserString();

    // ������������ �������� ������ ������� Worker
    Worker* newWorker = new Worker(id, name, position);
    workers.push_back(newWorker);
    system("cls");

    std::cout << "���������� � ��������� ���������.\n";
}

void WorkersManager::deleteWorker() {
    if (!canChange()) { return; }
    // bool found = false;

    std::cout << "������� ID ���������, �������� ������ �������: ";
    int id_to_delete = getId(workers);
    Worker* worker_to_delete = findWorkerById(id_to_delete);
    // ������ ������� �������� �� �������, ��������������� ����� ���������
    auto it = std::find(workers.begin(), workers.end(), worker_to_delete);
    workers.erase(it);

    system("cls");
    std::cout << "�������� ������.\n";


    /*for (auto it = workers.begin(); it != workers.end(); ++it) {
        if ((*it)->getId() == id_to_delete) {
            workers.erase(it);
            found = true;
            break;
        }
    }*/
    /*system("cls");
    if (found) {
        std::cout << "�������� ������.\n";
    }
    else {
        std::cout << "��������� � ����� ID �� �������.\n";
    }*/
}

void WorkersManager::updateWorker() {
    if (!canChange()) { return; }

    std::cout << "������� ID ���������, ���������� � ������� ������ ��������: ";
    int id_to_update = getId(workers);

    for (auto& worker : workers) {
        if (worker->getId() == id_to_update) {
            std::cout << "������� ����� ��� ���������: ";
            std::string new_name = getUserString();

            std::cout << "������� ����� ��������� ���������: ";
            std::string new_position = getUserString();

            worker->setName(new_name);
            worker->setPosition(new_position);
            system("cls");
            std::cout << "���������� � ��������� ���������.\n";
            return;
        }
    }

    std::cout << "��������� � ����� ID �� �������.\n";
}

void WorkersManager::findWorkerByAttribute() {
    if (!canChange()) { return; }
    
    std::cout << "������� �������� �������� ��� ������ (��� ��� ���������): ";
    std::string attribute_value = getUserString();
    system("cls");
    bool found = false;
    for (const auto& worker : workers) {
        if (worker->getName().find(attribute_value) != std::string::npos ||
            worker->getPosition().find(attribute_value) != std::string::npos) {
            std::cout << "ID: " << worker->getId()
                << ", ���: " << worker->getName()
                << ", ���������: " << worker->getPosition() << '\n';
            found = true;
        }
    }

    if (!found) {
        std::cout << "��������� � ��������� ��������� �������� �� �������.\n";
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
        std::cout << "��������� � ��������� ��������� �������� �� �������.\n";
    }
}


void WorkersManager::displayAllWorkers() {
    system("cls");
    if (workers.empty()) {
        std::cout << "������ ���������� ����.\n";
    }
    else {
        for (auto worker : workers) {
            std::cout << "ID: " << worker->getId();
            std::cout << ", ���: " << worker->getName();
            std::cout << ", ���������: " << worker->getPosition();
            std::cout << ", ���������� �������� �������: " << worker->getAcceptedOrders();
            std::cout << ", ���������� ������������� �������: " << worker->getCookedOrders();
            std::cout << ", ������ ������: ";
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
        std::cout << "�������� ����������. ������ ���������� ����.\n";
        return false;
    }
    else {
        return true;
    }
}

void WorkersManager::showCurrentSchedule()
{
    system("cls");
    std::vector<std::string> days = { "�����������", "�������", "�����", "�������", "�������", "�������", "�����������" };

    for (int i = 0; i < days.size(); i++) {
        std::cout << days[i] << ": ";
        for (auto worker : workers) {
            if ((worker->getWorkingHours())[i] > 0) {
                std::cout << worker->getName() << "(" << worker->getId() << "): " << worker->getWorkingHours()[i] << " �����, ";
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
    std::cout << "\n������� ID ���������, �������� ������ ���������� ������ ������: ";
    int workerId = getId(workers);

    for (auto& worker : workers) {
        if (worker->getId() == workerId) {
            std::cout << "�������� ������\n";
            for (int i = 0; i < 7; i++) { worker->setWorkingHours(i, 0); } // ������� ������� �������

            // ���������� ������ �������
            for (int i = 0; i < 7; ++i) {
                std::cout << "������� ���������� ����� ������ ��� ��� " << i + 1 << ": ";
                int hours = getUserChoice(0, 12);
                worker->setWorkingHours(i, hours);
            }
            system("cls");
            std::cout << "����� ������ ������ ������� ������!" << std::endl;
            return;
        }
    }

    std::cout << "�������� � ����� ID �� ������." << std::endl;

}

int workerMenu(WorkersManager* manager) {
    system("cls");
    int choice;

    while (true) {
        std::cout << "--- ���� ���������� ---\n";
        std::cout << "1. �������� ���������� � ���������\n";
        std::cout << "2. ������� ���������� � ���������\n";
        std::cout << "3. �������� ���������� � ���������\n";
        std::cout << "4. ����� ��������� �� ��������\n";
        std::cout << "5. ����������� ���� ����������\n";
        std::cout << "6. ������� ������ ������\n";
        std::cout << "7. ������� ����� ������ ������\n";
        std::cout << "8. ��������� � ������� ����\n";
        std::cout << "�������� �����: ";
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
            std::cout << "�������� ����. ���������� ��� ���.\n";
            break;
        }
    }

    return 0;

}