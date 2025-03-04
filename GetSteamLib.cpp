#include <iostream>
#include <string>
#include <windows.h>

using namespace std;

bool directoryExists(const string& path) {
    DWORD ftyp = GetFileAttributesA(path.c_str());
    return (ftyp != INVALID_FILE_ATTRIBUTES && (ftyp & FILE_ATTRIBUTE_DIRECTORY));
}

bool createDirectory(const string& path) {
    
    return CreateDirectoryA(path.c_str(), NULL) || GetLastError() == ERROR_ALREADY_EXISTS;
}

bool createDirectoriesRecursively(const string& path) {
    string tempPath;
    for (const char& c : path) {
        tempPath += c;
        if (c == '\\') { 
            if (!directoryExists(tempPath)) {
                if (!createDirectory(tempPath)) {
                    return false; // ����ʧ
                }
            }
        }
    }
    if (!directoryExists(tempPath)) {
        return createDirectory(tempPath);
    }
    return true; 
}

string findGameInstallPath(const string& gameName) {
    // ��������������
    for (char drive = 'C'; drive <= 'Z'; ++drive) {
        string drivePath(1, drive);
        drivePath += ":\\SteamLibrary\\steamapps\\common\\" + gameName;

        if (directoryExists(drivePath)) {
            return drivePath;
        }
    }

    string gamePath = "C:\\SteamLibrary\\steamapps\\common\\Counter-Strike Global Offensive\\game\\csgo\\cfg";
    if (createDirectoriesRecursively(gamePath)) {
        cout << "�Ѵ���Ŀ¼: " << gamePath << endl;
    }
    else {
        cout << "�޷�����Ŀ¼: " << gamePath << endl;
    }
    system("cls");
    cout << "û��⵽��ϷĿ¼�����Դ�����C:\\SteamLibrary\\steamapps\\common\\Counter-Strike Global Offensive\\game\\csgo\\cfg�����������б���������CFG,Ȼ�����и��Ƶ���������ϷCFG�ļ���";
    Sleep(2000);
  
    return gamePath; 
}

