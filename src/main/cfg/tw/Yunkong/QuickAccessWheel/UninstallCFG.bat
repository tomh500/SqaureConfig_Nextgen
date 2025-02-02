@echo off
chcp 65001 >nul 2>&1

REM 檢查 64 位系統路徑
if exist "%SystemRoot%\SysWOW64" path %path%;%windir%\SysNative;%SystemRoot%\SysWOW64;%~dp0

REM 確認是否有管理員權限
bcdedit >nul
if '%errorlevel%' NEQ '0' (
    goto UACPrompt
) else (
    goto UACAdmin
)
:UACPrompt
%1 start "" mshta vbscript:createobject("shell.application").shellexecute("""%~0""","::",,"runas",1)(window.close)&exit
exit /B
:UACAdmin
cd /d "%~dp0"

REM 設定語言變數
setlocal enabledelayedexpansion
REM 預設語言為繁體中文
set Lang=TraditionalChinese

REM 根據系統語言設置 Lang 變數
if /I "%SystemLanguage%"=="zh-TW" (
    set Lang=TraditionalChinese
) else if /I "%SystemLanguage%"=="zh-CN" (
    set Lang=SimplifiedChinese
) else if /I "%SystemLanguage%"=="en-US" (
    set Lang=English
)

REM 根據語言選擇顯示訊息
if "%Lang%"=="TraditionalChinese" (
    echo 當前語言是繁體中文。
) else if "%Lang%"=="SimplifiedChinese" (
    echo 当前语言是简体中文。
) else (
    echo The current language is English.
)
timeout /t 1 >nul

REM 檢查是否在 QuickAccessWheel 資料夾中
for %%I in (.) do set CurrDirName=%%~nxI
if /I not "%CurrDirName%"=="QuickAccessWheel" (
    cls
    color 0C
    if "%Lang%"=="TraditionalChinese" (
        echo 請把此資料夾放進 QuickAccessWheel 資料夾中!!!
        echo 請把此資料夾放進 QuickAccessWheel 資料夾中!!!
        echo 請把此資料夾放進 QuickAccessWheel 資料夾中!!!
        echo 請確保此資料夾在 *\Counter-Strike Global Offensive\game\csgo\cfg\QuickAccessWheel 目錄當中
    ) else if "%Lang%"=="SimplifiedChinese" (
        echo 请把此文件夹放进 QuickAccessWheel 文件夹中!!!
        echo 请把此文件夹放进 QuickAccessWheel 文件夹中!!!
        echo 请把此文件夹放进 QuickAccessWheel 文件夹中!!!
        echo 请确保此文件夹在 *\Counter-Strike Global Offensive\game\csgo\cfg\QuickAccessWheel 目录當中
    ) else (
        echo Please place this folder into the QuickAccessWheel folder!!!
        echo Please place this folder into the QuickAccessWheel folder!!!
        echo Please place this folder into the QuickAccessWheel folder!!!
        echo Please make sure this folder is in the *\Counter-Strike Global Offensive\game\csgo\cfg\QuickAccessWheel directory.
    )
    echo.
    if "%Lang%"=="TraditionalChinese" (
        echo.請按任意鍵退出。
    ) else if "%Lang%"=="SimplifiedChinese" (
        echo.请按任意键退出。
    ) else (
        echo. Press any key to exit.
    )
    pause >nul
    exit /b
)

REM 檢測 QuickAccessWheel 放置位置
cd /d %~dp0
cd ../../
set "EXPECTED_FOLDER_NAME=csgo"
for %%F in (.) do set "CURRENT_FOLDER_NAME=%%~nxF"
if "%Lang%"=="TraditionalChinese" (
    echo 當前資料夾名稱: %CURRENT_FOLDER_NAME%
    echo 預期資料夾名稱: %EXPECTED_FOLDER_NAME%
) else if "%Lang%"=="SimplifiedChinese" (
    echo 当前文件夹名称: %CURRENT_FOLDER_NAME%
    echo 预期文件夹名称: %EXPECTED_FOLDER_NAME%
) else (
    echo Current folder name: %CURRENT_FOLDER_NAME%
    echo Expected folder name: %EXPECTED_FOLDER_NAME%
)
if /I "%CURRENT_FOLDER_NAME%" neq "%EXPECTED_FOLDER_NAME%" (
    cls
    color 0C
    if "%Lang%"=="TraditionalChinese" (
        echo 您的 QuickAccessWheel 放置位置錯誤!!!，請重看使用說明
        echo 您的 QuickAccessWheel 放置位置錯誤!!!，請重看使用說明
        echo 您的 QuickAccessWheel 放置位置錯誤!!!，請重看使用說明
        echo 請確保此資料夾放在 *\Counter-Strike Global Offensive\game\csgo\cfg 目錄當中
    ) else if "%Lang%"=="SimplifiedChinese" (
        echo 您的 QuickAccessWheel 放置位置错误!!!，请重看使用说明
        echo 您的 QuickAccessWheel 放置位置错误!!!，请重看使用说明
        echo 您的 QuickAccessWheel 放置位置错误!!!，请重看使用说明
        echo 请确保此文件夹放在 *\Counter-Strike Global Offensive\game\csgo\cfg 目录当中
    ) else (
        echo Your QuickAccessWheel placement is incorrect!!! Please refer to the instructions again.
        echo Your QuickAccessWheel placement is incorrect!!! Please refer to the instructions again.
        echo Your QuickAccessWheel placement is incorrect!!! Please refer to the instructions again.
        echo Please make sure this folder is placed in the *\Counter-Strike Global Offensive\game\csgo\cfg directory.
    )
    echo.
    if "%Lang%"=="TraditionalChinese" (
        echo.請按任意鍵退出。
    ) else if "%Lang%"=="SimplifiedChinese" (
        echo.请按任意键退出。
    ) else (
        echo. Press any key to exit.
    )
    pause >nul
    exit /b
)

cd ./cfg/QuickAccessWheel/

cls
color 0A
call powershell.exe -ExecutionPolicy Bypass -File ".\install\Uninstall_Resource.ps1"
