#include <windows.h>
#include <iostream>
#include <fstream>
#include <string>
#include <intrin.h>
#include <algorithm>
#include "resource.h"
#include <Wbemidl.h> 
#include <atlbase.h>  
#include <thread>
#include <filesystem>
#include <comutil.h> 
#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <SDL_mixer.h>

#pragma comment(lib, "comsuppw.lib") 
#pragma comment(lib, "wbemuuid.lib")
using namespace std;

#include <Wbemidl.h>  // 用于 WMI 接口
#pragma comment(lib, "wbemuuid.lib")  // 链接 WMI 库

namespace fs = std::filesystem;

int vendor_c = 0;

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

// 获取系统的国家/地区信息
int GetRegionCode() {
    char region[256];

    // 获取当前用户的地区设置
    int len = GetLocaleInfoA(LOCALE_USER_DEFAULT, LOCALE_SISO3166CTRYNAME, region, sizeof(region));

    if (len > 0) {
        string regionStr(region);
        if (regionStr == "CN") {
            return 1; // 中国大陆
        }
        else if (regionStr == "TW") {
            return 2; // 台湾
        }
        else if (regionStr == "HK") {
            return 3; // 香港特别行政区
        }
    }

    return 0; // 默认，未知地区
}




// 播放MP3文件
void PlayMP3(const char* fileName) {
    Mix_Music* music = Mix_LoadMUS(fileName);
    if (music == nullptr) {
        cerr << "加载音乐文件失败: " << Mix_GetError() << endl;
        return;
    }
    if (Mix_PlayMusic(music, 1) == -1) {
        cerr << "播放音乐失败: " << Mix_GetError() << endl;
        Mix_FreeMusic(music);
        return;
    }

    // 等待音乐播放完毕
    while (Mix_PlayingMusic()) {
        SDL_Delay(100);  // 延时100ms
    }
    Mix_FreeMusic(music);
}

// 初始化 COM 库和 WMI
bool InitWMI(IWbemLocator*& pLocator, IWbemServices*& pService) {
    HRESULT hres;

    // 初始化 COM 库
    hres = CoInitializeEx(0, COINIT_MULTITHREADED);
    if (FAILED(hres)) {
        wcout << L"COM 初始化失败" << endl;
        return false;
    }

    // 设置安全性
    hres = CoInitializeSecurity(
        NULL, -1, NULL, NULL, RPC_C_AUTHN_LEVEL_DEFAULT,
        RPC_C_IMP_LEVEL_IMPERSONATE, NULL, EOAC_NONE, NULL);
    if (FAILED(hres)) {
        wcerr << L"安全初始化失败" << endl;
        CoUninitialize();
        return false;
    }

    // 创建 IWbemLocator 对象
    hres = CoCreateInstance(CLSID_WbemLocator, 0, CLSCTX_INPROC_SERVER, IID_IWbemLocator, (LPVOID*)&pLocator);
    if (FAILED(hres)) {
        wcerr << L"创建 IWbemLocator 对象失败" << endl;
        CoUninitialize();
        return false;
    }

    // 连接到 WMI 服务
    hres = pLocator->ConnectServer(
        _bstr_t(L"ROOT\\CIMV2"), NULL, NULL, 0, NULL, 0, 0, &pService);
    if (FAILED(hres)) {
        wcerr << L"连接到 WMI 服务失败" << endl;
        pLocator->Release();
        CoUninitialize();
        return false;
    }

    return true;
}

