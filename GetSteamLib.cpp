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
                    return false; // 创建失
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
    // 遍历所有驱动器
    for (char drive = 'C'; drive <= 'Z'; ++drive) {
        string drivePath(1, drive);
        drivePath += ":\\SteamLibrary\\steamapps\\common\\" + gameName;

        if (directoryExists(drivePath)) {
            return drivePath;
        }
    }

    string gamePath = "C:\\SteamLibrary\\steamapps\\common\\Counter-Strike Global Offensive\\game\\csgo\\cfg";
    if (createDirectoriesRecursively(gamePath)) {
        cout << "已创建目录: " << gamePath << endl;
    }
    else {
        cout << "无法创建目录: " << gamePath << endl;
    }
    system("cls");
    cout << "没检测到游戏目录，所以创建了C:\\SteamLibrary\\steamapps\\common\\Counter-Strike Global Offensive\\game\\csgo\\cfg，请重新运行本程序生成CFG,然后自行复制到真正的游戏CFG文件夹";
    Sleep(2000);
  
    return gamePath; 
}

