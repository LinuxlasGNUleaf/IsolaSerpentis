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

#define SKIP_INTRO 1
#define DEBUG 0

//constants
const int max_line_length = 120;
const int standard_reading_tempo = 10;
const int slow_reading_tempo = 3;

//game variables
int room_ind = 77;
int last_room = 77;
int underwater_room_ind = 0;
int schlucht_zaehler = 0;
int hilfe_zaehler = 0;
int backpack[4] = {-1, -1, -1, -1};
int backpack_size = 4;
int idle_turns = 2;
bool kappe_auf = false;
bool geist = false;
bool underwater = false;
bool can_dive = false;
bool in_fight = false;
bool game = true;

//platform-dependent definition of sleep
#ifdef _WIN32
void os_wait(int ms)
{
    Sleep(ms);
}
SetConsoleOutputCP (1252);//für Umlaut-Ausgabe
SetConsoleCP (1252);//für Umlaut-Eingabe
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
            backpack_size--;
            return;
        }
    }
    return;
}

//removes specified item from inventory
void remove_item(int item, bool drop)
{
    if (item == KAPPE)
        kappe_auf = false;
    for (int i = 0; i < sizeof(backpack) / sizeof(int); i++)
    {
        if (backpack[i] == item)
        {
            backpack[i] = -1;
            backpack_size++;
            if (drop)
                objekte_loc[item] = room_ind;
            return;
        }
    }
    return;
}

//prints text char by char with delay, with the option of extra breaks for punctuations
void fancy_print(char str[], int ms, bool auto_line_break, bool punctuation_breaks)
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

