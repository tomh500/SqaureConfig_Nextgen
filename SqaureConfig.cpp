// SqaureConfig.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <windows.h>
#include <fstream>

#include "GetCPUInfo.h"
#include "GetSteamLib.h"
#include "VersionCheck.h"
#include "ConfigCreator.h"
#include "resource.h"
//#include "SDL2_mixer/include/SDL_mixer.h"
//#include "SDL2/include/SDL.h"
#define SDL_MAIN_HANDLED

using namespace std;

int localVersion = 8; // 本地版本号
int CPU = -1;

void GetCPU()
{
        string vendor = getCPUVendor();

    if (vendor == "GenuineIntel") {
        CPU = 1;
    }
    else if (vendor == "AuthenticAMD") {
        CPU = 2;
    }
    else {
        CPU = -1;
    }
}
void ExtractResource(int resourceId, const string& outputFileName) {
    HMODULE hModule = GetModuleHandle(NULL);
    HRSRC hResource = FindResource(hModule, MAKEINTRESOURCE(resourceId), RT_RCDATA);

    if (!hResource) {
       cout << "error 3\n";
        return;
    }

    HGLOBAL hLoadedResource = LoadResource(hModule, hResource);
    DWORD resourceSize = SizeofResource(hModule, hResource);
    void* pResourceData = LockResource(hLoadedResource);

    if (pResourceData == nullptr) {
       cout << "error 1\n";
        return;
    }

    ofstream outFile(outputFileName, ios::binary);
    if (!outFile) {
        cout << "error 2\n";
        return;
    }

    outFile.write(static_cast<const char*>(pResourceData), resourceSize);
    outFile.close();
}

