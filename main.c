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
int underwater_room_ind = 0;
int last_room;
bool underwater = false;
bool can_dive = true;

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
    printf("\n");
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
        if (underwater)
        {
            if (!strcmp(attribute,"norden"))
            {
                for (int i = 0; i < sizeof(underwater_rooms)/sizeof(int); i++)
                {
                    if (room_ind-10 == underwater_rooms[i])
                    {
                        underwater_room_ind = i;
                        room_ind -= 10;
                        return;
                    }
                }
                fancy_print("In diese Richtung kannst du nicht weiter tauchen!\n",standard_reading_tempo,true);
            }
            else if (!strcmp(attribute,"osten"))
            {
                for (int i = 0; i < sizeof(underwater_rooms)/sizeof(int); i++)
                {
                    if (room_ind+1 == underwater_rooms[i])
                    {
                        underwater_room_ind = i;
                        room_ind += 1;
                        return;
                    }
                }
                fancy_print("In diese Richtung kannst du nicht weiter tauchen!\n",standard_reading_tempo,true);
            }
            else if (!strcmp(attribute,"westen"))
            {
                for (int i = 0; i < sizeof(underwater_rooms)/sizeof(int); i++)
                {
                    if (room_ind-1 == underwater_rooms[i])
                    {
                        underwater_room_ind = i;
                        room_ind -= 1;
                        return;
                    }
                }
                fancy_print("In diese Richtung kannst du nicht weiter tauchen!\n",standard_reading_tempo,true);
            }
            else if (!strcmp(attribute,"süden"))
            {
                for (int i = 0; i < sizeof(underwater_rooms)/sizeof(int); i++)
                {
                    if (room_ind+10 == underwater_rooms[i])
                    {
                        underwater_room_ind = i;
                        room_ind += 10;
                        return;
                    }
                }
                fancy_print("In diese Richtung kannst du nicht weiter tauchen!\n",standard_reading_tempo,true);
            }
        }
        else
        {
            if (!strcmp(attribute,"norden"))
            {
                for (int i = 0; i < 4; i++)
                {
                    if (rooms_directions[room_ind-1][i] == 'n')
                    {
                        room_ind -= 10;
                    }
                }
            }
            else if (!strcmp(attribute,"osten"))
            {
                for (int i = 0; i < 4; i++)
                {
                    if (rooms_directions[room_ind-1][i] == 'o')
                    {
                        room_ind += 1;
                    }
                }
            }
            else if (!strcmp(attribute,"westen"))
            {
                for (int i = 0; i < 4; i++)
                {
                    if (rooms_directions[room_ind-1][i] == 'w')
                    {
                        room_ind -= 1;
                    }
                }
            }
            else if (!strcmp(attribute,"süden"))
            {
                for (int i = 0; i < 4; i++)
                {
                    if (rooms_directions[room_ind-1][i] == 's')
                    {

                        room_ind += 10;
                    }
                }
            }
            else
            {
                fancy_print("Hm... diese Richtung kenne ich nicht... probiers mal mit Norden, Osten, Süden oder Westen.\n", standard_reading_tempo, true);
                return;
            }
            if (last_room == room_ind)
            {
                fancy_print("Dieser Weg steht dir nicht offen.\n", standard_reading_tempo, true);
            }
        }
    }
    else if (!strcmp(command,"tauche"))
    {
        if (biome[room_ind-1] == 'm')
        {
            if (!strcmp(attribute,"auf"))
            {
                if (underwater)
                {
                    fancy_print("Du schwimmst zurück an die Wasseroberfläche.\n", standard_reading_tempo, true);
                    underwater = false;
                }
                else
                {
                    fancy_print("Du bist schon oben, Transuse!\n", standard_reading_tempo, true);
                }
            }
            else if (!strcmp(attribute,"ab"))
            {

                if (underwater)
                {
                    fancy_print("Du bist schon unten!\n", standard_reading_tempo, true);
                }
                else if (can_dive)
                {
                    for (int i = 0; i < sizeof(underwater_rooms); i++)
                    {
                        if (room_ind == underwater_rooms[i])
                        {
                            fancy_print("Du tauchst ab in die Tiefe des Ozeans.\n", standard_reading_tempo, true);
                            underwater = true;
                            underwater_room_ind = i;
                            break;
                        }
                    }
                    if (!underwater)
                    {
                        fancy_print("Hier ist es zu flach zum Abtauchen!\n", standard_reading_tempo, true);
                    }
                }
                else
                {
                    fancy_print("Du tauchst ab, aber nach einigen Zügen geht dir die Luft aus, und du musst wieder auftauchen!\n", standard_reading_tempo, true);
                }
            }
            else
                fancy_print("Du kannst entweder auf- oder abtauchen.\n", standard_reading_tempo, true);

        }
        else
            fancy_print("Du versuchst dich kopfüber in den Boden zu wühlen. Erst dann merkst du, dass du dich an Land befindest...\n", standard_reading_tempo, true);
    }
    else if (!strcmp(command,"schreie"))
    {
        fancy_print("AAAAAAAAAAAAAAAARGH!!!\n", standard_reading_tempo, true);
    }
    else if (!strcmp(command,"weine"))
    {
        fancy_print("Schluchz! Schnüff!\n", standard_reading_tempo, true);
    }
    else if (!strcmp(command,"nehme"))
    {

    }
    else if (!strcmp(command,"lege"))
    {

    }
    else if (!strcmp(command,"lache"))
    {
        fancy_print("Huahuahua!\n", standard_reading_tempo, true);
    }
    else
    {
        fancy_print("Unbekannter Befehl. Gib \"Hilfe\" ein, um eine Liste von Befehlen zu erhalten.\n", standard_reading_tempo, true);
    }
}

