% Editor konfigurace Disam RT SCADA
%
%

Účel
====

Editor slouží k\ vizuální editaci konfiguračních souborů pro systém Disam RT SCADA. Nativním formátem je XML specifikace datového souboru. Pro zpětnou kompatibilitu editor podporuje otevření i starších datových souborů „src“. V\ obou případech (datové XML a „src“) je vyžadován příslušný konfigurační XML soubor. Tento konfigurační XML soubor slouží pro definici kategorií a typů jednotlivých položek v\ daném datovém souboru a je nutné ho vytvořit ručně předem (viz. `/usr/share/scada_editor/cfgview.xml`).

![Hlavní okno po otevření „src“ souboru](img/2013-12-11-154903_1680x1050+0+0_imlib2_grab-s.jpg)

Spuštění
========

Základní spuštění lze provést takto:

    cd /path/to/install/dir && ./scada_editor

Ke\ správné funkčnosti je zapotřebí skript `src2xml.sh`, jež zpracovává vstupní (otevírané soubory) jak ve formátu XML, tak v\ „src“. Tento se buď musí nacházet v\ `$PATH`, v\ aktuálním adresáři (`man 1 pwd`), nebo lze předat editoru parametr `-p /cesta/k/src2xml.sh`.

Editor uživatele neobtěžuje výstražnými dialogy ani méně významnými chybovými zprávami. Tyto jsou vypisovány na `stdout` a `stderr`. Je tedy možné tyto výpisy inkrementálně zálohovat např. takto:

    scada_editor >> "$HOME/muj_log" 2>&1

V\ případě otevírání datového souboru „src“ se lze setkat s\ výpisem

    WARN [12]: constraint ` (0/1)' found in comment starting at line 8
        such constraints are suitable for cfgview.xml

nebo jemu podobným, který pouze upozorňuje tvůrce konfiguračního XML souboru na typovou kontrolu vyskytující se datovém souboru v\ „src“.

![Výstup editoru na stdout/stderr](img/2013-12-11-154833_1680x1050+0+0_imlib2_grab-s.jpg)

Uživatelské rozhraní
====================

Editor umožňuje editaci pouze jednoho souboru. Hierarchie položek se zobrazuje v\ levém sloupci (stromový pohled) a samotné datové položky v\ tabulce napravo. Ovládání je zajištěno pomocí menu `File` a nástrojové lišty, jejíž tlačítka mají detailnější popisek v\ *tooltipu*.

Menu 'File'
-----------

`New file` slouží k\ vytvoření nového konfiguračního souboru. Po zvolení se objeví dialog pro výběr konfiguračního XML souboru. Výsledný datový XML soubor bude obsahovat referenci na zvolený konfigurační XML soubor a tento se při další manipulaci s datovým XML souborem vždy zvolí automaticky.

`Open` otevře již XML nebo „src“ datový soubor za pomocí dialogu pro výběr.

`Save` uloží právě editovaný datový soubor do formátu XML. Pokud není známo kam soubor uložit, zobrazí se dialog pro výběr názvu a umístění datového XML souboru.

`Save as` uloží právě editovaný datový soubor do formátu XML, přičemž vynutí zobrazení dialogu pro výběr názvu a umístění.

`Export to src` exportuje právě editovaný datový soubor do staršího formátu „src“. Po zvolení se vždy zobrazí dialog pro výběr názvu a umístění datového souboru „src“.

`Quit` okamžitě ukončí editor (nezobrazí žádný dialog typu „Opravdu chcete ukončit práci? Máte rozpracovaná, neuložená data.“).

Nástrojová lišta s\ tlačítky
----------------------------

### Práce se stromem

`Rename selected tree item` zobrazí dialog pro přejmenování označené položky, pokud tuto lze přejmenovat (viz. konfigurační XML soubor). Povolená jsou pouze číselná id.

`Add before selelected tree item` přidá novou položku včetně minimální cesty k\ tabulce před označenou položku, pokud je toto povoleno v\ konfiguračním XML souboru.

`Add after selected tree item` přidá novou položku včetně minimální cesty k\ tabulce za označenou položku, pokud je toto povoleno v\ konfiguračním XML souboru.

`Remove selected tree item` odebere označenou položku včetně všech podpoložek a tabulek. Pokud je odebírána poslední položka z\ hlavní kategorie, je ponechána minimální cesta k\ jedné vyprázdněné tabulce.

### Práce s\ obsahem buňek tabulky

`Cut` vyjme označený text a vloží ho do schránky.

`Copy` zkopíruje označený do schránky.

`Paste` vloží obsah schránky na místo, kde se nachází kurzor.

### Práce s\ tabulkou

`Add row before` přidá nový řádek do tabulky před řádek s\ buňkou, která má fokus. Pokud žádná buňka fokus nemá, je nový řádek přidán úplně na začátek tabulky.

`Add row after` přidá nový řádek do tabulky za řádek s\ buňkou, která má fokus. Pokud žádná buňka fokus nemá, je nový řádek přidán úplně na konec tabulky.

`Remove row` odstraní řádek s\ buňkou, která má fokus. Pokud žádná buňka fokus nemá, je odstraněn poslední řádek z\ konce tabulky.

Stromový pohled s tabulkou
--------------------------

Ve stromovém pohledu vlevo jsou hlavní kategorie (definované v\ konfiguračním XML souboru) spolu s\ podkategoriemi. Hlavní kategorie nelze přejmenovávat (názvy jsou pevně dané v konfiguračním XML souboru). Po zvolení položky, která neobsahuje již žádné další potomky, je v\ pravé části editoru zobrazena tabulka s\ hodnotami jednotlivých konfiguračních voleb. Pro zjištění kompletního názvu konfigurační volby lze využít *tooltip* nadpisu sloupce v\ tabulce. Obsah buněk v\ tabulce lze přímo editovat.

Ukázka přidávání nové položky
-----------------------------

![Přidávání nové položky](img/2013-12-11-155037_1680x1050+0+0_imlib2_grab-s.jpg)

![Po přidání nové položky a nového řádku](img/2013-12-11-155228_1680x1050+0+0_imlib2_grab-s.jpg)
