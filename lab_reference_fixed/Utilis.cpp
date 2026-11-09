#include "Utils.h"
#include <ctime>
#include <iomanip>
#include <stdexcept>

Logger::Logger() : file("log.txt", std::ios::app), previousBuffer(nullptr) {
    if (!file) throw std::runtime_error("Не удалось открыть log.txt");
    previousBuffer = std::clog.rdbuf(file.rdbuf());
}

Logger::~Logger() {
    std::clog.rdbuf(previousBuffer);
}

void writeLog(const char* file, int line, const char* function, const std::string& message) {
    std::time_t now = std::time(nullptr);
    std::tm time{};
#ifdef _WIN32
    localtime_s(&time, &now);
#else
    if (const std::tm* local = std::localtime(&now)) time = *local;
#endif
    std::clog << std::put_time(&time, "%Y-%m-%d %H:%M:%S") << " | "
        << file << ':' << line << " | " << function << " | " << message << std::endl;
    if (!std::clog) throw std::runtime_error("Не удалось записать log.txt");
}

bool readLine(std::istream& input, const std::string& prompt, std::string& text) {
    if (&input == &std::cin) std::cout << prompt;
    if (!std::getline(input, text)) {
        if (&input == &std::cin) throw InputClosed{};
        return false;
    }
    if (&input == &std::cin) {
        std::string label = prompt;
        for (char& ch : label) {
            if (ch == '\n' || ch == '\r') ch = ' ';
        }
        LOG("input prompt=" + label + " value=" + text);
    }
    return true;
}

int readInt(const std::string& prompt) {
    int value{};
    readNumber(std::cin, prompt, value);
    return value;
}

double readDouble(const std::string& prompt) {
    double value{};
    readNumber(std::cin, prompt, value);
    return value;
}

std::vector<int> findPipes(const std::map<int, Pipe>& pipes) {
    std::vector<int> foundIds;
    int choice = readInt("Искать по названию-1, по ремонту-2: ");

    if (choice == 1) {
        std::string query;
        readLine(std::cin, "Введите название: ", query);
        for (const auto& pair : pipes) {
            if (pair.second.matchesName(query)) {
                foundIds.push_back(pair.first);
            }
        }
    }
    else if (choice == 2) {
        int state = readInt("Искать в ремонте? (1 - да, 0 - нет): ");
        for (const auto& pair : pipes) {
            if (pair.second.matchesRepair(state)) {
                foundIds.push_back(pair.first);
            }
        }
    }
    return foundIds;
}

std::vector<int> findStations(const std::map<int, CompressorStation>& stations) {
    std::vector<int> foundIds;
    int choice = readInt("Искать по названию-1, по проценту-2: ");

    if (choice == 1) {
        std::string query;
        readLine(std::cin, "Введите название: ", query);
        for (const auto& pair : stations) {
            if (pair.second.matchesName(query)) {
                foundIds.push_back(pair.first);
            }
        }
    }
    else if (choice == 2) {
        double percent = readDouble("Введите минимальный процент: ");
        for (const auto& pair : stations) {
            if (pair.second.matchesIdlePercent(percent)) {
                foundIds.push_back(pair.first);
            }
        }
    }
    return foundIds;
}
