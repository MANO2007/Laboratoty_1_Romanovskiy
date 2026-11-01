#include "main.hpp"
void WriteLog(const char* filename, int line, const char* function, const string& message)
{
    time_t now = time(nullptr);
    tm currentTime{};
#ifdef _WIN32
    localtime_s(&currentTime, &now);
#else
    currentTime = *localtime(&now);
#endif
    string text = message;
    for (char& symbol : text)
    {
        if (symbol == '\n' || symbol == '\r') symbol = ' ';
    }
    clog << put_time(&currentTime, "%Y-%m-%d %H:%M:%S")
         << " | " << filename << ':' << line << " | " << function
         << " | " << text << endl;
    if (!clog) throw runtime_error("Could not write to log.txt.");
}
string ReadLine(const string& message)
{
    cout << message;
    string text;
    if (!getline(cin, text)) throw InputClosed();
    LogAction("Input [" + message + "]: " + text);
    return text;
}
string ReadText(const string& message)
{
    while (true)
    {
        string text = ReadLine(message);
        if (text.find_first_not_of(" \t\r") != string::npos) return text;
        cout << "The text cannot be empty.\n";
        LogAction("Rejected empty text");
    }
}
string ReadFilename(const string& message)
{
    while (true)
    {
        string name = ReadText(message);
        // Используем только имя файла в рабочей папке, без пути.
        if (name.find_first_of("/\\:*?\"<>|") != string::npos ||
            name.back() == '.' || name.back() == ' ')
        {
            cout << "Enter a filename without a folder path.\n";
            LogAction("Rejected filename");
            continue;
        }
        string lowerName = name;
        for (char& symbol : lowerName)
        {
            if (symbol >= 'A' && symbol <= 'Z') symbol = symbol - 'A' + 'a';
        }
        if (lowerName == "log.txt")
        {
            cout << "Choose a file other than log.txt.\n";
            LogAction("Rejected filename: log.txt is the journal");
            continue;
        }
        return name;
    }
}
char ReadStationClass()
{
    while (true)
    {
        string value = ReadLine("Enter station class (A, B or C): ");
        if (value == "A" || value == "B" || value == "C") return value[0];
        cout << "Station class must be A, B or C.\n";
        LogAction("Rejected station class");
    }
}
double CompressorStation::UnusedPercent() const
{
    if (countShops == 0) return 0;
    return 100.0 * (countShops - countShopsInWork) / countShops;
}
void AddPipe(vector<Pipe>& pipes, int& nextId)
{
    if (nextId == numeric_limits<int>::max())
    {
        cout << "No available IDs.\n";
        LogAction("Add rejected: ID limit");
        return;
    }
    Pipe pipe;
    cin >> pipe;
    pipe.id = nextId;
    pipes.push_back(pipe);
    nextId++;
    cout << "Added pipe ID: " << pipe.id << '\n';
    LogAction("Added pipe ID=" + to_string(pipe.id));
}
void AddCompressorStation(vector<CompressorStation>& stations, int& nextId)
{
    if (nextId == numeric_limits<int>::max())
    {
        cout << "No available IDs.\n";
        LogAction("Add rejected: ID limit");
        return;
    }
    CompressorStation station;
    cin >> station;
    station.id = nextId;
    stations.push_back(station);
    nextId++;
    cout << "Added station ID: " << station.id << '\n';
    LogAction("Added station ID=" + to_string(station.id));
}
void ShowObject(const Pipe& pipe)
{
    cout << "\nPipe ID: " << pipe.id << '\n';
    cout << "Kilometer mark: " << pipe.kilometerMark << '\n';
    cout << "Length: " << pipe.length << " km\n";
    cout << "Diameter: " << pipe.diameter << " mm\n";
    cout << "Under repair: " << (pipe.isUnderRepair ? "Yes" : "No") << '\n';
}
void ShowObject(const CompressorStation& station)
{
    cout << "\nStation ID: " << station.id << '\n';
    cout << "Name: " << station.name << '\n';
    cout << "Total shops: " << station.countShops << '\n';
    cout << "Working shops: " << station.countShopsInWork << '\n';
    cout << "Station class: " << station.stationClass << '\n';
    cout << "Unused shops: " << station.UnusedPercent() << "%\n";
}
vector<int> FindPipes(const vector<Pipe>& pipes)
{
    int filter = ReadNumber<int>("1. By name\n2. By repair status\n3. All pipes\n0. Cancel\nFilter: ", 0, 3);
    vector<int> ids;
    if (filter == 0) return ids;
    string name;
    bool repair = false;
    if (filter == 1) name = ReadText("Name fragment: ");
    if (filter == 2) repair = ReadNumber<int>("Under repair (0/1): ", 0, 1);
    for (const Pipe& pipe : pipes)
    {
        if (filter == 3 ||
            (filter == 1 && pipe.kilometerMark.find(name) != string::npos) ||
            (filter == 2 && pipe.isUnderRepair == repair))
        {
            ids.push_back(pipe.id);
        }
    }
    return ids;
}
vector<int> FindStations(const vector<CompressorStation>& stations)
{
    int filter = ReadNumber<int>("1. By name\n2. By unused shops percentage\n3. All stations\n0. Cancel\nFilter: ", 0, 3);
    vector<int> ids;
    if (filter == 0) return ids;
    string name;
    double minPercent = 0;
    double maxPercent = 100;
    if (filter == 1) name = ReadText("Name fragment: ");
    if (filter == 2)
    {
        minPercent = ReadNumber<double>("Minimum unused percentage: ", 0.0, 100.0);
        maxPercent = ReadNumber<double>("Maximum unused percentage: ", minPercent, 100.0);
    }
    for (const CompressorStation& station : stations)
    {
        if (filter == 3 ||
            (filter == 1 && station.name.find(name) != string::npos) ||
            (filter == 2 && station.UnusedPercent() >= minPercent && station.UnusedPercent() <= maxPercent))
        {
            ids.push_back(station.id);
        }
    }
    return ids;
}
void SearchObjects(const vector<Pipe>& pipes, const vector<CompressorStation>& stations)
{
    int action = ReadNumber<int>("1. View all objects\n2. Find pipes\n3. Find compressor stations\n0. Cancel\nSelect an action: ", 0, 3);
    if (action == 1)
    {
        cout << "\nPipes\n";
        ShowObjects(pipes);
        cout << "\nCompressor stations\n";
        ShowObjects(stations);
        LogAction("Viewed all objects");
    }
    else if (action == 2) ShowFound(pipes, FindPipes(pipes));
    else if (action == 3) ShowFound(stations, FindStations(stations));
}
ostream& operator<<(ostream& file, const Pipe& pipe)
{
    file << pipe.id << ' ' << quoted(pipe.kilometerMark) << ' ' << pipe.length << ' '
         << pipe.diameter << ' ' << pipe.isUnderRepair << '\n';
    return file;
}
ostream& operator<<(ostream& file, const CompressorStation& station)
{
    file << station.id << ' ' << quoted(station.name) << ' ' << station.countShops << ' '
         << station.countShopsInWork << ' ' << station.stationClass << '\n';
    return file;
}
bool LoadObject(istream& file, Pipe& pipe)
{
    int repair = 0;
    if (!(file >> pipe.id >> quoted(pipe.kilometerMark) >> pipe.length >> pipe.diameter >> repair))
        return false;
    if (pipe.id <= 0 || pipe.id == numeric_limits<int>::max() ||
        pipe.kilometerMark.find_first_not_of(" \t\r") == string::npos ||
        !isfinite(pipe.length) || !isfinite(pipe.diameter) ||
        pipe.length <= 0 || pipe.diameter <= 0 || (repair != 0 && repair != 1))
        return false;
    pipe.isUnderRepair = repair == 1;
    return true;
}
bool LoadObject(istream& file, CompressorStation& station)
{
    if (!(file >> station.id >> quoted(station.name) >> station.countShops >>
        station.countShopsInWork >> station.stationClass)) return false;
    return station.id > 0 && station.id < numeric_limits<int>::max() &&
        station.name.find_first_not_of(" \t\r") != string::npos &&
        station.countShops > 0 && station.countShopsInWork >= 0 &&
        station.countShopsInWork <= station.countShops &&
        (station.stationClass == 'A' || station.stationClass == 'B' || station.stationClass == 'C');
}
void SaveData(const vector<Pipe>& pipes, const vector<CompressorStation>& stations, int nextId)
{
    string filename = ReadFilename("Save filename (for example, data.txt): ");
    ofstream file(filename);
    if (!file)
    {
        cout << "Could not open file for writing.\n";
        LogAction("Save failed: " + filename);
        return;
    }
    file << setprecision(numeric_limits<double>::max_digits10);
    file << "GAS_TRANSPORT_V2\n" << nextId << '\n';
    SaveObjects(file, pipes);
    SaveObjects(file, stations);
    file.close();
    if (!file)
    {
        cout << "File writing failed.\n";
        LogAction("Save failed while writing: " + filename);
        return;
    }
    cout << "Data saved.\n";
    LogAction("Saved " + filename);
}
void LoadData(vector<Pipe>& pipes, vector<CompressorStation>& stations, int& nextId)
{
    string filename = ReadFilename("Load filename (for example, data.txt): ");
    ifstream file(filename);
    if (!file)
    {
        cout << "Could not open file for reading.\n";
        LogAction("Load failed: " + filename);
        return;
    }
    // Проверяем данные отдельно, чтобы повреждённый файл не испортил текущие объекты.
    vector<Pipe> loadedPipes;
    vector<CompressorStation> loadedStations;
    string header;
    string line;
    int loadedNextId = 0;
    bool valid = getline(file, header) && header == "GAS_TRANSPORT_V2" &&
        getline(file, line) && ParseNumber(line, loadedNextId) && loadedNextId > 0 &&
        LoadObjects(file, loadedPipes) && LoadObjects(file, loadedStations);
    if (valid)
    {
        file >> ws;
        valid = file.eof() && !file.bad();
        for (const Pipe& pipe : loadedPipes)
        {
            if (pipe.id >= loadedNextId || FindIndex(loadedStations, pipe.id) != -1) valid = false;
        }
        for (const CompressorStation& station : loadedStations)
        {
            if (station.id >= loadedNextId) valid = false;
        }
    }
    if (!valid)
    {
        cout << "Invalid file. Current objects were preserved.\n";
        LogAction("Load rejected: invalid file " + filename);
        return;
    }
    pipes = loadedPipes;
    stations = loadedStations;
    if (loadedNextId > nextId) nextId = loadedNextId;
    cout << "Data loaded.\n";
    LogAction("Loaded " + filename);
}
// При вводе из консоли задаём вопросы. Из файла читаем сохранённую запись.
istream& operator>>(istream& input, Pipe& pipe)
{
    Pipe value;
    if (&input == &cin)
    {
        value.kilometerMark = ReadText("Enter kilometer mark: ");
        value.length = ReadNumber<double>("Enter pipe length in kilometers: ", 0.0,
            numeric_limits<double>::max(), true);
        value.diameter = ReadNumber<double>("Enter pipe diameter in millimeters: ", 0.0,
            numeric_limits<double>::max(), true);
        value.isUnderRepair = ReadNumber<int>("Is the pipe under repair? (1 - yes, 0 - no): ", 0, 1);
        value.id = pipe.id;
    }
    else if (!LoadObject(input, value))
    {
        LogAction("Rejected pipe record: incorrect format or values");
        input.setstate(ios::failbit);
        return input;
    }
    pipe = value;
    LogAction("Pipe fields read successfully; current ID=" + to_string(pipe.id));
    return input;
}
istream& operator>>(istream& input, CompressorStation& station)
{
    CompressorStation value;
    if (&input == &cin)
    {
        value.name = ReadText("Enter station name: ");
        value.countShops = ReadNumber<int>("Enter total number of shops: ", 1, numeric_limits<int>::max());
        value.countShopsInWork = ReadNumber<int>("Enter number of working shops: ", 0, value.countShops);
        value.stationClass = ReadStationClass();
        value.id = station.id;
    }
    else if (!LoadObject(input, value))
    {
        LogAction("Rejected station record: incorrect format or values");
        input.setstate(ios::failbit);
        return input;
    }
    station = value;
    LogAction("Station fields read successfully; current ID=" + to_string(station.id));
    return input;
}
Manager::Manager() : logFile("log.txt", ios::app)
{
    if (!logFile) throw runtime_error("Could not open log.txt.");
    // clog теперь записывает технические сообщения в файл вместо консоли.
    oldLogBuffer = clog.rdbuf(logFile.rdbuf());
    clog.clear();
}
Manager::~Manager()
{
    clog.flush();
    clog.rdbuf(oldLogBuffer);
    clog.clear();
}
void Manager::ManagePipes()
{
    vector<int> found = FindPipes(pipes);
    ShowFound(pipes, found);
    if (found.empty()) return;
    vector<int> selected = SelectObjects(pipes, found);
    if (selected.empty()) return;
    int action = ReadNumber<int>("1. Change repair status\n2. Delete selected\n0. Cancel\nAction: ", 0, 2);
    if (action == 0) return;
    bool repair = false;
    if (action == 1) repair = ReadNumber<int>("New repair status (0/1): ", 0, 1);
    for (int id : selected)
    {
        int index = FindIndex(pipes, id);
        if (action == 1)
        {
            bool oldStatus = pipes[index].isUnderRepair;
            pipes[index].isUnderRepair = repair;
            LogAction("Batch edit pipe ID=" + to_string(id) +
                " old=" + to_string(oldStatus) + " new=" + to_string(repair));
        }
        else
        {
            LogAction("Deleted object ID=" + to_string(id) + " type=Pipe");
            pipes.erase(pipes.begin() + index);
        }
    }
    cout << "Processed pipes: " << selected.size() << '\n';
}
void Manager::ManageStations()
{
    vector<int> found = FindStations(stations);
    ShowFound(stations, found);
    if (found.empty()) return;
    vector<int> selected = SelectObjects(stations, found);
    if (selected.empty()) return;
    int action = ReadNumber<int>("1. Start one shop\n2. Stop one shop\n3. Delete selected\n0. Cancel\nAction: ", 0, 3);
    if (action == 0) return;
    int count = 0;
    for (int id : selected)
    {
        int index = FindIndex(stations, id);
        CompressorStation& station = stations[index];
        int oldWorking = station.countShopsInWork;
        if (action == 3)
        {
            LogAction("Deleted object ID=" + to_string(id) + " type=CompressorStation");
            stations.erase(stations.begin() + index);
            count++;
            continue;
        }
        if (action == 1 && oldWorking < station.countShops) station.countShopsInWork++;
        else if (action == 2 && oldWorking > 0) station.countShopsInWork--;
        else
        {
            cout << "Cannot perform action: shop limit reached. ID: " << id << '\n';
            LogAction("Station action rejected ID=" + to_string(id) + " action=" + to_string(action));
            continue;
        }
        count++;
        LogAction("Changed station ID=" + to_string(id) +
            " old=" + to_string(oldWorking) + " new=" + to_string(station.countShopsInWork));
    }
    cout << "Processed stations: " << count << '\n';
}
void Manager::Run()
{
    LogAction("Session started");
    try
    {
        int command = -1;
        while (command != 0)
        {
            command = ReadNumber<int>(
                "\n1. Add pipe\n2. Add compressor station\n3. View/search objects\n"
                "4. Find and manage pipes\n5. Find and manage compressor stations\n"
                "6. Save\n7. Load\n0. Exit\nSelect an action: ", 0, 7);
            LogAction("Command=" + to_string(command));
            switch (command)
            {
            case 1: AddPipe(pipes, nextId); break;
            case 2: AddCompressorStation(stations, nextId); break;
            case 3: SearchObjects(pipes, stations); break;
            case 4: ManagePipes(); break;
            case 5: ManageStations(); break;
            case 6: SaveData(pipes, stations, nextId); break;
            case 7: LoadData(pipes, stations, nextId); break;
            case 0: break;
            }
        }
        LogAction("Session ended by user");
    }
    catch (const InputClosed&)
    {
        LogAction("Session ended: input closed");
    }
    cout << "Program finished.\n";
}
