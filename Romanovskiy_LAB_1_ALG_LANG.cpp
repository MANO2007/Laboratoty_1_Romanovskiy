#include <iostream>
#include <string>
#include <fstream>
using namespace std;
struct Pipe
{
    string kilometerMark;
    double length = 0;
    double diameter = 0;
    bool isUnderRepair = false;
};
struct CompressorStation
{
    string name;
    int countShops = 0;
    int countShopsInWork = 0;
    char stationClass = '-';
};
int ReadInteger(const string& message, int minValue, int maxValue)
{
    int value;
    while (true)
    {
        cout << message;
        if (!(cin >> value) || cin.peek() != '\n')
        {
            cout << "Incorrect input. Enter one integer number.\n";
            cin.clear();
            cin.ignore(10000, '\n');
            continue;
        }
        cin.ignore(10000, '\n');
        if (value < minValue || value > maxValue)
        {
            cout << "Enter a number from " << minValue << " to " << maxValue << ".\n";
            continue;
        }
        return value;
    }
}
double ReadPositiveNumber(const string& message)
{
    double value;
    while (true)
    {
        cout << message;
        if (!(cin >> value) || cin.peek() != '\n')
        {
            cout << "Incorrect input. Enter one number.\n";
            cin.clear();
            cin.ignore(10000, '\n');
            continue;
        }
        cin.ignore(10000, '\n');
        if (value <= 0)
        {
            cout << "The number must be greater than zero.\n";
            continue;
        }
        return value;
    }
}
char ReadStationClass()
{
    char stationClass;
    while (true)
    {
        cout << "Enter station class (A, B or C): ";
        if (!(cin >> stationClass) || cin.peek() != '\n')
        {
            cout << "Enter one character.\n";
            cin.clear();
            cin.ignore(10000, '\n');
            continue;
        }
        cin.ignore(10000, '\n');
        if (stationClass == 'A' || stationClass == 'B' || stationClass == 'C')
        {
            return stationClass;
        }
        cout << "Station class must be A, B or C.\n";
    }
}
void AddPipe(Pipe& pipe)
{
    cout << "\nAdding a pipe\n";
    do
    {
        cout << "Enter kilometer mark: ";
        getline(cin, pipe.kilometerMark);
        if (pipe.kilometerMark.empty())
        {
            cout << "The kilometer mark cannot be empty.\n";
        }
    } while (pipe.kilometerMark.empty());
    pipe.length = ReadPositiveNumber("Enter pipe length in kilometers: ");
    pipe.diameter = ReadPositiveNumber("Enter pipe diameter in millimeters: ");
    pipe.isUnderRepair = ReadInteger("Is the pipe under repair? (1 - yes, 0 - no): ", 0, 1);
    cout << "Pipe added successfully.\n";
}
void AddCompressorStation(CompressorStation& station)
{
    cout << "\nAdding a compressor station\n";
    do
    {
        cout << "Enter station name: ";
        getline(cin, station.name);
        if (station.name.empty())
        {
            cout << "The station name cannot be empty.\n";
        }
    } while (station.name.empty());
    station.countShops = ReadInteger("Enter total number of shops: ", 1, 10000);
    station.countShopsInWork = ReadInteger("Enter number of working shops: ", 0, station.countShops);
    station.stationClass = ReadStationClass();
    cout << "Compressor station added successfully.\n";
}
void ShowPipe(const Pipe& pipe)
{
    cout << "\nPipe\n";
    cout << "Kilometer mark: " << pipe.kilometerMark << '\n';
    cout << "Length: " << pipe.length << " km\n";
    cout << "Diameter: " << pipe.diameter << " mm\n";
    cout << "Under repair: " << (pipe.isUnderRepair ? "Yes" : "No") << '\n';
}
void ShowCompressorStation(const CompressorStation& station)
{
    cout << "\nCompressor station\n";
    cout << "Name: " << station.name << '\n';
    cout << "Total shops: " << station.countShops << '\n';
    cout << "Working shops: " << station.countShopsInWork << '\n';
    cout << "Station class: " << station.stationClass << '\n';
}
void ShowAllObjects(const Pipe& pipe, bool pipeExists, const CompressorStation& station, bool stationExists)
{
    if (pipeExists)
    {
        ShowPipe(pipe);
    }
    else
    {
        cout << "\nPipe has not been added yet.\n";
    }
    if (stationExists)
    {
        ShowCompressorStation(station);
    }
    else
    {
        cout << "\nCompressor station has not been added yet.\n";
    }
}
void ChangePipe(Pipe& pipe)
{
    cout << "\nEditing pipe\n";
    cout << "Current repair status: " << (pipe.isUnderRepair ? "Yes" : "No") << '\n';
    pipe.isUnderRepair = ReadInteger("Enter new status (1 - repair, 0 - not repair): ", 0, 1);
    cout << "Pipe status changed successfully.\n";
}
void ChangeCompressorStation(CompressorStation& station)
{
    cout << "\nEditing compressor station\n";
    cout << "Working shops: " << station.countShopsInWork << " of " << station.countShops << '\n';
    int action = ReadInteger("1. Start one shop\n2. Stop one shop\n0. Cancel\nSelect an action: ", 0, 2);
    if (action == 1)
    {
        if (station.countShopsInWork == station.countShops)
        {
            cout << "All shops are already working.\n";
        }
        else
        {
            station.countShopsInWork++;
            cout << "One shop has been started.\n";
        }
    }
    else if (action == 2)
    {
        if (station.countShopsInWork == 0)
        {
            cout << "There are no working shops to stop.\n";
        }
        else
        {
            station.countShopsInWork--;
            cout << "One shop has been stopped.\n";
        }
    }
    else
    {
        cout << "Editing cancelled.\n";
    }
}
void SaveData(const Pipe& pipe, bool pipeExists, const CompressorStation& station, bool stationExists)
{
    if (!pipeExists && !stationExists)
    {
        cout << "There are no objects to save.\n";
        return;
    }
    ofstream file("data.txt");
    if (!file.is_open())
    {
        cout << "Error: could not open the file.\n";
        return;
    }
    file << pipeExists << '\n';
    if (pipeExists)
    {
        file << pipe.kilometerMark << '\n';
        file << pipe.length << '\n';
        file << pipe.diameter << '\n';
        file << pipe.isUnderRepair << '\n';
    }
    file << stationExists << '\n';
    if (stationExists)
    {
        file << station.name << '\n';
        file << station.countShops << '\n';
        file << station.countShopsInWork << '\n';
        file << station.stationClass << '\n';
    }
    file.close();
    cout << "Data saved successfully to data.txt.\n";
}
void LoadData(Pipe& pipe, bool& pipeExists, CompressorStation& station, bool& stationExists)
{
    ifstream file("data.txt");
    if (!file.is_open())
    {
        cout << "Error: data.txt was not found.\n";
        return;
    }
    Pipe loadedPipe;
    CompressorStation loadedStation;
    int loadedPipeExists;
    int loadedStationExists;
    int repairStatus;
    if (!(file >> loadedPipeExists) || (loadedPipeExists != 0 && loadedPipeExists != 1))
    {
        cout << "Error: incorrect pipe data in the file.\n";
        return;
    }
    file.ignore(10000, '\n');
    if (loadedPipeExists == 1)
    {
        getline(file, loadedPipe.kilometerMark);
        if (!(file >> loadedPipe.length) || !(file >> loadedPipe.diameter) || !(file >> repairStatus))
        {
            cout << "Error: incorrect pipe data in the file.\n";
            return;
        }
        if (loadedPipe.kilometerMark.empty() || loadedPipe.length <= 0 || loadedPipe.diameter <= 0 || (repairStatus != 0 && repairStatus != 1))
        {
            cout << "Error: incorrect pipe values in the file.\n";
            return;
        }
        loadedPipe.isUnderRepair = repairStatus == 1;
        file.ignore(10000, '\n');
    }
    if (!(file >> loadedStationExists) || (loadedStationExists != 0 && loadedStationExists != 1))
    {
        cout << "Error: incorrect station data in the file.\n";
        return;
    }
    file.ignore(10000, '\n');
    if (loadedStationExists == 1)
    {
        getline(file, loadedStation.name);
        if (!(file >> loadedStation.countShops) || !(file >> loadedStation.countShopsInWork) || !(file >> loadedStation.stationClass))
        {
            cout << "Error: incorrect station data in the file.\n";
            return;
        }
        if (loadedStation.name.empty() || loadedStation.countShops <= 0 || loadedStation.countShopsInWork < 0 || loadedStation.countShopsInWork > loadedStation.countShops || (loadedStation.stationClass != 'A' && loadedStation.stationClass != 'B' && loadedStation.stationClass != 'C'))
        {
            cout << "Error: incorrect station values in the file.\n";
            return;
        }
    }
    pipe = loadedPipe;
    station = loadedStation;
    pipeExists = loadedPipeExists == 1;
    stationExists = loadedStationExists == 1;
    file.close();
    cout << "Data loaded successfully from data.txt.\n";
}
int main()
{
    Pipe pipe;
    CompressorStation station;
    bool pipeExists = false;
    bool stationExists = false;
    while (true)
    {
        int command = ReadInteger("\n1. Add pipe\n2. Add compressor station\n3. View all objects\n4. Edit pipe\n5. Edit compressor station\n6. Save\n7. Load\n0. Exit\nSelect an action: ", 0, 7);
        switch (command)
        {
        case 1:
            AddPipe(pipe);
            pipeExists = true;
            break;
        case 2:
            AddCompressorStation(station);
            stationExists = true;
            break;
        case 3:
            ShowAllObjects(pipe, pipeExists, station, stationExists);
            break;
        case 4:
            if (pipeExists)
            {
                ChangePipe(pipe);
            }
            else
            {
                cout << "The pipe has not been added yet.\n";
            }
            break;
        case 5:
            if (stationExists)
            {
                ChangeCompressorStation(station);
            }
            else
            {
                cout << "The compressor station has not been added yet.\n";
            }
            break;
        case 6:
            SaveData(pipe, pipeExists, station, stationExists);
            break;
        case 7:
            LoadData(pipe, pipeExists, station, stationExists);
            break;
        case 0:
            cout << "Program finished.\n";
            return 0;
        }
    }
}