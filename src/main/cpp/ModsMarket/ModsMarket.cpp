#include <windows.h>
#include <wininet.h>
#include <iostream>
#include <sstream>
#include <vector>
#include <map>
#include <string>
#include <cwctype> // 用于 iswdigit

#pragma comment(lib, "wininet.lib")

using namespace std;

int debug = 0; // 调试模式，1 时打印更多调试信息

// 获取当前代理状态
wstring GetCurrentProxy() {
    HKEY hKey;
    const wchar_t* regPath = L"Software\\Microsoft\\Windows\\CurrentVersion\\Internet Settings";
    wchar_t buffer[256];
    DWORD bufferSize = sizeof(buffer);
    wstring proxy;

    if (RegOpenKeyEx(HKEY_CURRENT_USER, regPath, 0, KEY_QUERY_VALUE, &hKey) == ERROR_SUCCESS) {
        DWORD enable;
        DWORD enableSize = sizeof(enable);

        if (RegQueryValueEx(hKey, L"ProxyEnable", NULL, NULL, (LPBYTE)&enable, &enableSize) == ERROR_SUCCESS && enable == 1) {
            if (RegQueryValueEx(hKey, L"ProxyServer", NULL, NULL, (LPBYTE)buffer, &bufferSize) == ERROR_SUCCESS) {
                proxy = buffer;  // 读取代理地址
            }
        }
        RegCloseKey(hKey);
    }
    return proxy;
}

// 设置代理
void SetProxy(const wstring& proxy) {
    HKEY hKey;
    const wchar_t* regPath = L"Software\\Microsoft\\Windows\\CurrentVersion\\Internet Settings";

    if (RegOpenKeyEx(HKEY_CURRENT_USER, regPath, 0, KEY_SET_VALUE, &hKey) == ERROR_SUCCESS) {
        DWORD enable = proxy.empty() ? 0 : 1;
        RegSetValueEx(hKey, L"ProxyEnable", 0, REG_DWORD, (const BYTE*)&enable, sizeof(enable));

        if (!proxy.empty()) {
            RegSetValueEx(hKey, L"ProxyServer", 0, REG_SZ, (const BYTE*)proxy.c_str(), (proxy.size() + 1) * sizeof(wchar_t));
        }
        else {
            RegDeleteValue(hKey, L"ProxyServer");
        }
        RegCloseKey(hKey);
    }
}

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

// wstring 转 UTF-8 string
string WStringToString(const wstring& wstr) {
    int size_needed = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), (int)wstr.length(), NULL, 0, NULL, NULL);
    string str(size_needed, 0);
    WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), (int)wstr.length(), &str[0], size_needed, NULL, NULL);
    return str;
}

// 执行命令
void ExecuteCommand(const wstring& cmd) {
    if (debug == 1) {
        wcout << L"执行命令: " << cmd << endl;
    }
    _wsystem(cmd.c_str());
}

// 等待用户按键
void WaitForKeyPress() {
    if (debug == 1) {
        cout << "按任意键继续..." << endl;
        system("pause");
    }
}

int main() {
    system("chcp 65001");  // 设置 UTF-8 编码

    // 清屏
    ClearScreen();

    // 下载文件
    string modListContentStr = DownloadFile(L"https://www.luotiany1.cn/Sqaure/ModMarket/ModList.txt");
    string dlCommandContentStr = DownloadFile(L"https://www.luotiany1.cn/Sqaure/ModMarket/DLCommand.txt");

    wstring modListContent = StringToWString(modListContentStr);
    wstring dlCommandContent = StringToWString(dlCommandContentStr);

    if (modListContent.empty() || dlCommandContent.empty()) {
        cout << "下载失败，请检查网络连接。" << endl;
        return 1;
    }

    // 分析 DLCommand.txt 内容并标记被屏蔽的项
    wstringstream cmdStream(dlCommandContent);
    wstring line, currentKey;
    map<wstring, vector<wstring>> commandMap;
    map<wstring, bool> hiddenMods; // 用于存储被注释的 Mod

    while (getline(cmdStream, line)) {
        wstring trimmed = trim(line);
        if (!trimmed.empty() && iswdigit(trimmed[0])) {
            currentKey = trimmed.substr(0, trimmed.find(L':'));
            hiddenMods[currentKey] = false; // 默认不是被注释的
            commandMap[currentKey] = {};
        }
        else if (!currentKey.empty() && trimmed.find(L"#--") == 0) {
            commandMap[currentKey].push_back(trimmed.substr(3)); // 跳过#
        }
        else if (!currentKey.empty() && trimmed.find(L"--") == 0) {
            commandMap[currentKey].push_back(trimmed.substr(2));
        }
        else if (trimmed.find(L"#") == 0) {
            hiddenMods[currentKey] = true; // 标记为被屏蔽
        }
    }

    // 更新 ModList 内容，替换为被屏蔽的插件
    wstringstream updatedModList;
    wstringstream modListStream(modListContent);
    int modIndex = 1;
    while (getline(modListStream, line)) {
        wstring trimmedLine = trim(line);
        wstring displayText = hiddenMods[to_wstring(modIndex)] ? L"<被屏蔽的插件>" : trimmedLine;
        updatedModList << displayText << endl;
        modIndex++;
    }

    cout << "请选择一个或多个插件 (用逗号分隔):" << endl;
    cout << WStringToString(updatedModList.str()) << endl;

    string userInput;
    cin.ignore(); // 清除输入缓冲区
    getline(cin, userInput);

    stringstream userStream(userInput);
    string temp;
    vector<int> choices;

    // 解析用户选择
    while (getline(userStream, temp, ',')) {
        try {
            choices.push_back(stoi(temp));
        }
        catch (...) {
            cout << "无效输入！" << endl;
            return 1;
        }
    }

    // 遍历用户选择的插件
    for (int choice : choices) {
        wstring key = to_wstring(choice);
        if (hiddenMods[key]) {
            cout << "拒绝执行: 插件 " << choice << " 被屏蔽。" << endl;
        }
        else {
            if (commandMap.find(key) != commandMap.end()) {
                for (const auto& cmd : commandMap[key]) {
                    ExecuteCommand(cmd);
                }
            }
            else {
                cout << "插件 " << choice << " 无效。" << endl;
            }
        }
    }

    return 0;
}