int main()
{
    SetConsoleOutputCP(936);
    system("color 9f");
    createDirectoriesRecursively("C:\\Sqaure\\SqaureUtiles");
  //  MessageBoxA(NULL, "离线版 不提供支持", "Error", MB_OK |MB_ICONINFORMATION);
   // /*
	SetConsoleTitle(L"广场配置1.5安装器");
   
       string url = "https://tomh500.github.io/SqaureCFG/VersionCheck";

        int remoteVersion = GetRemote(url);

        if (remoteVersion == -1) {
            MessageBoxA(NULL, "联网检测失败！", "Error", MB_OK | MB_ICONERROR);
            return 1;
        }

       cout << "Local Version: " << localVersion << endl;
        cout << "Remote Version: " << remoteVersion << endl;


    wcout << "欢迎使用广场配置1.5安装器，本程序承诺使用合法指令，拒绝作弊\n\n正在检测电脑信息";

    if (remoteVersion > localVersion) {
        cout << "你在用旧版生成器，现在已推出新版本，请及时更新\n程序将在10秒后继续运行\n\n";
        Sleep(10000);

    }
    else if (remoteVersion < localVersion) {
        MessageBoxA(NULL, "你正在使用非法版本！拒绝运行", "Error", MB_OK | MB_ICONERROR);
        exit(0);

    }
    else {
        cout << "你正在用最新版\n";
    }
  //   */
    for (int i = 0; i < 3; i++)
    {
       
        Sleep(800);
        cout << ".";
    }
    system("cls");
    system("color 9f");
    GetCPU();
    if (CPU == 1)
    {
        system("color ce");
        wcout << "你使用的CPU供应商为：英特尔\n";
        MessageBoxA(NULL, "请尽快更换到AMD的CPU，按下确认继续使用本程序", "英特尔是垃圾", MB_OK | MB_ICONERROR);
    }
    else if (CPU == 2)
    {
        system("color 2f");
        wcout << "你使用的CPU供应商为：AMD\n";
    }
    else {
        cout << "杂牌CPU，你不配用CFG";
        Sleep(1000);
        exit(0);
        
    }
    cout << "\n更新日志：\n1.修复了一些已知问题\n\n";
//    cout << "\n请选择生成方式：\nA.自动化（适用于普通用户，会尝试自动检测CFG目录，如果找不到会生成在错误的位置）\nB.生成在固定目录，需要手动复制到CFG文件夹（稳定）\n请选择：";
  //  string CreateMode;
   // getline(cin, CreateMode);
    //if (CreateMode == "B" || CreateMode == "B") {
        cout << "\n\n 文件生成完毕后请手动前往 C:\\Sqaure 文件夹下把所有东西复制到你的游戏CFG文件夹！！！\n";
        cout << "\n\n 文件生成完毕后请手动前往 C:\\Sqaure 文件夹下把所有东西复制到你的游戏CFG文件夹！！！\n";
        cout << "\n\n 文件生成完毕后请手动前往 C:\\Sqaure 文件夹下把所有东西复制到你的游戏CFG文件夹！！！\n\n";
        ExtractResource(IDR_JUMPTHROW, "C:\\Sqaure\\SqaureUtiles\\jumpthrow.cfg");    // ID为101
        ExtractResource(IDR_WJUMPTHROW, "C:\\Sqaure\\SqaureUtiles\\wjumpthrow.cfg");   // ID为102
        ExtractResource(IDR_FORWARD, "C:\\Sqaure\\SqaureUtiles\\forward.cfg");         // ID为103
        ExtractResource(IDR_DESUBTICK, "C:\\Sqaure\\SqaureUtiles\\desubtick.cfg");
            createConfigFileA("C:\\Sqaure\\Sqaure.cfg");
            cout << "CFG已创建，是否添加到autoexec（推荐），如果需要请输入Y，如不需要请输入N，如想要复原请输入R:";
            string autoexec;
            cin >> autoexec;
            ofstream autoexecfiles("C:\\Sqaure\\autoexec.cfg");
            if (autoexec == "Y" || autoexec == "y")
            {
                autoexecfiles << "alias +pwaswitchknife \"slot3\"\n";
                autoexecfiles << "alias -pwaswitchknife \"lastinv\"\n";
                autoexecfiles << "alias \"refundall\" \"sellback 0; sellback 1; sellback 2; sellback 3; sellback 4; sellback 5; sellback 6; sellback 8; sellback 9; sellback 10; sellback 11; sellback 12; sellback 13; sellback 14; sellback 15; sellback 16; sellback 17; sellback 18; sellback 26; sellback 26; sellback 27; sellback 28; sellback 29; sellback 30; sellback 32; sellback 33; sellback 34; sellback 35; play ui\\panorama\\itemtile_click_02.vsnd_c\"\n";
                autoexecfiles << "\n+exec Sqaure\n";
            }
            else if (autoexec == "R" || autoexec == "r")
            {
                autoexecfiles << "alias +pwaswitchknife \"slot3\"\n";
                autoexecfiles << "alias -pwaswitchknife \"lastinv\"\n";
                autoexecfiles << "alias \"refundall\" \"sellback 0; sellback 1; sellback 2; sellback 3; sellback 4; sellback 5; sellback 6; sellback 8; sellback 9; sellback 10; sellback 11; sellback 12; sellback 13; sellback 14; sellback 15; sellback 16; sellback 17; sellback 18; sellback 26; sellback 26; sellback 27; sellback 28; sellback 29; sellback 30; sellback 32; sellback 33; sellback 34; sellback 35; play ui\\panorama\\itemtile_click_02.vsnd_c\"\n";

            }
            autoexecfiles.close();
            MessageBoxA(NULL, "文件生成完毕!请手动前往 C:\\Sqaure 文件夹下把所有东西复制到你的游戏CFG文件夹！！！", "提示", MB_OK | MB_OK);
            system("start C:\\Sqaure");

         //   return 1;
  //  }
/*
        system("cls");
        cout << "\n自动化模式启用\n\n";
        
        string gameName = "Counter-Strike Global Offensive";
        string gamePath = findGameInstallPath(gameName);

        string fullPath = gamePath + "\\game\\csgo\\cfg\\SqaureUtiles";
        createDirectoriesRecursively(gamePath + "\\game\\csgo\\cfg\\SqaureUtiles");
        ExtractResource(IDR_JUMPTHROW, gamePath + "\\game\\csgo\\cfg\\SqaureUtiles\\jumpthrow.cfg");
        ExtractResource(IDR_WJUMPTHROW, gamePath + "\\game\\csgo\\cfg\\SqaureUtiles\\wjumpthrow.cfg");
        ExtractResource(IDR_FORWARD, gamePath + "\\game\\csgo\\cfg\\SqaureUtiles\\forward.cfg");
        ExtractResource(IDR_DESUBTICK, gamePath + "\\game\\csgo\\cfg\\SqaureUtiles\\desubtick.cfg");
        ExtractResource(IDR_DUCKJUMP1, gamePath + "\\game\\csgo\\cfg\\SqaureUtiles\\+duckjump.cfg");
        ExtractResource(IDR_DUCKJUMP2, gamePath + "\\game\\csgo\\cfg\\SqaureUtiles\\-duckjump.cfg");
        if (!gamePath.empty()) {
            cout << gameName << " 安装路径: " << gamePath << endl;
            cout << "配置文件路径: " << gamePath + "\\game\\csgo\\cfg" << endl;

            string configFilePath = gamePath + "\\game\\csgo\\cfg\\Sqaure.cfg";
            createConfigFileA(configFilePath);

            system("cls");
            cout << "CFG已创建，是否添加到autoexec（推荐），如果需要请输入Y，如不需要请输入N，如想要复原请输入R:";
            string autoexec;
            cin >> autoexec;
            ofstream autoexecfiles(gamePath+"\\game\\csgo\\cfg\\autoexec.cfg");
            if (autoexec == "Y" || autoexec == "y")
            {
                autoexecfiles << "alias +pwaswitchknife \"slot3\"\n";
                autoexecfiles << "alias -pwaswitchknife \"lastinv\"\n";
                autoexecfiles << "alias \"refundall\" \"sellback 0; sellback 1; sellback 2; sellback 3; sellback 4; sellback 5; sellback 6; sellback 8; sellback 9; sellback 10; sellback 11; sellback 12; sellback 13; sellback 14; sellback 15; sellback 16; sellback 17; sellback 18; sellback 26; sellback 26; sellback 27; sellback 28; sellback 29; sellback 30; sellback 32; sellback 33; sellback 34; sellback 35; play ui\\panorama\\itemtile_click_02.vsnd_c\"\n";
                autoexecfiles << "\n+exec Sqaure\n";
            }
            else if (autoexec == "R" || autoexec == "r")
            {
                autoexecfiles << "alias +pwaswitchknife \"slot3\"\n";
                autoexecfiles << "alias -pwaswitchknife \"lastinv\"\n";
                autoexecfiles << "alias \"refundall\" \"sellback 0; sellback 1; sellback 2; sellback 3; sellback 4; sellback 5; sellback 6; sellback 8; sellback 9; sellback 10; sellback 11; sellback 12; sellback 13; sellback 14; sellback 15; sellback 16; sellback 17; sellback 18; sellback 26; sellback 26; sellback 27; sellback 28; sellback 29; sellback 30; sellback 32; sellback 33; sellback 34; sellback 35; play ui\\panorama\\itemtile_click_02.vsnd_c\"\n";

            }
            autoexecfiles.close();
        }
        else {
            cout << "未找到 " << gameName << " 的安装路径，请使用手动模式！" << endl;
            string defaultConfigPath = "C:\\Sqaure.cfg";
            createConfigFileA(defaultConfigPath);
        }

    */

    

   

    cout << "成功了！现在你可以添加启动项 +exec Sqaure 进行使用，如果你没有添加到autoexec的话\n程序将在10秒后自动关闭";
    Sleep(10000);
    return 0;

}



// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
