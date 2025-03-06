@echo off
color 0a
title FOV Calculator
chcp 65001 >nul
mode con: cols=105 lines=30
cls

rem 提示用户输入

echo prees "~" to open console in CS2, input " sensitivity "，to get value,we called "A"
echo then input "m_yaw" to get value we called "B"
echo then edit config setting  "yaw 11688.311688 1 1" 's "11688.311688"，replace the value to your
echo.
echo 
echo.

rem Now input 180, "A", "B"

:inputX
set /p X=Input X value here (Normally it's 180):
if "%X%"=="" (
    echo X can't be void！
    goto inputX
)

:inputA
set /p A=Please input "A" Value:
if "%A%"=="" (
    echo A cant be void！
    goto inputA
)
if "%A%"=="0" (
    echo A cant be 0！
    goto inputA
)

:inputB
set /p B=input B value here:
if "%B%"=="" (
    echo B cant be void！
    goto inputB
)
if "%B%"=="0" (
    echo B cant be 0！
    goto inputB
)

rem 使用 PowerShell 计算公式 X / (A * B) 并四舍五入到六位小数

for /f "tokens=*" %%R in ('powershell -command "[math]::Round((%X% / (%A% * %B%)), 6)"') do set result=%%R

rem 显示结果

echo your value: %result%
echo.
echo copy it with mouse2
echo.
echo prees any key exit
pause >nul
