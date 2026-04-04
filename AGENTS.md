## Project overview

**Popis hry:**  
Hra bude prebiehať v 2 fázach. V prvej fáze si bude hráč vyrábať vlastné náboje tak, že bude prenášať rôzne útvary zo zásobovacej kopy do boxov. Na začiatku prvej fázy sa do zásobovacej kopy, čo je predom určený priestor, nahádže množstvo rôznych útvarov, s rôznym tvarom (kváder, guľa, ihlan, dvanásťsten, atď…), s rôznou farbou a váhou. Recepty budú požadovať x útvarov so špecifickou farbou a tvarom. Medzi útvarmi v zásobovacej kope budú aj “zbytočne útvary”, ktoré sa nedajú využiť v žiadnom recepte (napr. všetky sivé útvary) a žolíky, ktoré sa dajú využiť vo všetkých (ak by k danej kolekcii objektov prislúchalo viacero receptov, vybral by sa náhodný). Presúvanie objektov bude uskutočnené silovým poľom, kde útvary s väčšou váhou sa budú ťažšie prenášať. Jednou z hlavných mechaník bude, že vyrobené náboje sa budú pridávať do zásobníka ako na stack, t.j. prvý vystrelený náboj bude posledný vyrobený. V rámci prvej fázy plánujem pridať obchod, kde hráč bude môcť predať náboje z vrchu stacku za zníženú cenu a kupovať náboje na vrch stack-u. Okrem toho bude môcť kúpiť vylepšenia \- viac výrobných boxov, rýchlejšia výroba, menej nepoužiteľných útvarov, oprava základne (po útoku) a pod.

Po x minutách sa hráč presunie do druhej fázy. V tejto fáze bude mať za úlohu zničiť nepriateľov. Každý nepriateľ bude mať nad sebou indikátor, akým nábojom ho musí hráč zasiahnuť. Ak ho zasiahne iným, tak sa nepriateľovi nič nestane. Niektorí, náročnejší nepriatelia budú mať viacero indikátorov nad sebou \- v takom prípade ich treba zasiahnuť takými nábojmi v poradí, v akom sú zobrazené. Nepriatelia sa postupne budú približovať k základni, a keď prejdú cez hranicu, ktorá bude vyznačená v leveli, tak základňa stratí “životy” a nepriateľ sa odstráni z levelu. Ak základňa stratí životy, hráč prehráva. Okrem toho aj hráč bude mať životy, a ak stratí všetky, tak všetci zostávajúci nepriatelia v leveli automaticky akoby prejdú cez hranicu, čím sa základni adekvátne odoberú životy. Fáza 2 konči, keď sú zničený všetci nepriatelia, alebo základňa. Ak sa zničí základňa, hŕač prehráva. Cieľom hry bude prežiť čo najviac cyklov týchto dvoch fáz.

Plánujem do hry zapracovať viacero typov nepriateľov:

1. Najbežnejší typ sa bude iba približovať k základni, vyžadujúc jeden náboj
2. “Boss” typ \- obyčajný typ, vyžadujúci viac nábojov
3. Nepriateľ, ku ktorému ak sa hráč približí, tak niečo spraví, napr. zatemní hráčovi obrazovku / vybuchne / invertuje ovládanie …
4. Nepriatelia, ktorí zachovávajú za sebou stopu, do ktorej, ak hráč stúpi, tak ho to spomalí / dá dmg, atď…
5. Minion typ \- nejde k základni, ale následujú hráča a útočia na neho.
6. Summon typ \- vyvolá x minon typ nepriateľov, ktorý sa dajú buď zoztreliť, alebo sa vyparia po y sekundách

Taktiež chcem pridať do hry strom permanentných schopností. Po zničení dostatočného počta nepriateľov bude hráč získavať body, za ktoré si bude vedieť odomykať / vylepšovať nasledujúce skúsenosti:

* Lvl 1: Hráč získa informáciu o počtoch nábojov, ktoré bude musieť vyrobiť. Lvl 2: Hráč získa informáciu o približnom poradí, v akom budú prichádzať nepriatelia
* Zastav x najbližších nepriateľov na dobu y sekúnd, cooldown z sekúnd. x, y a z závisia od levelu skill-u
* Na dobu x sekúnd obráť zásobník, cooldown y sekúnd. x, y od levelu
* Získaj o x percent viac peňazí z predaja nábojov.
* Získaj o x percent vyššiu rýchlosť. x od levelu
* Vystreľ x nábojov. Náboje zasiahnú korešpondujúcich nepriateľov, ak sú v okruhu y jednotiek.
* Vrchný náboj sa správa ako žolík, cooldown x sekúnd.

**Technické kritéria:**

1. Prepínanie sa medzi levelmi
2. Vypisovanie informácií v HUD
3. Herný objekt ovládaný hráčom
4. Herný objekt ovládaný jednoduchou umelou inteligenciou
5. Jednoduchý súbojový systém
6. Strom permanentných schopností
7. Jednoduché obchodovanie
8. Grafické používateľské rozhranie (GUI)
9. Zaujímavá fyzika nad ramec enginu
10. Dynamický systém hudby (možno)
11. Dynamické prostredie (možno)
12. Dešktrukcia objektov (možno)