void print_description()
{
    char desc[30];
    if (biome[room_ind-1] == 'h')      sprintf(desc, "%i - HÖHLE\n",room_ind);
    else if (biome[room_ind-1] == 's') sprintf(desc, "%i - SUMPF\n",room_ind);
    else if (biome[room_ind-1] == 'n') sprintf(desc, "%i - NEBEL\n",room_ind);
    else if (biome[room_ind-1] == 'b') sprintf(desc, "%i - GEBIRGE\n",room_ind);
    else if (biome[room_ind-1] == 'w') sprintf(desc, "%i - WIESE\n",room_ind);
    else if (biome[room_ind-1] == 'c') sprintf(desc, "%i - SCHLUCHT\n",room_ind);
    else if (biome[room_ind-1] == 'a') sprintf(desc, "%i - BAUMHAUS\n",room_ind);
    else if (biome[room_ind-1] == 'l') sprintf(desc, "%i - WALD\n",room_ind);
    else if (biome[room_ind-1] == 't') sprintf(desc, "%i - STRAND\n",room_ind);
    else if (biome[room_ind-1] == 'm')
        if (underwater) sprintf(desc, "%i - MEER (UNTER WASSER)\n",room_ind);
        else sprintf(desc, "%i - MEER (OBERFLÄCHE)\n",room_ind);
    else if (biome[room_ind-1] == 'q') sprintf(desc, "%i - HÖHLENQUELLE\n",room_ind);
    else sprintf(desc, "%i - FEHLER\n",room_ind);

    fancy_print(desc, slow_reading_tempo, true);
    os_wait(200);
    if (room_ind != last_room)
    {
        if (underwater)
        {
            int i;
            for (i=0; i < sizeof(underwater_rooms); i++)
            {
                if (room_ind == underwater_rooms[i])
                {
                    fancy_print(underwater_descriptions[i], standard_reading_tempo, true);
                    break;
                }
            };
        }
        else
        {
            fancy_print(rooms[room_ind-1], standard_reading_tempo, true);
        }
    }
    printf(" ");

    switch(rand()%10)
    {
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
        if (biome[room_ind-1] != 'm' && biome[room_ind-1] != 'h' && biome[room_ind-1] != 'q' && biome[room_ind-1] != 'a'){
            fancy_print(zufallsanhaenge[4], standard_reading_tempo, true);
            // TODO (jakob#1#): Kappe ablegen
        }
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
        if (biome[room_ind-1] == 'l'){
            fancy_print(zufallsanhaenge[8], standard_reading_tempo, true);
            // TODO (jakob#1#): zufälliges Item geben
        }
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
        print_description();
    }
    return 0;
}
