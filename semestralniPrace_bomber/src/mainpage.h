/** @mainpage zadání

@authors Oldřich Macháček

@section Bomberman

========= zadání =========

Implementujte následující varianty:
    - pro alespoň 2 hráče na jednom počítači
    - pro hru proti počítači

Hra musí splňovat následující funkcionality:
    - Načítání herních levelů (mapy) ze souboru (vytvořte vhodný formát)
    - Sbírání konfigurovatelných bonusů (zvětšení plamenu, více bomb, odpalování bomb na dálku,...)
    - Ukládání nejlepšího dosaženého skóre do souboru

Kde lze využít polymorfismus? (doporučené)
    - Ovládání hráčů: lokální hráč, umělá inteligence (různé druhy), síťový hráč
    - Uživatelské rozhraní: konzolové, ncurses, SDL, OpenGL (různé druhy),...
    - Efekty bonusů a nemocí: zvětšení plamenu, zvýšení počtu bomb,...

====== specifikace =======

hráč(i) splupracují (mohou si ovšem i škodit a navzájem se zabíjet) a snaží se zabít co nejvíc nepřátel a přežít co nejdelší dobu, zároveň mohou sbírat různé a různě silné bonusy, zabíjet mohou pouze bombami, ale je může zabít nejen nepřítel, ale i výbuch

hra běží v nekonečné smyčce dokud jeden z hráčů nezemře, tím hra končí. Hra běží i v případě že hráč nezadal žádný input, jinými slovy hra nečeká na stisk klávesy, běží i bez něj, zároveň se hráč v jednom ticku může posunout maximálně o 1 pole, ale stejně tak i nepřítel

====== implementace ======

naimplementované jsou následující varianty:
    - hra pro 1 hráče
    - hra pro 2 hráče
    - v obou variantách hrají hráči společně proti počítači

selekce herní varianty a mapy

načítání mapy ze souboru ve formátu
	šířka výška
	samotné objekty
	-   #	= nezničitelná zeď
	-	*	= zničitelná zeď
	-	' '	= prázdné místo
	-	E	= soupeř (obluda)
	-	B	= bomba
	-	cifra	= bonus (1 = range upgrade, 2 = max bomb count upgrade)
	-	ostatní = prázdné místo

error loging do souboru log.txt

sbírání bonusů (zvětšení dosahu plamenu, více bomb na hráče)
    - bonusy mají 3 tiery

ukládání nejlepšího dosaženého skóre do souboru (assets/record.txt)

pohyblivé objekty:
	ovládání hráče - klávesnice
		hráč 1: pohyb - šipky, pokládání bomby - 0
		hráč 2: pohyb - wasd, pokládání bomby - ' '
	soupeř (různé tiery "chytrosti")
		tier 1: soupeř se nehýbe
		tier 2: soupeř má 50% šanci na náhodný krok (nešlape do explozí a pokud je vedle hráče, zašlápne ho)
		tier 3: soupeř má 70% šanci na krok a jde na políčko, které je hráči nejblíž (nehledá cestu, pouze spočítá geometrickou vzdálenost)

uživatelské rozhraní: konzolové

efekty bonusů: zvětšení plamenu (větší dosah bomby), zvýšení počtu bomb

===== polymorfismus ======

v celém provedení jsou 3 třídy (Menu, Game a Map) bez hierarchie

dále je zde "superclass" Object, která shromažďuje všechny možné bloky na mapě a sama reprezentuje "vzduch"(/aneb prázdné místo) nebo explozi
tato třída má 4 podtřídy:
    - Bomb (jednoduchá třída reprezetující bomby)
    - Wall (ještě jednodušší třída reprezentující zničitelné i nezničitelné zdi)
    - Buff (třída reprezentuje herní buffy)
    - a Movable, tato třída má další 2 podtřídy:
        - Enemy (třída reprezentující nepřítele)
        - a Player (třída reprezentující hráče)

hlavní využití polymorfizmu je pro pohyb movable objektů po mapě a pro vykreslování jednotlivých objektů

jednou virtual metodou je move ve třídě Movable, kde je bez implementace a pro každou movable podtřídu tím pádem bude muset být naimplementována zvlášť, což mi přijde správně, protože každý objekt se pohybuje jinak
druhou virtual metodou je metoda print ve třídě Object, zde již implementována je, protože prázdé pole se též musí umět vykreslit
*/