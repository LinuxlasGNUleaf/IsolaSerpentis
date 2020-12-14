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

char intro1[] = "Du befindest dich an Bord deines Fischerbootes auf hoher See. Während deine zum Trocknen aufgehängte Netze in der Abendbrise schaukeln, liegst du im Bug deines Schiffes, schaust in die untergehende Sonne und denkst an dein zu Hause zurückgebliebene Familie. Die Wellen schlagen leise an den Schiffsrumpf. Allmählich fühlst du, wie du von einer wohligen Müdigkeit umfangen wirst. In einen tiefen, traumlosen Schlaf hinübergleitend, hörst du noch das leise Säuseln des Windes...\n";
char intro2[] = "Du versuchst deine Augen zu öffnen, musst sie aber sofort geblendet wieder schließen. Du fühlst dich wie zerschlagen. Die Augen mit der Hand bedeckend blinzelst du in die gleißende Sonne und versuchst mühsam, dich aufzurappeln. Taumelnd kommst du wieder auf die Beine und blickst dich verstört um:\n";
char intro3[] = "Du stehst auf einem weiten Strand der aus Myriaden von kleinen violett glänzenden Muschelschalen besteht. Er setzt sich nach Osten und Westen fort. Einige zehn Meter im Süden siehst du das spiegelglatte Meer. Nördlich von dir ragt die dichtgeschlossene Wand eines türkisgrünen Waldes zum glühenden Himmel auf. Nur an einigen Stellen scheinen die Lianen einen schmalen Durchgang zu gewähren.\n";

char half_breakers[] = ",;";
char breakers[] = ".:";

