﻿#include <windows.h>
#include <wininet.h>
#include <iostream>
#include <sstream>
#include <vector>
#include <map>
#include <string>
#include <cwctype>
#include <algorithm>
#include <fstream>

#pragma comment(lib, "wininet.lib")

using namespace std;

int debug = 1; // 调试模式，1 时打印更多调试信息



// 清屏
void ClearScreen() {
    system("cls");
}

// 去除字符串首尾的空白字符
wstring trim(const wstring& s) {
    size_t start = s.find_first_not_of(L" \t\r\n");
    if (start == wstring::npos)
        return L"";
    size_t end = s.find_last_not_of(L" \t\r\n");
    return s.substr(start, end - start + 1);
}

// UTF-8 string 转换为 wstring
wstring StringToWString(const string& str) {
    int size_needed = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), (int)str.length(), NULL, 0);
    wstring wstr(size_needed, 0);
    MultiByteToWideChar(CP_UTF8, 0, str.c_str(), (int)str.length(), &wstr[0], size_needed);
    return wstr;
}

// wstring 转 UTF-8 string
string WStringToString(const wstring& wstr) {
    int size_needed = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), (int)wstr.length(), NULL, 0, NULL, NULL);
    string str(size_needed, 0);
    WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), (int)wstr.length(), &str[0], size_needed, NULL, NULL);
    return str;
}

