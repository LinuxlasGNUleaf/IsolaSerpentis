#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <time.h>

#include "hash.h"
#include "texts.h"

#ifdef _WIN32
#include <windows.h>
#include <direct.h>
#define GetCurrentDir _getcwd
#else
#include <unistd.h>
#define GetCurrentDir getcwd
#endif

#define SKIP_INTRO 1
#define DEBUG 0
#define INTEGRITY_CHECK 1

//constants
const int max_line_length = 120;
const int standard_reading_tempo = 8;
const int slow_reading_tempo = 15;

char PATH[FILENAME_MAX];

//game variables
int monster_defeated[MONSTER_LEN] = {0, 0, 0, 0};
int backpack[4] = {-1, -1, -1, -1};

int room_ind = 77; //0
int last_room = 77;
int schlucht_zaehler = 0; //1
int hilfe_zaehler = 0;    //2
int cmd_count = 0;        //3

unsigned char kappe_auf = 0;  //4
unsigned char geist = 0;      //5
unsigned char underwater = 0; //6
unsigned char can_dive = 0;   //7
unsigned char in_fight = 0;   //8
unsigned char game = 1;       //9

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
unsigned char has_item(int item)
{
    for (int i = 0; i < sizeof(backpack) / sizeof(int); i++)
    {
        if (backpack[i] == item)
            return 1;
    }
    return 0;
}

//adds item to inventory
int add_item(int item)
{
    for (int i = 0; i < sizeof(backpack) / sizeof(int); i++)
    {
        if (backpack[i] == -1)
        {
            backpack[i] = item;
            objekte_loc[item] = -1;
            return 1;
        }
    }
    return 0;
}

//removes specified item from inventory
void remove_item(int item, unsigned char drop)
{
    if (item == KAPPE)
        kappe_auf = 0;
    for (int i = 0; i < sizeof(backpack) / sizeof(int); i++)
    {
        if (backpack[i] == item)
        {
            backpack[i] = -1;
            if (drop)
                objekte_loc[item] = room_ind;
            return;
        }
    }
    return;
}

