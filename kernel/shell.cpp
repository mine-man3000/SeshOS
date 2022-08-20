#include "shell.h"
#include "memory/mem.h"
#include "fs/ramfs.h"
#include "video/window.h"
#include "../tools/picsofbread.h"

Shell newShell;
bool xRunning = false;

void Shell::PrintPrompt()
{
    if (shouldPrint)
    {
        printf("%sroot@SeshOS%s / %s# ", Red, Blue, White);
    }    
}

void Shell::TestCMD(char* input)
{
    TwoStrings twoStrings = strsplit(input, ' ');
    if (mystrcmp(input, "ver"))
    {
        printf("SeshOS version 0.2.0\n");
    }
    else if (mystrcmp(input, "help"))
    {
        printf("List of available commands:\n");
        printf("    ver:         shows the version of SeshOS\n");
        printf("    help:        shows this menu\n");
        printf("    clear:       clears the screen\n");
        printf("    neofetch:    only a Linux user would understand\n");
        printf("    intro:       runs the NCommander Intro\n");
    }
    else if (mystrcmp(input, "clear"))
    {
        Clear(0);
    }
    else if (mystrcmp(input, "neofetch"))
    {
        uint64_t totMem = 0;
        uint64_t useMem = 0;
        uint64_t freMem = 0;
        for (int i = 0; i < mmap.response->entry_count; i++)
        {
            uint64_t type = mmap.response->entries[i]->type;
            if (type == 0 || type == 2 || type == 5)
            {
                freMem += mmap.response->entries[i]->length;
            }
            else
            {
                useMem += mmap.response->entries[i]->length;
            }
        }
        totMem = useMem + freMem;

        printf("SSSSSS   EEEEEE  SSSSSS  HH  HH    root@SeshOS\n");
        printf("SSSSSS   EEEEEE  SSSSSS  HH  HH    -----------\n");
        printf("SSS      EE      SSS     HH  HH    OS: SeshOS 0.2.0\n");
        printf("SSS      EE      SSS     HH  HH    Kernel: 0.2.0\n");
        printf("SSSSSS   EEEEEE  SSSSSS  HHHHHH    Shell: SESH (SEsh SHell)\n");
        printf("SSSSSS   EEEEEE  SSSSSS  HHHHHH    Memory: %i KB/%i KB\n", useMem / 1024, totMem / 1024); 
        printf("   SSS   EE         SSS  HH  HH    Resolution: %ix%i\n", buffer->width, buffer->height);
        printf("   SSS   EE         SSS  HH  HH    \n");
        printf("SSSSSS   EEEEEE  SSSSSS  HH  HH    \n");
        printf("SSSSSS   EEEEEE  SSSSSS  HH  HH    \n");
        printf("                                   \n");
        printf("                 OOOOOO  SSSSSS    \n");
        printf("    II  II       OO  OO  SSSSSS    \n");
        printf("    II  II       OO  OO  SSS       \n");
        printf("    II  II       OO  OO  SSS       \n");
        printf("  \\\\      //     OO  OO  SSSSSS    \n");
        printf("   \\\\====//      OO  OO  SSSSSS    \n");
        printf("                 OO  OO     SSS    \n");
        printf("                 OO  OO     SSS    \n");
        printf("                 OO  OO  SSSSSS    \n");
        printf("                 OOOOOO  SSSSSS    \n");
    }
    else if (mystrcmp(input, "")){}
    else if (mystrcmp(input, "panic"))
    {
        Panic("User Caused Panic");
    }
    else if (mystrcmp(input, "exitx"))
    {
        xRunning = false;
        Clear(0);
        newShell.shouldPrint = true;
    }
    else if (mystrcmp(input, "startx"))
    {
        xRunning = true;
        drawRect(0, 0, buffer->width, buffer->height, 0xD97F1956);
        Window yes = Window(10, 10, 200, 200, "Hello World        ", 0x12345678);
        Window no = Window(100, 100, 200, 200, "1234567890!@#$%^&*()", 0x12345678);

        drawRect(0, buffer->height - 40, buffer->width, 40, 0xff888888);        
    
        newShell.shouldPrint = false;
    }
    else if (mystrcmp(input, "pob"))
    {
        drawImage(g_picsofbread_data, 10, 10);
    }
    else if (mystrcmp(input, "tree"))
    {
        for (int i = 0; headers[i]->filename != NULL; i++)
        {
            printf("%s\n", headers[i]->filename);
        }
        
    }
    else if (mystrcmp(twoStrings.a, "cat"))
    {
        printf(readFile(twoStrings.b));
    }
    else if (mystrcmp(input, "ls"))
    {
        ls();
    }
    else if (mystrcmp(input, "exit"))
    {
        Clear(0);
        printf("SHUT DOWN SEQUENCE INITIATED");
        sleep(2);
        outw(0xB004, 0x2000);
        outw(0x604, 0x2000);
        outw(0x4004, 0x3400);
    }
    else if (mystrcmp(input, "intro"))
    {
        Clear(0);
        nComIntro();
    }
    else
    {
        printf("Unknown command: \"%s\"\n", input);
    }
}