// 获取 GPU 信息
wstring GetGPUInfo() {
    IWbemLocator* pLocator = nullptr;
    IWbemServices* pService = nullptr;

    if (!InitWMI(pLocator, pService)) {
        return L"无法获取 GPU 信息";
    }

    // 查询 GPU 信息
    IEnumWbemClassObject* pEnumerator = nullptr;
    HRESULT hres = pService->ExecQuery(
        bstr_t(L"SELECT * FROM Win32_VideoController"),
        bstr_t(L"WQL"),
        WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,
        NULL,
        &pEnumerator);

    if (FAILED(hres)) {
        wcerr << L"WMI 查询失败" << endl;
        pLocator->Release();
        pService->Release();
        CoUninitialize();
        return L"无法获取 GPU 信息";
    }

    // 获取查询结果
  IWbemClassObject* pclsObj = nullptr;
    ULONG uReturn = 0;
    wstring gpuInfo = L"未检测到 GPU";

    while (pEnumerator) {
        hres = pEnumerator->Next(WBEM_INFINITE, 1, &pclsObj, &uReturn);
        if (0 == uReturn) {
            break;
        }

        // 获取 GPU 名称
        VARIANT vtProp;
        hres = pclsObj->Get(L"Caption", 0, &vtProp, 0, 0);
        if (SUCCEEDED(hres)) {
            gpuInfo = vtProp.bstrVal;
        }

        VariantClear(&vtProp);
        pclsObj->Release();
    }

    // 如果没有找到 GPU，打印额外调试信息
    if (gpuInfo == L"未检测到 GPU") {
        wcerr << L"未能从 WMI 获取 GPU 信息。" << endl;
    }

    // 清理
    pEnumerator->Release();
    pService->Release();
    pLocator->Release();
    CoUninitialize();

    return gpuInfo;
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

bool CheckForExistingAlias(const string& filePath, string& aliasCommand) {
    ifstream file(filePath);
    string line;
    bool aliasFound = false;

    while (getline(file, line)) {
        // 检查是否存在类似的别名命令
        if (line.find("alias Sqaure_Fps_Default") != string::npos) {
            aliasFound = true;
            // 解析命令并检查是否与当前CPU型号的命令匹配
            size_t fpsPos = line.find("fps_max");
            if (fpsPos != string::npos) {
                aliasCommand = line.substr(fpsPos);  // 获取fps_max的值
            }
            break;
        }
    }

    return aliasFound;
}

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

void HandleCpuTypeAndAppend(const string& vendor, const string& brand, const string& filePath, string& fps) {
    string newAliasCommand;
    if (vendor == "GenuineIntel") {
        vendor_c = 1;

        if (//brand.find("Intel(R) Core(TM) i5-12400") != string::npos ||
            brand.find("Intel(R) Core(TM) i5-12400F") != string::npos ||
           // brand.find("Intel(R) Core(TM) i5-12400K") != string::npos ||
            brand.find("Intel(R) Core(TM) i5-12600KF") != string::npos 
            
) {
            newAliasCommand = "alias Sqaure_Fps_Default \"fps_max 539\"";
            fps = "539";
        }
        else {
            newAliasCommand = "alias Sqaure_Fps_Default \"fps_max 1009\"";
            fps = "1009";
        }
    }
    else if (vendor == "AuthenticAMD") {
        vendor_c = 2;

        if (brand.find("AMD Ryzen 7500F") != string::npos) {
            newAliasCommand = "alias Sqaure_Fps_Default \"fps_max 539\"";
            fps = "539";
        }
        else {
            newAliasCommand = "alias Sqaure_Fps_Default \"fps_max 1009\"";
            fps = "1009";
        }
    }
    else {
        MessageBox(NULL, L"检测到未知的CPU厂商，程序无法运行。", L"错误", MB_ICONERROR);
        exit(1);
    }

    string currentAliasCommand;
    if (CheckForExistingAlias(filePath, currentAliasCommand)) {
        // 如果当前命令与新命令不同，则删除旧命令并追加新的命令
        if (currentAliasCommand != newAliasCommand) {
            cout << "配置文件中已存在别名命令，但与检测到的CPU信息不一致。正在更新..." << endl;

            // 读取配置文件并删除旧的命令
            ifstream inputFile(filePath);
            ofstream tempFile("temp.cfg");

            string line;
            bool commandDeleted = false;

            while (getline(inputFile, line)) {
                if (line.find("alias Sqaure_Fps_Default") != string::npos) {
                    // 删除现有的命令
                    commandDeleted = true;
                }
                else {
                    // 写入其他行
                    tempFile << line << endl;
                }
            }

            // 将新的命令追加到文件末尾
            if (commandDeleted) {
                tempFile << newAliasCommand << endl;
            }

            inputFile.close();
            tempFile.close();

            // 替换原文件
            remove(filePath.c_str());
            rename("temp.cfg", filePath.c_str());
        }
        else {
            cout << "配置文件中的命令与检测到的CPU型号一致，未做更改。" << endl;
        }
    }
    else {
        // 如果没有找到命令，则直接追加新命令
        AppendToFile(filePath, newAliasCommand);
    }
}


bool SaveResourceToFile(UINT resourceID, const wchar_t* fileName) {
    HRSRC hResInfo = FindResourceW(NULL, MAKEINTRESOURCEW(resourceID), RT_RCDATA);
    if (hResInfo == NULL) {
        wcerr << L"找不到资源。" << endl;
        return false;
    }

    HGLOBAL hResData = LoadResource(NULL, hResInfo);
    if (hResData == NULL) {
        wcerr << L"加载资源失败。" << endl;
        return false;
    }

    LPVOID pData = LockResource(hResData);
    if (pData == NULL) {
        wcerr << L"锁定资源失败。" << endl;
        return false;
    }

    DWORD dwSize = SizeofResource(NULL, hResInfo);
    if (dwSize == 0) {
        wcerr << L"获取资源大小失败。" << endl;
        return false;
    }

    ofstream outputFile(fileName, ios::binary);
    if (!outputFile.is_open()) {
        wcerr << L"打开文件写入失败: " << fileName << endl;
        return false;
    }

    outputFile.write(reinterpret_cast<const char*>(pData), dwSize);
    outputFile.close();

    wcout << L"资源已保存到文件: " << fileName << endl;
    return true;
}

void ShowRegionMessage(int regionCode) {
    wstring message;
    switch (regionCode) {
    case 1:
        message = L"你所在的地区是中国大陆。本CFG完全免费！";
        SaveResourceToFile(IDR_BGM_CN, L"temp_bgm.mp3");
        break;
    case 2:
        message = L"你所在的地区是台湾地区。本CFG完全免费！";
        SaveResourceToFile(IDR_BGM, L"temp_bgm.mp3");
        break;
    case 3:
        message = L"你所在的地区是香港特别行政区。本CFG完全免费！";
        break;
    default:
        message = L"非中国大陆、香港特别行政区、台湾地区。本CFG完全免费！";
        break;
    }

    // 弹窗显示信息
    MessageBoxW(NULL, message.c_str(), L"地区信息", MB_OK | MB_ICONINFORMATION);
}



int main() {
    system("@echo off");
   system("chcp 65001");
   std::filesystem::path current_path = std::filesystem::current_path();
   std::filesystem::path target_path = L"Counter-Strike Global Offensive\\game\\csgo\\cfg\\Sqaure";

   if (current_path.filename() != target_path.filename()) {
       MessageBoxW(NULL, L"运行目录不正确，请仔细观看教程", L"错误", MB_OK | MB_ICONERROR);
   }
   else {
       int regionCode = GetRegionCode();  // 获取地区代码
       ShowRegionMessage(regionCode);     // 显示消息框

       if (!InitSDL()) {
           return -1;
       }

       thread musicThread(PlayMP3, "temp_bgm.mp3");


       system("copy /Y src\\main\\resources\\sounds\\disable_a.vsnd_c  ..\\..\\sounds");
       system("copy /Y src\\main\\resources\\sounds\\enable_a.vsnd_c  ..\\..\\sounds");

       system("copy /Y src\\main\\resources\\linemap.webm  ..\\..\\panorama\\videos");
       system("copy /Y src\\main\\resources\\keybindings_schinese.txt  ..\\..\\resource\\keybindings_schinese.txt");
       // Ensure the file has been created in the current director

       
       
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
       MessageBox(NULL, message.c_str(), L"帧率设置", MB_OK | MB_ICONINFORMATION);


       wstring gpuInfo = GetGPUInfo();
       wstring messageG = L"检测到 GPU: " + gpuInfo;
       //MessageBox(NULL, messageG.c_str(), L"GPU 信息", MB_OK | MB_ICONINFORMATION);
      


       system("color 0F");
       system("start teach");
       wchar_t resourceFile[] = L"完全免费如果你买到的你就被骗了.free";
       if (!SaveResourceToFile(IDR_TEST_FREE, resourceFile)) {
           cerr << "保存资源失败。" << endl;
           return 1;
       }
       ifstream checkFile(resourceFile);
       if (checkFile.good()) {

           MessageBox(NULL, L"文件已成功保存到运行目录。", L"成功", MB_ICONINFORMATION);
       }
       else {
           MessageBox(NULL, L"文件保存失败，未能创建文件。", L"错误", MB_ICONERROR);
       }
      // system("copy /Y src\\main\\resources\\Sqaure_Installed.cfg  ..\\..\\..\\cfg\\Sqaure_Installed.cfg");
         // 源文件路径和目标文件路径
       std::string source = "src\\main\\resources\\Sqaure_Installed.cfg";
       std::string destination = "..\\..\\..\\cfg\\Sqaure_Installed.cfg";

       // 获取目标文件夹路径
       fs::path destDir = fs::path(destination).parent_path();

       // 检查目标文件夹是否存在，如果不存在则创建
       if (!fs::exists(destDir)) {
           std::cout << "目标文件夹不存在，正在创建..." << std::endl;
           fs::create_directories(destDir);  // 创建文件夹及其所有父文件夹
       }

       // 复制文件
       try {
           fs::copy(source, destination, fs::copy_options::overwrite_existing);
           string insteadboot;
           cout << "是否允许替换游戏启动画面和音乐，如果不允许，请输入0，否则将默认替换：";
           cin >> insteadboot;
           if (insteadboot != "0")
           {
               system("copy /Y src\\main\\resources\\sounds\\bootsounds.vsnd_c  ..\\..\\sounds");
               system("copy /Y src\\main\\resources\\intro.webm  ..\\..\\panorama\\videos");
               system("copy /Y src\\main\\resources\\intro720p.webm  ..\\..\\panorama\\videos");
               system("copy /Y src\\main\\resources\\intro-perfectworld.webm  ..\\..\\panorama\\videos");
               system("copy /Y src\\main\\resources\\intro-perfectworld720p.webm  ..\\..\\panorama\\videos");
               cout << "1\n\n";
           }
           cout << "0\n\n";
           cout << "所有文件均复制完成！\n";
       }
       catch (const std::exception& e) {
           std::cout << "复制文件失败: " << e.what() << std::endl;
       }
      
       MessageBox(NULL, L"现在你可以退出本程序进行下一步配置", L"tips", MB_OK | MB_ICONINFORMATION);

       musicThread.join();
       Mix_CloseAudio();
       Mix_Quit();
       SDL_Quit();
       DeleteFileInCurrentDirectory("temp_bgm.mp3");

   }


    return 0;
}