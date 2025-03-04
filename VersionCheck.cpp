#include <iostream>
#include <string>
#include <windows.h>
#include <wininet.h>

#pragma comment(lib, "wininet.lib")

using namespace std;

int GetRemote(const string& url) {
    HINTERNET hInternet = InternetOpenA("VersionChecker", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
    if (!hInternet) {
        DWORD dwError = GetLastError();
        cerr << "InternetOpenA failed. Error code: " << dwError << endl;
        return -1;
    }

    HINTERNET hConnect = InternetOpenUrlA(hInternet, url.c_str(), NULL, 0, INTERNET_FLAG_RELOAD, 0);
    if (!hConnect) {
        DWORD dwError = GetLastError();
        cerr << "InternetOpenUrlA failed. Error code: " << dwError << endl;
        InternetCloseHandle(hInternet);
        return -1;
    }

    char buffer[128];
    DWORD bytesRead;
    string readBuffer;

    while (InternetReadFile(hConnect, buffer, sizeof(buffer) - 1, &bytesRead) && bytesRead != 0) {
        buffer[bytesRead] = '\0'; // Null-terminate string
        readBuffer += buffer;
    }

    // Check if we received any content
    if (readBuffer.empty()) {
        cerr << "Received empty content from the server." << endl;
        InternetCloseHandle(hConnect);
        InternetCloseHandle(hInternet);
        return -1;
    }

    cout << "Received data: " << readBuffer << endl;

    InternetCloseHandle(hConnect);
    InternetCloseHandle(hInternet);

    try {
        return stoi(readBuffer);
    }
    catch (const invalid_argument&) {
        cerr << "Invalid version format from server." << endl;
        return -1;
    }
    catch (const out_of_range&) {
        cerr << "Version number is out of range." << endl;
        return -1;
    }
}
        