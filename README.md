
# Hi, I'm ROHIDH! 👋
# Trojan BackDoor

#### It is a C program to make possible to have a reverse shell connection to another pc by just executing the backdoor.exe[provided in the repo] in windows , you can connect the pc by running server.o file in any linux os


#### You have to make sure that the victim should be using windows in order to run the program and the attacker have to be in a linux environment



### * compile the backdoor.c file in windows os (if you want)
### if you encounder any error like the following while compiling with gcc compiler
    C:\Users\USER\AppData\Local\Temp\ccc67Kmi.o:backdoor.c:(.text+0x5c5): undefined reference to `send@16'
    C:\Users\USER\AppData\Local\Temp\ccc67Kmi.o:backdoor.c:(.text+0x620): undefined reference to `send@16'
    C:\Users\USER\AppData\Local\Temp\ccc67Kmi.o:backdoor.c:(.text+0x6ab): undefined reference to `send@16'
    C:\Users\USER\AppData\Local\Temp\ccc67Kmi.o:backdoor.c:(.text+0x6ef): undefined reference to `send@16'
    C:\Users\USER\AppData\Local\Temp\ccc67Kmi.o:backdoor.c:(.text+0x790): undefined reference to `recv@16'
    C:\Users\USER\AppData\Local\Temp\ccc67Kmi.o:backdoor.c:(.text+0x7c2): undefined reference to `closesocket@4'
    C:\Users\USER\AppData\Local\Temp\ccc67Kmi.o:backdoor.c:(.text+0x7ca): undefined reference to `WSACleanup@0'
    C:\Users\USER\AppData\Local\Temp\ccc67Kmi.o:backdoor.c:(.text+0x92d): undefined reference to `send@16'
    C:\Users\USER\AppData\Local\Temp\ccc67Kmi.o:backdoor.c:(.text+0x9a1): undefined reference to `WSAStartup@8'
    C:\Users\USER\AppData\Local\Temp\ccc67Kmi.o:backdoor.c:(.text+0x9d0): undefined reference to `socket@12'
    C:\Users\USER\AppData\Local\Temp\ccc67Kmi.o:backdoor.c:(.text+0xa04): undefined reference to `inet_addr@4'
    C:\Users\USER\AppData\Local\Temp\ccc67Kmi.o:backdoor.c:(.text+0xa16): undefined reference to `htons@4'
    C:\Users\USER\AppData\Local\Temp\ccc67Kmi.o:backdoor.c:(.text+0xa3c): undefined reference to `connect@12'
 ### you can use the following command to resolve that
 ```bash
  gcc <filename.c> -o <outputFileName.exe> -lws2_32
 ```



  * if don't have windows and need to compile it in any linux os go through the following commands
 ```bash
  sudo apt update
  ```
  ```bash
  sudo apt install mingw-w64
  ```
  ```bash
  i686-w64-mingw32-gcc -o backdoor.exe backdoor.c -lwininet -lwsock32
  ```
 * here -lwininet and -lwsock32 are the libraries that we are used extra that linux don't have in default

