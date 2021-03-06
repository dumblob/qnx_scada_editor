+/etc/photon
  ruzne varianty konfigurace Launch buttonu
    root
    standarni uzivatel (operator systemu MCS) na pracovni stanici (_ps)
    operatori s vyssi autorizaci (_auth)

+/home/ms
  home adresar operatora, ze ktereho je provozovan SW scady, instalace
  viz /home/ms/.ph/README.txt, chown -R ms:101 /home/ms !

+/home/ps
  home adresar operatora, ze ktereho je provozovan SW HMI scady, instalace
  viz /home/ps/.ph/README.txt, chown -R ps:101 /home/ps !

+/libr/MCS
  SW scady (PROJEKT="MCS")

+etc/hostnames.run
  slouzi pro variabilni funci stanice, parametry ORIG_HOSTNAME,
  ALIAS_HOSTNAME by se mely nastavit dle skutecneho jmena stanice,
  HOST_FCE dle funkce
  tento soubor je modifkovan aplikaci rep02

+etc/rc.d/rc.ntp
  skript pro podminene spousteni ntpd pri rozbehu pocitace (klicovano
  existenci souboru /etc/ntp.cfg), pro spravnou funkci je nutne
  upravit /etc/ntp/ntp.conf - zalezi na tom jestli pocitac dosahne na
  intenet nebo musi pouzivat ntp-server napr. z podnikove site

+usr/disam
  obsahuje nektere soubory a skripty, ktere jsou blizko k systemu a SCADe,
  ale jsou na pocitaci jenom jednou bez ohledu pocet operatoru pripadne
  souviseji s konkretni implentaci scady: zalohovani "systemovych dat",
  zrcadleni dat na zalozni merici stanici, konfigurace listy a jeji
  spousteni, symlinky na ulohy sap0x, protoze nektere z nich jsou
  spousteny ze systemu, ktery nemusi mit v PATH /libr/$PROJEKT/bin...

+usr/help
  help pro MCS (v pripade, ze je system instalovan na vyvojove verzi QNX,
  je nezbytne dodelat symlink do QNX_TARGET/usr/help/product

+usr/qnx630
  nekolik souboru, ktere jsou instalovany na pocitac s vyvojovym
  prostredim a to pouze v pripade, ze existuje link /usr/qnx630 na Base
  directoty (viz qconfig) tzn. /usr/qnx641

+usr/share
  pozadi pro operatora MCS, predpoklada se, ze dle funkce stanice je
  vytvorena spravna linka

+~lib
  = dll moduly
  experimetalni verze dll knihoven (originaly pokud existuji jsou
  zkopirovany jako *.so.1, nove verze jako linka k nejvyssi verzi so,
  audio moduly nemaji plnou funkcnost (nefuguje napr. ovladani z shelf
  CD Player), ale alespon prehravaji zvuky, instalace techto modulu
  urcite pro
    did: 293e,27de,7012
    matroxp: pro PCE verzi double headed (vyzaduje svoji verzi
      display.conf), nelze menit typ driveru za jizdy
    radeon: ten uz asi nepotkame

+~sbin
  dtto [aforementioned; abovementioned] ~lib

+~etc-64x/photon/launchmenu
  finalni podoba konfigurace launch buttonu

+~etc-64x/profile.d/scada.sh
  standardni nastaveni environmentu pro operatora scady, promenne PROJEKT
  a SCADA_MS_NODE budou vyzadovat aktualni nastaveni dle skutecnosti a
  typu instalovane stanice. Tento modul je automaticky spousten pri
  prihlaseni uzivatele (jako vsechny soubory s priponou ".sh" umistene v
  adresari /etc/.profile.d/), tento zpusob resi, ze vlastne vsechny scada
  soubory jsou spolecne vsem uzivatelum na dane stanici. Pokud by na
  pracovni stanici meli byt zcela nezavisli operatori, muselo by se
  nahradit LOCAL_USR=/home/ps prirazenim odkazem LOCAL_USR=$HOME. Urcite
  bych to nedaval do .profile (specificky pro kazdeho uzivatele, protoze
  pripadna zmena nastaveni je potom komplikovanejsi).
  Skript predoklada, ze vsechny soubory scady na merici stanici jsou v
  /home/ms, kdyby se to melo stat variabilni, vyzadovalo by to taky zmenu.
  Mozna by se to dalo udelat 2-stupnove, kde zakladni odkazy by byly v
  jednom souboru (ktery by si kazdy, kdo to potrebuje precetl jako 1.),
  a odkazy na tyto zakladni odkazy se rozpracovavali az v druhem kroku

+~etc-64x/rc.d/rc.sysinit
  lehce modifikovany standarni systemovy soubor (automaticky spousteny
  pri rozbehu pocitace)

+~etc-64x/rc.d/rc.ham_setup
  spusteni HAM

+~etc-64x/rc.d/rc.start_scada
  automaticky rozbeh scady pri najeti pocitace - merici stanice

+~etc-64x/rc.d/set_cpu_affinity
  skript, ktery resi problem, ktery ma qxn v kombinaci 2-headed monitoru
  a vicejadroveho procesoru

+~etc-64x/rc.d/set_tcp_addr
  skript, ktery mel uz hodne podob, vlastne nastavuje/modifikuje IP
  adresy dle funkce stanice a /etc/hosts

+~etc-64x/rc.d/rc.pnet
  neco podobneho, ale pro pripad nezavislych instanci uloh io-pkt...

+~etc-64x/rc.d/rc.local
  automaticky spousteny pri rozbehu pocitace z /etc/rc.d/rc.sysinit, jeho
  obsah je nutne modifikovat dle funkce stanice (MS/ZMS/PS) a konkretni
  implementace

+~etc-64x/rc.d/rc.local-vk
  spousteny z /etc/rc.d/rc.local

+~etc-64x/IP-cfg.*
  soubory pro script /etc/rc.d/set_tcp_addr, obsah dle konkretni
  implementace

+~etc-64x/ham-general.cfg
  konfigurace HAM via sap02

+~etc-64x/ham-ph.cfg
  konfigurace HAM via sap02 pro grafiku, parametry vid a did je nezbytne
  nastavit dle skutecnosti

+~etc-64x/sap04.cfg
  konfigurace pro sap04

+~etc-64x/sap08.cfg
  konfigurace pro sap08

+etc-ip-qnet
  nekolik souboru, ktere v pripade, ze se system provozuje s qnet nad
  protokolem IP prijdou do /etc