//prints text char by char with delay, with the option of extra breaks for punctuations
void fancy_print(char str[], int ms, unsigned char auto_line_break, unsigned char punctuation_breaks)
{
    int len = strlen(str);
    int last_line_break = 0;
    int next_possible_break = 0;
    for (int i = 0; i < len; i++)
    {
        if (i >= next_possible_break && i < len - 1 && auto_line_break)
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

void end_fight(unsigned char won)
{
    if (!won) //end game if player lost fight
        game = 0;
    else
        in_fight = 0;

    switch (room_ind)
    {
    case TROLL_LOC:
        switch (won)
        {
        case 0:
            fancy_print("Mit einem schaurigen Gebrüll stürzt sich der Troll über dich. Er ist dir an Kraft bei weitem überlegen und zerknackt dir mühelos das Genick...\n", standard_reading_tempo, 1, 1);
            break;
        case 1:
            fancy_print("Ein Blutstrahl schießt aus der Wunde des Trolls. Er bricht tot zusammen!\n", standard_reading_tempo, 1, 1);
            monster_defeated[TROLL] = 1;
            break;
        }
        break;
    case SUMPFMONSTER_LOC:
        switch (won)
        {
        case 0:
            fancy_print("Das Sumpfwesen umschlingt dich mit seinen glitschigen Armen und zieht dich ins Moor, wo du elendig ersäufst.\n", standard_reading_tempo, 1, 1);
            break;
        case 1:
            fancy_print("Der Geist erscheint. Wutentbrannt legt er die Flinte an und schießt auf das Monster. Mit lautem Getöse explodiert das Schießeisen, Splitter fliegen umher. Das Monster sinkt mit schmerzverzerrter Miene ins Moorwasser zurück. Glücklich über deine Rettung bedankst du dich beim Geist. „Gern geschehen“, damit verschwindet er.\n", standard_reading_tempo, 1, 1);
            monster_defeated[SUMPFMONSTER] = 1;
            break;
        }
        break;
    case AMEISEN_LOC:
        switch (won)
        {
        case 0:
            fancy_print("Tausende von Killerameisen überfluten dich. Glühende Nägel scheinen dir ins Fleisch getrieben zu werden, als sie dir Fleischstückchen aus dem Körper reißen. Mach einer halben Minute zeugt nur ein Skelett von dir.\n", standard_reading_tempo, 1, 1);
            break;
        case 1:
            fancy_print("Als die Ameisen deinen Schild wahrnehmen, flüchten sie von Panik ergriffen in ihren Bau. Jetzt bemerkst du den blauen Hals einer Flasche, die aus dem Haufen ragt.\n", standard_reading_tempo, 1, 1);
            monster_defeated[AMEISEN] = 1;
            break;
        }
        break;
    case FISCH_LOC:
        switch (won)
        {
        case 0:
            fancy_print("Der Fisch umschwimmt dich in immer enger werdenden Kreisen. Er beißt sich in deinem Bein fest! Durch das Blut angelockt, schießen hunderte von kleinen, silbernen Blitzen auf dich zu. Die Fischschwärme lassen nicht viel von dir übrig...\n", standard_reading_tempo, 1, 1);
            in_fight = 1;
            return;
        case 1:
            fancy_print("Der Fisch umschwimmt die Blüte und beäugt sie misstrauisch. Plötzlich schießt er vor und schnappt sie. Du starrst ihn fasziniert an: Sein Bauch glüht in einem flackernden Rot auf. Er zuckt krampfhaft mit der Schwanzflosse und zerfällt zu Fischmehl, das langsam zu Boden sinkt. Vor Erleichterung stößt du einen tiefen Seufzer aus. Auf einmal erblickst du vor dir einen weißen Schimmer. Er geht von einem kleinen, blanken Ei aus, das zwischen dem Fischmehl liegt.\n", standard_reading_tempo, 1, 1);
            monster_defeated[FISCH] = 1;
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
    fancy_print(title, 3, 0, 0);
    printf("Drücke ENTER, um zu starten.");
    getchar();
    printf("\n");
    fancy_print(intro1, standard_reading_tempo, 1, 1);
    printf("Drücke ENTER, um fortzufahren.");
    getchar();
    printf("\n");
    fancy_print(intro2, standard_reading_tempo, 1, 1);
    printf("Drücke ENTER, um fortzufahren.");
    getchar();
    printf("\n");
    fancy_print(intro3, standard_reading_tempo, 1, 1);
}

void save_game(char name[])
{
    char savefile[FILENAME_MAX];
#ifdef _WIN32
    sprintf(savefile, "%.3995s\\%s.iss", PATH, name);
#else
    sprintf(savefile, "%.3995s/%s.iss", PATH, name);
#endif
    if (!access(savefile, F_OK))
    {
        if (access(savefile, W_OK))
        {
            fancy_print("Fehler: Schreiberechte für diesen Spielstand fehlt!\n", standard_reading_tempo, 1, 1);
            return;
        }
        char answer;
        fancy_print("Speicherstand mit diesem Namen existiert bereits. Überschreiben? (y/N)", standard_reading_tempo, 1, 1);
        answer = getc(stdin);
        while (getchar() != '\n')
            ; //clear stdin buffer
        if (tolower(answer) != 'y')
        {
            fancy_print("Abbruch.\n", standard_reading_tempo, 1, 1);
            return;
        }
        fflush(stdout);
    }

    printf("%s\n", savefile);

    char savestring[400];
    sprintf(savestring, "%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i|", room_ind, last_room, schlucht_zaehler, hilfe_zaehler, cmd_count, kappe_auf, geist, underwater, can_dive, in_fight, game);
    char temp[10];
    for (int i = 0; i < sizeof(backpack) / sizeof(int); i++)
    {
        if (i)
            sprintf(temp, ",%i", backpack[i]);
        else
            sprintf(temp, "%i", backpack[i]);
        strcat(savestring, temp);
    }
    for (int i = 0; i < OBJEKTE_LEN; i++)
    {
        sprintf(temp, ",%i", objekte_loc[i]);
        strcat(savestring, temp);
    }
    for (int i = 0; i < MONSTER_LEN; i++)
    {
        sprintf(temp, ",%i", monster_defeated[i]);
        strcat(savestring, temp);
    }
    FILE *fp = fopen(savefile, "w");
    fprintf(fp, savestring);
    uint32_t hash = SuperFastHash(savestring, strlen(savestring));
    fprintf(fp, "\n%08x", hash);
    fclose(fp);
    fancy_print("\nSpeichern abgeschlossen.\n", standard_reading_tempo, 1, 1);
}

void load_game(char name[])
{
    char savefile[FILENAME_MAX];
#ifdef _WIN32
    sprintf(savefile, "%.3995s\\%s.iss", PATH, name);
#else
    sprintf(savefile, "%.3995s/%s.iss", PATH, name);
#endif
    if (access(savefile, F_OK))
    {
        fancy_print("Es existiert in diesem Verzeichnis kein Spielstand mit diesem Namen!\n", standard_reading_tempo, 1, 1);
        return;
    }
    else if (access(savefile, R_OK))
    {
        fancy_print("Fehler: Leserechte für diesen Spielstand fehlen!\n", standard_reading_tempo, 1, 1);
        return;
    }
    fancy_print("Spielstand wird geladen und überprüft...", standard_reading_tempo, 1, 1);
    FILE *fp;
    fp = fopen(savefile, "r");
    char savestring[400];
    uint32_t hash_given = 0;
    if (fscanf(fp, "%s\n%08x", savestring, &hash_given) != 2)
    {
        fancy_print("\nAus dieser Datei konnte kein Spielstand wiederhergestellt werden. Wähle einen anderen Spielstand zum Laden aus.\n", standard_reading_tempo, 1, 1);
        return;
    }
    uint32_t hash_calculated = SuperFastHash(savestring, strlen(savestring));
    fclose(fp);
    if (hash_given != hash_calculated)
    {
        fancy_print("\nSpielstand ist korrupt. Wähle einen anderen Spielstand zum Laden aus.\n", standard_reading_tempo, 1, 1);
        return;
    }
    char temp[100];
    sscanf(savestring, "%i,%i,%i,%i,%i,%hhi,%hhi,%hhi,%hhi,%hhi,%hhi|%s", &room_ind, &last_room, &schlucht_zaehler, &hilfe_zaehler, &cmd_count, &kappe_auf, &geist, &underwater, &can_dive, &in_fight, &game, temp);
    char *pch;
    pch = strtok(temp, ",");
    for (int i = 0; i < sizeof(backpack) / sizeof(int); i++)
    {

        backpack[i] = atoi(pch);
        pch = strtok(NULL, ",");
        if (pch == NULL)
        {
            fancy_print("\nFehler beim Wiederherstellen des Spielstandes. Spielstand korrupt. Bitte Spiel neustarten.", standard_reading_tempo, 1, 1);
            exit(0);
        }
    }
    for (int i = 0; i < OBJEKTE_LEN; i++)
    {
        objekte_loc[i] = atoi(pch);
        pch = strtok(NULL, ",");
        if (pch == NULL)
        {
            fancy_print("\nFehler beim Wiederherstellen des Spielstandes. Spielstand korrupt. Bitte Spiel neustarten.", standard_reading_tempo, 1, 1);
            exit(0);
        }
    }
    for (int i = 0; i < MONSTER_LEN; i++)
    {
        monster_defeated[i] = atoi(pch);
        pch = strtok(NULL, ",");
        if (pch == NULL && i < MONSTER_LEN-1)
        {
            fancy_print("\nFehler beim Wiederherstellen des Spielstandes. Spielstand korrupt. Bitte Spiel neustarten.", standard_reading_tempo, 1, 1);
            exit(0);
        }
    }
    fancy_print("fertig. Spielstand erfolgreich wiederhergestellt.\n", standard_reading_tempo, 1, 1);
    last_room = -1;
}

//handle the command input
void handle_command()
{
    char buffer[40];
    char command[20] = "";
    char attribute[20] = "";

    fancy_print("Eingabe: ", standard_reading_tempo, 0, 0);
    fgets(buffer, sizeof(buffer), stdin);

    cmd_count++;

    int j = 0;
    unsigned char is_command = 1;
    for (int i = 0; i < sizeof(buffer); i++)
    {
        if (buffer[i] == ' ')
        {
            if (!is_command)
                break;
            is_command = 0;
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
            fancy_print("Du versuchst zu fliehen, jedoch vergeblich...\n", standard_reading_tempo, 1, 1);
            end_fight(0);
            return;
        }
        if (underwater)
        {
            if (!strcmp(attribute, "norden") || !strcmp(attribute, "n"))
            {
                for (int i = 0; i < sizeof(underwater_rooms) / sizeof(int); i++)
                {
                    if (room_ind - 10 == underwater_rooms[i])
                    {
                        room_ind -= 10;
                        return;
                    }
                }
            }
            else if (!strcmp(attribute, "osten") || !strcmp(attribute, "o"))
            {
                for (int i = 0; i < sizeof(underwater_rooms) / sizeof(int); i++)
                {
                    if (room_ind + 1 == underwater_rooms[i])
                    {
                        room_ind += 1;
                        return;
                    }
                }
            }
            else if (!strcmp(attribute, "westen") || !strcmp(attribute, "w"))
            {
                for (int i = 0; i < sizeof(underwater_rooms) / sizeof(int); i++)
                {
                    if (room_ind - 1 == underwater_rooms[i])
                    {
                        room_ind -= 1;
                        return;
                    }
                }
            }
#ifdef _WIN32
            else if (!strcmp(attribute, "sueden") || !strcmp(attribute, "s"))
#else
            else if (!strcmp(attribute, "süden") || !strcmp(attribute, "s"))
#endif
            {
                for (int i = 0; i < sizeof(underwater_rooms) / sizeof(int); i++)
                {
                    if (room_ind + 10 == underwater_rooms[i])
                    {
                        room_ind += 10;
                        return;
                    }
                }
            }
            else
            {
#ifdef _WIN32
                fancy_print("Hm... diese Richtung kenne ich nicht... probiers mal mit Norden, Osten, Sueden oder Westen.\n", standard_reading_tempo, 1, 1);
#else
                fancy_print("Hm... diese Richtung kenne ich nicht... probiers mal mit Norden, Osten, Süden oder Westen.\n", standard_reading_tempo, 1, 1);
#endif
                return;
            }
            fancy_print("In diese Richtung kannst du nicht weiter tauchen!\n", standard_reading_tempo, 1, 1);
        }
        else
        {
            if (!strcmp(attribute, "norden") || !strcmp(attribute, "n"))
            {
                for (int i = 0; i < 4; i++)
                {
                    if (rooms_directions[room_ind - 1][i] == 'n')
                    {
                        room_ind -= 10;
                        return;
                    }
                }
            }
            else if (!strcmp(attribute, "osten") || !strcmp(attribute, "o"))
            {
                for (int i = 0; i < 4; i++)
                {
                    if (rooms_directions[room_ind - 1][i] == 'o')
                    {
                        room_ind += 1;
                        return;
                    }
                }
            }
            else if (!strcmp(attribute, "westen") || !strcmp(attribute, "w"))
            {
                for (int i = 0; i < 4; i++)
                {
                    if (rooms_directions[room_ind - 1][i] == 'w' || !strcmp(attribute, "w"))
                    {
                        room_ind -= 1;
                        return;
                    }
                }
            }
#ifdef _WIN32
            else if (!strcmp(attribute, "sueden") || !strcmp(attribute, "s"))
#else
            else if (!strcmp(attribute, "süden") || !strcmp(attribute, "s"))
#endif
            {
                for (int i = 0; i < 4; i++)
                {
                    if (rooms_directions[room_ind - 1][i] == 's')
                    {
                        room_ind += 10;
                        return;
                    }
                }
            }
            else
            {
#ifdef _WIN32
                fancy_print("Hm... diese Richtung kenne ich nicht... probiers mal mit Norden, Osten, Sueden oder Westen.\n", standard_reading_tempo, 1, 1);
#else
                fancy_print("Hm... diese Richtung kenne ich nicht... probiers mal mit Norden, Osten, Süden oder Westen.\n", standard_reading_tempo, 1, 1);
#endif
                return;
            }
            if (last_room == room_ind)
            {
                fancy_print("Dieser Weg steht dir nicht offen.\n", standard_reading_tempo, 1, 1);
            }
        }
    }
    else if (!strcmp(command, "tauche"))
    {
        if (biome[room_ind - 1] == MEER)
        {
            if (in_fight)
            {
                fancy_print("Du versuchst zu fliehen, jedoch vergeblich...\n", standard_reading_tempo, 1, 1);
                end_fight(0);
                return;
            }
            if (!strcmp(attribute, "auf"))
            {
                if (underwater)
                {
                    fancy_print("Du schwimmst zurück an die Wasseroberfläche.\n", standard_reading_tempo, 1, 1);
                    underwater = 0;
                }
                else
                    fancy_print("Du bist schon oben, Transuse!\n", standard_reading_tempo, 1, 1);
            }
            else if (!strcmp(attribute, "ab") || !strcmp(attribute, "unter"))
            {
                if (underwater)
                    fancy_print("Du bist schon unten!\n", standard_reading_tempo, 1, 1);
                else if (can_dive)
                {
                    for (int i = 0; i < sizeof(underwater_rooms); i++)
                    {
                        if (room_ind == underwater_rooms[i])
                        {
                            fancy_print("Du tauchst ab in die Tiefe des Ozeans.\n", standard_reading_tempo, 1, 1);
                            underwater = 1;
                            break;
                        }
                    }
                    if (!underwater)
                        fancy_print("Hier ist es zu flach zum Abtauchen!\n", standard_reading_tempo, 1, 1);
                }
                else
                    fancy_print("Du tauchst ab, aber nach einigen Zügen geht dir die Luft aus, und du musst wieder auftauchen!\n", standard_reading_tempo, 1, 1);
            }
            else
                fancy_print("Du kannst entweder auf- oder abtauchen.\n", standard_reading_tempo, 1, 1);
        }
        else if (!strcmp(attribute, "auf"))
            fancy_print("Du schnappst nach Luft und merkst, dass du dich auf festem Boden befindest und somit nicht auftauchen musst...\n", standard_reading_tempo, 1, 1);
        else if (!strcmp(attribute, "ab") || !strcmp(attribute, "unter"))
            fancy_print("Du versuchst dich kopfüber in den Boden zu wühlen. Erst dann merkst du, dass du dich an Land befindest...\n", standard_reading_tempo, 1, 1);
        else
            fancy_print("Also erstens mal bist du an Land, und was genau soll der zweite Teil sein?\n", standard_reading_tempo, 1, 1);
    }
    else if (!strcmp(command, "schreie"))
    {
        fancy_print("AAAAAAAAAAAAAAAARGH!!!\n", standard_reading_tempo, 1, 1);
    }
    else if (!strcmp(command, "weine"))
    {
        fancy_print("Schluchz! Schnüff!\n", standard_reading_tempo, 1, 1);
    }
    else if (!strcmp(command, "nehme"))
    {
        if (in_fight)
        {
            fancy_print("Dafür ist keine Zeit!\n", standard_reading_tempo, 1, 1);
            return;
        }
        if (strlen(attribute))
        {
            if (biome[room_ind - 1] == MEER && !underwater)
            {
                fancy_print("Du schaust dich um, kannst ein solches Objekt jedoch nirgendwo finden. Vielleicht hast du mehr Glück am Grunde des Meeres...\n", standard_reading_tempo, 1, 1);
                return;
            }
            char buffer2[10];
            for (int i = 0; i < OBJEKTE_LEN; i++)
            {
                strcpy(buffer2, objekte[i]);
                buffer2[0] = tolower(buffer2[0]);
                if (!strcmp(attribute, "buch") && room_ind == 29)
                {
                    fancy_print("Das Buch ist angekettet, und du bist nicht in der Lage es aus der Verankerung zu reißen.\n", standard_reading_tempo, 1, 1);
                    return;
                }
                if (!strcmp(attribute, buffer2) && room_ind == objekte_loc[i])
                {
                    char buffer3[140];
                    if (add_item(i))
                    {
                        sprintf(buffer3, "Du verstaust %.5s %.11s in deinem Rucksack.\n", objekte_artk2[i], objekte[i]);
                        fancy_print(buffer3, standard_reading_tempo, 1, 1);
                    }
                    else
                    {
                        sprintf(buffer3, "Dein Rucksack hat keinen Platz mehr für %.5s %.11s. Du überlegst dir, ob du etwas Unwichtiges auspacken solltest.\n", objekte_artk2[i], objekte[i]);
                        fancy_print(buffer3, standard_reading_tempo, 1, 1);
                        return;
                    }
                    break;
                }
                if (i == OBJEKTE_LEN - 1)
                    fancy_print("Du schaust dich um, kannst ein solches Objekt jedoch nirgendwo finden.\n", standard_reading_tempo, 1, 1);
            }
        }
        else
        {
            fancy_print("Geht das noch etwas genauer?\n", standard_reading_tempo, 1, 1);
        }
    }
    else if (!strcmp(command, "lege"))
    {
        if (strlen(attribute))
        {
            char buffer2[11];
            for (int i = 0; i < OBJEKTE_LEN; i++)
            {
                strcpy(buffer2, objekte[i]);
                buffer2[0] = tolower(buffer2[0]);
                if (!strcmp(attribute, buffer2) && has_item(i))
                {
                    if (biome[room_ind - 1] == MEER && !underwater && !can_dive)
                    {
                        fancy_print("Du zögerst: Wenn du hier etwas ablegest, sinkt es hinab und du kannst es dir nicht mehr wiederholen!\n", standard_reading_tempo, 1, 1);
                        return;
                    }
                    char buffer3[100];
                    sprintf(buffer3, "Du greifst in deinen Rucksack und legst %.5s %.11s ab.\n", objekte_artk2[i], objekte[i]);
                    fancy_print(buffer3, standard_reading_tempo, 1, 1);

                    if (i == BLUETE && room_ind == 98 && underwater)
                    {
                        remove_item(i, 0);
                        end_fight(1);
                        return;
                    }
                    remove_item(i, 1);
                    break;
                }
                if (i == OBJEKTE_LEN - 1)
                    fancy_print("Du durchwühlst deine Taschen, aber so ein Objekt findest du nicht bei dir.\n", standard_reading_tempo, 1, 1);
            }
        }
        else
        {
            fancy_print("Was willst du den ablegen?\n", standard_reading_tempo, 1, 1);
        }
    }
    else if (!strcmp(command, "lache"))
    {
        fancy_print("Huahuahua!\n", standard_reading_tempo, 1, 1);
    }
    else if (!strcmp(command, "hilfe"))
    {
        fancy_print(hilfe, standard_reading_tempo, 0, 1);
    }
    else if (!strcmp(command, "suche") || !strcmp(command, "finde"))
    {
        fancy_print("Ich soll für dich suchen? Wer spielt denn hier, ich oder du?\n", standard_reading_tempo, 1, 1);
    }
    else if (!strcmp(command, "nutze") || !strcmp(command, "benutze") || !strcmp(command, "kämpfe"))
    {
        if (in_fight && !strcmp(attribute, "geist") && geist && room_ind == 18) // Sumpfmonster besiegen
        {
            if (has_item(FLINTE))
            {
                remove_item(FLINTE, 0);
                end_fight(1);
                return;
            }
            else
            {
                fancy_print("Der Geist erscheint. „Hör mal, ohne Waffe kann auch ich hier nichts ausrichten!“ Verzweifelt blickst du deinem Ende entgegen...\n", standard_reading_tempo, 1, 1);
                end_fight(0);
                return;
            }
        }
        else if (in_fight && !strcmp(attribute, "flinte") && has_item(FLINTE)) // Flinte benutzen
        {
            fancy_print("Du spannst den Hahn, zielst und drückst ab. Der rostige Schießprügel birst mit einem gewaltigen Knall. Du spürst einen stechenden Schmerz im Kopf – und dann nichts mehr.\n", standard_reading_tempo, 1, 1);
            game = 0;
            return;
        }
        else if (!strcmp(attribute, "messer") && has_item(MESSER)) // Messer benutzen
        {
            if (in_fight)
            {
                switch (room_ind)
                {
                case FISCH_LOC:
                    fancy_print("An den stahlharten Schuppen zerbricht die Klinge wie morsches Holz.\n", standard_reading_tempo, 1, 1);
                    end_fight(0);
                    break;
                case SUMPFMONSTER_LOC:
                    fancy_print("Das Messer glitscht an der Haut des Monsters ab und rutscht dir in ein Sumpfloch. Verzweifelt siehst du es versinken.\n", standard_reading_tempo, 1, 1);
                    end_fight(0);
                    break;
                case TROLL_LOC:
                    end_fight(1);
                    break;
                }
            }
            else
            {
                fancy_print("Das Messer scheint scheint sehr scharf zu sein, aber momentan kannst du nichts damit anfangen...\n", standard_reading_tempo, 1, 1);
            }
        }
        else if (!strcmp(attribute, "schild") && has_item(SCHILD)) //Schild benutzen
        {
            if (in_fight)
            {
                switch (room_ind)
                {
                case SUMPFMONSTER_LOC:
                    fancy_print("Dein Schild bleibt in einem Sumpfgewächs hängen und wird dir aus der Hand gerissen. Schutzlos stehst du vor dem Untier.\n", standard_reading_tempo, 1, 1);
                    end_fight(0);
                    break;
                case TROLL_LOC:
                    fancy_print("Mit einer lässigen Handbewegung wischt der Troll deine Deckung weg.\n", standard_reading_tempo, 1, 1);
                    end_fight(0);
                    break;
                case AMEISEN_LOC:
                    end_fight(1);
                    break;
                case FISCH_LOC:
                    fancy_print("Unter Wasser kannst du den Schild nicht schnell genug bewegen. Es ist dem Fisch ein leichtes, deine Deckung zu umschwimmen.\n", standard_reading_tempo, 1, 1);
                    end_fight(0);
                    break;
                }
            }
            else
            {
                fancy_print("Deinen Schild kannst du nur im Kampf benutzen!\n", standard_reading_tempo, 1, 1);
            }
        }
        else if (!strcmp(attribute, "flasche") && has_item(FLASCHE))
        {
            fancy_print("Du entfernst den Siegellack mit den Zähnen aus der Flasche. Milchig-weißer Rauch schießt heraus, der sich zu einer Gestalt formt. Es ist ein Geist! Mit einem Wutschrei schleudert er die Flasche weit weg. Nur eine Vorsichtsmaßnahme! „Ich hatte mal einen angeheirateten Vetter der... Übrigens, wenn du mal Hilfe brauchen solltest, wende dich an mich, ja? Du kannst mir aber auch was schenken, denn kleine Geschenke erhalten die Freundschaft.“\n", standard_reading_tempo, 1, 1);
            geist = 1;
            remove_item(FLASCHE, 0);
        }
        else if (!strcmp(attribute, "geist") && geist) // Geist-Aktionen
        {
            if (hilfe_zaehler == 3)
            {
                fancy_print("Der Geist erscheint und zuckt mit den Schultern. „Ich weiß leider keinen Rat mehr.“\n", standard_reading_tempo, 1, 1);
                return;
            }
            else
            {
                hilfe_zaehler++;
                fancy_print("Der Geist erscheint in einer Wolke.\n", standard_reading_tempo, 1, 1);

                if (room_ind == 98 && monster_defeated[FISCH])
                    fancy_print("„Ei? Das habe ich doch irgendwann schon mal gehört...“\n", standard_reading_tempo, 1, 1);
                else if (biome[room_ind - 1] == STRAND && objekte_loc[KAPPE] == 85 && room_ind != 85)
                    fancy_print("„Das Meer schwemmt vieles an...“", standard_reading_tempo, 1, 1);
                else if (biome[room_ind - 1] == SCHLUCHT && objekte_loc[KAPPE] == 49 && room_ind != 49)
                    fancy_print("„Es ist etwas in der Nähe mit dem du vielleicht dein Glück machen könntest...“\n", standard_reading_tempo, 1, 1);
                else if (room_ind == 36 && (objekte_loc[FLINTE] == 36 || has_item(FLINTE)))
                    fancy_print("„Eine Flinte ist eine gefährliche Waffe...“\n", standard_reading_tempo, 1, 1);
                else if (room_ind == 38)
                    fancy_print("„Tödliche Gefahr lauert in der Nähe...“\n", standard_reading_tempo, 1, 1);
                else if (biome[room_ind - 1] == HOEHLE && !can_dive)
                    fancy_print("„Höhlen bergen gefährliche Fallen ebenso wie zauberische Orte und unschätzbare Hilfen...“\n", standard_reading_tempo, 1, 1);
                else
                {
                    hilfe_zaehler--;
                    switch (rand() % 5)
                    {
                    case 0:
                        fancy_print("„Vielleicht findest du in der Nähe einen nützlichen Gegenstand?“\n", standard_reading_tempo, 1, 1);
                        break;
                    case 1:
                        fancy_print("„Du solltest dir gerade jetzt mehr Mühe geben!“\n", standard_reading_tempo, 1, 1);
                        break;
                    case 2:
                        fancy_print("„Ich habe jetzt keine Zeit für dich! Wir feiern in Schottland ein Familienfest!“\n", standard_reading_tempo, 1, 1);
                        break;
                    case 3:
                        fancy_print("„Selbst Geister wissen nicht immer Rat!“\n", standard_reading_tempo, 1, 1);
                        break;
                    case 4:
                        fancy_print("„Du langweilst mich mit deiner ewigen Fragerei!“ Und er gähnt herzhaft.\n", standard_reading_tempo, 1, 1);
                        break;
                    }
                }
                fancy_print("Hiermit verschwindet er.\n", standard_reading_tempo, 1, 1);
            }
        }
        else if (!strcmp(attribute, "löffel") && has_item(LOEFFEL) && room_ind == 98 && underwater && !in_fight) // Ei zerstören
        {
            fancy_print("DU HAST GEWONNEN, HERZLICHEN GLÜCKWUNSCH!\n", slow_reading_tempo, 1, 1);
            game = 0;
        }
        else if (!strcmp(attribute, "löffel") || !strcmp(attribute, "gabel"))
        {
            fancy_print("Mit diesem Besteck wirst du wohl vorerst nichts anfangen können, Frühstück ist nirgendwo in Sicht.\n", slow_reading_tempo, 1, 1);
        }
        else if (!strcmp(attribute, "kappe") && has_item(KAPPE))
        {
            if (kappe_auf)
            {
                fancy_print("Du hast sie bereits auf dem Kopf.\n", standard_reading_tempo, 1, 1);
            }
            else
            {
                fancy_print("Du hast sie jetzt auf dem Kopf.\n", standard_reading_tempo, 1, 1);
                kappe_auf = 1;
            }
        }
        else if (!strlen(attribute))
        {
            if (in_fight)
                fancy_print("Nur ein Superman könnte so ein Untier mit den Händen erwürgen!\n", standard_reading_tempo, 1, 1);
            else
                fancy_print("Was genau willst du den benutzen?\n", standard_reading_tempo, 1, 1);
        }
        else
        {
            fancy_print("So ein Objekt hast du nicht!\n", standard_reading_tempo, 1, 1);
        }
    }
    else if (!strcmp(command, "lese"))
    {
        if (strlen(attribute))
        {
            if (!strcmp(attribute, "buch") && room_ind == 29)
            {
                fancy_print(buchtext, standard_reading_tempo, 1, 1);
            }
        }
        else
        {
            fancy_print("Was denn bitte?\n", standard_reading_tempo, 1, 1);
        }
    }
    else if (!strcmp(command, "info"))
    {
        char buffer2[10];
        for (int i = 0; i < OBJEKTE_LEN; i++)
        {
            strcpy(buffer2, objekte[i]);
            buffer2[0] = tolower(buffer2[0]);
            if (!strcmp(attribute, buffer2) && has_item(i))
            {
                fancy_print(objekte_desc[i], standard_reading_tempo, 1, 1);
                break;
            }
            if (i == OBJEKTE_LEN - 1)
                fancy_print("Du hast dieses Objekt nicht!\n", standard_reading_tempo, 1, 1);
        }
    }
    else if (!strcmp(command, "trinke"))
    {
        if (biome[room_ind - 1] == MEER || biome[room_ind - 1] == STRAND)
        {
            fancy_print("Du schluckst das salzige Meerwasser und erbrichst dich, von Krämpfen geschüttelt.\n", standard_reading_tempo, 1, 1);
        }
        else if (biome[room_ind - 1] == HOEHLENQUELLE)
        {
            if (!can_dive)
            {
                fancy_print("Das Wasser rinnt kühl durch deine Kehle. Plötzlich hast du das Gefühl, dass dein Kopf platzt. Als du dich im Wasser der Schale betrachtest, springst du entsetzt zurück: Zwei riesige Kiemenbüschel sind dir gewachsen. Die Kopfschmerzen lassen jedoch nach und du fühlst dich erfrischt und tatendurstig.\n", standard_reading_tempo, 1, 1);
                can_dive = 1;
            }
            else
                fancy_print("Repetita non placent!\n", standard_reading_tempo, 1, 1);
        }
    }
    else if (!strcmp(command, "klettere"))
    {
        if (!strlen(attribute))
        {
            fancy_print("Das war wohl nichts... gib das nächste Mal besser an, ob du hinauf oder hinab willst.\n", standard_reading_tempo, 1, 1);
            return;
        }
        if (!strcmp(attribute, "hinauf"))
        {
            switch (room_ind)
            {
            case 62:
                fancy_print("Du bist schon auf der Baumkrone, weiter nach oben geht es nicht!\n", standard_reading_tempo, 1, 1);
                break;
            case 53:
                fancy_print("Vorsichtig kletterst du an den Löchern hoch in die Baumkrone.\n", standard_reading_tempo, 1, 1);
                room_ind = 62;
                break;
            case 49:
                fancy_print("Die Böschung ist zu steil, um sie von dieser Seite aus zu ersteigen.\n", standard_reading_tempo, 1, 1);
                break;
            default:
                fancy_print("Hier gibt es nichts, wo du hinaufklettern könntest.\n", standard_reading_tempo, 1, 1);
                break;
            }
        }
        else if (!strcmp(attribute, "hinab"))
        {
            switch (room_ind)
            {
            case 62:
                fancy_print("Bedächtig hangelst du dich wieder hinab auf den Boden.\n", standard_reading_tempo, 1, 1);
                room_ind = 53;
                break;
            case 59:
                fancy_print("Vorsichtig steigst du die steile Böschung in die Schlucht hinunter.\n", standard_reading_tempo, 1, 1);
                room_ind = 49;
                break;
            default:
                fancy_print("Hier gibt es nichts, wo du hinabklettern könntest.\n", standard_reading_tempo, 1, 1);
                break;
            }
        }
    }
    else if (!strcmp(command, "rucksack"))
    {
        int item_count = 0;
        for (int i = 0; i < sizeof(backpack) / sizeof(int); i++)
        {
            if (backpack[i] != -1)
            {
                if (item_count == 0)
                {
                    item_count++;
                    char buffer[50];
                    sprintf(buffer, "In deinem Rucksack hast du %.7s %.11s", objekte_artk1[backpack[i]], objekte[backpack[i]]);
                    fancy_print(buffer, standard_reading_tempo, 1, 1);
                }
                else
                {
                    char buffer[30];
                    sprintf(buffer, ", %.7s %.11s", objekte_artk1[backpack[i]], objekte[backpack[i]]);
                    fancy_print(buffer, standard_reading_tempo, 1, 1);
                }
            }
        }
        if (item_count)
            printf(".\n");
        else
            fancy_print("Du, öffnest deinen Rucksack, in dem jedoch gähnende Leere herrscht.\n", standard_reading_tempo, 1, 1);
    }
    else if (!strcmp(command, "schenke") || !strcmp(command, "verschenke"))
    {
        if (!strlen(attribute))
        {
            fancy_print("Was willst du denn verschenken?\n", standard_reading_tempo, 1, 1);
            return;
        }
        if (!geist)
        {
            fancy_print("Hier ist niemand, dem du etwas schenken könntest.\n", standard_reading_tempo, 1, 1);
            return;
        }
        char buffer2[11];
        for (int i = 0; i < OBJEKTE_LEN; i++)
        {
            strcpy(buffer2, objekte[i]);
            buffer2[0] = tolower(buffer2[0]);
            if (!strcmp(attribute, buffer2) && has_item(i))
            {
                unsigned char can_be_gifted = 0;
                for (int j = 0; j < sizeof(objekte_giftable) / sizeof(int); j++)
                {
                    if (objekte_giftable[j] == i)
                        can_be_gifted = 1;
                }
                if (!can_be_gifted)
                {
                    fancy_print("Das kannst du nicht verschenken!\n", standard_reading_tempo, 1, 1);
                    return;
                }
                char buffer3[100];
                sprintf(buffer3, "Du rufst den Geist und gibst ihm %.5s %.11s aus deinen Rucksack.\n", objekte_artk2[i], objekte[i]);
                fancy_print(buffer3, standard_reading_tempo, 1, 1);
                hilfe_zaehler--;
                fancy_print("„Dankeschön!“,der Geist verbeugt sich und verschwindet in einer Rauchwolke.\n", standard_reading_tempo, 1, 1);
                remove_item(i, 0);
                break;
            }
            if (i == OBJEKTE_LEN - 1)
                fancy_print("Du durchwühlst deine Taschen, aber so ein Objekt findest du nicht bei dir.\n", standard_reading_tempo, 1, 1);
        }
    }
    else if (!strcmp(command, "speichere") || !strcmp(command, "speichern"))
    {
        if (!strlen(attribute))
        {
            fancy_print("Gebe den Namen an, unter dem du den Spielstand sichern willst.\n", standard_reading_tempo, 1, 1);
            return;
        }
        save_game(attribute);
    }
    else if (!strcmp(command, "lade") || !strcmp(command, "laden"))
    {
        if (!strlen(attribute))
        {
            fancy_print("Gebe den Namen des Spielstandes an, den du laden willst.\n", standard_reading_tempo, 1, 1);
            return;
        }
        load_game(attribute);
    }
    else
    {
        cmd_count--;
        fancy_print("Unbekannter Befehl. Gib \"Hilfe\" ein, um eine Liste von Befehlen zu erhalten.\n", standard_reading_tempo, 1, 1);
    }
}

//prints the room description
void print_description()
{
    if (biome[last_room - 1] != MEER && biome[room_ind - 1] == MEER)
        fancy_print("Langsam watest du ins Meer hinein.\n", standard_reading_tempo, 1, 1);
    else if (biome[last_room - 1] == MEER && biome[room_ind - 1] != MEER)
        fancy_print("Von Meerwasser triefend kommst du wieder an Land und lässt deine Kleidung in der Sonne trocknen.\n", standard_reading_tempo, 1, 1);

    char desc[30];
    printf("\n");
    switch (biome[room_ind - 1])
    {
    case HOEHLE:
        sprintf(desc, "%i - HÖHLE\n", room_ind);
        break;

    case SUMPF:
        sprintf(desc, "%i - SUMPF\n", room_ind);
        break;
    case NEBEL:
        sprintf(desc, "%i - NEBEL\n", room_ind);
        break;
    case BERGE:
        sprintf(desc, "%i - GEBIRGE\n", room_ind);
        break;
    case WIESE:
        sprintf(desc, "%i - WIESE\n", room_ind);
        break;
    case SCHLUCHT:
        sprintf(desc, "%i - SCHLUCHT\n", room_ind);
        break;
    case BAUMHAUS:
        sprintf(desc, "%i - BAUMHAUS\n", room_ind);
        break;
    case WALD:
        sprintf(desc, "%i - WALD\n", room_ind);
        break;
    case STRAND:
        sprintf(desc, "%i - STRAND\n", room_ind);
        break;
    case MEER:
        if (underwater)
            sprintf(desc, "%i - MEER (UNTER WASSER)\n", room_ind);
        else
            sprintf(desc, "%i - MEER (OBERFLÄCHE)\n", room_ind);
        break;
    case HOEHLENQUELLE:
        sprintf(desc, "%i - HÖHLENQUELLE\n", room_ind);
        break;
    default:
        sprintf(desc, "%i - FEHLER\n", room_ind);
        break;
    }

    fancy_print(desc, slow_reading_tempo, 1, 1);
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
                    fancy_print(underwater_descriptions[i], standard_reading_tempo, 1, 1);
                    break;
                }
            };
        }
        else
            fancy_print(rooms[room_ind - 1], standard_reading_tempo, 1, 1);
    }
    printf("\n");
}

