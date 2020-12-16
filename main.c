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

#define DEBUG 1

const int max_line_length = 120;
const int standard_reading_tempo = 10;
const int slow_reading_tempo = 3;
const char commands[5][20]= {"gehe","nehme","lege","",""};

int room_ind = 77;
int last_room;
bool underwater = false;

#ifdef _WIN32
void os_wait(int ms)
{
    Sleep(ms);
}
#else
void os_wait(int ms)
{
    usleep(ms*1000);
}
#endif

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
}

void start_seq()
{
    fancy_print(title, slow_reading_tempo, false);
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
    fancy_print("Eingabe: ", standard_reading_tempo, true);
    scanf("%s %s",command, attribute);

    for (int i = 0; i < 20; i++)
        command[i] = tolower(command[i]);
    for (int i = 0; i < 20; i++)
        attribute[i] = tolower(attribute[i]);

    if (!strcmp(command,"gehe"))
    {
        last_room = room_ind;

        if (!strcmp(attribute,"norden"))
        {
            for (int i = 0; i < 4; i++)
                if (rooms_directions[room_ind-1][i] == 'n')
                    room_ind -= 10;
        }
        else if (!strcmp(attribute,"osten"))
        {
            for (int i = 0; i < 4; i++)
                if (rooms_directions[room_ind-1][i] == 'n')
                    room_ind += 1;
        }
        else if (!strcmp(attribute,"westen"))
        {
            for (int i = 0; i < 4; i++)
                if (rooms_directions[room_ind-1][i] == 'n')
                    room_ind -= 1;
        }
        else if (!strcmp(attribute,"süden"))
        {
            for (int i = 0; i < 4; i++)
                if (rooms_directions[room_ind-1][i] == 'n')
                    room_ind += 10;
        }
        else if (last_room == room_ind)
        {
            fancy_print("Dieser Weg steht dir nicht offen.\n", standard_reading_tempo, true);
        }
        else
        {
            fancy_print("Hm... diese Richtung kenne ich nicht... probiers mal mit Norden, Osten, Süden oder Westen.\n", standard_reading_tempo, true);
            return;
        }
    }
}

void print_description()
{
    if (biome[room_ind-1] == 'h') fancy_print("HÖHLE\n", slow_reading_tempo, true);
    else if (biome[room_ind-1] == 's') fancy_print("SUMPF\n", slow_reading_tempo, true);
    else if (biome[room_ind-1] == 'n') fancy_print("NEBEL\n", slow_reading_tempo, true);
    else if (biome[room_ind-1] == 'b') fancy_print("GEBIRGE\n", slow_reading_tempo, true);
    else if (biome[room_ind-1] == 'w') fancy_print("WIESE\n", slow_reading_tempo, true);
    else if (biome[room_ind-1] == 'c') fancy_print("SCHLUCHT\n", slow_reading_tempo, true);
    else if (biome[room_ind-1] == 'a') fancy_print("BAUMHAUS\n", slow_reading_tempo, true);
    else if (biome[room_ind-1] == 'l') fancy_print("WALD\n", slow_reading_tempo, true);
    else if (biome[room_ind-1] == 't') fancy_print("STRAND\n", slow_reading_tempo, true);
    else if (biome[room_ind-1] == 'm') fancy_print("MEER\n", slow_reading_tempo, true);
    else if (biome[room_ind-1] == 'q') fancy_print("HÖHLENQUELLE\n", slow_reading_tempo, true);
    else fancy_print("Fehler! Unbekanntes Gebiet!\n", standard_reading_tempo, true);

    os_wait(200);
    fancy_print(rooms[room_ind-1], standard_reading_tempo, true);

    switch(rand()%10){
        case 0:
            if (biome[room_ind-1] != 'm' && biome[room_ind-1] != 'a')
                fancy_print(zufallsanhaenge[0], standard_reading_tempo, true);
            break;
        case 1:
            if (biome[room_ind-1] != 'm' && biome[room_ind-1] != 's')
                fancy_print(zufallsanhaenge[1], standard_reading_tempo, true);
            break;
        case 2:
            if (biome[room_ind-1] == 'w' || biome[room_ind-1] == 'l')
                fancy_print(zufallsanhaenge[2], standard_reading_tempo, true);
            break;
        case 3:
            if (biome[room_ind-1] != 'm' && biome[room_ind-1] != 'h' && biome[room_ind-1] != 'q')
                fancy_print(zufallsanhaenge[3], standard_reading_tempo, true);
            break;
        case 4:
            if (biome[room_ind-1] != 'm' && biome[room_ind-1] != 'h' && biome[room_ind-1] != 'q' && biome[room_ind-1] != 'a')
                fancy_print(zufallsanhaenge[4], standard_reading_tempo, true);
            // TODO (jakob#1#): Kappe ablegen
            break;
        case 5:
            if (biome[room_ind-1] == 'm' && !underwater)
                fancy_print(zufallsanhaenge[5], standard_reading_tempo, true);
            break;
        case 6:
            if (biome[room_ind-1] == 'm' && underwater)
                fancy_print(zufallsanhaenge[6], standard_reading_tempo, true);
            break;
        case 7:
            if (biome[room_ind-1] == 'w')
                fancy_print(zufallsanhaenge[7], standard_reading_tempo, true);
            break;
        case 8:
            if (biome[room_ind-1] == 'l')
                fancy_print(zufallsanhaenge[8], standard_reading_tempo, true);
            // TODO (jakob#1#): zufälliges Item geben
            break;
        case 9:
            if (biome[room_ind-1] == 'h')
                fancy_print(zufallsanhaenge[9], standard_reading_tempo, true);
            break;
    };
    printf("\n");
}

bool game = true;
int main()
{
    time_t t;
    srand((unsigned) time(&t));
    if (!DEBUG)
        start_seq();

    while(game)  //main loop
    {
        handle_command();
        if (room_ind != last_room)
        {
            print_description();
        }
    }
    return 0;
}
