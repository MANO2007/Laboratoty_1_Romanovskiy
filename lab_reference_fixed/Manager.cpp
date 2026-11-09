#include "Manager.h"
#include "Utils.h"
#include <filesystem>
#include <iostream>
#ifdef _WIN32
#include <windows.h>
#endif

void managePipes(std::map<int, Pipe>& pipes) {
    if (pipes.empty()) {
        std::cout << "Нет труб для работы.\n";
        return;
    }

    std::vector<int> foundIds = findPipes(pipes);
    if (foundIds.empty()) {
        std::cout << "Ничего не найдено.\n";
        return;
    }

    std::cout << "Найдено труб: " << foundIds.size() << "\n";
    for (int id : foundIds) {
        pipes.at(id).print();
    }

    int action = readInt("\nДействие:\n1-В ремонт\n2-Из ремонта\n3-Удалить\n0-Отмена\nВыбор: ");
    if (action == 0) return;

    int count = 0;
    for (int id : foundIds) {
        if (action == 1) { pipes.at(id).setRepairing(true); count++; }
        else if (action == 2) { pipes.at(id).setRepairing(false); count++; }
        else if (action == 3) {
            pipes.erase(id);
            LOG("delete pipes id=" + std::to_string(id));
            count++;
        }
    }
    std::cout << "Обработано труб: " << count << "\n";
}

void manageStations(std::map<int, CompressorStation>& stations) {
    if (stations.empty()) {
        std::cout << "Нет КС для работы.\n";
        return;
    }

    std::vector<int> foundIds = findStations(stations);
    if (foundIds.empty()) {
        std::cout << "Ничего не найдено.\n";
        return;
    }

    std::cout << "Найдено КС: " << foundIds.size() << "\n";
    for (int id : foundIds) {
        stations.at(id).print();
    }

    int action = readInt("\nДействие:\n1-Запустить цех\n2-Остановить цех\n3-Удалить\n0-Отмена\nВыбор: ");
    if (action == 0) return;

    int count = 0;
    for (int id : foundIds) {
        if (action == 1) { stations.at(id).startWorkshop(); count++; }
        else if (action == 2) { stations.at(id).stopWorkshop(); count++; }
        else if (action == 3) {
            stations.erase(id);
            LOG("delete stations id=" + std::to_string(id));
            count++;
        }
    }
    std::cout << "Обработано КС: " << count << "\n";
}

namespace {
int menu() {
    std::map<int, Pipe> pipe;
    std::map<int, CompressorStation> compressorStation;

    while (true) {
        std::cout << "\nМеню\n"
            << "1. Добавить трубу\n"
            << "2. Добавить кс\n"
            << "3. Просмотр всех объектов\n"
            << "4. Поиск и управление объектами\n"
            << "5. Сохранить в файл\n"
            << "6. Загрузить из файла\n"
            << "0. Выход\n\n";

        int choice = readInt("Ваш выбор: \n");

        switch (choice) {
        case 1: {
            Pipe newPipe;
            std::cin >> newPipe;
            pipe.emplace(newPipe.getId(), newPipe);
            std::cout << "Труба добавлена (Id" << newPipe.getId() << ")\n";
            LOG("add_pipe id=" + std::to_string(newPipe.getId()));
            break;
        }
        case 2: {
            CompressorStation newCs;
            std::cin >> newCs;
            compressorStation.emplace(newCs.getId(), newCs);
            std::cout << "Добавлена кс (Id" << newCs.getId() << ")\n";
            LOG("add_station id=" + std::to_string(newCs.getId()));
            break;
        }
        case 3: {
            if (pipe.empty() && compressorStation.empty()) {
                std::cout << "Пока ничего не введено\n";
            }
            else {
                for (const auto& pair : pipe) pair.second.print();
                for (const auto& pair : compressorStation) pair.second.print();
            }
            break;
        }
        case 4: {
            int type = readInt("С чем работаем? (1 - Трубы, 2 - КС): ");
            if (type == 1) {
                managePipes(pipe);
                LOG("manage_pipes");
            }
            else if (type == 2) {
                manageStations(compressorStation);
                LOG("manage_stations");
            }
            else std::cout << "Неверный выбор.\n";
            break;
        }
        case 5: {
            std::string filename;
            readLine(std::cin, "Введите имя файла для сохранения: ", filename);
            std::error_code error;
            if (std::filesystem::equivalent(filename, "log.txt", error)) {
                std::cout << "Нельзя сохранять данные в журнал.\n";
                LOG("save_rejected log_file");
                break;
            }
            std::ofstream file(filename);
            if (!file.is_open()) { LOG("save_open_failed file=" + filename); std::cout << "Ошибка открытия файла\n"; break; }

            file << pipe.size() << "\n";
            for (auto& pair : pipe) pair.second.saveToFile(file);
            file << compressorStation.size() << "\n";
            for (auto& pair : compressorStation) pair.second.saveToFile(file);

            file.close();
            if (!file) {
                LOG("save_write_failed file=" + filename);
                std::cout << "Ошибка записи файла\n";
                break;
            }
            std::cout << "Данные сохранены\n";
            LOG("save file=" + filename);
            break;
        }
        case 6: {
            std::string filename;
            readLine(std::cin, "Введите имя файла для загрузки: ", filename);
            std::ifstream file(filename);
            if (!file.is_open()) { LOG("load_open_failed file=" + filename); std::cout << "Файл не найден\n"; break; }

            int pipeCount, csCount;
            if (!(file >> pipeCount)) { LOG("load_invalid file=" + filename); std::cout << "Ошибка: файл поврежден!\n"; break; }
            file.ignore();

            pipe.clear();
            compressorStation.clear();
            bool isBroken = false;

            for (int i = 0; i < pipeCount; ++i) {
                Pipe p;
                if (!p.loadFromFile(file)) { isBroken = true; break; }
                pipe.emplace(p.getId(), p);
            }
            if (isBroken) { LOG("load_invalid file=" + filename); std::cout << "Ошибка чтения труб!\n"; pipe.clear(); break; }

            if (!(file >> csCount)) { LOG("load_invalid file=" + filename); std::cout << "Ошибка: файл поврежден!\n"; pipe.clear(); break; }
            file.ignore();

            for (int i = 0; i < csCount; ++i) {
                CompressorStation cs;
                if (!cs.loadFromFile(file)) { isBroken = true; break; }
                compressorStation.emplace(cs.getId(), cs);
            }
            if (isBroken) {
                LOG("load_invalid file=" + filename); std::cout << "Ошибка чтения КС!\n";
                pipe.clear();
                compressorStation.clear();
                break;
            }

            file.close();
            std::cout << "Загружено труб: " << pipe.size() << ", КС: " << compressorStation.size() << "\n";
            LOG("load file=" + filename);
            break;
        }
        case 0:
            std::cout << "Выход\n";
            LOG("session_end");
            return 0;
        default:
            std::cout << "Неверный пункт меню!\n";
        }
    }
    return 0;
}
}

int runProgram() {
#ifdef _WIN32
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);
#endif
    try {
        Logger logger;
        LOG("session_start");
        try {
            return menu();
        }
        catch (const InputClosed&) {
            LOG("session_end input_closed");
            return 0;
        }
    }
    catch (const std::exception& error) {
        std::cerr << error.what() << '\n';
        return 1;
    }
}
