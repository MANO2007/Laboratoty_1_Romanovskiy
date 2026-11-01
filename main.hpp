#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <limits>
#include <iomanip>
#include <cmath>
#include <stdexcept>
#include <ctime>
using namespace std;
// Поля открыты, чтобы работать с ними так же, как в первой лабораторной.
class Pipe
{
public:
    int id = 0;
    string kilometerMark;
    double length = 0;
    double diameter = 0;
    bool isUnderRepair = false;
};
class CompressorStation
{
public:
    int id = 0;
    string name;
    int countShops = 0;
    int countShopsInWork = 0;
    char stationClass = '-';
    double UnusedPercent() const;
};
// Это исключение позволяет закончить программу, если ввод закрыт.
class InputClosed {};
// Макрос автоматически добавляет место вызова в технический журнал.
void WriteLog(const char* filename, int line, const char* function, const string& message);
#define LogAction(message) WriteLog(__FILE__, __LINE__, __func__, message)
class Manager
{
    vector<Pipe> pipes;
    vector<CompressorStation> stations;
    int nextId = 1;
    ofstream logFile;
    streambuf* oldLogBuffer;
public:
    Manager();
    ~Manager();
    void Run();
    void ManagePipes();
    void ManageStations();
};
istream& operator>>(istream& input, Pipe& pipe);
istream& operator>>(istream& input, CompressorStation& station);
ostream& operator<<(ostream& output, const Pipe& pipe);
ostream& operator<<(ostream& output, const CompressorStation& station);
string ReadLine(const string& message);
string ReadText(const string& message);
string ReadFilename(const string& message);
char ReadStationClass();
void AddPipe(vector<Pipe>& pipes, int& nextId);
void AddCompressorStation(vector<CompressorStation>& stations, int& nextId);
// Одинаковое имя, разные типы параметров: перегрузка функции.
void ShowObject(const Pipe& pipe);
void ShowObject(const CompressorStation& station);
vector<int> FindPipes(const vector<Pipe>& pipes);
vector<int> FindStations(const vector<CompressorStation>& stations);
void SearchObjects(const vector<Pipe>& pipes, const vector<CompressorStation>& stations);
bool LoadObject(istream& file, Pipe& pipe);
bool LoadObject(istream& file, CompressorStation& station);
void SaveData(const vector<Pipe>& pipes, const vector<CompressorStation>& stations, int nextId);
void LoadData(vector<Pipe>& pipes, vector<CompressorStation>& stations, int& nextId);
// Разбирает число целиком: строка "2abc" не считается числом.
template <typename T>
bool ParseNumber(const string& text, T& value)
{
    istringstream input(text);
    if (!(input >> value)) return false;
    input >> ws;
    return input.eof() && isfinite(static_cast<double>(value));
}
// Одна функция вместо ReadInteger и ReadPositiveNumber.
template <typename T>
T ReadNumber(const string& message, T minValue, T maxValue, bool excludeMin = false)
{
    T value{};
    while (true)
    {
        string text = ReadLine(message);
        if (!ParseNumber(text, value))
        {
            cout << "Incorrect input. Enter one number.\n";
            LogAction("Rejected number: " + text);
            continue;
        }
        if (value < minValue || value > maxValue || (excludeMin && value == minValue))
        {
            cout << "Incorrect value. Allowed range: " << minValue << " to " << maxValue;
            if (excludeMin) cout << " (minimum excluded)";
            cout << ".\n";
            LogAction("Rejected number: outside allowed range");
            continue;
        }
        return value;
    }
}
// Возвращает позицию объекта в списке, а не его ID.
template <typename T>
int FindIndex(const vector<T>& objects, int id)
{
    for (int i = 0; i < static_cast<int>(objects.size()); i++)
    {
        if (objects[i].id == id) return i;
    }
    return -1;
}
template <typename T>
void ShowObjects(const vector<T>& objects)
{
    if (objects.empty()) cout << "No objects of this type.\n";
    for (const T& object : objects) ShowObject(object);
}
template <typename T>
void ShowFound(const vector<T>& objects, const vector<int>& ids)
{
    cout << "Found: " << ids.size() << '\n';
    string result = "Search results (IDs):";
    for (int id : ids)
    {
        ShowObject(objects[FindIndex(objects, id)]);
        result += " " + to_string(id);
    }
    LogAction(result);
}
// Выбор всех найденных объектов или отдельных объектов.
template <typename T>
vector<int> SelectObjects(const vector<T>& objects, const vector<int>& found)
{
    int mode = ReadNumber<int>("1. All found\n2. Choose objects one by one\n0. Cancel\nSelection: ", 0, 2);
    vector<int> selected;
    if (mode == 0) return selected;
    for (int id : found)
    {
        if (mode == 2) ShowObject(objects[FindIndex(objects, id)]);
        if (mode == 1 || ReadNumber<int>("Select ID " + to_string(id) + " (0/1): ", 0, 1) == 1)
        {
            selected.push_back(id);
        }
    }
    string result = "Selected IDs:";
    for (int id : selected) result += " " + to_string(id);
    LogAction(result);
    return selected;
}
template <typename T>
void SaveObjects(ofstream& file, const vector<T>& objects)
{
    file << objects.size() << '\n';
    for (const T& object : objects) file << object;
}
template <typename T>
bool LoadObjects(ifstream& file, vector<T>& objects)
{
    string line;
    int count = 0;
    if (!getline(file, line) || !ParseNumber(line, count) || count < 0) return false;
    for (int i = 0; i < count; i++)
    {
        if (!getline(file, line)) return false;
        istringstream input(line);
        T object;
        if (!(input >> object)) return false;
        input >> ws;
        if (!input.eof() || FindIndex(objects, object.id) != -1) return false;
        objects.push_back(object);
    }
    return true;
}