//prints a random suffix
void print_zufallsanhaenge()
{
    switch (rand() % 9)
    {
    case 0:
        if (biome[room_ind - 1] != MEER && biome[room_ind - 1] != BAUMHAUS)
            fancy_print(zufallsanhaenge[0], standard_reading_tempo, 1, 1);
        break;
    case 1:
        if (biome[room_ind - 1] != MEER && biome[room_ind - 1] != SUMPF)
            fancy_print(zufallsanhaenge[1], standard_reading_tempo, 1, 1);
        break;
    case 2:
        if (biome[room_ind - 1] == WIESE || biome[room_ind - 1] == WALD)
            fancy_print(zufallsanhaenge[2], standard_reading_tempo, 1, 1);
        break;
    case 3:
        if (biome[room_ind - 1] != MEER && biome[room_ind - 1] != HOEHLE && biome[room_ind - 1] != HOEHLENQUELLE)
            fancy_print(zufallsanhaenge[3], standard_reading_tempo, 1, 1);
        break;
    case 4:
        if (biome[room_ind - 1] != MEER && biome[room_ind - 1] != HOEHLE && biome[room_ind - 1] != HOEHLENQUELLE && biome[room_ind - 1] != 'a' && kappe_auf)
        {
            fancy_print(zufallsanhaenge[4], standard_reading_tempo, 1, 1);
            remove_item(KAPPE, 1);
        }
        break;
    case 5:
        if (biome[room_ind - 1] == MEER && !underwater)
            fancy_print(zufallsanhaenge[5], standard_reading_tempo, 1, 1);
        break;
    case 6:
        if (biome[room_ind - 1] == MEER && underwater)
            fancy_print(zufallsanhaenge[6], standard_reading_tempo, 1, 1);
        break;
    case 7:
        if (biome[room_ind - 1] == WIESE)
            fancy_print(zufallsanhaenge[7], standard_reading_tempo, 1, 1);
        break;
    case 8:
        if (biome[room_ind - 1] == HOEHLE)
            fancy_print(zufallsanhaenge[8], standard_reading_tempo, 1, 1);
        break;
    };
}

