#pragma once
#include <fstream>
#include <iostream>
#include <string>

class Pipe {
    static int nextId;
    int id;
    std::string name;
    double length;
    double diameter;
    bool repairing;
    void updateNextId();
public:
    Pipe();
    int getId() const;
    std::string getName() const;
    bool isRepairing() const;
    void setRepairing(bool newRepairing);
    void print() const;
    void inTheRepair();
    void saveToFile(std::ofstream& file) const;
    bool loadFromFile(std::ifstream& file);
    bool matchesName(const std::string& query) const;
    bool matchesRepair(bool repairState) const;
    friend std::istream& operator>>(std::istream& input, Pipe& pipe);
    friend std::ostream& operator<<(std::ostream& output, const Pipe& pipe);
};
