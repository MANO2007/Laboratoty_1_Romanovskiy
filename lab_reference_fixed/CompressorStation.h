#pragma once
#include <fstream>
#include <iostream>
#include <string>

class CompressorStation {
    static int nextId;
    int id;
    std::string name;
    int numberOfWorkshops;
    int operatingWorkshops;
    int stationClass;
    void updateNextId();
public:
    CompressorStation();
    int getId() const;
    std::string getName() const;
    void print() const;
    void startWorkshop();
    void stopWorkshop();
    void saveToFile(std::ofstream& file) const;
    bool loadFromFile(std::ifstream& file);
    double getIdlePercent() const;
    bool matchesName(const std::string& query) const;
    bool matchesIdlePercent(double minPercent) const;
    friend std::istream& operator>>(std::istream& input, CompressorStation& station);
    friend std::ostream& operator<<(std::ostream& output, const CompressorStation& station);
};
