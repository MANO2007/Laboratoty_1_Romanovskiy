#include <iostream>
#include <string>

using namespace std;

int main()
{
    int x = 1;
    while (x != 0) {
        cout << "Enter a digit from 0 to 7, where each digit corresponds to: \n1. Add pipe\n 2. Add compressor station\n 3. View all objects\n 4. Edit pipe\n 5. Edit compressor station\n 6. Save\n 7. Load\n 0. Exit\n";
        cin >> x;
        switch (x) {
        case 1:
            cout << "\n";
            break;
        case 2:
            cout << "\n";
            break;
        case 3:
            cout << "\n";
            break;
        case 4:
            cout << "\n";
            break;
        case 5:
            cout << "\n";
            break;
        case 6:
            cout << "\n";
            break;
        case 7:
            cout << "\n";
            break;
        case 0:
            cout << "\n";
            break;
        }
    }


    
}
struct Pipe
{
    string mark;
    double length;
    double diametr;
    string sign;


};
struct CompressorStation
{
    string name;
    int count_shops;
    int count_shops_in_work;
    char station_klass;
};
void AddPipe()
{

};
void AddCompressorStation()
{

};
void ShowAllObjects() {

};
void ChangePipe() {

};
void ChangeCompessorStation() {

};
void Save() {

};
void Load() {

};