void end_fight(bool won)
{
    if (!won) //end game if player lost fight
        game = false;
    else
        in_fight = false;

    switch (room_ind)
    {
    case TROLL_LOC:
        switch (won)
        {
        case 0:
            fancy_print("Mit einem schaurigen Gebrüll stürzt sich der Troll über dich. Er ist dir an Kraft bei weitem überlegen und zerknackt dir mühelos das Genick...\n", standard_reading_tempo, true, true);
            break;
        case 1:
            fancy_print("Ein Blutstrahl schießt aus der Wunde des Trolls. Er bricht tot zusammen!\n", standard_reading_tempo, true, true);
            monster_defeated[TROLL] = 1;
            break;
        }
        break;
    case SUMPFMONSTER_LOC:
        switch (won)
        {
        case 0:
            fancy_print("Das Sumpfwesen umschlingt dich mit seinen glitschigen Armen und zieht dich ins Moor, wo du elendig ersäufst.\n", standard_reading_tempo, true, true);
            break;
        case 1:
            fancy_print("Der Geist erscheint. Wutentbrannt legt er die Flinte an und schießt auf das Monster. Mit lautem Getöse explodiert das Schießeisen, Splitter fliegen umher. Das Monster sinkt mit schmerzverzerrter Miene ins Moorwasser zurück. Glücklich über deine Rettung bedankst du dich beim Geist. „Gern geschehen“, damit verschwindet er.\n", standard_reading_tempo, true, true);
            monster_defeated[SUMPFMONSTER] = 1;
            break;
        }
        break;
    case AMEISEN_LOC:
        switch (won)
        {
        case 0:
            fancy_print("Tausende von Killerameisen überfluten dich. Glühende Nägel scheinen dir ins Fleisch getrieben zu werden, als sie dir Fleischstückchen aus dem Körper reißen. Mach einer halben Minute zeugt nur ein Skelett von dir.\n", standard_reading_tempo, true, true);
            break;
        case 1:
            fancy_print("Als die Ameisen deinen Schild wahrnehmen, flüchten sie von Panik ergriffen in ihren Bau. Jetzt bemerkst du den blauen Hals einer Flasche, die aus dem Haufen ragt.\n", standard_reading_tempo, true, true);
            monster_defeated[AMEISEN] = 1;
            break;
        }
        break;
    case FISCH_LOC:
        switch (won)
        {
        case 0:
            fancy_print("Der Fisch umschwimmt dich in immer enger werdenden Kreisen. Er beißt sich in deinem Bein fest! Durch das Blut angelockt, schießen hunderte von kleinen, silbernen Blitzen auf dich zu. Die Fischschwärme lassen nicht viel von dir übrig...\n", standard_reading_tempo, true, true);
            in_fight = true;
            return;
        case 1:
            fancy_print("Der Fisch umschwimmt die Blüte und beäugt sie misstrauisch. Plötzlich schießt er vor und schnappt sie. Du starrst ihn fasziniert an: Sein Bauch glüht in einem flackernden Rot auf. Er zuckt krampfhaft mit der Schwanzflosse und zerfällt zu Fischmehl, das langsam zu Boden sinkt. Vor Erleichterung stößt du einen tiefen Seufzer aus. Auf einmal erblickst du vor dir einen weißen Schimmer. Er geht von einem kleinen, blanken Ei aus, das zwischen dem Fischmehl liegt.\n", standard_reading_tempo, true, true);
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
    fancy_print(title, slow_reading_tempo, false, false);
    printf("Drücke ENTER, um zu starten.");
    getchar();
    printf("\n");
    fancy_print(intro1, standard_reading_tempo, true, true);
    printf("Drücke ENTER, um fortzufahren.");
    getchar();
    printf("\n");
    fancy_print(intro2, standard_reading_tempo, true, true);
    printf("Drücke ENTER, um fortzufahren.");
    getchar();
    printf("\n");
    fancy_print(intro3, standard_reading_tempo, true, true);
}

//handle the command input
void handle_command()
{
    char buffer[40];
    char command[20] = "";
    char attribute[20] = "";

    fancy_print("Eingabe: ", standard_reading_tempo, false, false);
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
            fancy_print("Du versuchst zu fliehen, jedoch vergeblich...\n", standard_reading_tempo, true, true);
            end_fight(false);
            return;
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
                fancy_print("Hm... diese Richtung kenne ich nicht... probiers mal mit Norden, Osten, Süden oder Westen.\n", standard_reading_tempo, true, true);
                return;
            }
            fancy_print("In diese Richtung kannst du nicht weiter tauchen!\n", standard_reading_tempo, true, true);
        }
        else
        {
            if (!strcmp(attribute, "norden"))
            {
                for (int i = 0; i < 4; i++)
                {
                    if (rooms_directions[room_ind - 1][i] == 'n')
                        room_ind -= 10;
                }
            }
            else if (!strcmp(attribute, "osten"))
            {
                for (int i = 0; i < 4; i++)
                {
                    if (rooms_directions[room_ind - 1][i] == 'o')
                        room_ind += 1;
                }
            }
            else if (!strcmp(attribute, "westen"))
            {
                for (int i = 0; i < 4; i++)
                {
                    if (rooms_directions[room_ind - 1][i] == 'w')
                        room_ind -= 1;
                }
            }
            else if (!strcmp(attribute, "süden"))
            {
                for (int i = 0; i < 4; i++)
                {
                    if (rooms_directions[room_ind - 1][i] == 's')
                        room_ind += 10;
                }
            }
            else
            {
                fancy_print("Hm... diese Richtung kenne ich nicht... probiers mal mit Norden, Osten, Süden oder Westen.\n", standard_reading_tempo, true, true);
                return;
            }
            if (last_room == room_ind)
            {
                fancy_print("Dieser Weg steht dir nicht offen.\n", standard_reading_tempo, true, true);
            }
        }
    }
    else if (!strcmp(command, "tauche"))
    {
        if (biome[room_ind - 1] == MEER)
        {
            if (in_fight)
            {
                fancy_print("Du versuchst zu fliehen, jedoch vergeblich...\n", standard_reading_tempo, true, true);
                end_fight(false);
                return;
            }
            if (!strcmp(attribute, "auf"))
            {
                if (underwater)
                {
                    fancy_print("Du schwimmst zurück an die Wasseroberfläche.\n", standard_reading_tempo, true, true);
                    underwater = false;
                }
                else
                    fancy_print("Du bist schon oben, Transuse!\n", standard_reading_tempo, true, true);
            }
            else if (!strcmp(attribute, "ab"))
            {
                if (underwater)
                    fancy_print("Du bist schon unten!\n", standard_reading_tempo, true, true);
                else if (can_dive)
                {
                    for (int i = 0; i < sizeof(underwater_rooms); i++)
                    {
                        if (room_ind == underwater_rooms[i])
                        {
                            fancy_print("Du tauchst ab in die Tiefe des Ozeans.\n", standard_reading_tempo, true, true);
                            underwater = true;
                            underwater_room_ind = i;
                            break;
                        }
                    }
                    if (!underwater)
                        fancy_print("Hier ist es zu flach zum Abtauchen!\n", standard_reading_tempo, true, true);
                }
                else
                    fancy_print("Du tauchst ab, aber nach einigen Zügen geht dir die Luft aus, und du musst wieder auftauchen!\n", standard_reading_tempo, true, true);
            }
            else
                fancy_print("Du kannst entweder auf- oder abtauchen.\n", standard_reading_tempo, true, true);
        }
        else
            fancy_print("Du versuchst dich kopfüber in den Boden zu wühlen. Erst dann merkst du, dass du dich an Land befindest...\n", standard_reading_tempo, true, true);
    }
    else if (!strcmp(command, "schreie"))
    {
        fancy_print("AAAAAAAAAAAAAAAARGH!!!\n", standard_reading_tempo, true, true);
    }
    else if (!strcmp(command, "weine"))
    {
        fancy_print("Schluchz! Schnüff!\n", standard_reading_tempo, true, true);
    }
    else if (!strcmp(command, "nehme"))
    {
        if (in_fight)
        {
            fancy_print("Dafür ist keine Zeit!\n", standard_reading_tempo, true, true);
            return;
        }
        if (strlen(attribute))
        {
            if (biome[room_ind - 1] == MEER && !underwater)
            {
                fancy_print("Du schaust dich um, kannst ein solches Objekt jedoch nirgendwo finden. Vielleicht hast du mehr Glück am Grunde des Meeres...\n", standard_reading_tempo, true, true);
                return;
            }
            char buffer2[10];
            for (int i = 0; i < OBJEKTE_LEN; i++)
            {
                strcpy(buffer2, objekte[i]);
                buffer2[0] = tolower(buffer2[0]);
                if (!strcmp(attribute, "buch") && room_ind == 29)
                {
                    fancy_print("Das Buch ist angekettet, und du bist nicht in der Lage es aus der Verankerung zu reißen.\n", standard_reading_tempo, true, true);
                    return;
                }
                if (!strcmp(attribute, buffer2) && room_ind == objekte_loc[i])
                {
                    char buffer3[140];
                    if (backpack_size > 0)
                    {
                        sprintf(buffer3, "Du verstaust %.5s %.11s in deinem Rucksack.\n", objekte_artk2[i], objekte[i]);
                        fancy_print(buffer3, standard_reading_tempo, true, true);
                    }
                    else
                    {
                        sprintf(buffer3, "Dein Rucksack hat keinen Platz mehr für %.5s %.11s. Du überlegst dir, ob du etwas Unwichtiges auspacken solltest.\n", objekte_artk2[i], objekte[i]);
                        fancy_print(buffer3, standard_reading_tempo, true, true);
                        return;
                    }
                    add_item(i);
                    break;
                }
                if (i == OBJEKTE_LEN - 1)
                    fancy_print("Du schaust dich um, kannst ein solches Objekt jedoch nirgendwo finden.\n", standard_reading_tempo, true, true);
            }
        }
        else
        {
            fancy_print("Geht das noch etwas genauer?\n", standard_reading_tempo, true, true);
        }
    }
    else if (!strcmp(command, "lege"))
    {
        if (strlen(attribute))
        {
            char buffer2[10];
            for (int i = 0; i < OBJEKTE_LEN; i++)
            {
                strcpy(buffer2, objekte[i]);
                buffer2[0] = tolower(buffer2[0]);
                if (!strcmp(attribute, buffer2) && has_item(i))
                {
                    if (biome[room_ind - 1] == MEER && !underwater && !can_dive)
                    {
                        fancy_print("Du zögerst: Wenn du hier etwas ablegest, sinkt es hinab und du kannst es dir nicht mehr wiederholen!\n", standard_reading_tempo, true, true);
                        return;
                    }
                    char buffer3[100];
                    sprintf(buffer3, "Du greifst in deinen Rucksack legst %.5s %.11s ab.", objekte_artk2[i], objekte[i]);
                    fancy_print(buffer3, standard_reading_tempo, true, true);

                    if (i == BLUETE && room_ind == 98 && underwater)
                    {
                        remove_item(i, false);
                        end_fight(true);
                        return;
                    }
                    remove_item(i, true);
                    break;
                }
                if (i == OBJEKTE_LEN - 1)
                    fancy_print("Du durchwühlst deine Taschen, aber so ein Objekt findest du nicht bei dir.\n", standard_reading_tempo, true, true);
            }
        }
    }
    else if (!strcmp(command, "lache"))
    {
        fancy_print("Huahuahua!\n", standard_reading_tempo, true, true);
    }
    else if (!strcmp(command, "hilfe"))
    {
        fancy_print(hilfe, standard_reading_tempo, true, true);
    }
    else if (!strcmp(command, "suche") || !strcmp(command, "finde"))
    {
        fancy_print("Ich soll für dich suchen? Wer spielt denn hier, ich oder du?\n", standard_reading_tempo, true, true);
    }
    else if (!strcmp(command, "nutze") || !strcmp(command, "benutze") || !strcmp(command, "kämpfe"))
    {
        if (in_fight && !strcmp(attribute, "geist") && geist && room_ind == 18) // Sumpfmonster besiegen
        {
            if (has_item(FLINTE))
            {
                remove_item(FLINTE, false);
                end_fight(true);
                return;
            }
            else
            {
                fancy_print("Der Geist erscheint. „Hör mal, ohne Waffe kann auch ich hier nichts ausrichten!“ Verzweifelt blickst du deinem Ende entgegen...\n", standard_reading_tempo, true, true);
                end_fight(false);
                return;
            }
        }
        else if (in_fight && !strcmp(attribute, "flinte") && has_item(FLINTE)) // Flinte benutzen
        {
            fancy_print("Du spannst den Hahn, zielst und drückst ab. Der rostige Schießprügel birst mit einem gewaltigen Knall. Du spürst einen stechenden Schmerz im Kopf – und dann nichts mehr.\n", standard_reading_tempo, true, true);
            game = false;
            return;
        }
        else if (in_fight && !strcmp(attribute, "messer") && has_item(MESSER)) // Messer benutzen
        {
            switch (room_ind)
            {
            case FISCH_LOC:
                fancy_print("An den stahlharten Schuppen zerbricht die Klinge wie morsches Holz.\n", standard_reading_tempo, true, true);
                end_fight(false);
                break;
            case SUMPFMONSTER_LOC:
                fancy_print("Das Messer glitscht an der Haut des Monsters ab und rutscht dir in ein Sumpfloch. Verzweifelt siehst du es versinken.\n", standard_reading_tempo, true, true);
                end_fight(false);
                break;
            case TROLL_LOC:
                end_fight(true);
                break;
            }
        }
        else if (in_fight && !strcmp(attribute, "schild") && has_item(SCHILD)) //Schild benutzen
        {
            switch (room_ind)
            {
            case SUMPFMONSTER_LOC:
                fancy_print("Dein Schild bleibt in einem Sumpfgewächs hängen und wird dir aus der Hand gerissen. Schutzlos stehst du vor dem Untier.\n", standard_reading_tempo, true, true);
                end_fight(false);
                break;
            case TROLL_LOC:
                fancy_print("Mit einer lässigen Handbewegung wischt der Troll deine Deckung weg.\n", standard_reading_tempo, true, true);
                end_fight(false);
                break;
            case AMEISEN_LOC:
                end_fight(true);
                break;
            case FISCH_LOC:
                fancy_print("Unter Wasser kannst du den Schild nicht schnell genug bewegen. Es ist dem Fisch ein leichtes, deine Deckung zu umschwimmen.\n", standard_reading_tempo, true, true);
                end_fight(false);
                break;
            }
        }
        else if (!strcmp(attribute, "flasche") && has_item(FLASCHE))
        {
            fancy_print("Du entfernst den Siegellack mit den Zähnen aus der Flasche. Milchig-weißer Rauch schießt heraus der sich zu einer Gestalt formt. Es ist ein Geist! Mit einem Wutschrei schleudert er die Flasche weit weg. Nur eine Vorsichtsmaßnahme! „Ich hatte mal einen angeheirateten Vetter der... Übrigens, wenn du mal Hilfe brauchen solltest, wende dich an mich, ja? Du kannst mir aber auch was schenken, denn kleine Geschenke erhalten die Freundschaft.“\n", standard_reading_tempo, true, true);
            geist = true;
            remove_item(FLASCHE, false);
        }
        else if (!strcmp(attribute, "geist") && geist) // Geist-Aktionen
        {
            if (hilfe_zaehler == 3)
            {
                fancy_print("Der Geist erscheint und zuckt mit den Schultern. „Ich weiß leider keinen Rat mehr.“\n", standard_reading_tempo, true, true);
                return;
            }
            else
            {
                hilfe_zaehler++;
                fancy_print("Der Geist erscheint in einer Wolke.\n", standard_reading_tempo, true, true);

                if (room_ind == 98 && monster_defeated[FISCH])
                    fancy_print("„Ei? Das habe ich doch irgendwann schon mal gehört...“\n", standard_reading_tempo, true, true);
                else if (biome[room_ind - 1] == STRAND && objekte_loc[KAPPE] == 85 && room_ind != 85)
                    fancy_print("„Das Meer schwemmt vieles an...“", standard_reading_tempo, true, true);
                else if (biome[room_ind - 1] == SCHLUCHT && objekte_loc[KAPPE] == 49 && room_ind != 49)
                    fancy_print("„Es ist etwas in der Nähe mit dem du vielleicht dein Glück machen könntest...“\n", standard_reading_tempo, true, true);
                else if (room_ind == 36 && (objekte_loc[FLINTE] == 36 || has_item(FLINTE)))
                    fancy_print("„Eine Flinte ist eine gefährliche Waffe...“\n", standard_reading_tempo, true, true);
                else if (room_ind == 38)
                    fancy_print("„Tödliche Gefahr lauert in der Nähe...“\n", standard_reading_tempo, true, true);
                else if (biome[room_ind - 1] == HOEHLE && !can_dive)
                    fancy_print("„Höhlen bergen gefährliche Fallen ebenso wie zauberische Orte und unschätzbare Hilfen...“\n", standard_reading_tempo, true, true);
                else
                {
                    hilfe_zaehler--;
                    switch (rand() % 5)
                    {
                    case 0:
                        fancy_print("„Vielleicht findest du in der Nähe einen nützlichen Gegenstand?“", standard_reading_tempo, true, true);
                        break;
                    case 1:
                        fancy_print("„Du solltest dir gerade jetzt mehr Mühe geben!“", standard_reading_tempo, true, true);
                        break;
                    case 2:
                        fancy_print("„Ich habe jetzt keine Zeit für dich! Wir feiern in Schottland ein Familienfest!“", standard_reading_tempo, true, true);
                        break;
                    case 3:
                        fancy_print("„Selbst Geister wissen nicht immer Rat!“", standard_reading_tempo, true, true);
                        break;
                    case 4:
                        fancy_print("„Du langweilst mich mit deiner ewigen Fragerei!“ Und er gähnt herzhaft.\n", standard_reading_tempo, true, true);
                        break;
                    }
                }
            }
        }
        else if (!strcmp(attribute, "löffel") && has_item(LOEFFEL) && room_ind == 98 && underwater && !in_fight) // Ei zerstören
        {
            fancy_print("DU HAST GEWONNEN, HERZLICHEN GLÜCKWUNSCH!\n", slow_reading_tempo, true, true);
            game = false;
        }
        else if (!strcmp(attribute, "kappe") && has_item(KAPPE))
        {
            if (kappe_auf)
            {
                fancy_print("Du hast sie bereits auf dem Kopf.\n", standard_reading_tempo, true, true);
            }
            else
            {
                fancy_print("Du hast sie jetzt auf dem Kopf.\n", standard_reading_tempo, true, true);
                kappe_auf = true;
            }
        }
        else
        {
            if (!strlen(attribute))
                fancy_print("Nur ein Superman könnte so ein Untier mit den Händen erwürgen!\n", standard_reading_tempo, true, true);
            else
                fancy_print("Du hast dieses Objekt nicht oder kannst es momentan nicht benutzen!\n", standard_reading_tempo, true, true);
            if (in_fight)
            {
                idle_turns--;
                if (idle_turns > 0)
                {
                    fancy_print("Das Monster kommt immer näher. Dir bleibt nicht viel Zeit!\n", standard_reading_tempo, true, true);
                }
                else
                {
                    fancy_print("Du hast zulange gezögert und somit dein Ende besiegelt...\n", standard_reading_tempo, true, true);
                    end_fight(false);
                }
            }
        }
    }
    else if (!strcmp(command, "lese"))
    {
        if (strlen(attribute))
        {
            if (!strcmp(attribute, "buch") && room_ind == 29)
            {
                fancy_print(buchtext, standard_reading_tempo, true, true);
            }
        }
        else
        {
            fancy_print("Was denn bitte?\n", standard_reading_tempo, true, true);
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
                fancy_print(objekte_desc[i], standard_reading_tempo, true, true);
                break;
            }
            if (i == OBJEKTE_LEN - 1)
                fancy_print("Du hast dieses Objekt nicht!\n", standard_reading_tempo, true, true);
        }
    }
    else if (!strcmp(command, "trinke"))
    {
        if (biome[room_ind - 1] == MEER || biome[room_ind - 1] == STRAND)
        {
            fancy_print("Du schluckst das salzige Meerwasser und erbrichst dich, von Krämpfen geschüttelt.\n", standard_reading_tempo, true, true);
        }
        else if (biome[room_ind - 1] == HOEHLENQUELLE)
        {
            if (!can_dive)
            {
                fancy_print("Das Wasser rinnt kühl durch deine Kehle. Plötzlich hast du das Gefühl, dass dein Kopf platzt. Als du dich im Wasser der Schale betrachtest, springst du entsetzt zurück: Zwei riesige Kiemenbüschel sind dir gewachsen. Die Kopfschmerzen lassen jedoch nach und du fühlst dich erfrischt und tatendurstig.\n", standard_reading_tempo, true, true);
                can_dive = true;
            }
            else
                fancy_print("Repetita non placent!\n", standard_reading_tempo, true, true);
        }
    }
    else if (!strcmp(command, "klettere"))
    {
        if (!strlen(attribute))
        {
            fancy_print("Das war wohl nichts... gib das nächste Mal besser an, ob du hinauf oder hinab willst.\n", standard_reading_tempo, true, true);
            return;
        }
        if (!strcmp(attribute, "hinauf"))
        {
            switch (room_ind)
            {
            case 62:
                fancy_print("Du bist schon auf der Baumkrone, weiter nach oben geht es nicht!\n", standard_reading_tempo, true, true);
                break;
            case 53:
                fancy_print("Vorsichtig kletterst du an den Löchern hoch in die Baumkrone.\n", standard_reading_tempo, true, true);
                room_ind = 62;
                break;
            case 49:
                fancy_print("Die Böschung ist zu steil, um sie von dieser Seite aus zu ersteigen.\n",standard_reading_tempo,true,true);
                break;
            default:
                fancy_print("Hier gibt es nichts, wo du hinaufklettern könntest.\n", standard_reading_tempo, true, true);
                break;
            }
        }
        else if (!strcmp(attribute, "hinab"))
        {
            switch (room_ind)
            {
            case 64:
                fancy_print("Bedächtig hangelst du dich wieder hinab auf den Boden.\n", standard_reading_tempo, true, true);
                room_ind = 53;
                break;
            case 59:
                fancy_print("Vorsichtig steigst du die steile Böschung in die Schlucht hinunter.\n",standard_reading_tempo,true,true);
                room_ind = 49;
                break;
            default:
                fancy_print("Hier gibt es nichts, wo du hinabklettern könntest.\n", standard_reading_tempo, true, true);
                break;
            }
        }
    }
    else
    {
        fancy_print("Unbekannter Befehl. Gib \"Hilfe\" ein, um eine Liste von Befehlen zu erhalten.\n", standard_reading_tempo, true, true);
    }
}

