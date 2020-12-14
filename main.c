#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "texts.h"

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

const int max_line_length = 120;
const int standard_reading_tempo = 10;

void os_wait(int ms)
{
#ifdef _WIN32
    Sleep(ms);
#else
    usleep(ms*1000);
#endif
}

void fancy_print(char str[], int len, int ms, bool auto_line_break)
{
    int last_line_break = 0;
    int next_possible_break = 0;
    for (int i = 0; i<len; i++)
    {
        if (auto_line_break && i >= next_possible_break  && i < len-1)
        {
            for (int j = i; j < len-1; j++)
            {
                if (str[j] == ' ')
                {
                    if (j > last_line_break+max_line_length)
                    {
                        printf("\n");
                        fflush(stdout);
                        last_line_break = i;
                    }
                    next_possible_break = j;
                    break;
                }
            }
        }
        printf("%c",str[i]);
        fflush(stdout);
        if (auto_line_break)
        {
            for (int k = 0; k < sizeof(breakers); k++)
            {
                if (str[i] == breakers[k])
                {
                    os_wait(ms*24);
                    break;
                }
            }
            for (int k = 0; k < sizeof(half_breakers); k++)
            {
                if (str[i] == half_breakers[k])
                {
                    os_wait(ms*9);
                    break;
                }
            }
        }
        os_wait(ms);
    }
    printf("\n");
}

void start_seq()
{
    fancy_print(title, sizeof(title), 3, false);
    printf("Drücke ENTER, um zu starten.");
    getchar();
    printf("\n");
    fancy_print(intro1, sizeof(intro1), standard_reading_tempo, true);
    printf("Drücke ENTER, um fortzufahren.");
    getchar();
    printf("\n");
    fancy_print(intro2, sizeof(intro2), standard_reading_tempo, true);
    printf("Drücke ENTER, um fortzufahren.");
    getchar();
    fancy_print(intro3, sizeof(intro3), standard_reading_tempo, true);
}

int main()
{
    start_seq();
    bool game = true
    while(game){ //main loop

    }
    return 0;
}
