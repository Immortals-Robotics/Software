#pragma once

#ifdef _WIN32
#include <conio.h>
#else
void changemode(int);
int  kbhit(void);
#endif