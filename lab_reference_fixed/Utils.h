#pragma once
#include "Pipe.h"
#include "CompressorStation.h"
#include <cmath>
#include <map>
#include <sstream>
#include <vector>

class InputClosed {};
class Logger {
    std::ofstream file;
    std::streambuf* previousBuffer;
public:
    Logger();
    ~Logger();
};

void writeLog(const char* file, int line, const char* function, const std::string& message);
#define LOG(message) writeLog(__FILE__, __LINE__, __func__, (message))

bool readLine(std::istream& input, const std::string& prompt, std::string& text);
int readInt(const std::string& prompt);
double readDouble(const std::string& prompt);
std::vector<int> findPipes(const std::map<int, Pipe>& pipes);
std::vector<int> findStations(const std::map<int, CompressorStation>& stations);

template <typename T>
bool readNumber(std::istream& input, const std::string& prompt, T& value, bool positive = false) {
    std::string text;
    while (readLine(input, prompt, text)) {
        std::istringstream number(text);
        T result{};
        if (number >> result) {
            number >> std::ws;
            if (number.eof() && std::isfinite(result) && (!positive || result > 0)) {
                value = result;
                return true;
            }
        }
        if (&input != &std::cin) {
            input.setstate(std::ios::failbit);
            return false;
        }
        LOG("invalid_number value=" + text);
        std::cout << (positive ? "Ошибка! Введите положительное число.\n" : "Ошибка! Введите число.\n");
    }
    return false;
}
