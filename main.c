#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "texts.h"
#include <ctype.h>
#include <string.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

const int max_line_length = 120;
const int standard_reading_tempo = 10;
const char commands[5][20]= {"gehe","nehme","lege","",""};

int room_ind = 77;

void os_wait(int ms)
{
#ifdef _WIN32
    Sleep(ms);
#else
    usleep(ms*1000);
#endif
}

void fancy_print(char str[], int ms, bool auto_line_break)
{
    int len = strlen(str);
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
    fancy_print(title, 3, false);
    printf("Drücke ENTER, um zu starten.");
    getchar();
    printf("\n");
    fancy_print(intro1, standard_reading_tempo, true);
    printf("Drücke ENTER, um fortzufahren.");
    getchar();
    printf("\n");
    fancy_print(intro2, standard_reading_tempo, true);
    printf("Drücke ENTER, um fortzufahren.");
    getchar();
    fancy_print(intro3, standard_reading_tempo, true);
}

void handle_command()
{
    char command[20];
    char attribute[20];
    printf("Eingabe: ");
    scanf("%s %s",command, attribute);

    for (int i = 0; i < 20; i++)
        command[i] = tolower(command[i]);
    for (int i = 0; i < 20; i++)
        attribute[i] = tolower(attribute[i]);

    if (!strcmp(command,"gehe"))
    {
        char direction[4];
        strcpy(direction,rooms_directions[room_ind-1]);

        if (!strcmp(attribute,"norden"))
        {
            for (int i = 0; i < 4; i++)
            {
                if (direction[i] == 'n')
                {
                    room_ind -= 10;
                    fancy_print("Du gehst nach Norden.\n", standard_reading_tempo, true);
                    return;
                }
            }
        }
        else if (!strcmp(attribute,"osten"))
        {
            for (int i = 0; i < 4; i++)
            {
                if (direction[i] == 'n')
                {
                    room_ind += 1;
                    fancy_print("Du gehst nach Norden.\n", standard_reading_tempo, true);
                    return;
                }
            }
        }
        else if (!strcmp(attribute,"westen"))
        {
            for (int i = 0; i < 4; i++)
            {
                if (direction[i] == 'n')
                {
                    room_ind -= 1;
                    fancy_print("Du gehst nach Norden.\n", standard_reading_tempo, true);
                    return;
                }
            }
        }
        else if (!strcmp(attribute,"süden"))
        {
            for (int i = 0; i < 4; i++)
            {
                if (direction[i] == 'n')
                {
                    room_ind += 10;
                    fancy_print("Du gehst nach Süden.\n", standard_reading_tempo, true);
                    return;
                }
            }
        }
        else
        {
            fancy_print("Hm... diese Richtung kenne ich nicht...\n", standard_reading_tempo, true);
        }
        fancy_print("Dieser Weg steht dir nicht offen.\n", standard_reading_tempo, true);
    }
}

bool game = true;
int main()
{
    start_seq();

    while(game)  //main loop
    {
        handle_command();
        fancy_print(rooms[room_ind-1], standard_reading_tempo, true);
    }
    return 0;
}
