#include <string>
#include <cctype> // ��� ������� isdigit()
#include <limits>
#include <iostream>
#include <iomanip>
#include <ctime>
#include <chrono>
#include <sstream>
#include <string>
#include <regex>


int MAX_STRING_LENGTH = 20;
int MAX_ID = 100000;

class BaseClass {
protected:
    int id;
public:
    explicit BaseClass(int id = 0) : id(id) {}

    virtual ~BaseClass() {} // ����������� ���������� ��� ������������

    int getId() const { return id; }
};

// ������� ��� ��������, ������� �� ������ ������ �� ����
bool isNumber(const std::string& str) {
    return std::all_of(str.begin(), str.end(), ::isdigit);
}

int stringToInt(const std::string& str) {
    try {
        return std::stoi(str);
    }
    catch (const std::exception&) {
        throw std::runtime_error("���������� ������������� ������ � �����.");
    }
}

std::string removeNewLines(const std::string& input) {
    std::string output = input;
    size_t pos = 0;
    while ((pos = output.find('\n', pos)) != std::string::npos) {
        output.replace(pos, 1, "");
        pos += 1;
    }
    while ((pos = output.find(' ', pos)) != std::string::npos) {
        output.replace(pos, 1, "");
        pos += 1;
    }
    return output;
}

// ������� ��� ������ ������������ ���������� id � ������� ��������
template<typename T>
int findNextAvailableId(const std::vector<T>& objects) {

    // ������� ��������� ��� �������� ���� ������������ id
    std::vector<int> usedIds;
    for (const auto& obj : objects) {
        usedIds.push_back(obj.getId());
    }

    // ��������� ������ ��� ��������� ������
    std::sort(usedIds.begin(), usedIds.end());

    int nextId = 0;
    while (std::binary_search(usedIds.begin(), usedIds.end(), nextId)) {
        ++nextId;
    }

    return nextId;
}

// ������� ��� ������ ������������ ���������� id � ������� ���������� �� �������
template<typename T>
int findNextAvailableId(const std::vector<T*>& pointers) {

    // ������� ��������� ��� �������� ���� ������������ id
    std::vector<int> usedIds;
    for (const auto& ptr : pointers) {
        usedIds.push_back(ptr->getId());
    }

    // ��������� ������ ��� ��������� ������
    std::sort(usedIds.begin(), usedIds.end());

    int nextId = 0;
    while (std::binary_search(usedIds.begin(), usedIds.end(), nextId)) {
        ++nextId;
    }

    return nextId;
}

void clearInputBuffer() {
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // ������� ����� �����
    std::cin.clear(); // ���������� ����� ������
}


int getUserChoice(int min, int max) {
    int choice;
    while (true) {
        if (std::cin >> choice) {  // ���� ������� ��������� ����� �����
            if (choice >= min && choice <= max) {
                clearInputBuffer();  // ������� ����� �����
                return choice;
            }
            std::cout << "������������ ����. ��������� �������: ";
        }
        else {
            std::cin.clear();  // ���������� ����� ������
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');  // ������� ��� ������� �� ����� ������
            std::cout << "������ �����. ������� �����: ";
        }
    }
}

std::string getUserString(int minLength = MAX_STRING_LENGTH) {
    std::string str;
    while (true) {

        std::getline(std::cin, str);
        if (str.empty()) {
            std::cout << "������ ������. ��������� �������: ";
            continue;
        }

        if (str.length() <= MAX_STRING_LENGTH) {
            return str;
        }
        std::cout << "������������ ����� ������ - " << MAX_STRING_LENGTH << " ��������.��������� ������� : ";

    }
}

template<typename T>
int getId(const std::vector<T>& objects) {
    int targetId;
    while (true) {
        targetId = getUserChoice(0, MAX_ID);

        auto iterator = std::find_if(objects.begin(), objects.end(),
            [&](const T& object) {
                return object.getId() == targetId;
            });

        if (iterator != objects.end()) {
            return iterator->getId();
        }
        else {
            std::cout << "������� � ����� id �� ����������. ���������� ��� ���: ";
        }
    }
}

