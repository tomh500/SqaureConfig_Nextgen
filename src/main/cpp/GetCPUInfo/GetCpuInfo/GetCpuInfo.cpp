#include <windows.h>
#include <iostream>
#include <fstream>
#include <string>
#include <intrin.h>
#include <algorithm>
#include "resource.h"

using namespace std;

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
int main() {
   system("chcp 65001");

   wchar_t resourceFile[] = L"完全免费如果你买到的你就被骗了.free";
   if (!SaveResourceToFile(IDR_TEST_FREE, resourceFile)) {
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

    return 0;
}