char rooms[100][712] = {\
"Auch hier findet sich ein Pfeil. Er zeigt jedoch nach Osten. Ein weiterer Gang liegt im Süden.",
"„Reingefallen!“ schallt dir entgegen. Verwundert siehst du dich um. Von woher kann die Stimme gekommen sein? Überall schimmern die grünen Wände, und von Westen bist du ja gekommen. Sollte dir jemand (oder etwas) gefolgt sein?",
"Bumms! In deinem Eifer bist du vor die den Gang abschließende Wand gelaufen. Stöhnend reibst du dir den schmerzenden Schädel, als ein Irrlicht aus der Wand rollt. Langsam tanzt es vor deinen Augen hin und her und löst sich schließlich auf...",
"Der von Osten kommende Gang, in dem du stehst, setzt sich nach Westen fort.",
"Der von Süden kommende Gang gabelt sich hier nach Westen und Osten. Du musst dich entscheiden...",
"Du gelangst in ein totes Ende des von Westen kommenden Ganges. Plötzlich fährst du zusammen! Dröhnendes Gelächter erklingt, läuft durch die Gänge und bricht sich in unzähligen Echos.",
"",
"",
"",
"",
"In die westliche Wand ist ein Pfeil eingemeißelt, der in den Gang nach Norden weist. Hat ein freundliches Wesen dir einen Fingerzeig gegeben, oder ist es eine tödliche Falle? Du hast freilich auch die Möglichkeit, nach Süden oder Osten zu gehen.",
"Du gehst durch eine Ost-West-Passage, die noch durch den Schmutz des Monsters gekennzeichnet ist.",
"Du watest durch eine zentimeterdicke Schicht von Staub, Lumpen, Knochen und Abfall. Angeekelt blickst du dich um. In der Mitte des Ganges steht ein aus Knochen verfertigter Tisch mit einem ebensolchen Stuhl. Auf dem Tisch liegen Reste angefaulter Fleischbrocken, die einen abstoßenden Geruch verströmen. Im Moder auf dem Fußboden kannst du Klauenabdrücke erkennen, deren Größe dich erschreckt. Mit Ausnahme von Norden, wo dir eine wand den Weg versperrt, kannst du in alle Richtungen gehen.",
"Du befindest dich in einem toten Ende, das mit Beuteüberresten schrecklich übersät ist. Vielleicht war es die Speisekammer des Monsters?",
"Du befindest dich in einem Nord-Süd-Gang, der nach Osten abzweigt. Von irgendwoher hörst du das monotone Tropfen von Wasser.",
"Aus den Wänden wachsen bizarre Kristalle von erstaunlicher Größe hervor. Du versinkst in ihrem Anblick, reißt dich aber schließlich wieder los und schlägst entweder den Weg nach Westen oder den nach Süden ein.",
"",
"Auf dem Boden stehen Hunderte von grellbunten Blumen, deren widerlicher Gestank sich mit dem der Faulgase vermischt und dir fast den Atem raubt. Mit Übelkeit kämpfend, kommst du jetzt erst dazu dich umzudrehen. Du siehst im Norden und Westen einen dichten Schilfgürtel. Das Schilf hat messerscharfe Blätter. Du berührst einen von ihnen und ein Bluttropfen quillt hervor.",
"Ein schauriges Heulen dringt an dein Ohr, Es klingt wie das Jammern und Schluchzen von verlorenen Seelen. Du erschauderst. In Gedanken versunken stößt du beinahe an die gewaltige Felsplatte im Süden und siehst dich jetzt nun auch um. Im Westen erstreckt sich das schaurige Moor. Im Osten wallt der Nebel.",
"Nebelschwaden umhüllen dich. Du verlierst vollkommen die Orientierung. Wo liegt der rettende Weg, wo lauert das Verderben?",
"Hier ist wieder ein Pfeil zu sehen. Willst du ihm nach Norden folgen oder in östlicher Richtung weitergehen?",
"Du stehst vor zwei Wänden im Norden und Osten. Ein Gang führt nach Westen. Im Süden lastet ägyptische Finsternis, aus der du das leise Rauschen von Wasser zu hören ist.",
"Von Norden her dringt süßlicher Verwesungsgeruch in deine Nase. Du kannst ihm nachgehen oder den Weg nach Osten einschlagen.",
"Ein Weg führt nach Westen und Osten. Du vermeinst, ferne Musik zu hören.",
"Die Wände strahlen ein weiches, grünes Leuchten aus. Du stehst in einem abgeknickten Gang, der von Norden kommt und nach Westen weiterführt. Nach Süden trittst du hinaus auf die Wiese.",
"Du merkst jetzt, dass du in eine Sackgasse gekommen bist. Von fern her hörst du verhaltenes Kichern und das Leuchten der Wände flackert höhnisch. Dir bleibt nur der Weg nach Norden.",
"",
"Der üble Geruch von Moorfaulgasen beißt dir in die Nase. Um dich herum siehst du Gas blasen mit einem gedämpften „Plopp“ hervorbrechen. Drei Meter weiter im Westen beginnt die grünlich-schillernde Oberfläche eines Tümpels, dessen ekelhaft stinkender Schlamm dich abstößt. Dein Blick nach Osten zeigt dir ein hoch aufragende Steinplatte, die du jedoch nicht erklettern kannst.",
"Du arbeitest dich mühsam einige roh ausgehauene Steinstufen hinauf. Der Nebel wird hier oben lichter. Du erkennst nun, dass du auf einer großen Steinplatte stehst. Zu deinen Füßen liegt ein in dickes Leder eingebundenes Buch. Sein Rücken ist mit goldenen Ornamenten verziert. Es ist mit einer silbernen Kette an den Fels angeschlossen. Nach Norden, Westen und Süden fällt die Platte steil ab.",
"Du bist völlig im Nebel gefangen. Plötzlich siehst du aus dem Augenwinkel ein Licht aufblitzen. War es im Norden? Im Westen? Oder vielleicht im Osten oder Süden? Du weißt es nicht...",
"",
"Du gehst durch einen engen Durchgang und bleibst wie gebannt stehen. In der Dunkelheit schwebt eine dunkelrot glühende Schale, in die von irgendwoher ein Wasserstrahl hineinplätschert. Das rote Licht bricht sich in den sprühenden Wassertropfen und lässt sie zu glitzernden Edelsteinen werden. Allmählich erkennst du, dass du dich in einem kleinen Raum befindest. Aus der gegenüberliegenden Wand schießt der Wasserstrahl hervor. Über ihm kannst du in goldenen Lettern lesen: „Aqua Spirantis“",
"Vorsichtig jeden Stein mit dem Fuß prüfend, tastest du dich einige Meter in die Geröllfläche hinein. Ein warnendes Rumpeln hinter dir lässt dich erstarren. Als du herumschnellst, lösen sich die Steine unter deinen Füßen. Sie fahren erst langsam, dann immer schneller zu Tal und reißen auf ihrem Weg andere mit sich. Wild schreiend saust du im Strom der Steine mit. Rasender Schmerz ergreift dich, als ein großer Granitklumpen deinen Schädel trifft. Rote Ringe kreisen vor deinen Augen, die aber schnell von einem alles beherrschenden Schwarz ausgelöscht werden...",
"Hier liegen viele graufleckige Steine im Gras. Als du nach Westen siehst, erkennst du den Grund: eine Geröllhalde führt bergauf. Du kannst sie jedoch wegen der drohenden Gefahr eines Steinschlags nicht betreten. Stattdessen führt im Süden ein Weg bergauf. Im Norden steigt eine smaragdgrüne, glatte Wand senkrecht in den Himmel. Im Osten liegt ein weiterer Teil der Wiese.",
"Buntschillernde Schmetterlinge taumeln von Blüte zu Blüte, um Nektar zu finden. Erinnerungen an deine Kindheit ergreifen dich. Ob es deinen Lieben wohl ebenso gut geht? Gen Norden blickend siehst du eine grüne Steinwand mit einem schmalen Loch, ungefähr in 3 Fuß Höhe. Ein Mensch könnte sich vielleicht hindurchzwängen. Ansonsten breitet sich die Wiese um dich aus wie ein Teppich.",
"Die Pflanzen scheinen hier kürzlich niedergetrampelt worden zu sein, als ob ein Wesen von ungeheurer Größe hinübergegangen sei. Aber wo sollte es hergekommen sein, da sich doch sonst keine Spuren zeigen? Im Norden liegt eine grüne Steinwand, von dort kam es also nicht! Geben dir vielleicht die Wiesenstücke in der anderen Richtung Aufschluss?",
"Zwischen dem Gras und Blumen siehst du große, von Moos bedeckte Stellen. Nach Osten hin verbinden sich diese Flecken zu einer geschlossenen Fläche. Im Norden liegt gar ein Tümpel, der dich mit seiner schlammigen Oberfläche an die todbringenden Sümpfe deiner Heimat erinnert. Im Süden fallen die Steilwände der Schlucht ab. Es bleibt noch der Weg nach Westen, zurück auf die Wiese.",
"Dieses Gebiet stellt den Übergang zwischen der Wiese im Westen und dem Moor im Norden und Osten dar. Der Boden schwankt sachte unter deinen Füßen. Er ist von einer Vielzahl kleiner, moosartiger Pflänzchen bedeckt. Im Süden kannst du auf die kahle Schluchtsohle herabsehen.",
"Du siehst nach Osten und siehst eine grün-goldene Ballonflasche auf dem Wasser eines Sumpftümpels treiben. Sie ist mit einem verrotteten Korken verschlossen und enthält ein altes Pergament, das vielleicht eine geheime Botschaft enthält...? Während du sie gespannt beobachtest, achtest du nicht auf den Weg. Plötzlich stolperst du und fällst kopfüber in den Schlammtümpel. Zäher Schlamm, mit schleimig-schmieriger Algen durchsetzt, dringt in deine Lungen ein. Verzweifelnd ringst du nach Luft, verlierst aber schnell das Bewusstsein...",
"",
"",
"",
"Du befindest dich auf einem schwarzen Felsplateau. Die Aussicht ist überwältigend. Unter dir im Westen erstreckt sich Urwald bis zum Horizont. Nach Osten siehst du den Felsbrocken, der dir so bedrohlich vorkam. Weiter hinten liegt die Wiese im Tale. Im Norden geht es einen mit Geröll bedeckten Hang bergab, der in leichten Kurven auf die Wiese im Nordosten führt. Er lässt sich allerdings nicht begehen ohne einen Steinschlag auszulösen. Über dir im grenzenlosen Blau des Himmels schweben einige kleine Punkte ziellos dahin.",
"Du folgst dem natürlichen Pfad an der Bergwand im Westen, die bedrohlich überhängt. Du siehst nach oben und erblickst einen dicken Felsbrocken, der gerade noch auf der Kante des Berges ruht. In der Wand wachsen vereinzelt Blümchen. Nach Osten hin geht es steil auf die Wiese hinab. Du musst dich entscheiden, ob du nach Norden auf die Wiese zurückgehst oder im Süden dem Verlauf des Pfades bergauf folgst.",
"Im Westen ragt ein Berg auf, an dessen Wand sich in halber Höhe ein Pfad entlangschlängelt, der von Norden nach Süden verläuft. Im Süden erblickst du in der Schlucht eine blassgrüne Pflanze mit roten Blüten. Nördlich und östlich von dir setzt sich die Wiese fort.",
"Du gehst über eine Sommerwiese. Der Boden federt unter deinen Füßen leicht. Die Wiese ist übersät mit Kräutern und Blumen, die du noch nie in deinem Leben gesehen hast. In die Schlucht im Osten führt ein schmaler Weg. Im Süden kannst du nicht in sie herabsteigen. Die Wiese erstreckt sich in die anderen Richtungen.",
"Die aus dem Osten kommende Schlucht macht hier einen Knick nach Süden. Im Westen führt ein überhängender Grat an einer Seite empor. Die Wand im Norden glimmt im Sonnenglast.",
"Du stehst auf dem Boden einer Schlucht, die in Ost-West-Richtung verläuft. Die Luft ist staubtrocken und glühend heiß. Du fühlst dich etwas schwindlig. Vielleicht solltest du doch besser im Süden den Wald hochklettern.",
"Der einzige Ausgang liegt im Westen. Alle anderen Richtungen werden durch senkrecht hochsteigende Felswände begrenzt. Unter einer Felsscholle wartet ein großer, schwarzer Skorpion bewegungslos auf Beute. Vorsichtshalber wagst du dich nicht näher heran.",
"",
"",
"",
"Vor dir ragt ein mächtiger alter Baumriese mit rauer, rissiger Rinde auf. Du bist erstaunt, in dieser kargen Gegend eine so große Pflanze zu finden. Bei näherer Betrachtung findest du einige ausgehauene Löcher im Stamm, die vielleicht als Tritte gedient haben könnten...? Von hier aus hast du einen weiten Ausblick: Im Süden und Westen liegt tief unter dir dichter Urwald. Der von Osten kommende Pfad macht hier einen Knick nach Norden.",
"Hier stehst du vor einem Abgrund im Süden. Unter dir schießt aus einem Loch in der Felswand mit lautem Brausen ein mächtiger Fluss hervor. Im Osten schaust du in die tiefe Schlucht auf deren Grund du grau-grünen Pflanzenwuchs mit roten Flecken erkennen kannst. Es bleibt dir nur der Weg bergab nach Norden oder bergauf nach Westen, wo du die Umrisse eines Baumes ausmachen kannst.",
"Hier hast du das Westende der Schlucht erreicht, das sich in Nichts von den anderen Teilen der Schlucht unterscheidet – abgesehen von dem riesigen Feigenkaktus, der vor dir in den Himmel ragt. Er ist mit dolchartigen Stacheln übersät. Hoch oben siehst du einige Blüten, aus denen giftig aussehender Nektar tropft.",
"Die Schlucht verläuft schnurgerade in West-Ost-Richtung. In Westen siehst du eine gigantische Pflanze mit roten Kelchblüten. Ansonsten lässt sich nicht besonderes erkennen.",
"Aus einer Felsspalte starrt dich ein Vipernkopf mit kalten Reptilienaugen an. Bei deinem Näherkommen verschwindet sie in ihrem Loch. Trotzdem ist dir dieser Ort nicht ganz geheuer. Du solltest besser nach Norden oder Westen gehen. Eine andere Möglichkeit bleibt dir nicht.",
"Dieses Waldstück wird an zwei Seiten durch eine tiefe Schlucht begrenzt; höchstens im Norden gibt es vielleicht eine Möglichkeit, hinabzusteigen. Im Süden und Westen geht es wieder in Wald hinein.",
"Dieser Teil des Waldes wirkt freundlicher als die anderen, da die Baumkronen mehr Licht durchlassen. Das Unterholz ist deshalb so dicht, dass du nicht nach Osten gehen kannst. Der Wald geht nach Westen und Süden weiter. Im Norden liegt die Schlucht, an deren Wand du jedoch einige Vorsprünge entdeckst, sodass du vielleicht hinabsteigen kannst.",
"",
"",
"Die Steiglöcher führen zu einem kleinen Baumhaus, das dicht an den mächtigen Stamm angeschmiegt ist. Die Krone des Baumes bildet das Dach. Du trittst ein. Die Möbelstücke, ein Bett, ein Tisch mit einem Stuhl und ein kleiner Hocker sind wahllos über den Raum verstreut und umgestürzt. Hat hier ein Kampf stattgefunden?",
"",
"",
"Du gehst durch den Wald und betrachtest die fremdartigen, überall wuchernden Pilze. Als du einen von ihnen leicht berührst, quillt eine Woche grauer Sporen hervor. Sie verfliegt langsam und plötzlich siehst du einen gigantischen Ameisenhaufen, der dir nur den Weg nach Osten freilässt. Du siehst genauer hin und würgender Ekel steigt in dir auf: Legionen von rubinroter Riesenameisen mit knackenden Kieferklammern strömen auf dich zu.",
"Du bahnst dir deine Weg durch den Urwald und bewunderst die wunderschönen Orchideen, die von den Bäumen herabhängen. Der Wald setzt nach Osten und Westen fort. Im Norden fällt der Boden steil zu einer Schlucht ab. Im Süden schimmert zwischen den Bäumen der cyanblaue Strand hervor.",
"Du gehst durch den Wald und zerteilst die Lianenvorhänge mit den Händen. Große, fremdartig anmutende Insekten schwirren um dich herum. Im Norden blickst du in eine steile Schlucht. Nach Osten und Westen geht es tiefer in den Wald. Im Süden betrittst du einen Strand.",
"Plötzlich stehst du vor einer riesigen fleischfressenden Pflanze, die ihre klebrigen Tentakel nach dir ausstreckt. Mit Mühe gelingt es dir, sie zu umgehen. Die Pflanze macht dich darauf aufmerksam, dass es in diesem Wald auch noch andere Gefahren geben kann... Im Süden leuchtet es orange, auf den anderen drei Seiten liegt Wald.",
"Hier stehen weniger große Bäume, dafür wuchert das Unterholz um so üppiger und versperrt dir den Weg nach Osten. Nach Süden geht es auf den Strand. Im Westen und Norden ist dichter Wald.",
"",
"",
"",
"",
"",
"Ein scheinbar verlassener Ameisenhaufen türmt sich im Norden vor dir auf. Im Westen braust ein Fluss, sodass dir nur der Weg nach Osten und Süden auf den Strand freibleibt.",
"Die cyanblauen Muschelschalen unter deinen Füßen knirschen leise. Nach Osten hin setzt sich der Strand fort, im Süden braust der Ozean. Im Norden und Westen rauscht der unergründliche Wald.",
"Vor dir in den violetten Muscheln siehst du halb verschüttete Fußstapfen. Vielleicht bist du nicht das einzige menschliche Wesen hier? Der Strand erstreckt sich nach Ost und West. Im Süden liegt der Spiegel des Meeres, im Norden wie eine Mauer der Wald.",
"Hier haben die Muscheln eine leuchtend orange Färbung. Im Norden liegt der Wald, im Süden das Meer, im Osten und Westen weiterhin der Strand.",
"Hier gehst du auf kornblumenblauen Muscheln. Zwischen ihnen liegen riesige weiße Schneckenhäuser, die wie die Sterne am Firmament leuchten. Du blickst nach Osten und siehst ein großes, mit Sand bedecktes Gebiet, dass du zum Glück noch rechtzeitig als Treibsand erkennst. Im Süden und Westen liegen weitere Strandstücke, im Norden trittst du in den Wald hinein.",
"Du betrittst die trügerische Sandoberfläche und panischer Schrecken ergreift dich. Verzweifelnd zappelnd, sinkst du immer tiefer in den Sand ein, der dich wie mit eisernen Klammern festhält. Du erhebst noch einmal den Blick zum Himmel, bevor sich die Sandoberfläche endgültig über dir schließt...",
"",
"",
"",
"",
"Der Strand hat hier eine ziegelrote Färbung. Du musst aufpassen, um den vielen Wasserlachen auszuweichen, die ihn bedecken, Im Osten versperrt dir ein reißender Fluss den Weg, der seiner Mündung zutreibt, südlich liegt eine Strandfläche, hinter der sich Klippen erheben. Im Westen ist das Meer, das dich mit seinem Rauschen geheimnisvoll anzieht. Nördlich von dir beginnt der Wald.",
"Im Norden und Westen liegt der Strand.",
"Im Norden liegt der Strand.",
"Im Norden und Osten liegt der Strand.",
"Auf diesem Teil des Strandes scheine die Muscheln zu glühen. Voller Erstaunen hebst du eine Handvoll auf und stellst fest, dass dein Eindruck nur von der pulsierenden Rotfärbung herrührt. Du siehst dich im Westen und Süden von Meer umgeben. Im Osten machst du die trügerisch schimmernde Fläche von Treibsand, im Norden ein weiteres Strandstück aus.",
"Du betrittst die trügerische Sandoberfläche und panischer Schrecken ergreift dich. Verzweifelnd zappelnd, sinkst du immer tiefer in den Sand ein, der dich wie mit eisernen Klammern festhält. Du erhebst noch einmal den Blick zum Himmel, bevor sich die Sandoberfläche endgültig über dir schließt...",
"",
"",
"",
"",
"Der zitronengelbe Strand ist hier stark begrenzt; im Osten durch die Mündung des Flusses, im Süden durch die roten Granitklippen, deren Besteigung ein lebensgefährliches Wagnis wäre. Lediglich im Westen, wo sich das Meer erstreckt, und im Norden, wo sich der Strand fortsetzt, ist der Weg frei.",
"Im Westen liegt der Strand, im Süden versperrt dir ein Korallenriff den Weg.",
"Im Süden ragt ein scharfkantiges Korallenriff hoch.",
"Im Süden versperrt dir ein unbesteigbares Riff den Weg.",
"Das Korallenriff versperrt dir den Weg nach Süden und Osten. Im Norden liegt der Strand.",
""};

char rooms_directions[100][4] = {\
"os",
"w",
"o",
"wo",
"wos",
"w",
"",
"",
"",
"",
"nos",
"wo",
"wos",
"w",
"nos",
"ws",
"",
"so",
"wo",
"ws",
"no",
"sw",
"no",
"wo",
"nws",
"n",
"",
"ns",
"o",
"nw",
"",
"n",
"",
"wso",
"nosw",
"osw",
"wo",
"wno",
"",
"",
"",
"",
"ns",
"nos",
"wno",
"wno",
"wos",
"wos",
"w",
"",
"",
"",
"no",
"nw",
"o",
"wo",
"nw",
"nos",
"nws",
"",
"",
"",
"",
"",
"o",
"wos",
"wos",
"nosw",
"nws",
"",
"",
"",
"",
"",
"os",
"nosw",
"nosw",
"nosw",
"nosw",
"",
"",
"",
"",
"",
"nos",
"nosw",
"nosw",
"nosw",
"nosw",
"",
"",
"",
"",
"",
"no",
"wno",
"wno",
"wno",
"wn",
""};
