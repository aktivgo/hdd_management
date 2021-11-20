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
            cout << "Disk: " << path << endl;
            wstring wPath = wstring(path.begin(), path.end());
            cout << "Type: " << getDriveType(wPath.c_str()) << endl;

            char nameBuffer[100];
            char fileSystemBuffer[100];
            unsigned long driveSN;

            if (GetVolumeInformationA(path.c_str(), nameBuffer, sizeof(nameBuffer), &driveSN, nullptr, nullptr,
                                      fileSystemBuffer, sizeof(fileSystemBuffer))) {
                cout << "Volume label: " << nameBuffer << endl;
                cout << "File system: " << fileSystemBuffer << endl;
                cout << "Serial number: " << fromDecToHex(driveSN) << endl;
            } else {
                cout << "Error in determining information about the volume label, serial number file system" << endl;
            }

            DWORD secPerClus;
            DWORD bytePerSec;
            DWORD freeClus;
            DWORD totalClus;
            if (GetDiskFreeSpace(reinterpret_cast<LPCSTR>(wPath.c_str()),
                                 &secPerClus, &bytePerSec, &freeClus, &totalClus)) {
                cout << "Total: "
                     << ((double) totalClus * secPerClus * bytePerSec) / 1024 / 1024 / 1024 << " Gb" << endl;
                cout << "Used: "
                     <<
                     (((double) totalClus * secPerClus * bytePerSec) - ((double) freeClus * secPerClus * bytePerSec)) /
                     1024 / 1024 / 1024 << " Gb" << endl;
                cout << "Free: "
                     << ((double) freeClus * secPerClus * bytePerSec) / 1024 / 1024 / 1024 << " Gb\n" << endl;
            } else {
                cout << "Error in determining information about the amount of used and free space" << endl;
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
            return "The disk type could not be determined";
        case 1:
            return "The root directory does not exist";
        case 2:
            return "Removable disk";
        case 3:
            return "Hard drive";
        case 4:
            return "Network Drive";
        case 5:
            return "CD-ROM";
        case 6:
            return "Virtual Disk";
        default:
            return "Error in determining the disk type";
    }
}