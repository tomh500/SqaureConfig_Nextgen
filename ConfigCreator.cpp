#include <iostream>
#include <windows.h>
#include <fstream>
#include <string>

#include "GetCPUInfo.h"
using namespace std;
void createConfigFileA(const string& path) {

    string bindCrossHairSwitch;
    string bunnyhop;
    string faststop;
    string faststopchanger;
    string jm;
    string autolookweapon;
    string shootcanceldef;
    string desubtick;
    string jumpthrow;
    string wjumpthrow;
    string duckjump;
    string walk;
    string howtomove;
    string betterbhop;
    string mwheelshooter;
    ofstream configFile(path, std::ios::out | std::ios::binary);

    /*
     cout << "\n请输入跳投绑定键，留空为不启用：";
     getline(cin, jumpthrow);
     configFile << "alias +JT \"echo \"SqaureUtiles/jumpthrow\" | exec; \"\n";
     configFile << "bind " << jumpthrow << " +JT\n";

     cout << "\n请输入W+跳投绑定键，留空为不启用：";
     getline(cin, wjumpthrow);
     configFile << "alias +WJT \"echo \"SqaureUtiles/wjumpthrow\" | exec; \"\n";
     configFile << "alias -WJT \"echo \"SqaureUtiles/forward\" | exec; \"\n";
     configFile << "bind " << wjumpthrow << " +WJT\n";

     cout << "\n请输入大跳绑定键，留空为不启用：";
     getline(cin, duckjump);
     configFile << "alias \"+ag3\" \"echo SqaureUtiles/+duckjump | exec;\"\n";
     configFile << "alias \"-ag3\" \"echo SqaureUtiles/-duckjump | exec;\"\n";
     configFile << "bind " << duckjump << " +ag3\n\n";

    */
    const char BOM[] = { 0xEF, 0xBB, 0xBF };
    configFile.write(BOM, sizeof(BOM));  
    int configbind;
    cout << "\n\n本程序提供：\n0.生成配置文件\n1.跟随后坐力绑定\n2.滚轮绑定\n3.移动方案\n4.特殊视角绑定\n5.自动检视绑定\n6.滚轮手枪速射\n\n";
        cout << "\n\n请输入绑定跟随后坐力的按键（推荐CapsLock）：";
        getline(cin, bindCrossHairSwitch);

        cout << "\n请输入滚轮绑定模式（大写字母）\nA 全滚轮跳\nB 上滚轮跳下滚轮切枪\nC 下滚轮跳上滚轮切枪\nD 切枪模式\nE DeSubtick跳\nF null\n你选择：";
        getline(cin, bunnyhop);

        if (configFile.is_open()) {
            string vendor = getCPUVendor();

            if (vendor == "GenuineIntel") {
                //    configFile << "fps_max 159\n";
            }
            configFile << "sv_cheats 1\n";
            configFile << "alias t \"TickerEventer1;TickerEventer2;TickerEventer3;TickerEventer4\"\n";
            configFile << "alias TickerEventer1 \"\"\n";
            configFile << "alias TickerEventer2 \"\"\n";
            configFile << "alias TickerEventer3 \"\"\n";
            configFile << "alias TickerEventer4 \"\"\n";

            configFile << "alias +DuckJump \"alias TickerEventer3 +DuckJump_T\"\n";
            configFile << "alias -DuckJump \"alias TickerEventer3 -DuckJump_T\"\n";
            configFile << "alias +DuckJump_T \"+duck; +jump; alias TickerEventer3 \"\";\"\n";
            configFile << "alias -DuckJump_T \"duck -999 0 0; jump -999 0 0; alias TickerEventer3 \"\";\"\n";

            configFile << "alias WheelJump___ \"alias TickerEventer4 TickerWheelJump\"\n";
            configFile << "alias TickerWheelJump \"-jump;+jump;-jump;+jump;-jump;+jump;-jump;-jump; alias TickerEventer4 \"\";\"\n";

            // 投出
            configFile << "alias ThrowingAction \"alias TickerEventer1 ThrowingAction_t\"\n";
            configFile << "alias ThrowingAction_t \"+attack; -attack; -attack; -attack2; -attack2; alias TickerEventer1 \"\";\"\n";

            // 正常跳投
            configFile << "alias +JumpingAction \"alias TickerEventer2 +JumpingAction_T\"\n";
            configFile << "alias -JumpingAction \"alias TickerEventer2 -JumpingAction_T\"\n";
            configFile << "alias +JumpingAction_T \"+jump; ThrowingAction; alias TickerEventer2 \"\";\"\n";
            configFile << "alias -JumpingAction_T \"-jump; alias TickerEventer2 \"\";\"\n";

            configFile << "alias +JumpingActionW \"alias TickerEventer2 +JumpingActionW_T\"\n";
            configFile << "alias -JumpingActionW \"alias TickerEventer2 -JumpingActionW_T\"\n";
            configFile << "alias +JumpingActionW_T \"+forward; +jump; ThrowingAction; alias TickerEventer2 \"\";\"\n";
            configFile << "alias -JumpingActionW_T \"-forward; -jump; alias TickerEventer2 \"\";\"\n";

            configFile << "alias +JumpingActionS \"alias TickerEventer2 +JumpingActionS_T\"\n";
            configFile << "alias -JumpingActionS \"alias TickerEventer2 -JumpingActionS_T\"\n";
            configFile << "alias +JumpingActionS_T \"+back; +jump; ThrowingAction; alias TickerEventer2 \"\";\"\n";
            configFile << "alias -JumpingActionS_T \"-back; -jump; alias TickerEventer2 \"\";\"\n";

            configFile << "alias +JumpingActionA \"alias TickerEventer2 +JumpingActionA_T\"\n";
            configFile << "alias -JumpingActionA \"alias TickerEventer2 -JumpingActionA_T\"\n";
            configFile << "alias +JumpingActionA_T \"+right; +jump; ThrowingAction; alias TickerEventer2 \"\";\"\n";
            configFile << "alias -JumpingActionA_T \"-right; -jump; alias TickerEventer2 \"\";\"\n";

            configFile << "alias +JumpingActionD \"alias TickerEventer2 +JumpingActionD_T\"\n";
            configFile << "alias -JumpingActionD \"alias TickerEventer2 -JumpingActionD_T\"\n";
            configFile << "alias +JumpingActionD_T \"+left; +jump; ThrowingAction; alias TickerEventer2 \"\";\"\n";
            configFile << "alias -JumpingActionD_T \"-left; -jump; alias TickerEventer2 \"\";\"\n";

            configFile << "alias JumpingActionW____ \"alias +JumpThrow_ +JumpingActionW; alias -JumpThrow_ -JumpingActionW\"\n";
            configFile << "alias JumpingActionS____ \"alias +JumpThrow_ +JumpingActionS; alias -JumpThrow_ -JumpingActionS\"\n";
            configFile << "alias JumpingActionA____ \"alias +JumpThrow_ +JumpingActionA; alias -JumpThrow_ -JumpingActionA\"\n";
            configFile << "alias JumpingActionD____ \"alias +JumpThrow_ +JumpingActionD; alias -JumpThrow_ -JumpingActionD\"\n";

            configFile << "alias JumpingActionSwitcher_ \"JumpingActionW__\"\n";
            configFile << "alias JumpingActionW__ \"JumpingActionW____; alias JumpingActionSwitcher_ JumpingActionS__\"\n";
            configFile << "alias JumpingActionS__ \"JumpingActionS____; alias JumpingActionSwitcher_ JumpingActionA__\"\n";
            configFile << "alias JumpingActionA__ \"JumpingActionA____; alias JumpingActionSwitcher_ JumpingActionD__\"\n";
            configFile << "alias JumpingActionD__ \"JumpingActionD____; alias JumpingActionSwitcher_ JumpingActionW__\"\n";

            configFile << "alias +JumpThrow_2 \"+JumpingAction\"\n";
            configFile << "alias -JumpThrow_2 \"-JumpingAction\"\n";

            configFile << "exec_async JumpThrow/setup_async\n";


            //准星部分
            configFile << "alias active \"cl_crosshaircolor_r 255; cl_crosshaircolor_g 0; cl_crosshaircolor_b 0; cl_crosshair_recoil true\"\n";
            configFile << "alias passive \"cl_crosshaircolor_r 0; cl_crosshaircolor_g 255; cl_crosshaircolor_b 0; cl_crosshair_recoil false\"\n";
            configFile << "alias xhair \"xhair1\"\n";
            configFile << "alias xhair1 \"active; alias xhair xhair2\"\n";
            configFile << "alias xhair2 \"passive; alias xhair xhair1\"\n";
            configFile << "bind " << bindCrossHairSwitch << " xhair\n";

            cout << "\n请绑定连跳增强，留空以不启用：";
            getline(cin, betterbhop);
            if (betterbhop == "space" || betterbhop == "SPACE")
            {
                configFile << "alias +spacejump \"fps_max 64;bind MWHEELUP doup;bind MWHEELDOWN doup;+jump\"\n";
                configFile << "alias -spacejump \"fps_max 0;-jump;\"\n";
            }
            else
            {
                configFile << "alias +btbhop \"fps_max 64\"\n";
                configFile << "alias -btbhop \"fps_max 0\"\n";
                configFile << "bind " << betterbhop << " \"+btbhop\"\n";

            }

            //滚轮跳
            if (bunnyhop == "A" || bunnyhop == "a")
            {
                configFile << "alias sq_mwtools_up \"+jump\"\n";
                configFile << "alias sq_mwtools_down \"+jump\"\n";
                if (betterbhop == "space" || betterbhop == "SPACE")
                {
                    configFile << "bind MWHEELDOWN \"doup\"\n";
                    configFile << "bind MWHEELUP \"doup\"\n";
                    configFile << "alias doup \"-jump;bind MWHEELDOWN +jump;bind MWHEELUP +jump\"\n";
                    configFile << "bind space +spacejump\n";

                }
                else
                {
                    configFile << "bind MWHEELDOWN \"+jump\"\n";
                    configFile << "bind MWHEELUP \"+jump\"\n";
                }
            }
            else if (bunnyhop == "B" || bunnyhop == "b")
            {
                
                configFile << "alias sq_mwtools_down \"invnext\"\n";
    

                if (betterbhop == "space" || betterbhop == "SPACE")
                {
                    configFile << "alias sq_mwtools_up \"doup\"\n";
                    configFile << "bind MWHEELDOWN \"sq_mwtools_down\"\n";
                    configFile << "bind MWHEELUP \"sq_mwtools_up\"\n";
                    configFile << "alias doup \"-jump;bind MWHEELUP sq_mwtools_up\"\n";
                    configFile << "bind space +spacejump\n";

                }
                else
                {
                    configFile << "alias sq_mwtools_up \"+jump\"\n";
                    configFile << "bind MWHEELDOWN \"sq_mwtools_down\"\n";
                    configFile << "bind MWHEELUP \"sq_mwtools_up\"\n";
                }
            }
            else if (bunnyhop == "C" || bunnyhop == "c")
            {
          
                configFile << "alias sq_mwtools_up \"invprev\"\n";
                
                if (betterbhop == "space" || betterbhop == "SPACE")
                {
                    configFile << "alias sq_mwtools_down \"doup\"\n";
                    configFile << "bind MWHEELDOWN \"sq_mwtools_down\"\n";
                    configFile << "bind MWHEELUP \"invprev\"\n";

                    configFile << "alias doup \"-jump;bind MWHEELDOWN +jump;bind MWHEELUP +jump\"\n";
                    configFile << "bind space +spacejump\n";
                }
                else
                {
                    configFile << "alias sq_mwtools_down \"+jump\"\n";
                    configFile << "bind MWHEELDOWN \"+jump\"\n";
                    configFile << "bind MWHEELUP \"invprev\"\n";
                }
            }
            else if (bunnyhop == "D" || bunnyhop == "d")
            {
                configFile << "alias sq_mwtools_up \"invprev\"\n";
                configFile << "alias sq_mwtools_down \"invnext\"\n";
                configFile << "bind MWHEELDOWN \"sq_mwtools_down\"\n";
                configFile << "bind MWHEELUP \"sq_mwtools_up\"\n";
            }
            else if (bunnyhop == "E" || bunnyhop == "e")
            {
                configFile << "alias sq_mwtools_up \"doup\"\n";
                configFile << "alias sq_mwtools_down \"doup\"\n";
                if (betterbhop == "SPACE" || betterbhop == "space")
                {
                    configFile << "bind space +spacejump\n";
                    configFile << "bind MWHEELDOWN \"sq_mwtools_up\"\n";
                    configFile << "bind MWHEELUP \"sq_mwtools_up\"\n";
                    configFile << "alias +execjump \"exec SqaureUtiles/Desubtick\"\n";
                    configFile << "alias -execjump \"-jump\"\n";
                    configFile << "alias doup \"-jump;bind MWHEELDOWN +execjump;bind MWHEELUP +execjump\"\n\n";

                }
                else
                {
                    configFile << "alias sq_mwtools_up \"+execjump;clear\"\n";
                    configFile << "alias sq_mwtools_down \"+execjump;clear\"\n";
                    configFile << "bind MWHEELDOWN \"sq_mwtools_down\"\n";
                    configFile << "bind MWHEELUP \"sq_mwtools_up\"\n";
                    configFile << "alias +execjump\"exec SqaureUtiles/desubtick\"\n";
                    configFile << "alias -execjump \"-jump\"\n\n";
                }
          

            }
            else {

            }
            cout << "\n请输入滚轮手枪速射开关：";
            getline(cin, mwheelshooter);
            configFile << "alias shooter_on \"bind mwheelup +attack;bind mwheeldown +attack\"\n";
            configFile << "alias shooter_off \"bind mwheelup sq_mwtools_up;bind mwheeldown sq_mwtools_down;\"\n";
            configFile << "alias st1 \"st2\"\n";
            configFile << "alias st2 \"shooter_on;alias st1 st3;say_team [shooter]1\"\n";
            configFile << "alias st3 \"shooter_off;alias st1 st2;say_team [shooter]0\"\n\n";
            configFile << "bind " << mwheelshooter << " st1\n";

            configFile << "\n";

            //急停部分
            /*
            cout << "\n\n是否启用急停CONFIG，如需使用请输入Y，否则输入N：";
            getline(cin, faststop);
            cout << "\n检测到你开启急停，请绑定一个临时急停开关键：";
            getline(cin, faststopchanger);
            if (faststop == "Y" || faststop == "y")
            {
               cout << "请输入急停方案(A/B):";
           string  jtfa = "A";
            getline(cin, jtfa);
                if (jtfa == "A" || jtfa == "a")
                {
                    cout << "\n请输入你绑定的静步键：";
                    getline(cin, walk);
                    configFile << "\nalias SetupMv \"Mv.True\"\n";
                    configFile << "alias Mv.True \"alias +AS_FMethod +AS_Forward;alias +AS_BMethod +AS_Back;alias +AS_LMethod +AS_Left;alias +AS_RMethod +AS_Right; alias -AS_FMethod -AS_Forward;alias -AS_BMethod -AS_Back;alias -AS_LMethod -AS_Left;alias -AS_RMethod -AS_Right; ResetJoy\"\n";
                    configFile << "alias LoadMv \"bind W +actf;bind S +acts;bind A +acta;bind D +actr\"\n";
                    configFile << "alias MvFuse \"alias MvFuse; setinfo MvFuse 0; alias DoLoadMv DefaultMovement; alias sv_shutdown_requested alias DoLoadMv LoadMv; status_json | alias; sv_shutdown_requested; SetupMv\"\n";
                    configFile << "MvFuse\n";
                    configFile << "DoLoadMv\n";
                    configFile << "skill 1\n";
                    configFile << "joy_response_move 1\n";
                    configFile << "joy_side_sensitivity 1\n";
                    configFile << "joy_forward_sensitivity 1\n";
                    configFile << "ResetJoy\n";
                    configFile << "forward -31200 0 0\n";
                    configFile << "back -31200 0 0\n";
                    configFile << "left -31200 0 0\n";
                    configFile << "right -31200 0 0\n";
                    configFile << "alias \"+AS_Forward\" \"+forward;joy_forward_sensitivity 0.0001;joy_side_sensitivity 0\"\n";
                    configFile << "alias \"-AS_Forward\" \"-forward;joy_forward_sensitivity -0.0001;joy_side_sensitivity 0\"\n";
                    configFile << "alias \"+AS_Back\" \"+back;joy_forward_sensitivity -0.0001;joy_side_sensitivity 0\"\n";
                    configFile << "alias \"-AS_Back\" \"-back;joy_forward_sensitivity 0.0001;joy_side_sensitivity 0\"\n";
                    configFile << "alias \"+AS_Left\" \"+left;joy_side_sensitivity -0.0001;joy_forward_sensitivity 0\"\n";
                    configFile << "alias \"-AS_Left\" \"-left;joy_side_sensitivity 0.0001;joy_forward_sensitivity 0\"\n";
                    configFile << "alias \"+AS_Right\" \"+right;joy_side_sensitivity 0.0001;joy_forward_sensitivity 0\"\n";
                    configFile << "alias \"-AS_Right\" \"-right;joy_side_sensitivity -0.0001;joy_forward_sensitivity 0\"\n";
                    configFile << "alias +actf \"forwardback 1 0 0;bind W +AS_FMethod;bind S +AS_BMethod\"\n";
                    configFile << "alias +acts \"forwardback 1 0 0;joy_forward_sensitivity -1;bind W +AS_FMethod;bind S +AS_BMethod\"\n";
                    configFile << "alias +actr \"rightleft 1 0 0;bind A +AS_LMethod;bind D +AS_RMethod\"\n";
                    configFile << "alias +acta \"rightleft 1 0 0;joy_side_sensitivity -1;bind A +AS_LMethod;bind D +AS_RMethod\"\n";
                    configFile << "alias \"+AS_Forwardb\" \"+forward;joy_forward_sensitivity 0.0001;joy_side_sensitivity 0\"\n";
                    configFile << "alias \"-AS_Forwardb\" \"-forward;joy_forward_sensitivity 0;joy_side_sensitivity 0\"\n";
                    configFile << "alias \"+AS_Backb\" \"+back;joy_forward_sensitivity -0.0001;joy_side_sensitivity 0\"\n";
                    configFile << "alias \"-AS_Backb\" \"-back;joy_forward_sensitivity 0;joy_side_sensitivity 0\"\n";
                    configFile << "alias \"+AS_Leftb\" \"+left;joy_side_sensitivity -0.0001;joy_forward_sensitivity 0\"\n";
                    configFile << "alias \"-AS_Leftb\" \"-left;joy_side_sensitivity 0;joy_forward_sensitivity 0\"\n";
                    configFile << "alias \"+AS_Rightb\" \"+right;joy_side_sensitivity 0.0001;joy_forward_sensitivity 0\"\n";
                    configFile << "alias \"-AS_Rightb\" \"-right;joy_side_sensitivity 0;joy_forward_sensitivity 0\"\n";
                    configFile << "alias \"+AS_Forwards\" \"+forward;joy_forward_sensitivity 0.0001;joy_side_sensitivity 0\"\n";
                    configFile << "alias \"-AS_Forwards\" \"-forward;joy_forward_sensitivity 0;joy_side_sensitivity 0\"\n";
                    configFile << "alias \"+AS_Backs\" \"+back;joy_forward_sensitivity -0.0001;joy_side_sensitivity 0\"\n";
                    configFile << "alias \"-AS_Backs\" \"-back;joy_forward_sensitivity 0;joy_side_sensitivity 0\"\n";
                    configFile << "alias \"+AS_Lefts\" \"+left;joy_side_sensitivity -0.0001;joy_forward_sensitivity 0\"\n";
                    configFile << "alias \"-AS_Lefts\" \"-left;joy_side_sensitivity 0;joy_forward_sensitivity 0\"\n";
                    configFile << "alias \"+AS_Rights\" \"+right;joy_side_sensitivity 0.0001;joy_forward_sensitivity 0\"\n";
                    configFile << "alias \"-AS_Rights\" \"-right;joy_side_sensitivity 0;joy_forward_sensitivity 0\"\n";
                    configFile << "alias +space \"+sprint;alias +AS_FMethod +AS_Forwardb;alias +AS_LMethod +AS_Leftb;alias +AS_BMethod +AS_Backb;alias +AS_RMethod +AS_Rightb;alias -AS_FMethod -AS_Forwardb;alias -AS_LMethod -AS_Leftb;alias -AS_BMethod -AS_Backb;alias -AS_RMethod -AS_Rightb\"\n";
                    configFile << "alias -space \"-sprint;alias +AS_FMethod +AS_Forward;alias +AS_LMethod +AS_Left;alias +AS_BMethod +AS_Back;alias +AS_RMethod +AS_Right;alias -AS_FMethod -AS_Forward;alias -AS_LMethod -AS_Left;alias -AS_BMethod -AS_Back;alias -AS_RMethod -AS_Right\"\n";
                    configFile << "bind " << walk << " +space\"\n";
                    configFile << "alias both \"alias +AS_FMethod +AS_Forwardb;alias +AS_LMethod +AS_Leftb;alias +AS_BMethod +AS_Backb;alias +AS_RMethod +AS_Rightb;alias -AS_FMethod -AS_Forwardb;alias -AS_LMethod -AS_Leftb;alias -AS_BMethod -AS_Backb;alias -AS_RMethod -AS_Rightb;alias both single\"\n";
                    configFile << "alias single \"alias +AS_FMethod +AS_Forward;alias +AS_LMethod +AS_Left;alias +AS_BMethod +AS_Back;alias +AS_RMethod +AS_Right;alias -AS_FMethod -AS_Forward;alias -AS_LMethod -AS_Left;alias -AS_BMethod -AS_Back;alias -AS_RMethod -AS_Right;alias both both1\"\n";
                    configFile << "alias both1 \"alias +AS_FMethod +AS_Forwardb;alias +AS_LMethod +AS_Leftb;alias +AS_BMethod +AS_Backb;alias +AS_RMethod +AS_Rightb;alias -AS_FMethod -AS_Forwardb;alias -AS_LMethod -AS_Leftb;alias -AS_BMethod -AS_Backb;alias -AS_RMethod -AS_Rightb;alias both single\"\n";
                    configFile << "alias ++w +forward\nalias ++s +back\nalias ++a +left\nalias ++d +right\nalias -+w -forward\nalias -+s -back\nalias -+a -left\nalias -+d -right\n\n\n";

                    configFile << "alias jton \"bind w +actf; bind s +acts; bind d +actr; bind a +acta\"\n";
                    configFile << "alias jtoff \"bind w ++w; bind s ++s; bind d ++d; bind a ++a;joy_side_sensitivity 0;joy_forward_sensitivity 0\n\n";
                    configFile << "alias jt1 \"jt2;\"\n";
                    configFile << "alias jt2 \"jton; alias jt1 jt3\"\n";
                    configFile << "alias jt3 \"jtoff; alias jt1 jt2\"\n";
                
                    configFile << "bind " << faststopchanger << " jt1\n\n";
                }
                else
                {
                    configFile << "\nalias output_team \"alias cout_on say_team [JN9]Enabled;alias cout_off say_team [JN9]Disabled\"\n";
                    configFile << "alias output_all \"alias cout_on say [JN9]Enabled;alias cout_off say [JN9]Disabled\"\n";

                    configFile << "alias joy_response_move \"1\"\n";
                    configFile << "alias joy_forward_sensitivity \"1\"\n";
                    configFile << "alias joy_side_sensitivity \"1\"\n";
                    configFile << "alias forward \"-31200 0 0\"\n";
                    configFile << "alias back \"-31200 0 0\"\n";
                    configFile << "alias left \"-31200 0 0\"\n";
                    configFile << "alias right \"-31200 0 0\"\n";
                    configFile << "alias forwardback \"0 0 0\"\n";
                    configFile << "alias rightleft \"0 0 0\"\n";
                    configFile << "alias \"+w\" \"w1;\"\n";
                    configFile << "alias \"w1\" \"forwardback 1 0 0;joy_forward_sensitivity 1;joy_side_sensitivity 0;alias +s ws;alias +a wa;alias +d wd;alias -w w2;alias -s s2\"\n";
                    configFile << "alias \"+a\" \"a1\"\n";
                    configFile << "alias \"a1\" \"rightleft 1 0 0;joy_side_sensitivity -1;joy_forward_sensitivity 0;alias +d ad;alias +w aw;alias +s as;alias -a a2;alias -d d2\"\n";
                    configFile << "alias \"+s\" \"s1\"\n";
                    configFile << "alias \"s1\" \"forwardback 1 0 0;joy_forward_sensitivity -1;joy_side_sensitivity 0;alias +w sw;alias +a sa;alias +d sd;alias -s s2;alias -w w2\"\n";
                    configFile << "alias \"+d\" \"d1\"\n";
                    configFile << "alias \"d1\" \"rightleft 1 0 0;joy_side_sensitivity 1;joy_forward_sensitivity 0;alias +a da;alias +w dw;alias +s ds;alias -d d2;alias -a a2\"\n";
                    configFile << "alias \"-w\" \"w2;\"\n";
                    configFile << "alias \"w2\" \"joy_forward_sensitivity -0.000001;joy_side_sensitivity 0;alias +s s1;alias +a a1;alias +d d1;alias +w w1;\"\n";
                    configFile << "alias \"-a\" \"a2\"\n";
                    configFile << "alias \"a2\" \"joy_side_sensitivity 0.000001;joy_forward_sensitivity 0;alias +d d1;alias +w w1;alias +s s1;alias +a a1\"\n";
                    configFile << "alias \"-s\" \"s2\"\n";
                    configFile << "alias \"s2\" \"joy_forward_sensitivity 0.000001;joy_side_sensitivity 0;alias +w w1;alias +a a1;alias +d d1;alias +s s1\"\n";
                    configFile << "alias \"-d\" \"d2\"\n";
                    configFile << "alias \"d2\" \"joy_side_sensitivity -0.000001;joy_forward_sensitivity 0;alias +a a1;alias +w w1;alias +s s1;alias +d d1\"\n";
                    configFile << "alias \"wasdall\" \"joy_forward_sensitivity 0;joy_side_sensitivity 0;alias -w rs;alias -a fd;alias -d fa;alias -s rw\"\n";
                    configFile << "alias \"ws\" \"joy_forward_sensitivity -0.000001;joy_side_sensitivity 0;alias -s w1;alias -w s1;alias +a fa;alias +d fd\"\n";
                    configFile << "alias \"sw\" \"joy_forward_sensitivity 0.000001;joy_side_sensitivity 0;alias -w s1;alias -s w1;alias +a fa;alias +d fd\"\n";
                    configFile << "alias \"fa\" \"rightleft 1 0 0;joy_side_sensitivity -1;alias -a fa2;alias +d wasdall;alias -w as;alias -s aw\"\n";
                    configFile << "alias \"fd\" \"rightleft 1 0 0;joy_side_sensitivity 1;alias -d fd2;alias +a wasdall;alias -w ds;alias -s dw\"\n";
                    configFile << "alias \"fa2\" \"joy_side_sensitivity 0.000001;joy_forward_sensitivity 0;alias -s w1;alias -w s1;alias +a fa;alias +d fd\"\n";
                    configFile << "alias \"fd2\" \"joy_side_sensitivity -0.000001;joy_forward_sensitivity 0;alias -s w1;alias -w s1;alias +a fa;alias +d fd\"\n";
                    configFile << "alias \"ad\" \"joy_side_sensitivity 0.000001;joy_forward_sensitivity 0;alias -a d1;alias -d a1;alias +w rw;alias +s rs\"\n";
                    configFile << "alias \"da\" \"joy_side_sensitivity -0.000001;joy_forward_sensitivity 0;alias -d a1;alias -a d1;alias +w rw;alias +s rs\"\n";
                    configFile << "alias \"rw\" \"forwardback 1 0 0;joy_forward_sensitivity 1;alias -a wd;alias -d wa;alias -w rw2;alias +s wasdall\"\n";
                    configFile << "alias \"rs\" \"forwardback 1 0 0;joy_forward_sensitivity -1;alias -a sd;alias -d sa;alias -s rs2;alias +w wasdall\"\n";
                    configFile << "alias \"rw2\" \"joy_forward_sensitivity -0.000001;joy_side_sensitivity 0;alias -a d1;alias -d a1;alias +w rw;alias +s rs\"\n";
                    configFile << "alias \"rs2\" \"joy_forward_sensitivity 0.000001;joy_side_sensitivity 0;alias -a d1;alias -d a1;alias +w rw;alias +s rs\"\n";
                    configFile << "alias \"wa\" \"rightleft 1 0 0;joy_side_sensitivity -1;alias -a waa;alias -w waw;alias +d wad;alias +s was\"\n";
                    configFile << "alias \"aw\" \"forwardback 1 0 0;joy_forward_sensitivity 1;alias -a waa;alias -w waw;alias +d wad;alias +s was\"\n";
                    configFile << "alias \"was\" \"joy_forward_sensitivity -0.000001;alias -s aw;alias -w as;alias +d wasdall;alias -a fa2\"\n";
                    configFile << "alias \"wad\" \"joy_side_sensitivity 0.000001;alias -w rw2;alias +s wasdall;alias -d wa;alias -a wd\"\n";
                    configFile << "alias \"wd\" \"rightleft 1 0 0;joy_side_sensitivity 1;alias -d wdd;alias -w wdw;alias +a wda;alias +s wds\"\n";
                    configFile << "alias \"dw\" \"forwardback 1 0 0;joy_forward_sensitivity 1;alias -d wdd;alias -w wdw;alias +a wda;alias +s wds\"\n";
                    configFile << "alias \"wda\" \"joy_side_sensitivity 0.000001;alias -a wd;alias -d wa;alias -w rw2;alias +s wasdall\"\n";
                    configFile << "alias \"wds\" \"joy_forward_sensitivity -0.000001;alias -w ds;alias -d fd2;alias -s dw;alias +a wasdall\"\n";
                    configFile << "alias \"sa\" \"rightleft 1 0 0;joy_side_sensitivity -1;alias -a saa;alias -s sas;alias +d sad;alias +w saw\"\n";
                    configFile << "alias \"as\" \"forwardback 1 0 0;joy_forward_sensitivity -1;alias -a saa;alias -s sas;alias +d sad;alias +w saw\"\n";
                    configFile << "alias \"sad\" \"joy_side_sensitivity 0.000001;alias -a sd;alias -d sa;alias -s rs2;alias +w wasdall\"\n";
                    configFile << "alias \"saw\" \"joy_forward_sensitivity 0.000001;alias -w as;alias -a fa2;alias -s aw;alias +d wasdall\"\n";
                    configFile << "alias \"sd\" \"rightleft 1 0 0;joy_side_sensitivity 1;alias -d sdd;alias -s sds;alias +a sda;alias +w sdw\"\n";
                    configFile << "alias \"ds\" \"forwardback 1 0 0;joy_forward_sensitivity -1;alias -s sds;alias -d sdd;alias +a sda;alias +w sdw\"\n";
                    configFile << "alias \"sda\" \"joy_side_sensitivity 0.000001;alias -a sd;alias -d sa;alias +w wasdall;alias -s rs2\"\n";
                    configFile << "alias \"sdw\" \"joy_forward_sensitivity 0.000001;alias -w sd;alias -d fd2;alias -s dw;alias +a wasdall\"\n";
                    configFile << "alias \"waa\" \"joy_side_sensitivity 0;alias +s ws;alias +a wa;alias +d wd;alias -w w2\"\n";
                    configFile << "alias \"waw\" \"joy_forward_sensitivity 0;alias +d ad;alias +w aw;alias +s as;alias -a a2\"\n";
                    configFile << "alias \"saa\" \"joy_side_sensitivity 0;alias +w sw;alias +a sa;alias +d sd;alias -s s2\"\n";
                    configFile << "alias \"sas\" \"joy_forward_sensitivity 0;alias +d ad;alias +w aw;alias +s as;alias -a a2\"\n";
                    configFile << "alias \"wdd\" \"joy_side_sensitivity 0;alias +s ws;alias +a wa;alias +d wd;alias -w w2\"\n";
                    configFile << "alias \"wdw\" \"joy_forward_sensitivity 0;alias +a da;alias +w dw;alias +s ds;alias -d d2\"\n";
                    configFile << "alias \"sdd\" \"joy_side_sensitivity 0;alias +w sw;alias +a sa;alias +d sd;alias -s s2\"\n";
                    configFile << "alias \"sds\" \"joy_forward_sensitivity 0;alias +a da;alias +w dw;alias +s ds;alias -d d2\"\n";

                    configFile << "alias +firstw \"+w;say_team JN9 Loaded\"\n";
                    configFile << "alias -firstw \"bind w +w;-w\"\n";

                    configFile << "bind w +firstw\n";
                    configFile << "bind a +a\n";
                    configFile << "bind s +s\n";
                    configFile << "bind d +d\n";

                    configFile << "alias buygui \"buymenu;jtistrue2;\"\n";
                    configFile << "bind b buygui\n";

                    configFile << "alias cout_on \"say_team [JN9]Enabled\"\n";
                    configFile << "alias cout_off \"say_team [JN9]Disabled\"\n";

                    configFile << "alias jtistrue1 \"jtistrue2\"\n";
                    configFile << "alias jtistrue2 \"joy_forward_sensitivity 0;joy_side_sensitivity 0;bind w +forward;bind s +back;bind a +left;bind d +right;alias jtistrue1 jtistrue3;cout_off\"\n";
                    configFile << "alias jtistrue3 \"-forward;bind w +w;bind a +a;bind s +s;bind d +d;alias jtistrue1 jtistrue2;cout_on\"\n";
                    configFile << "bind "<< faststopchanger<<" jtistrue1\n\n";

                }
                
            }
            */
cout << "\n请输入移动方案（A/B）：";
    getline(cin, howtomove);
           if(howtomove=="A"||howtomove=="a")
            {
                configFile << "bind w +forward\n";
                configFile << "bind s +back\n";
                configFile << "bind d +right\n";
                
                configFile << "bind a +left\n\n";
                configFile << "bind mouse2 +attack2\n";
            }
           else if (howtomove == "b"||howtomove=="B")
           {
               configFile << "alias +joy_attack2 \"+attack2\"\n";
               configFile << "alias -joy_attack2 \"-attack2\"\n";
               configFile << "alias +joy_radialradio2 \"+radialradio2\"\n";
               configFile << "alias -joy_radialradio2 \"-radialradio2\"\n";
               configFile << "bind c +joy_radialradio2\n";
               configFile << "bind mouse2 +joy_attack2\n";
               configFile << "cl_scoreboard_mouse_enable_binding +joy_attack2\n";

               configFile << "alias open_buymenu \"-s;-a;-d;-w;move_keyboard\"\n";
               configFile << "alias close_buymenu \"-left;-right;-back;-forward;move_joy\"\n";

               configFile << "bind key move_switch\n";
               configFile << "alias move_keyboard \"bind a +left;bind d +right;bind w +forward;bind s +back;say_team 0\"\n";
               configFile << "alias move_joy \"bind a +a;bind d +d;bind w +w;bind s +s;say_team 1\"\n";
               configFile << "alias move_switch \"move_keyboard;\"\n";
               configFile << "alias move_switch_keyboard \"move_keyboard;alias move_switch move_joy\"\n";
               configFile << "alias move_switch_joy \"move_switch_joy;alias move_switch move_keyboard\"\n";

               configFile << "joy_response_move 1\n";
               configFile << "joy_forward_sensitivity 1\n";
               configFile << "joy_side_sensitivity 1\n";
               configFile << "forward -1009 0 0\n";
               configFile << "back -1009 0 0\n";
               configFile << "left -1009 0 0\n";
               configFile << "right -1009 0 0\n";
               configFile << "forwardback 0 0 0\n";
               configFile << "rightleft 0 0 0\n";

               configFile << "bind w \"+w;-radialradio2\"\n";
               configFile << "bind a \"+a;-radialradio2\"\n";
               configFile << "bind s \"+s;-radialradio2\"\n";
               configFile << "bind d \"+d;-radialradio2\"\n";

               configFile << "alias +w \"forwardback 1 0 0;joy_forward_sensitivity 1;alias +s ws\"\n";
               configFile << "alias w1 \"forwardback 1 0 0;joy_forward_sensitivity 1;alias +s ws\"\n";
               configFile << "alias ws \"forwardback 0 0 0;alias -s wss;alias -w wsw\"\n";
               configFile << "alias -w \"forwardback 0 0 0;alias +s s1;alias -s s2;alias -w w2;alias +w w1\"\n";
               configFile << "alias w2 \"forwardback 0 0 0;alias +s s1;alias -s s2;alias -w w2;alias +w w1\"\n";
               configFile << "alias +s \"forwardback 1 0 0;joy_forward_sensitivity -1;alias +w sw\"\n";
               configFile << "alias s1 \"forwardback 1 0 0;joy_forward_sensitivity -1;alias +w sw\"\n";
               configFile << "alias sw \"forwardback 0 0 0;alias -w wsw;alias -s wss\"\n";
               configFile << "alias -s \"forwardback 0 0 0;alias +w w1;alias -w w2;alias -s s2;alias +s s1\"\n";
               configFile << "alias s2 \"forwardback 0 0 0;alias +w w1;alias -w w2;alias -s s2;alias +s s1\"\n";
               configFile << "alias wss \"forwardback 1 0 0;joy_forward_sensitivity 1;alias +s ws;alias -w w2;alias -s s2\"\n";
               configFile << "alias wsw \"forwardback 1 0 0;joy_forward_sensitivity -1;alias +w sw;alias -s s2;alias -w w2\"\n";

               configFile << "alias +a \"rightleft 1 0 0;joy_side_sensitivity -1;alias +d ad\"\n";
               configFile << "alias a1 \"rightleft 1 0 0;joy_side_sensitivity -1;alias +d ad\"\n";
               configFile << "alias ad \"rightleft 0 0 0;alias -d add;alias -a ada\"\n";
               configFile << "alias -a \"rightleft 0 0 0;alias +d d1;alias -d d2;alias -a a2;alias +a a1\"\n";
               configFile << "alias a2 \"rightleft 0 0 0;alias +d d1;alias -d d2;alias -a a2;alias +a a1\"\n";
               configFile << "alias +d \"rightleft 1 0 0;joy_side_sensitivity 1;alias +a da\"\n";
               configFile << "alias d1 \"rightleft 1 0 0;joy_side_sensitivity 1;alias +a da\"\n";
               configFile << "alias da \"rightleft 0 0 0;alias -a ada;alias -d add\"\n";
               configFile << "alias -d \"rightleft 0 0 0;alias +a a1;alias -a a2;alias -d d2;alias +d d1\"\n";
               configFile << "alias d2 \"rightleft 0 0 0;alias +a a1;alias -a a2;alias -d d2;alias +d d1\"\n";
               configFile << "alias add \"rightleft 1 0 0;joy_side_sensitivity -1;alias +d ad;alias -a a2;alias -d d2\"\n";
               configFile << "alias ada \"rightleft 1 0 0;joy_side_sensitivity 1;alias +a da;alias -d d2;alias -a a2\"\n";

               configFile << "ThanksForXuan\n";



           }
            configFile << "alias jmon \"viewmodel_fov 68; viewmodel_offset_x -5; viewmodel_offset_y 5; viewmodel_offset_z 5\"\n";
            configFile << "alias jmoff \"viewmodel_fov 68; viewmodel_offset_x 2.5; viewmodel_offset_y 2; viewmodel_offset_z -2\"\n";
            configFile << "alias jm1 \"jm2;\"\n";
            configFile << "alias jm2 \"jmon; alias jm1 jm3\"\n";
            configFile << "alias jm3 \"jmoff; alias jm1 jm2\"\n";

            cout << "\n请绑定机瞄视角按键,留空不启用：";
            getline(cin, jm);
            configFile << "bind " << jm << " jm1\n\n";
            cout << "\n\n";
            cout << "请提供自动检视方案\nA 按3切刀并检视\nB 按4切道具并检视\nC 按3和4均检视\nD 不启用自动检视\n";
            getline(cin, autolookweapon);
            if (autolookweapon == "A" || autolookweapon == "a")
            {
                configFile << "bind \"3\" \"slot3; +lookatweapon; -lookatweapon\"\n";
            }
            else if (autolookweapon == "B" || autolookweapon == "b")
            {
                configFile << "bind \"4\" \"slot4; +lookatweapon; -lookatweapon\"\n";
            }
            else if (autolookweapon == "C" || autolookweapon == "c")
            {
                configFile << "bind \"3\" \"slot3; +lookatweapon; -lookatweapon\"\n";
                configFile << "bind \"4\" \"slot4; +lookatweapon; -lookatweapon\"\n";
            }
            else {
                configFile << "bind \"3\" slot3\n";
                configFile << "bind \"4\" slot4\n";
            }

            configFile.close();

            cout << "已创建配置文件: " << path << endl;
        }
        else {
            cout << "无法创建配置文件: " << path << endl;
        }
    }