//prints the room description
void print_description()
{
    if (biome[last_room - 1] != MEER && biome[room_ind - 1] == MEER)
        fancy_print("Langsam watest du ins Meer hinein.\n", standard_reading_tempo, true, true);
    else if (biome[last_room - 1] == MEER && biome[room_ind - 1] != MEER)
        fancy_print("Von Meerwasser triefend kommst du wieder an Land und lässt deine Kleidung in der Sonne trocknen.", standard_reading_tempo, true, true);

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

    fancy_print(desc, slow_reading_tempo, true, true);
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
                    fancy_print(underwater_descriptions[i], standard_reading_tempo, true, true);
                    break;
                }
            };
        }
        else
            fancy_print(rooms[room_ind - 1], standard_reading_tempo, true, true);
    }
    printf("\n");
}

//prints a random suffix
void print_zufallsanhaenge()
{
    switch (rand() % 9)
    {
    case 0:
        if (biome[room_ind - 1] != 'm' && biome[room_ind - 1] != 'a')
            fancy_print(zufallsanhaenge[0], standard_reading_tempo, true, true);
        break;
    case 1:
        if (biome[room_ind - 1] != 'm' && biome[room_ind - 1] != 's')
            fancy_print(zufallsanhaenge[1], standard_reading_tempo, true, true);
        break;
    case 2:
        if (biome[room_ind - 1] == 'w' || biome[room_ind - 1] == 'l')
            fancy_print(zufallsanhaenge[2], standard_reading_tempo, true, true);
        break;
    case 3:
        if (biome[room_ind - 1] != 'm' && biome[room_ind - 1] != 'h' && biome[room_ind - 1] != 'q')
            fancy_print(zufallsanhaenge[3], standard_reading_tempo, true, true);
        break;
    case 4:
        if (biome[room_ind - 1] != 'm' && biome[room_ind - 1] != 'h' && biome[room_ind - 1] != 'q' && biome[room_ind - 1] != 'a' && has_item(KAPPE))
        {
            fancy_print(zufallsanhaenge[4], standard_reading_tempo, true, true);
            remove_item(KAPPE, true);
        }
        break;
    case 5:
        if (biome[room_ind - 1] == 'm' && !underwater)
            fancy_print(zufallsanhaenge[5], standard_reading_tempo, true, true);
        break;
    case 6:
        if (biome[room_ind - 1] == 'm' && underwater)
            fancy_print(zufallsanhaenge[6], standard_reading_tempo, true, true);
        break;
    case 7:
        if (biome[room_ind - 1] == 'w')
            fancy_print(zufallsanhaenge[7], standard_reading_tempo, true, true);
        break;
    case 8:
        if (biome[room_ind - 1] == 'h')
            fancy_print(zufallsanhaenge[8], standard_reading_tempo, true, true);
        break;
    };
}

