#pragma once

#include "video/renderer.h"
#include "video/video.h"

#define     White   "\033[1;00m"
#define     Red     "\033[1;31m"
#define     Green   "\033[1;32m"
#define     Yellow  "\033[1;33m"
#define     Blue    "\033[1;34m"
#define     Purple  "\033[1;35m"
#define     Cyan    "\033[1;36m"
#define     Black   "\033[1;37m"


class Shell
{
    public:
        void PrintPrompt();
        void TestCMD(char* input);
        bool shouldPrint;
};

//extern Shell newShell;