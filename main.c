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
const int standard_reading_tempo = 14;

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
        if (auto_line_break && i >= next_possible_break && i < len-1)
        {
            if (i > last_line_break+max_line_length)
            {
                printf("\n");
                fflush(stdout);
                last_line_break = i;
                if (str[i] == ' ')
                    i += 1;
            }
            else
            {
                for (int k = i; k < len-1; k++)
                {
                    if (str[k] == ' ')
                    {
                        next_possible_break = k;
                        break;
                    }
                }
            }
        }
        printf("%c",str[i]);
        fflush(stdout);
        int breaker_stat = 0;
        for (int k = 0; k < sizeof(breakers); k++)
        {
            if (str[i] == breakers[k])
                breaker_stat = 2;
        }
        if (breaker_stat == 0)
        {
            for (int k = 0; k < sizeof(half_breakers); k++)
            {
                if (str[i] == half_breakers[k])
                    breaker_stat = 1;
            }
        }
        if (auto_line_break && breaker_stat != 0)
        {
            if (breaker_stat == 1)
                os_wait(ms*10);
            else if (breaker_stat == 2)
                os_wait(ms*35);
            continue;
        }
        else
            os_wait(ms);
    }
}

void start_seq()
{
    fancy_print(title, sizeof(title), 4, false);
    printf("Drücke ENTER, um zu starten.");
    getchar();
    printf("\n\n");
    fancy_print(intro1, sizeof(intro1), standard_reading_tempo, true);
    printf("Drücke ENTER, um weiterzufahren.");
    getchar();
    printf("\n\n");
    fancy_print(intro2, sizeof(intro2), standard_reading_tempo, true);
}

int main()
{
    start_seq();
    return 0;
}