// 下载文件并返回 UTF-8 编码的 string
string DownloadFile(const wstring& url) {
    HINTERNET hInternet = InternetOpen(L"HTTPGET", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
    if (!hInternet) return "";

    HINTERNET hConnect = InternetOpenUrl(hInternet, url.c_str(), NULL, 0, INTERNET_FLAG_RELOAD, 0);
    if (!hConnect) {
        InternetCloseHandle(hInternet);
        return "";
    }

    char buffer[4096];
    DWORD bytesRead;
    string content;
    while (InternetReadFile(hConnect, buffer, sizeof(buffer), &bytesRead) && bytesRead) {
        content.append(buffer, bytesRead);
    }

    InternetCloseHandle(hConnect);
    InternetCloseHandle(hInternet);
    return content;
}



// 执行命令
void ExecuteCommand(const wstring& cmd) {
    if (debug == 1) {
        wcout << L"Execute Command: " << cmd << endl;
    }
    _wsystem(cmd.c_str());
}

// 等待用户按键
void WaitForKeyPress() {
    if (debug == 1) {
        system("pause");
    }
}

void RemoveExistingProxySettings(const string& filePath) {
    ifstream file(filePath);  // 打开文件读取
    if (!file) {
        cerr << "Can't open folder: " << filePath << endl;
        return;
    }

    // 使用一个字符串流来存储所有非代理行
    stringstream buffer;
    string line;
    while (getline(file, line)) {
        // 检查是否是代理配置行（http_proxy、https_proxy 或 all_proxy），如果是就跳过
        if (line.find("http_proxy") == string::npos &&
            line.find("https_proxy") == string::npos &&
            line.find("all_proxy") == string::npos) {
            buffer << line << endl;
        }
    }
    file.close();

    // 重新写回文件，更新代理配置
    ofstream outFile(filePath);
    if (!outFile) {
        cerr << "Can't open file: " << filePath << endl;
        return;
    }

    outFile << buffer.str();  // 写入新的内容
    outFile.close();

    cout << "proxy has been clean。" << endl;
}

// 用于设置新的代理配置到 wget 配置文件
void SetProxyToWgetRC(const string& proxyAddress) {
    string wgetRCFilePath = "src/main/lib/.wgetrc";  // 假设的 .wgetrc 配置文件路径

    // 删除现有的代理设置
    RemoveExistingProxySettings(wgetRCFilePath);

    // 打开文件（追加模式）
    ofstream file(wgetRCFilePath, ios::app);  // 确保使用 std::ofstream
    if (!file) {
        cerr << "can't open file: " << wgetRCFilePath << endl;
        return;
    }

    // 写入代理设置
    //file << "# Proxy settings added via C++ program\n";

    // 只设置 http_proxy 和 https_proxy
    if (!proxyAddress.empty()) {
        file << "http_proxy = " << proxyAddress << "\n";
        file << "https_proxy = " << proxyAddress << "\n";
    }
    else {
        // 如果没有代理，清空配置文件中的代理设置
        file << "http_proxy = \n";
        file << "https_proxy = \n";
    }

    file.close();

    cout << "proxy has been set " << wgetRCFilePath << endl;
}


int main() {
    system("chcp 65001");  // 设置 UTF-8 编码
    int choiceProxy;

    // 使用宽字符串进行交互，但通过窄字符串输出
    wstring wmessage = L"Enable Proxy?\n";
    wmessage += L"  1. Needn't thanks\n";
    wmessage += L"  2. local proxy（127.0.0.1:7890）\n";
    wmessage += L"  3. Custom Proxy\n\n";
    wmessage += L"input your choice > ";

    // 将宽字符串转回窄字符串
    cout << WStringToString(wmessage);

    cin >> choiceProxy; // 用户输入选择

    string proxyAddress;

    if (choiceProxy == 1) {
        // 不使用代理
        proxyAddress = "";
        cout << "neednt proxy" << endl;
    }
    else if (choiceProxy == 2) {
        // 使用本地 HTTP 代理
        proxyAddress = "http://127.0.0.1:7890";
        cout << "proxy 127.0.0.1：7890" << endl;
    }
    else if (choiceProxy == 3) {
        // 自定义代理
        wstring wcustomMessage = L"input your proxy adress（example：http://127.0.0.1:8080）：";
        cout << WStringToString(wcustomMessage);
        cin >> proxyAddress;
        cout << "setted：" << proxyAddress << endl;
    }
    else {
        cout << "vaild choice!" << endl;
    }

    // 将代理设置写入 .wgetrc 文件
    SetProxyToWgetRC(proxyAddress);
    Sleep(500);
    // 清屏
    ClearScreen();

    // 下载文件
    string modListContentStr = DownloadFile(L"https://www.luotiany1.cn/Sqaure/ModMarket/ModList.txt");
    string dlCommandContentStr = DownloadFile(L"https://www.luotiany1.cn/Sqaure/ModMarket/DLCommand.txt");

    wstring modListContent = StringToWString(modListContentStr);
    wstring dlCommandContent = StringToWString(dlCommandContentStr);

    if (modListContent.empty() || dlCommandContent.empty()) {
        cout << "Failed Please check your internet connect。" << endl;
       // return 1;
    }

    // 分析 DLCommand.txt 内容并标记被屏蔽的项
// 分析 DLCommand.txt 内容并标记被屏蔽的项
    wstringstream cmdStream(dlCommandContent);
    wstring line, currentKey;
    map<wstring, vector<wstring>> commandMap;
    map<wstring, bool> hiddenMods; // 用于存储模组是否被屏蔽

    while (getline(cmdStream, line)) {
        wstring trimmed = trim(line);
        if (trimmed.empty())
            continue;

        // 判断是否为 header 行（模组编号行），要求以冒号结尾
        if (trimmed.back() == L':') {
            bool headerBlocked = false;
            wstring headerLine = trimmed;
            // 如果以 '#' 开头，则标记整个模组被屏蔽，并去掉这个符号
            if (headerLine[0] == L'#') {
                headerBlocked = true;
                headerLine = headerLine.substr(1); // 去除首字符 #
                headerLine = trim(headerLine);
            }
            // headerLine 应该形如 "1:"，提取冒号前面的数字作为 key
            size_t pos = headerLine.find(L':');
            if (pos != wstring::npos) {
                currentKey = headerLine.substr(0, pos);
                hiddenMods[currentKey] = headerBlocked;
                commandMap[currentKey] = vector<wstring>();
            }
        }
        else if (!currentKey.empty()) {
            // 判断是否为被注释的命令行（仅跳过该行，不将模组标记为屏蔽）
            if (trimmed.find(L"#--") == 0) {
                // 跳过被注释的命令，不执行
                continue;
            }
            // 正常命令行
            else if (trimmed.find(L"--") == 0) {
                commandMap[currentKey].push_back(trimmed.substr(2));
            }
        }
    }


    // 更新 ModList 内容，替换为被屏蔽的模组
    wstringstream updatedModList;
    wstringstream modListStream(modListContent);
    int modIndex = 1;
    while (getline(modListStream, line)) {
        wstring trimmedLine = trim(line);
        wstring displayText = hiddenMods[to_wstring(modIndex)] ? L"<blocked mod>" : trimmedLine;
        updatedModList << displayText << endl;
        modIndex++;
    }

    while (1)
    {
        ClearScreen();
        // 用户输入模组编号
        cout << "The operation is irreversible. If you need to delete the module, you need to reinstall the CFG official release package\n";
        cout << "Please select a module (enter number):\n\n---------------------------------------------------------\n\n\n";
        cout << WStringToString(updatedModList.str()) << endl;
        cout << "\n-----------------------------------------------------\n\nOf course, in the future, you can also open this market through the CFG directory shortcut. This market updates some modules from time to time, which may require you to select the module number you want to install.\n\n Only one module can be installed at a time：";

        string userInput;
        cin >> userInput;
        if (userInput == "0") return 0;

        // 检查输入是否是数字
        bool validInput = all_of(userInput.begin(), userInput.end(), ::isdigit);
        if (!validInput) {
            cout << "Invalid input! Please enter a correct number" << endl;
            // return 1;
        }

        int selectedChoice = stoi(userInput);
        wstring key = to_wstring(selectedChoice);

        // 检查是否被屏蔽
        if (hiddenMods[key]) {
            cout << "Can't install" << endl;
        }
        else {
            if (commandMap.find(key) != commandMap.end() && !commandMap[key].empty()) {
                for (const auto& cmd : commandMap[key]) {

                    ExecuteCommand(cmd);
                }
            }
            else {
                cout << "mod " << selectedChoice << " can't install now" << endl;
            }
        }
    }
    Sleep(2000);

    return 0;
}
