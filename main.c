#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include <time.h>

#include "texts.h"

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

#define DEBUG 1

//constants
const int max_line_length = 120;
const int standard_reading_tempo = 10;
const int slow_reading_tempo = 3;
const char commands[5][20] = {"gehe", "nehme", "lege", "", ""};

//game variables
int room_ind = 77;
int last_room = 77;
int underwater_room_ind = 0;
int schlucht_zaehler = 0;
int backpack[11] = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1};
bool underwater = false;
bool can_dive = true;
bool in_fight = false;
bool game = true;

//platform-dependent definition of sleep
#ifdef _WIN32
void os_wait(int ms)
{
    Sleep(ms);
}
#else
void os_wait(int ms)
{
    usleep(ms * 1000);
}
#endif

//checks for item in inventory
bool has_item(int item)
{
    for (int i = 0; i < sizeof(backpack) / sizeof(int); i++)
    {
        if (backpack[i] == item)
            return true;
    }
    return false;
}

//adds item to inventory
void add_item(int item)
{
    for (int i = 0; i < sizeof(backpack) / sizeof(int); i++)
    {
        if (backpack[i] == -1)
        {
            backpack[i] = item;
            objekte_loc[item] = -1;
            return;
        }
    }
    return;
}

//removes specified item from inventory
void remove_item(int item)
{
    for (int i = 0; i < sizeof(backpack) / sizeof(int); i++)
    {
        if (backpack[i] == item)
        {
            backpack[i] = -1;
            objekte_loc[item] = room_ind;
            return;
        }
    }
    return;
}

