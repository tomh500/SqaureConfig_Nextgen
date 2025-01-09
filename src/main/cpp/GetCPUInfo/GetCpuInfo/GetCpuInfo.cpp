#include <windows.h>
#include <iostream>
#include <fstream>
#include <string>
#include <intrin.h>
#include <algorithm>
#include <thread>  // 添加多线程支持
#include "resource.h"
#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <SDL_mixer.h>

using namespace std;

// 初始化SDL2和SDL_mixer
bool InitSDL() {
    if (SDL_Init(SDL_INIT_AUDIO) < 0) {
        cerr << "SDL 初始化失败: " << SDL_GetError() << endl;
        return false;
    }

    if (Mix_Init(MIX_INIT_MP3) != MIX_INIT_MP3) {
        cerr << "SDL_mixer 初始化失败: " << Mix_GetError() << endl;
        SDL_Quit();
        return false;
    }

    if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 1024) < 0) {
        cerr << "音频设备打开失败: " << Mix_GetError() << endl;
        Mix_Quit();
        SDL_Quit();
        return false;
    }

    return true;
}

// 播放MP3文件
void PlayMP3(const char* fileName) {
    Mix_Music* music = Mix_LoadMUS(fileName);
    if (music == nullptr) {
        return;
    }
    if (Mix_PlayMusic(music, 1) == -1) {
        Mix_FreeMusic(music);
        return;
    }

    while (Mix_PlayingMusic()) {
        SDL_Delay(100); 
    }
    Mix_FreeMusic(music);
}

// 退出清理SDL
void QuitSDL() {
    Mix_CloseAudio();
    Mix_Quit();
    SDL_Quit();
}

// 获取CPU品牌
string GetCpuBrand() {
    int cpuInfo[4] = { 0 };
    char brand[0x40] = { 0 };

    __cpuid(reinterpret_cast<int*>(cpuInfo), 0x80000002);
    memcpy(brand, cpuInfo, sizeof(cpuInfo));
    __cpuid(reinterpret_cast<int*>(cpuInfo), 0x80000003);
    memcpy(brand + 16, cpuInfo, sizeof(cpuInfo));
    __cpuid(reinterpret_cast<int*>(cpuInfo), 0x80000004);
    memcpy(brand + 32, cpuInfo, sizeof(cpuInfo));

    return string(brand);
}

// 获取CPU信息
void GetCPUInfo() {
    int cpuInfo[4] = { 0 };
    char cpuVendor[13] = { 0 };

    __cpuid(cpuInfo, 0);
    memcpy(cpuVendor, &cpuInfo[1], 4);
    memcpy(cpuVendor + 4, &cpuInfo[3], 4);
    memcpy(cpuVendor + 8, &cpuInfo[2], 4);

    string vendor(cpuVendor);
    cout << "CPU 厂商: " << vendor << endl;

    string brand = GetCpuBrand();
    cout << "CPU 型号: " << brand << endl;
}

// 设置背景颜色
void SetBackgroundColor(const string& vendor) {
    if (vendor == "GenuineIntel") {
        system("color CE");
    }
    else if (vendor == "AuthenticAMD") {
        system("color 9F");
    }
    else {
        system("color 0F");
    }
}

// 检查文件中是否已存在某个别名
bool CheckForExistingAlias(const string& filePath) {
    ifstream file(filePath);
    string line;
    while (getline(file, line)) {
        if (line.find("alias Sqaure_Fps_Default") != string::npos) {
            return true;
        }
    }
    return false;
}

// 将内容追加到文件中
void AppendToFile(const string& filePath, const string& content) {
    ofstream file(filePath, ios::app);
    if (file.is_open()) {
        file << content << endl;
        file.close();
    }
    else {
        MessageBox(NULL, L"追加失败。", L"错误", MB_ICONERROR);
    }
}

// 处理CPU类型并追加配置
void HandleCpuTypeAndAppend(const string& vendor, const string& brand, const string& filePath, string& fps) {
    if (CheckForExistingAlias(filePath)) {
       cout << "文件中已存在 Sqaure_Fps_Default 命令. 未做任何更改." << endl;
        return;
    }

    string aliasCommand;
    if (vendor == "GenuineIntel") {
        if (brand.find("Intel(R) Core(TM) i5-12400") != string::npos ||
            brand.find("Intel(R) Core(TM) i5-12400F") != string::npos ||
            brand.find("Intel(R) Core(TM) i5-12400K") != string::npos ||
            brand.find("Intel(R) Core(TM) i5-12400KF") != string::npos ||
            brand.find("Intel(R) Core(TM) i5-12600") != string::npos) {
            aliasCommand = "alias Sqaure_Fps_Default \"fps_max 239\"";
            fps = "164";
        }
        else {
            aliasCommand = "alias Sqaure_Fps_Default \"fps_max 539\"";
            fps = "239";
        }
    }
    else if (vendor == "AuthenticAMD") {
        if (brand.find("AMD Ryzen 7500F") != string::npos) {
            aliasCommand = "alias Sqaure_Fps_Default \"fps_max 539\"";
            fps = "539";
        }
        else {
            aliasCommand = "alias Sqaure_Fps_Default \"fps_max 1009\"";
            fps = "1009";
        }
    }
    else {
        MessageBox(NULL, L"检测到未知的CPU厂商，程序无法运行。", L"错误", MB_ICONERROR);
        exit(1);
    }

    AppendToFile(filePath, aliasCommand);
}