//on death
void death()
{
    fancy_print("\nGAME OVER\n", slow_reading_tempo, 1, 0);
    game = 0;
}

//on room enter coroutine
void on_room_enter()
{
    if (biome[room_ind - 1] == 'c' && !has_item(KAPPE))
    {
        switch (schlucht_zaehler)
        {
        case 1:
            fancy_print("Du fühlst dich müde und ausgedörrt.\n", standard_reading_tempo, 1, 1);
            break;
        case 2:
            fancy_print("Die Zunge liegt dir geschwollen im ausgetrockneten Mund. Du kannst dich nur noch taumelnd weiterschleppen.\n", standard_reading_tempo, 1, 1);
            break;
        case 3:
            fancy_print("Du kannst nicht mehr und brichst zusammen. Langsam verlierst du das Bewusstsein.\n", standard_reading_tempo, 1, 1);
            game = 0;
            return;
        };
        schlucht_zaehler += 1;
    }
    else if (schlucht_zaehler != 0)
    {
        switch (schlucht_zaehler)
        {
        case 2:
            fancy_print("Du setzt dich hin und ruhst dich im Schatten von den Strapazen der Schlucht aus.\n", standard_reading_tempo, 1, 1);
            break;
        case 3:
            fancy_print("Du hast es gerade noch rechtzeitig aus der Schlucht geschafft, lange hättest du nicht mehr durchgehalten.\n", standard_reading_tempo, 1, 1);
            break;
        }
        schlucht_zaehler = 0;
    }

    switch (room_ind)
    {
    case TROLL_LOC:
        switch (monster_defeated[TROLL])
        {
        case 0:
            fancy_print("Da du dich jedoch für die Klauenabdrücke interessierst, folgst du der ihrer Spur. Mit einem Mal weht dir heißer, fauliger Atem entgegen. Du hebst den Blick und ein Troll mit langem, verfilztem Fell und verzerrter Fratze wankt auf dich zu. Er beobachtet dich lauernd aus halb geschlossenen, tückischen Augen.\n", standard_reading_tempo, 1, 1);
            in_fight = 1;
            return;
        case 1:
            fancy_print("Der massige Körper des Trolls liegt hier auf dem Boden. Vorsichtig steigst du über ihn hinweg.\n", standard_reading_tempo, 1, 1);
            break;
        }
        break;
    case SUMPFMONSTER_LOC:
        switch (monster_defeated[SUMPFMONSTER])
        {
        case 0:
            fancy_print("Du bemerkst eine heftige Bewegung des Schilfs. Die Schilfwand spaltet sich und ein Wesen tritt heraus, dessen grässliche Erscheinung zu beschreiben der Computer sich sträubt. Grünen Schleim sabbernd, schwabbelt es auf dich zu.\n", standard_reading_tempo, 1, 1);
            in_fight = 1;
            return;
        case 1:
            break;
        }
        break;
    case AMEISEN_LOC:
        switch (monster_defeated[AMEISEN])
        {
        case 0:
            fancy_print("Du siehst genauer hin und würgender Ekel steigt in dir auf: Legionen von rubinroter Riesenameisen mit knackenden Kieferklammern strömen auf dich zu.\n", standard_reading_tempo, 1, 1);
            in_fight = 1;
            return;
        case 1:
            fancy_print("Der Ameisenhaufen wirkt verlassen, keine Spur mehr von den Riesenameisen.\n", standard_reading_tempo, 1, 1);
            break;
        }
        break;
    case FISCH_LOC:
        if (underwater)
        {
            switch (monster_defeated[FISCH])
            {
            case 0:
                fancy_print("Aus diesem Riff saust dir ein meterlanger, silbrig glänzender Fisch entgegen. In seinem Maul blinken Reihen von dreieckigen Zähnen.\n", standard_reading_tempo, 1, 1);
                in_fight = 1;
                return;
            case 1:
                fancy_print("Halb im Sand vergraben liegt hier ein weiß blinkendes, kleines Ei.\n", standard_reading_tempo, 1, 1);
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

            unsigned char is_hidden = 0;
            for (int j = 0; j < sizeof(objekte_hidden) / sizeof(int); j++)
            {
                if (objekte_hidden[j] == i)
                {
                    is_hidden = 1;
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
                fancy_print(buffer, standard_reading_tempo, 1, 1);
            }
            else
            {
                char buffer[30];
                sprintf(buffer, ", %s %s", objekte_artk1[i], objekte[i]);
                fancy_print(buffer, standard_reading_tempo, 1, 1);
            }
        }
    }
    if (item_count)
        printf(".\n");
}

void special_event()
{
    fancy_print("Du hörst ein entferntes Grollen. Auf einmal schwankt der Boden unter dir.\n", standard_reading_tempo, 1, 1);

    switch (biome[room_ind - 1])
    {
    case MEER:
        fancy_print("Eine Riesenwelle wirft dich aufs Land.	Ein klaffender Abgrund tut sich vor dir auf. Wasser strudelt um deine Füße", standard_reading_tempo, 1, 1);
        room_ind = 77;
        break;

    case HOEHLE:
        fancy_print("Ein zentimeterbreiter Riss läuft über die Decke. Tonnenschwere Felsbrocken ersticken deinen Schrei.\n", standard_reading_tempo, 1, 1);
        game = 0;
        break;

    case HOEHLENQUELLE:
        fancy_print("Ein zentimeterbreiter Riss läuft über die Decke. Tonnenschwere Felsbrocken ersticken deinen Schrei.\n", standard_reading_tempo, 1, 1);
        game = 0;
        break;

    case BAUMHAUS:
        fancy_print("Du kannst dich nicht mehr halten und stürzt mit einem Aufschrei ab.\n", standard_reading_tempo, 1, 1);
        room_ind = 53;
        break;
    }
}
//main loop
int main()
{
    srand((unsigned)time(NULL));
    GetCurrentDir(PATH, FILENAME_MAX);
    if (access(PATH, W_OK) || access(PATH, R_OK))
    {
        printf("Fehler: In diesem Verzeichnis können keine Spielstände gespeichert oder gelesen werden. Bitte führe das Program in einem Verzeichnis aus, in dem du Schreib- und Leserechte hast.\n");
        return -1;
    }

#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
#endif
    if (!DEBUG && !SKIP_INTRO)
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
        if (cmd_count == 110)
            special_event();
    }
    death();
    return 0;
}