//prints text char by char with delay, with the option of extra breaks for punctuations
void fancy_print(char str[], int ms, bool punctuation_breaks)
{
    int len = strlen(str);
    int last_line_break = 0;
    int next_possible_break = 0;
    for (int i = 0; i < len; i++)
    {
        if (i >= next_possible_break && i < len - 1)
        {
            for (int j = i; j < len - 1; j++)
            {
                if (str[j] == ' ')
                {
                    if (j > last_line_break + max_line_length)
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
        printf("%c", str[i]);
        fflush(stdout);
        if (punctuation_breaks)
        {
            for (int k = 0; k < sizeof(breakers); k++)
            {
                if (str[i] == breakers[k])
                {
                    os_wait(ms * 24);
                    break;
                }
            }
            for (int k = 0; k < sizeof(half_breakers); k++)
            {
                if (str[i] == half_breakers[k])
                {
                    os_wait(ms * 9);
                    break;
                }
            }
        }
        os_wait(ms);
    }
}

void on_fight_end(bool won)
{
    if (!won) //end game if player lost fight
        game = false;
    else
        in_fight = false;

    switch (room_ind)
    {
    case 13: //TROLL
        switch (won)
        {
        case 0:
            fancy_print("Mit einem schaurigen Gebrüll stürzt sich der Troll über dich. Er ist dir an Kraft bei weitem überlegen und zerknackt dir mühelos das Genick...\n", standard_reading_tempo, true);
            break;
        case 1:
            fancy_print("Ein Blutstrahl schießt aus der Wunde des Trolls. Er bricht tot zusammen!\n", standard_reading_tempo, true);
            monster_stat[TROLL] = 1;
            break;
        }
        break;
    case 18: //SUMPFMONSTER
        switch (won)
        {
        case 0:
            fancy_print("Das Sumpfwesen umschlingt dich mit seinen glitschigen Armen und zieht dich ins Moor, wo du elendig ersäufst.\n", standard_reading_tempo, true);
            break;
        case 1:
            fancy_print("Der Geist erscheint. Wutentbrannt legt er die Flinte an und schießt auf das Monster. Mit lautem Getöse explodiert das Schießeisen, Splitter fliegen umher. Das Monster sinkt mit schmerzverzerrter Miene ins Moorwasser zurück. Glücklich über deine Rettung bedankst du dich beim Geist. „Gern geschehen“, damit verschwindet er.\n", standard_reading_tempo, true);
            monster_stat[SUMPFMONSTER] = 1;
            break;
        }
        break;
    case 65:
        switch (won)
        {
        case 0:
            fancy_print("Tausende von Killerameisen überfluten dich. Glühende Nägel scheinen dir ins Fleisch getrieben zu werden, als sie dir Fleischstückchen aus dem Körper reißen. Mach einer halben Minute zeugt nur ein Skelett von dir.\n", standard_reading_tempo, true);
            break;
        case 1:
            fancy_print("Als die Ameisen deinen Schild wahrnehmen, flüchten sie von Panik ergriffen in ihren Bau. Jetzt bemerkst du den blauen Hals einer Flasche, die aus dem Haufen ragt.\n", standard_reading_tempo, true);
            monster_stat[AMEISEN] = 1;
            break;
        }
        break;
    case 98:
        switch (won)
        {
        case 0:
            fancy_print("Der Fisch umschwimmt dich in immer enger werdenden Kreisen. Er beißt sich in deinem Bein fest! Durch das Blut angelockt, schießen hunderte von kleinen, silbernen Blitzen auf dich zu. Die Fischschwärme lassen nicht viel von dir übrig...\n", standard_reading_tempo, true);
            in_fight = true;
            return;
        case 1:
            fancy_print("Der Fisch umschwimmt die Blüte und beäugt sie misstrauisch. Plötzlich schießt er vor und schnappt sie. Du starrst ihn fasziniert an: Sein Bauch glüht in einem flackernden Rot auf. Er zuckt krampfhaft mit der Schwanzflosse und zerfällt zu Fischmehl, das langsam zu Boden sinkt. Vor Erleichterung stößt du einen tiefen Seufzer aus. Auf einmal erblickst du vor dir einen weißen Schimmer. Er geht von einem kleinen, blanken Ei aus, das zwischen dem Fischmehl liegt.\n", standard_reading_tempo, true);
            monster_stat[FISCH] = 1;
            break;
        }
        break;
    }
}

//init sequence
void start_seq()
{
    for (int i = 0; i < sizeof(backpack) / sizeof(int); i++)
        backpack[i] = -1;
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

//handle the command input
void handle_command()
{
    char buffer[40];
    char command[20] = "";
    char attribute[20] = "";

    fancy_print("Eingabe: ", standard_reading_tempo, false);
    fgets(buffer, sizeof(buffer), stdin);

    int j = 0;
    bool is_command = true;
    for (int i = 0; i < sizeof(buffer); i++)
    {
        if (buffer[i] == ' ')
        {
            if (!is_command)
                break;
            is_command = false;
            j = 0;
            continue;
        }
        if (buffer[i] == '\n')
        {
            if (is_command)
            {
                command[j++] = '\0';
                break;
            }
            else
            {
                attribute[j++] = '\0';
                break;
            }
            break;
        }
        if (is_command && j < sizeof(command))
        {
            command[j++] = buffer[i];
        }
        else if (j < sizeof(attribute))
        {
            attribute[j++] = buffer[i];
        }
        else
            break;
    }

    for (int i = 0; i < strlen(command); i++)
        command[i] = tolower(command[i]);
    for (int i = 0; i < strlen(attribute); i++)
        attribute[i] = tolower(attribute[i]);

    if (DEBUG)
        printf("|%s|%s|\n", command, attribute);

    if (!strcmp(command, "gehe"))
    {
        if (in_fight)
        {
            fancy_print("Flucht is keine Option...", standard_reading_tempo, true);
            on_fight_end(false);
        }
        if (underwater)
        {
            if (!strcmp(attribute, "norden"))
            {
                for (int i = 0; i < sizeof(underwater_rooms) / sizeof(int); i++)
                {
                    if (room_ind - 10 == underwater_rooms[i])
                    {
                        underwater_room_ind = i;
                        room_ind -= 10;
                        return;
                    }
                }
            }
            else if (!strcmp(attribute, "osten"))
            {
                for (int i = 0; i < sizeof(underwater_rooms) / sizeof(int); i++)
                {
                    if (room_ind + 1 == underwater_rooms[i])
                    {
                        underwater_room_ind = i;
                        room_ind += 1;
                        return;
                    }
                }
            }
            else if (!strcmp(attribute, "westen"))
            {
                for (int i = 0; i < sizeof(underwater_rooms) / sizeof(int); i++)
                {
                    if (room_ind - 1 == underwater_rooms[i])
                    {
                        underwater_room_ind = i;
                        room_ind -= 1;
                        return;
                    }
                }
            }
            else if (!strcmp(attribute, "süden"))
            {
                for (int i = 0; i < sizeof(underwater_rooms) / sizeof(int); i++)
                {
                    if (room_ind + 10 == underwater_rooms[i])
                    {
                        underwater_room_ind = i;
                        room_ind += 10;
                        return;
                    }
                }
            }
            else
            {
                fancy_print("Hm... diese Richtung kenne ich nicht... probiers mal mit Norden, Osten, Süden oder Westen.\n", standard_reading_tempo, true);
                return;
            }
            fancy_print("In diese Richtung kannst du nicht weiter tauchen!\n", standard_reading_tempo, true);
        }
        else
        {
            if (!strcmp(attribute, "norden"))
            {
                for (int i = 0; i < 4; i++)
                {
                    if (rooms_directions[room_ind - 1][i] == 'n')
                    {
                        room_ind -= 10;
                    }
                }
            }
            else if (!strcmp(attribute, "osten"))
            {
                for (int i = 0; i < 4; i++)
                {
                    if (rooms_directions[room_ind - 1][i] == 'o')
                    {
                        room_ind += 1;
                    }
                }
            }
            else if (!strcmp(attribute, "westen"))
            {
                for (int i = 0; i < 4; i++)
                {
                    if (rooms_directions[room_ind - 1][i] == 'w')
                    {
                        room_ind -= 1;
                    }
                }
            }
            else if (!strcmp(attribute, "süden"))
            {
                for (int i = 0; i < 4; i++)
                {
                    if (rooms_directions[room_ind - 1][i] == 's')
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
    else if (!strcmp(command, "tauche"))
    {
        if (in_fight)
        {
            fancy_print("Flucht is keine Option...", standard_reading_tempo, true);
            on_fight_end(false);
        }
        if (biome[room_ind - 1] == 'm')
        {
            if (!strcmp(attribute, "auf"))
            {
                if (underwater)
                {
                    fancy_print("Du schwimmst zurück an die Wasseroberfläche.\n", standard_reading_tempo, true);
                    underwater = false;
                }
                else
                    fancy_print("Du bist schon oben, Transuse!\n", standard_reading_tempo, true);
            }
            else if (!strcmp(attribute, "ab"))
            {
                if (underwater)
                    fancy_print("Du bist schon unten!\n", standard_reading_tempo, true);
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
                        fancy_print("Hier ist es zu flach zum Abtauchen!\n", standard_reading_tempo, true);
                }
                else
                    fancy_print("Du tauchst ab, aber nach einigen Zügen geht dir die Luft aus, und du musst wieder auftauchen!\n", standard_reading_tempo, true);
            }
            else
                fancy_print("Du kannst entweder auf- oder abtauchen.\n", standard_reading_tempo, true);
        }
        else
            fancy_print("Du versuchst dich kopfüber in den Boden zu wühlen. Erst dann merkst du, dass du dich an Land befindest...\n", standard_reading_tempo, true);
    }
    else if (!strcmp(command, "schreie"))
    {
        fancy_print("AAAAAAAAAAAAAAAARGH!!!\n", standard_reading_tempo, true);
    }
    else if (!strcmp(command, "weine"))
    {
        fancy_print("Schluchz! Schnüff!\n", standard_reading_tempo, true);
    }
    else if (!strcmp(command, "nehme"))
    {
        if (in_fight)
        {
            fancy_print("Dafür ist keine Zeit!", standard_reading_tempo, true);
            return;
        }
        if (strlen(attribute))
        {
            char buffer2[10];
            for (int i = 0; i < OBJEKTE_LEN; i++)
            {
                strcpy(buffer2, objekte[i]);
                buffer2[0] = tolower(buffer2[0]);
                if (!strcmp(attribute, buffer2) && room_ind == objekte_loc[i])
                {
                    char buffer3[70];
                    sprintf(buffer3, "Du verstaust %.5s %.11s in deinem Rucksack.\n", objekte_artk2[i], objekte[i]);
                    fancy_print(buffer3, standard_reading_tempo, true);
                    add_item(i);
                    break;
                }
                if (i == OBJEKTE_LEN - 1)
                    fancy_print("Du schaust dich um, kannst ein solches Objekt jedoch nirgendwo finden.\n", standard_reading_tempo, true);
            }
        }
        else
        {
            fancy_print("Geht das noch etwas genauer?\n", standard_reading_tempo, true);
        }
    }
    else if (!strcmp(command, "lege"))
    {
        if (in_fight)
        {
            fancy_print("Dafür ist keine Zeit!", standard_reading_tempo, true);
            return;
        }
        if (strlen(attribute))
        {
            char buffer2[10];
            for (int i = 0; i < OBJEKTE_LEN; i++)
            {
                strcpy(buffer2, objekte[i]);
                buffer2[0] = tolower(buffer2[0]);
                if (!strcmp(attribute, buffer2) && has_item(i))
                {
                    if (biome[room_ind - 1] == 'm')
                    {
                        fancy_print("Hier kannst du nichts ablegen, das wird ja ganz nass!", standard_reading_tempo, true);
                    }
                    char buffer3[100];
                    sprintf(buffer3, "Du greifst in deinen Rucksack legst %.5s %.11s ab.", objekte_artk2[i], objekte[i]);
                    fancy_print(buffer3, standard_reading_tempo, true);
                    remove_item(i);
                    break;
                }
                if (i == OBJEKTE_LEN - 1)
                    fancy_print("Du durchwühlst deine Taschen, aber so ein Objekt findest du nicht bei dir.\n", standard_reading_tempo, true);
            }
        }
    }
    else if (!strcmp(command, "lache"))
    {
        fancy_print("Huahuahua!\n", standard_reading_tempo, true);
    }
    else if (!strcmp(command, "hilfe"))
    {
        fancy_print(hilfe, standard_reading_tempo, true);
    }
    else if (!strcmp(command, "töte") || !strcmp(command, "kämpfe"))
    {
        if (in_fight)
        {
            if (!strcmp(attribute, "geist"))
            {
            }
            else if (!strcmp(attribute, "geist"))
            {
            }
        }
    }
    else if (!strcmp(command, "nutze") || !strcmp(command, "benutze"))
    {
        if (!strcmp(attribute, "geist"))
        {
        }
        else if (!strcmp(attribute, "löffel"))
        {
            if (room_ind == 98 && underwater)
            {
                fancy_print("YOU WON THE GAME, CONGRATULATIONS!\n", slow_reading_tempo, true);
                game = false;
            }
            else
            {
                fancy_print("Was sollst du den mit dem Besteck hier anrichten? Vielleicht brauchst du es ja später...\n", standard_reading_tempo, true);
            }
        }
        else if (!strcmp(attribute, "gabel") || !strcmp(attribute, "messer"))
        {
            fancy_print("Was sollst du den mit dem Besteck hier anrichten? Vielleicht brauchst du es ja später...\n", standard_reading_tempo, true);
        }
        else if (!strcmp(attribute, "löffel"))
        {
        }
        else if (!strcmp(attribute, "amulett") || !strcmp(attribute, "ring") || !strcmp(attribute, "kette"))
        {
            fancy_print("Ein schönes Ding, aber im Moment relativ nutzlos. Es würde aber sicher ein schönes Geschenk machen...", standard_reading_tempo, true);
        }
        else
        {
            fancy_print("Dieses Objekt hast du nicht oder kannst es (momentan) nicht benutzen.", standard_reading_tempo, true);
        }
    }
    else
    {
        fancy_print("Unbekannter Befehl. Gib \"Hilfe\" ein, um eine Liste von Befehlen zu erhalten.\n", standard_reading_tempo, true);
    }
}

//prints the room description
void print_description()
{
    char desc[30];
    printf("\n");
    if (biome[room_ind - 1] == 'h')
        sprintf(desc, "%i - HÖHLE\n", room_ind);
    else if (biome[room_ind - 1] == 's')
        sprintf(desc, "%i - SUMPF\n", room_ind);
    else if (biome[room_ind - 1] == 'n')
        sprintf(desc, "%i - NEBEL\n", room_ind);
    else if (biome[room_ind - 1] == 'b')
        sprintf(desc, "%i - GEBIRGE\n", room_ind);
    else if (biome[room_ind - 1] == 'w')
        sprintf(desc, "%i - WIESE\n", room_ind);
    else if (biome[room_ind - 1] == 'c')
        sprintf(desc, "%i - SCHLUCHT\n", room_ind);
    else if (biome[room_ind - 1] == 'a')
        sprintf(desc, "%i - BAUMHAUS\n", room_ind);
    else if (biome[room_ind - 1] == 'l')
        sprintf(desc, "%i - WALD\n", room_ind);
    else if (biome[room_ind - 1] == 't')
        sprintf(desc, "%i - STRAND\n", room_ind);
    else if (biome[room_ind - 1] == 'm')
        if (underwater)
            sprintf(desc, "%i - MEER (UNTER WASSER)\n", room_ind);
        else
            sprintf(desc, "%i - MEER (OBERFLÄCHE)\n", room_ind);
    else if (biome[room_ind - 1] == 'q')
        sprintf(desc, "%i - HÖHLENQUELLE\n", room_ind);
    else
        sprintf(desc, "%i - FEHLER\n", room_ind);

    fancy_print(desc, slow_reading_tempo, true);
    os_wait(200);
    if (room_ind != last_room)
    {
        if (underwater)
        {
            int i;
            for (i = 0; i < sizeof(underwater_rooms); i++)
            {
                if (room_ind == underwater_rooms[i])
                {
                    fancy_print(underwater_descriptions[i], standard_reading_tempo, true);
                    break;
                }
            };
        }
        else
            fancy_print(rooms[room_ind - 1], standard_reading_tempo, true);
    }
    printf("\n");
}

void print_zufallsanhaenge()
{
    switch (rand() % 9)
    {
    case 0:
        if (biome[room_ind - 1] != 'm' && biome[room_ind - 1] != 'a')
            fancy_print(zufallsanhaenge[0], standard_reading_tempo, true);
        break;
    case 1:
        if (biome[room_ind - 1] != 'm' && biome[room_ind - 1] != 's')
            fancy_print(zufallsanhaenge[1], standard_reading_tempo, true);
        break;
    case 2:
        if (biome[room_ind - 1] == 'w' || biome[room_ind - 1] == 'l')
            fancy_print(zufallsanhaenge[2], standard_reading_tempo, true);
        break;
    case 3:
        if (biome[room_ind - 1] != 'm' && biome[room_ind - 1] != 'h' && biome[room_ind - 1] != 'q')
            fancy_print(zufallsanhaenge[3], standard_reading_tempo, true);
        break;
    case 4:
        if (biome[room_ind - 1] != 'm' && biome[room_ind - 1] != 'h' && biome[room_ind - 1] != 'q' && biome[room_ind - 1] != 'a' && has_item(KAPPE))
        {
            fancy_print(zufallsanhaenge[4], standard_reading_tempo, true);
            remove_item(KAPPE);
        }
        break;
    case 5:
        if (biome[room_ind - 1] == 'm' && !underwater)
            fancy_print(zufallsanhaenge[5], standard_reading_tempo, true);
        break;
    case 6:
        if (biome[room_ind - 1] == 'm' && underwater)
            fancy_print(zufallsanhaenge[6], standard_reading_tempo, true);
        break;
    case 7:
        if (biome[room_ind - 1] == 'w')
            fancy_print(zufallsanhaenge[7], standard_reading_tempo, true);
        break;
    case 8:
        if (biome[room_ind - 1] == 'h')
            fancy_print(zufallsanhaenge[8], standard_reading_tempo, true);
        break;
    };
}

void death()
{
    fancy_print("\nGAME OVER\n", slow_reading_tempo, false);
    game = false;
}

//on room enter coroutine
void on_room_enter()
{
    if (biome[room_ind - 1] == 'c' && !has_item(KAPPE))
    {
        switch (schlucht_zaehler)
        {
        case 1:
            fancy_print("Du fühlst dich müde und ausgedörrt.\n", standard_reading_tempo, true);
            break;
        case 2:
            fancy_print("Die Zunge liegt dir geschwollen im ausgetrockneten Mund. Du kannst dich nur noch taumelnd weiterschleppen.\n", standard_reading_tempo, true);
            break;
        case 3:
            fancy_print("Du kannst nicht mehr und brichst zusammen. Langsam verlierst du das Bewusstsein.\n", standard_reading_tempo, true);
            game = false;
            return;
        };
        schlucht_zaehler += 1;
    }
    else if (schlucht_zaehler != 0)
    {
        switch (schlucht_zaehler)
        {
        case 2:
            fancy_print("Du setzt dich hin und ruhst dich im Schatten von den Strapazen der Schlucht aus.\n", standard_reading_tempo, true);
            break;
        case 3:
            fancy_print("Du hast es gerade noch rechtzeitig aus der Schlucht geschafft, lange hättest du nicht mehr durchgehalten.\n", standard_reading_tempo, true);
            break;
        }
        schlucht_zaehler = 0;
    }

    switch (room_ind)
    {
    case 13: //TROLL
        switch (monster_stat[TROLL])
        {
        case 0:
            fancy_print("Da du dich jedoch für die Klauenabdrücke interessierst, folgst du der ihrer Spur. Mit einem Mal weht dir heißer, fauliger Atem entgegen. Du hebst den Blick und ein Troll mit langem, verfilztem Fell und verzerrter Fratze wankt auf dich zu. Er beobachtet dich lauernd aus halb geschlossenen, tückischen Augen.\n", standard_reading_tempo, true);
            in_fight = true;
            return;
        case 1:
            fancy_print("Der massige Körper des Trolls liegt hier auf dem Boden. Vorsichtig steigst du über ihn hinweg.\n", standard_reading_tempo, true);
            break;
        }
        break;
    case 18: //SUMPFMONSTER
        switch (monster_stat[SUMPFMONSTER])
        {
        case 0:
            fancy_print("Du bemerkst eine heftige Bewegung des Schilfs. Die Schilfwand spaltet sich und ein Wesen tritt heraus, dessen grässliche Erscheinung zu beschreiben der Computer sich sträubt. Grünen Schleim sabbernd, schwabbelt es auf dich zu.\n", standard_reading_tempo, true);
            in_fight = true;
            return;
        case 1:
            break;
        }
        break;
    case 65:
        switch (monster_stat[AMEISEN])
        {
        case 0:
            fancy_print("Du siehst genauer hin und würgender Ekel steigt in dir auf: Legionen von rubinroter Riesenameisen mit knackenden Kieferklammern strömen auf dich zu.\n", standard_reading_tempo, true);
            in_fight = true;
            return;
        case 1:
            fancy_print("Der Ameisenhaufen wirkt verlassen, keine Spur mehr von den Riesenameisen.", standard_reading_tempo, true);
            break;
        }
        break;
    case 98:
        if (underwater)
        {
            switch (monster_stat[FISCH])
            {
            case 0:
                fancy_print("Aus diesem Riff saust dir ein meterlanger, silbrig glänzender Fisch entgegen. In seinem Maul blinken Reihen von dreieckigen Zähnen.\n", standard_reading_tempo, true);
                in_fight = true;
                return;
            case 1:
                fancy_print("Halb im Sand vergraben liegt hier ein weiß blinkendes, kleines Ei.\n", standard_reading_tempo, true);
                break;
            }
        }
        break;
    }

    int item_count = 0;
    for (int i = 0; i < OBJEKTE_LEN; i++)
    {
        if (room_ind == objekte_loc[i])
        {

            bool is_hidden = false;
            for (int j = 0; j < sizeof(objekte_hidden) / sizeof(int); j++)
            {
                if (objekte_hidden[j] == i)
                {
                    is_hidden = true;
                    break;
                }
            }
            if (is_hidden)
                continue;
            if (!item_count)
            {
                item_count++;
                char buffer[50];
                sprintf(buffer, new_object_lines[rand() % 4], objekte_artk1[i], objekte[i]);
                fancy_print(buffer, standard_reading_tempo, true);
            }
            else
            {
                char buffer[30];
                sprintf(buffer, ", %s %s", objekte_artk1[i], objekte[i]);
                fancy_print(buffer, standard_reading_tempo, true);
            }
        }
    }
    if (item_count)
        printf(".\n");
}

//main loop
int main()
{
    srand((unsigned)time(NULL));
    if (!DEBUG)
        start_seq();

    while (game)
    {
        if (room_ind != last_room)
        {
            print_description();
            on_room_enter();
            if (!in_fight)
                print_zufallsanhaenge();
        }
        last_room = room_ind;
        handle_command();
    }
    death();
    return 0;
}
