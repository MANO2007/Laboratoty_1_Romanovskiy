#include "CompressorStation.h"
#include "Utils.h"

int CompressorStation::nextId = 1;

CompressorStation::CompressorStation()
    : id(nextId++), name(""), numberOfWorkshops(0), operatingWorkshops(0), stationClass(0) {
}

void CompressorStation::updateNextId() {
    if (id >= nextId) nextId = id + 1;
}

int CompressorStation::getId() const { return id; }
std::string CompressorStation::getName() const { return name; }

std::istream& operator>>(std::istream& input, CompressorStation& station) {
    std::string name;
    int workshops = 0, stationClass = 0;
    if (readLine(input, "Введите название кс: ", name) &&
        readNumber(input, "Количество цехов: ", workshops, true) &&
        readNumber(input, "Класс станции: ", stationClass, true)) {
        station.name = name;
        station.numberOfWorkshops = workshops;
        station.operatingWorkshops = 0;
        station.stationClass = stationClass;
        LOG("station_input id=" + std::to_string(station.id));
    }
    return input;
}

std::ostream& operator<<(std::ostream& output, const CompressorStation& station) {
    output << "\n КС с Id" << station.id << "\n"
        << "Название: " << station.name << "\n"
        << "Количество цехов: " << station.numberOfWorkshops << "\n"
        << "Количество работающих цехов: " << station.operatingWorkshops << "\n"
        << "Класс станции: " << station.stationClass << "\n"
        << "Станция " << (station.operatingWorkshops > 0 ? "Работает " : "Простаивает") << "\n";
    return output;
}

void CompressorStation::print() const {
    std::cout << *this;
}

void CompressorStation::startWorkshop() {
    if (operatingWorkshops < numberOfWorkshops) {
        int old = operatingWorkshops;
        operatingWorkshops++;
        LOG("workshops id=" + std::to_string(id) + " old=" + std::to_string(old) +
            " new=" + std::to_string(operatingWorkshops));
        std::cout << "Цех запущен.\n";
    }
    else {
        LOG("workshop_limit id=" + std::to_string(id));
        std::cout << "Нельзя, все цеха в работе\n";
    }
}

void CompressorStation::stopWorkshop() {
    if (operatingWorkshops > 0) {
        int old = operatingWorkshops;
        operatingWorkshops--;
        LOG("workshops id=" + std::to_string(id) + " old=" + std::to_string(old) +
            " new=" + std::to_string(operatingWorkshops));
        std::cout << "Цех остановлен.\n";
    }
    else {
        LOG("workshop_limit id=" + std::to_string(id));
        std::cout << "Нельзя, нет работающих цехов\n";
    }
}

void CompressorStation::saveToFile(std::ofstream& file) const {
    file << id << "\n" << name << "\n" << numberOfWorkshops << "\n" << operatingWorkshops << "\n" << stationClass << "\n";
}

bool CompressorStation::loadFromFile(std::ifstream& file) {
    if (!(file >> id)) return false;
    file.ignore();
    if (!std::getline(file, name)) return false;
    if (!(file >> numberOfWorkshops)) return false;
    if (!(file >> operatingWorkshops)) return false;
    if (!(file >> stationClass)) return false;
    file.ignore();

    updateNextId();
    return true;
}

double CompressorStation::getIdlePercent() const {
    if (numberOfWorkshops == 0) return 0;
    return (numberOfWorkshops - operatingWorkshops) * 100.0 / numberOfWorkshops;
}

bool CompressorStation::matchesName(const std::string& query) const {
    if (query.empty()) return false;
    return name.rfind(query, 0) == 0;
}
bool CompressorStation::matchesIdlePercent(double minPercent) const {
    return getIdlePercent() >= minPercent;
}
