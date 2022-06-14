#include "shell.h"
#include "memory/mem.h"
#include "fs/ramfs.h"

void Shell::PrintPrompt()
{
    printf("%sroot@SeshOS%s / %s# ", Red, Blue, White);
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
        printf("    intro: runs the NCommander Intro\n");
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
        printf("Unknown command: \"");
        printf(input);
        printf("\"\n");
    }
}

Shell *newShell;