bool SaveResourceToFile(UINT resourceID, const char* fileName) {
    HRSRC hResInfo = FindResource(NULL, MAKEINTRESOURCE(resourceID), RT_RCDATA);
    if (hResInfo == NULL) {
        cerr << "找不到资源。" << endl;
        return false;
    }

    HGLOBAL hResData = LoadResource(NULL, hResInfo);
    if (hResData == NULL) {
        cerr << "加载资源失败。" << endl;
        return false;
    }

    LPVOID pData = LockResource(hResData);
    if (pData == NULL) {
        cerr << "锁定资源失败。" << endl;
        return false;
    }

    DWORD dwSize = SizeofResource(NULL, hResInfo);
    if (dwSize == 0) {
        cerr << "获取资源大小失败。" << endl;
        return false;
    }

    ofstream outputFile(fileName, ios::binary);
    if (!outputFile.is_open()) {
        cerr << "打开文件写入失败: " << fileName << endl;
        return false;
    }

    outputFile.write(reinterpret_cast<const char*>(pData), dwSize);
    outputFile.close();

    cout << "资源已保存到文件: " << fileName << endl;
    return true;
}
bool DeleteFileInCurrentDirectory(const std::string& filename) {
    char currentDir[MAX_PATH];
    if (!GetCurrentDirectoryA(MAX_PATH, currentDir)) {
        return false;
    }
    std::string filePath = std::string(currentDir) + "\\" + filename;

    if (DeleteFileA(filePath.c_str())) {
        return true;
    }
    else {
        return false;
    }
}



int main() {


    if (!InitSDL()) {
        return -1;
    }

    if (SaveResourceToFile(IDR_BGM, "temp_bgm.mp3")) {
        thread musicThread(PlayMP3, "temp_bgm.mp3");
        string resourceFile = "完全免费如果你买到的你就被骗了.free";
        Sleep(1000);
        if (!SaveResourceToFile(IDR_TEST_FREE, resourceFile.c_str())) {
            cerr << "保存资源失败。" << endl;
            return 1;
        }

        // Ensure the file has been created in the current directory
        ifstream checkFile(resourceFile);
        if (checkFile.good()) {
            MessageBox(NULL, L"文件已成功保存到运行目录。", L"成功", MB_ICONINFORMATION);
        }
        else {
            MessageBox(NULL, L"文件保存失败，未能创建文件。", L"错误", MB_ICONERROR);
        }

        system("cls");
        GetCPUInfo();

        char cpuVendor[13] = { 0 };
        int cpuInfo[4] = { 0 };
        __cpuid(cpuInfo, 0);
        memcpy(cpuVendor, &cpuInfo[1], 4);
        memcpy(cpuVendor + 4, &cpuInfo[3], 4);
        memcpy(cpuVendor + 8, &cpuInfo[2], 4);
        string vendor(cpuVendor);

        SetBackgroundColor(vendor);

        string filePath = "src\\main\\cfg\\cn\\luotiany1\\SqaureNextgen\\Sqaure.cfg";

        string brand = GetCpuBrand();
        string fps;
        HandleCpuTypeAndAppend(vendor, brand, filePath, fps);

        wstring message = L"CPU 厂商: " + wstring(vendor.begin(), vendor.end()) + L"\n" +
                          L"CPU 型号: " + wstring(brand.begin(), brand.end()) + L"\n" +
                          L"因为你的CPU已经为你设定帧率： " + wstring(fps.begin(), fps.end());

        // 显示弹窗
        MessageBox(NULL, message.c_str(), L"帧率设置", MB_OK | MB_ICONINFORMATION);
        Sleep(5000);

        system("color 0F");

        // 等待音乐播放完毕
        musicThread.join();
    }

    // 清理SDL
    QuitSDL();
    DeleteFileInCurrentDirectory("temp_bgm.mp3");

    return 0;
}
