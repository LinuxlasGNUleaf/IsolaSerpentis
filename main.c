#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

#include <stdio.h>
#include <stdlib.h>

char title[] = "d888888b .d8888.  .d88b.  db       .d8b.      .d8888. d88888b d8888b. d8888b. d88888b d8b   db d888888b d888888b .d8888. \n\
  `88'   88'  YP .8P  Y8. 88      d8' `8b     88'  YP 88'     88  `8D 88  `8D 88'     888o  88 `~~88~~'   `88'   88'  YP \n\
   88    `8bo.   88    88 88      88ooo88     `8bo.   88ooooo 88oobY' 88oodD' 88ooooo 88V8o 88    88       88    `8bo.   \n\
   88      `Y8b. 88    88 88      88~~~88       `Y8b. 88~~~~~ 88`8b   88~~~   88~~~~~ 88 V8o88    88       88      `Y8b. \n\
  .88.   db   8D `8b  d8' 88booo. 88   88     db   8D 88.     88 `88. 88      88.     88  V888    88      .88.   db   8D \n\
Y888888P `8888Y'  `Y88P'  Y88888P YP   YP     `8888Y' Y88888P 88   YD 88      Y88888P VP   V8P    YP    Y888888P `8888Y'\n\n\
//=============================\\\\\n\
||Written in BASIC by:         ||\n\
||Daniel Weiß \t Andreas Enge  ||\n\
\\\\=============================//\n\n\
//==================================\\\\\n\
||Reconstructed and ported to C by: ||\n\
||Jakob Weiß                        ||\n\
\\\\==================================//\n";

void start_seq()
{
    for (int i=0; i<sizeof(title); i++)
    {
        printf("%c",title[i]);
        if (title[i]=='\n')
        {
#ifdef _WIN32
            Sleep(500);
#else
            usleep(200*1000);
#endif
        }
    }
    return 0;
}

int main()
{
    start_seq();
}
