#include <iostream>
#include <Windows.h>
#include <string>

using namespace ::std;

void getDrivesInfo();

string getDriveType(LPCWSTR);

string fromDecToHex(unsigned long);

string switchDecToHex(unsigned int);

int main() {
    setlocale(LC_ALL, "Rus");
    getDrivesInfo();
    return 0;
}

void getDrivesInfo() {
    int drives = GetLogicalDrives();
    const int alphabetSize = 26;
    for (int i = 0, p = 1; i < alphabetSize; i++, p = p << 1) {
        if ((drives & p) != 0) {
            char diskLetter = 'A' + i;
            string path = diskLetter + (string) ":\\";
            cout << "Диск: " << path << endl;
            wstring wPath = wstring(path.begin(), path.end());
            cout << "Тип: " << getDriveType(wPath.c_str()) << endl;

            char nameBuffer[100];
            char fileSystemBuffer[100];
            unsigned long driveSN;

            if (GetVolumeInformationA(path.c_str(), nameBuffer, sizeof(nameBuffer), &driveSN, nullptr, nullptr,
                                      fileSystemBuffer, sizeof(fileSystemBuffer))) {
                cout << "Метка тома: " << nameBuffer << endl;
                cout << "Файлова система: " << fileSystemBuffer << endl;
                cout << "Серийный номер: " << fromDecToHex(driveSN) << endl;
            } else {
                cout << "Ошибка при определении информации о метке тома, файловой системе и серийного номера" << endl;
            }

            DWORD secPerClus;
            DWORD bytePerSec;
            DWORD freeClus;
            DWORD totalClus;
            if (GetDiskFreeSpace(reinterpret_cast<LPCSTR>(wPath.c_str()),
                                 &secPerClus, &bytePerSec, &freeClus, &totalClus)) {
                cout << "Всего: "
                     << ((double) totalClus * secPerClus * bytePerSec) / 1024 / 1024 / 1024 << " Gb" << endl;
                cout << "Занято: "
                     <<
                     (((double) totalClus * secPerClus * bytePerSec) - ((double) freeClus * secPerClus * bytePerSec)) /
                     1024 / 1024 / 1024 << " Gb" << endl;
                cout << "Свободно: "
                     << ((double) freeClus * secPerClus * bytePerSec) / 1024 / 1024 / 1024 << " Gb\n" << endl;
            } else {
                cout << "Ошибка при определении информации о объёме занятого и свободного пространства" << endl;
            }
        }
    }
}

string fromDecToHex(unsigned long num) {
    string hexNum;
    while (num > 0) {
        if (hexNum.length() == 4) {
            hexNum = "-" + hexNum;
        }
        if (num % 16 > 9) {
            hexNum = switchDecToHex(num % 16) + hexNum;
        } else {
            hexNum = to_string(num % 16) + hexNum;
        }
        num /= 16;
    }
    return hexNum;
}

string switchDecToHex(unsigned int num) {
    switch (num) {
        case 10:
            return "A";
        case 11:
            return "B";
        case 12:
            return "C";
        case 13:
            return "D";
        case 14:
            return "E";
        case 15:
            return "F";
        default:
            return "A";
    }
}

string getDriveType(LPCWSTR path) {
    switch (GetDriveType(reinterpret_cast<LPCSTR>(path))) {
        case 0:
            return "Тип диска определить не удалось";
        case 1:
            return "Корневой директории не существует";
        case 2:
            return "Сменный диск";
        case 3:
            return "Жёсткий диск";
        case 4:
            return "Сетевой диск";
        case 5:
            return "CD-ROM";
        case 6:
            return "Виртуальный диск";
        default:
            return "Ошибка при определении типа диска";
    }
}