template<typename T>
int getId(const std::vector<T*>& objects) {
    int targetId;
    while (true) {
        targetId = getUserChoice(0, MAX_ID);

        auto iterator = std::find_if(objects.begin(), objects.end(),
            [&](const T* object) {
                return object->getId() == targetId;
            });

        if (iterator != objects.end()) {
            return (*iterator)->getId();
        }
        else {
            std::cout << "������� � ����� id �� ����������. ���������� ��� ���: ";
        }
    }
}

template<typename T>
bool checkId(const std::vector<T>& objects, int targetId) {
    auto iterator = std::find_if(objects.begin(), objects.end(),
        [&](const T& object) {
            return object.getId() == targetId;
        });

    if (iterator != objects.end()) {
        return true;
    }
    else {
        return false;
    }
}

template<typename T>
bool checkId(const std::vector<T*>& objects, int targetId) {
    auto iterator = std::find_if(objects.begin(), objects.end(),
        [&](const T* object) {
            return object->getId() == targetId;
        });

    if (iterator != objects.end()) {
        return true;
    }
    else {
        return false;
    }
}

// ������� ��� ��������� ������� ���� � ������� "��.��.����"
std::string getCurrentDateString()
{
    auto now = std::chrono::system_clock::now();
    std::time_t nowTimeT = std::chrono::system_clock::to_time_t(now);
    std::tm* localTm = std::localtime(&nowTimeT);
    char buffer[11];
    std::strftime(buffer, sizeof(buffer), "%d.%m.%Y", localTm);
    return std::string(buffer);
}

// ������� ��� �������������� ������ � ������ time_point
std::chrono::system_clock::time_point parseDate(const std::string& dateStr)
{
    std::tm t = {};
    std::istringstream ss(dateStr);

    // ����������� ������ ���� ��� ������
    ss >> std::get_time(&t, "%d.%m.%Y");

    if (ss.fail()) {
        throw std::runtime_error("Invalid date format");
    }

    // ����������� ��������� tm � ������ time_point
    std::time_t tt = std::mktime(&t);
    return std::chrono::system_clock::from_time_t(tt);
}

// ������� ��� ��������, �������� �� ���� ����� ������ ��� �����
bool isDateMoreThanAWeekAway(const std::string& dateStr)
{
    using namespace std::chrono_literals;

    // ������� ���� + 7 ����
    auto now = std::chrono::system_clock::now();
    auto oneWeekLater = now + 7 * 24h;

    // ������ �������� ����
    auto parsedDate = parseDate(dateStr);

    // ���������� ����
    return parsedDate <= oneWeekLater;
}

bool isValidDate(const std::string& dateStr)
{
    // ���������� ��������� ��� �������� ������� ��.��.����
    const std::regex dateRegex(R"(^([0-9]{2})\.([0-9]{2})\.([0-9]{4})$)");

    // ��������� ������ �� ������������ ����������� ���������
    if (!std::regex_match(dateStr, dateRegex))
        return false;

    // ��������� ���������� ����
    int day = std::stoi(dateStr.substr(0, 2));
    int month = std::stoi(dateStr.substr(3, 2));
    int year = std::stoi(dateStr.substr(6, 4));

    // �������� ���������� �������� ���, ������ � ����
    if (day < 1 || day > 31 || month < 1 || month > 12 || year < 1900 || year > 2100)
        return false;

    // �������������� �������� �� ���������� ���� � ������
    bool isLeapYear = ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0);
    switch (month)
    {
    case 2:
        return (isLeapYear ? day <= 29 : day <= 28);
    case 4:
    case 6:
    case 9:
    case 11:
        return day <= 30;
    default:
        return true;
    }
}

// ������� ��� ������ ��������� �� ������ � ��������� ����� ��������� �����
Batch* findNearestExpirationDate(std::vector<Batch>* selectedBatchesPtr)
{
    if (!selectedBatchesPtr || selectedBatchesPtr->empty()) {
        return nullptr;
    }

    auto currentDate = parseDate(getCurrentDateString());

    auto closestIt = std::min_element(selectedBatchesPtr->begin(), selectedBatchesPtr->end(),
        [&currentDate](Batch& a, Batch& b) {
            auto dateA = parseDate(a.getExpirationDate());
            auto dateB = parseDate(b.getExpirationDate());
            auto diffA = std::chrono::abs(currentDate - dateA);
            auto diffB = std::chrono::abs(currentDate - dateB);
            return diffA < diffB;
        });

    return &*closestIt;
}