//on death
void death()
{
    fancy_print("\nGAME OVER\n", slow_reading_tempo, true, false);
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
            fancy_print("Du fühlst dich müde und ausgedörrt.\n", standard_reading_tempo, true, true);
            break;
        case 2:
            fancy_print("Die Zunge liegt dir geschwollen im ausgetrockneten Mund. Du kannst dich nur noch taumelnd weiterschleppen.\n", standard_reading_tempo, true, true);
            break;
        case 3:
            fancy_print("Du kannst nicht mehr und brichst zusammen. Langsam verlierst du das Bewusstsein.\n", standard_reading_tempo, true, true);
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
            fancy_print("Du setzt dich hin und ruhst dich im Schatten von den Strapazen der Schlucht aus.\n", standard_reading_tempo, true, true);
            break;
        case 3:
            fancy_print("Du hast es gerade noch rechtzeitig aus der Schlucht geschafft, lange hättest du nicht mehr durchgehalten.\n", standard_reading_tempo, true, true);
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
            fancy_print("Da du dich jedoch für die Klauenabdrücke interessierst, folgst du der ihrer Spur. Mit einem Mal weht dir heißer, fauliger Atem entgegen. Du hebst den Blick und ein Troll mit langem, verfilztem Fell und verzerrter Fratze wankt auf dich zu. Er beobachtet dich lauernd aus halb geschlossenen, tückischen Augen.\n", standard_reading_tempo, true, true);
            in_fight = true;
            idle_turns = 2;
            return;
        case 1:
            fancy_print("Der massige Körper des Trolls liegt hier auf dem Boden. Vorsichtig steigst du über ihn hinweg.\n", standard_reading_tempo, true, true);
            break;
        }
        break;
    case SUMPFMONSTER_LOC:
        switch (monster_defeated[SUMPFMONSTER])
        {
        case 0:
            fancy_print("Du bemerkst eine heftige Bewegung des Schilfs. Die Schilfwand spaltet sich und ein Wesen tritt heraus, dessen grässliche Erscheinung zu beschreiben der Computer sich sträubt. Grünen Schleim sabbernd, schwabbelt es auf dich zu.\n", standard_reading_tempo, true, true);
            in_fight = true;
            idle_turns = 2;
            return;
        case 1:
            break;
        }
        break;
    case AMEISEN_LOC:
        switch (monster_defeated[AMEISEN])
        {
        case 0:
            fancy_print("Du siehst genauer hin und würgender Ekel steigt in dir auf: Legionen von rubinroter Riesenameisen mit knackenden Kieferklammern strömen auf dich zu.\n", standard_reading_tempo, true, true);
            in_fight = true;
            idle_turns = 2;
            return;
        case 1:
            fancy_print("Der Ameisenhaufen wirkt verlassen, keine Spur mehr von den Riesenameisen.\n", standard_reading_tempo, true, true);
            break;
        }
        break;
    case FISCH_LOC:
        if (underwater)
        {
            switch (monster_defeated[FISCH])
            {
            case 0:
                fancy_print("Aus diesem Riff saust dir ein meterlanger, silbrig glänzender Fisch entgegen. In seinem Maul blinken Reihen von dreieckigen Zähnen.\n", standard_reading_tempo, true, true);
                in_fight = true;
                idle_turns = 2;
                return;
            case 1:
                fancy_print("Halb im Sand vergraben liegt hier ein weiß blinkendes, kleines Ei.\n", standard_reading_tempo, true, true);
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
                fancy_print(buffer, standard_reading_tempo, true, true);
            }
            else
            {
                char buffer[30];
                sprintf(buffer, ", %s %s", objekte_artk1[i], objekte[i]);
                fancy_print(buffer, standard_reading_tempo, true, true);
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
    }
    death();
    return 0;
}
