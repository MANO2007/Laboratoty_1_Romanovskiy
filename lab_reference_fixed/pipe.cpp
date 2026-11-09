#include "Pipe.h"
#include "Utils.h"

int Pipe::nextId = 1;

Pipe::Pipe()
    : id(nextId++), name(""), length(0.0), diameter(0.0), repairing(false) {
}

void Pipe::updateNextId() {
    if (id >= nextId) nextId = id + 1;
}

int Pipe::getId() const { return id; }
std::string Pipe::getName() const { return name; }
bool Pipe::isRepairing() const { return repairing; }

void Pipe::setRepairing(bool newRepairing) {
    LOG("pipe_repair id=" + std::to_string(id) + " old=" + std::to_string(repairing) +
        " new=" + std::to_string(newRepairing));
    repairing = newRepairing;
}

std::istream& operator>>(std::istream& input, Pipe& pipe) {
    std::string name;
    double length = 0, diameter = 0;
    if (readLine(input, "Введите название трубы:  ", name) &&
        readNumber(input, "Введите длину (км): ", length, true) &&
        readNumber(input, "Введите диаметр (мм): ", diameter, true)) {
        pipe.name = name;
        pipe.length = length;
        pipe.diameter = diameter;
        pipe.repairing = false;
        LOG("pipe_input id=" + std::to_string(pipe.id));
    }
    return input;
}

std::ostream& operator<<(std::ostream& output, const Pipe& pipe) {
    output << "\nТруба с Id" << pipe.id << "\n"
        << "Название: " << pipe.name << "\n"
        << "Длина: " << pipe.length << " км\n"
        << "Диаметр: " << pipe.diameter << " мм\n"
        << "Состояние: " << (pipe.repairing ? "В ремонте" : "В работе") << "\n";
    return output;
}

void Pipe::print() const {
    std::cout << *this;
}

void Pipe::inTheRepair() {
    setRepairing(!repairing);
    std::cout << "Признак изменён. Труба теперь: " << (repairing ? "В ремонте" : "В работе") << "\n";
}

void Pipe::saveToFile(std::ofstream& file) const {
    file << id << "\n" << name << "\n" << length << "\n" << diameter << "\n" << repairing << "\n";
}

bool Pipe::loadFromFile(std::ifstream& file) {
    if (!(file >> id)) return false;
    file.ignore();
    if (!std::getline(file, name)) return false;
    if (!(file >> length)) return false;
    if (!(file >> diameter)) return false;
    if (!(file >> repairing)) return false;
    file.ignore();

    updateNextId();
    return true;
}

bool Pipe::matchesName(const std::string& query) const {
    if (query.empty()) return false;
    return name.rfind(query, 0) == 0;
}

bool Pipe::matchesRepair(bool repairState) const {
    return repairing == repairState;
}
