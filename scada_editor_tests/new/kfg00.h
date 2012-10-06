/***************************************************************************
 *$Header: /Cvs/tt/include/kfg00.h,v 1.9 2012/08/24 12:29:54 tt Exp $
 *
 *                       Disam RT s.r.o.
 *
 *
 *  Projekt		 :  SCADA 6
 *
 *  Jmeno modulu :  kfg00.h
 *
 *	Funkce		 :  Definice struktur obsahujicich konfiguracni udaje
 *
 *  Autor		 :  H. Chmielova, I. Batkova, V. Kostal 
 *
 **************************************************************************
 *  Verze:   Datum:   Autor:      Popis:
 *  ======   ======   ======      ======
 *   1.1    12-05-94   VK         Zakladni verze SCADA         
 *   2.1    14-06-94   BAT        zmena struktury portu pro V1_5   
 *   3.1    31-07-95   HCH        zruseni zcela zbytecneho i_roc v e_ana_lim 
 *								  a nahrazeni f_konstanta
 *   3.2    06-09-95   VK         priznak KFG00_NASOBEK
 *   3.3    24-10-95   VK         definice typu OS (c_preset_os_types)
 *   3.4    04-04-96   BAT        CNT_VERSION : KFG_S_OS_DATA, KFG_S_RG_DATA, 
 *								  KFG_S_CTLIM, KFG_S_PNT_ALR
 *          09-04-96   VK         slouceni verzi V32, V33 a CNT   
 *   
 *   3.5    10-09-97   BAT        + KFG_OSID_LG
 *   3.6    02-12-97   BAT		  + technologicke meze v typu cidla
 *   3.7    16-01-98   BAT        + VAL101A, predchozi uprava pouze pro CNT
 *   3.8    20-04-98   VK         + KFG_S_MAP_PORT (ICN 016)
 *   3.9    27-04-98   VK         + user id do kfg KFG_S_PNT_DATA
 *   3.10   30-07-98   BAT        + KFG_OSID_DOPRAVNIK 
 *   3.11   08-01-99   BAT        + KFG_OSID_S7_300
 *   3.12   10-11-99   VK         formalni upravy
 *   3.13   05-05-00   VK         icn 020
 *   3.14   02-06-00   VK         dalsi upravy v souvislosti s icn 020
 *   3.15   10-10-00   VK         + inverzni hodnota binaru, KFG_SHORT_PNT_LIST
 *   3.16   09-04-01   VK         + ELCOR, SAUTER, KFGC_MODEM
 *   3.17   25-06-01   VK         on_line mapovani portu, rozsireni typu simatic
 *   3.18   12-07-01   VK         + KFG_OS_PROP_OUTPUT_MASTER
 *   5.1    15-10-04   VK  		 Zakladni verze pro QNX 6
 *   5.2    18-08-05   VK  		 uprava KFG_S_MAP_PORT
 *   5.3    17-08-06   VK  		 doplneni KFG_OSID_...
 *   5.4    29-09-06   VK  		 uprava priznaku KFG_PF_...
 *   5.5    18-12-06   VK  		 uprava KFG_S_OS_DATA (doplneni i_specif_timeout)
 *   5.6    25-08-09   VK  		 pnt_id sint->PntId

 *   1.6    06-06-12   HS  		 Pracovni verze 1.6 - aktualizace podle kfg00.h z projektu Stonava verze 5.7
 * 
 ***************************************************************************/

#ifndef _KFG00_

#include <stdio.h>
#include <g_const.h>

/**************************************************************************/
/*                   Predefinovane typu OS                                */
/**************************************************************************/

enum _os_types_ids {
	_KFG_OS_FAMILY_BASE  = 1,						
	_KFG_OS_FAMILY_VENTURON	= _KFG_OS_FAMILY_BASE,						
	_KFG_OS_FAMILY_ZETTLER	= 100,						
	_KFG_OS_FAMILY_GIANT		= 200,						
	_KFG_OS_FAMILY_SIMATIC	= 300,						
	_KFG_OS_FAMILY_SYS2SYS	= 400,						
	_KFG_OS_FAMILY_OTHER		= 1000						
};

enum os_type_ids {

	KFG_OSID_VAL101_8 = _KFG_OS_FAMILY_VENTURON,		// Venturon (kom_vent_in/out)
	KFG_OSID_VAL101_10,
	KFG_OSID_VAL102_8,
	KFG_OSID_VAL102_10,
	KFG_OSID_VAL101A, 				              	// VAL101 s 4 byty pro DI 

	KFG_OSID_Z_DVOUHLASICE = _KFG_OS_FAMILY_ZETTLER,	// Zettler (kom_zettler)
	KFG_OSID_Z_HLASICE,
	KFG_OSID_Z_OVLADACI_VYST,
	KFG_OSID_Z_POPLACH_VYST,
	KFG_OSID_Z_INFORM_VYST,
	KFG_OSID_Z_VIRTUALNI_VYST,
	KFG_OSID_Z_EXTERNI_SIRENA,
	KFG_OSID_Z_PORUCHA,
	KFG_OSID_Z_DALKOVY_PRENOS,
	KFG_OSID_Z_VSTUPY,
	KFG_OSID_Z_USTREDNA,

	KFG_OSID_TG_DAP = _KFG_OS_FAMILY_GIANT,			// dukla (kom_tg)
	KFG_OSID_TG_TERMINAL,								// Litomysl (kom_term_tg)
	KFG_OSID_MP,										// Maxiprint (kom_mptg)
	KFG_OSID_LG,										// Little Giant (kom_kdtg)
	KFG_OSID_TG,										// Tiny Giant (kom_kdtg)
	KFG_OSID_TG_a16,								   	// Tiny Giant s 16 AI  (kom_kdtg)

	KFG_OSID_S7_300 = _KFG_OS_FAMILY_SIMATIC,		// Siemens S7 - 300 
	KFG_OSID_S7_315,							       // Siemens S7 - 315 
	KFG_OSID_S7_316,                					// Siemens S7 - 316
	KFG_OSID_S7_400,                					// Siemens S7 - 400
	KFG_OSID_S7_400_AH,             					// Siemens S7 - 400 - Automat horaku

	KFG_OSID_AUTEL = _KFG_OS_FAMILY_SYS2SYS,      // Autel

	KFG_OSID_SAIT1 = _KFG_OS_FAMILY_OTHER,			// Sait Lazy (kom_sait) 
	KFG_OSID_SAIT2,
	KFG_OSID_DOPRAVNIK_APD,							// dopravnik APD - Lazy 
	KFG_OSID_GRAMOT_DAP,			   					// Doubrava (kom_arp)
	KFG_OSID_MTA	,									// Dukla (kom_mta, kom_mta_s)
	KFG_OSID_MICRO_110,								// AEG Micro 110 
	KFG_OSID_ELCOR_94,									// Elcor 94 
	KFG_OSID_SAUTER,									// Sauter
	KFG_OSID_ZAM,										// ZAM - DKD 2000 
	KFG_OSID_UE300,									// ustredna UE300 (Tesla Liptovsky Hradok)
	KFG_OSID_VIRTUAL,									// virtualni stanice
	KFG_OSID_UE60,										// ustredna UE60 (Tesla Liptovsky Hradok)
	KFG_OSID_AMIT,										// AMIT
	KFG_OSID_TROLEX_9040,
	KFG_OSID_DAP,
	KFG_OSID_FTP_FILE,									// soubor
	KFG_OSID_MARA02,
	KFG_OSID_SIMATIC_MP, 								// maxiprint v provedeni simatic via Modbus
	KFG_OSID_TR320, 									// Tociva redukce via Modbus
	
	KFG_OSID_end
};
 
/*************************************************************************/
/*            Popis technologickeho typu OS                              */
/*************************************************************************/

/*OUTSTATION DESCRIPTION for each OS type */

                                /*...................... vlastnosti stanice */  

#define KFG_PF_I2_ROTATE    0x00010000         /* obracene poradi bytes v short integeru proti normalu */
#define KFG_PF_I4_ROTATE    0x00020000         /* obracene poradi bytes v long integeru proti normalu */
#define KFG_PF_F4_ROTATE    0x00040000         /* obracene poradi bytes ve float proti normalu */
#define KFG_PF_F8_ROTATE    0x00080000         /* obracene poradi bytes v double float proti normalu */
#define KFG_PF_SINGLE_REQ   0x10000000         /* zakaz optimalizace zadosti o data (adresace MB, DDE) */
#define KFG_PF_IGN_VALSTAT  0x20000000         /* zakaz nabizet masku Ok stavu hodnoty */

#define KFG_PF_RIGID_MB  (KFG_PF_I2_ROTATE|KFG_PF_I4_ROTATE|KFG_PF_F4_ROTATE|KFG_PF_F8_ROTATE)

typedef struct 					/* ...................... popis typu O/S */	
{
	sint			i_ostype_id;
	char			c_ostype_name [GC_MAX_DESC_LEN+1];
	int				i_ai_max_number;
	int				i_di_max_number;
	int				i_do_max_number;
	int				i_ao_max_number;
	int				i_ct_max_number;
	int				i_anal_data_length;
	DWORD			dw_properties;               // KFG_PF_...
	DWORD			dw_fix_properties;				// maska blokujici prepis dw_properties operatorsky
	void			*v_os_adv_specific;			// specificka doplnkova data dle typu stanice napr. predpis linearizace transl. tabulek (SIMATIC_S_DB_LIST_POOL)
		
} KFG_S_OS_TYPE;

enum _i_os_adv_data_type_	 {						// ciselnik pro v_os_adv_specific:

KFG_OS_ADV_UNDEFINED,					
KFG_OS_ADV_LINEARIZED_APL_DATA,					
};

typedef struct										// standardni hlavicka pro v_os_adv_specific
{
	sint						i_segment_id;
	sint						i_segment_len;
	int							i_serialized_size;

} KFG_S_OSTYPE_ADV_SPECIFIC;

typedef struct                  /* ........................ kfg data pro OS */
{
	sint				i_os_scan_interval;			/* scan interval analogu */
	sint				i_os_scbin_interval;			/* scan interval binaru */
	sint				i_os_sccnt_interval;			/* scan interval citacu */
	sintU				i_os_alarm_mask;				/* maska zpracovani alarmu O/S, prog*/
	DWORD			dw_prop_flags;					/* priznaky konfig.vlastnosti OS */
	sint				i_os_scan_type;				/* priznak zasilani bodu pri zmene (v poradi AI, DI, CT) */
	sint				i_os_index;						/* index OS do translacnich tab. TRAN_TPNT */
	sint				i_adr_type;						/* typ adresace bodu (dle ringu) */ 
	sint				i_specif_timeout;				/* specificky timeout pro cekani na odpoved */
	
} KFG_S_OS_DATA;

typedef struct                  /*...................... kfg data pro ring*/
{
   sint            i_rng_index;       /* index ringu do translacnich tab TRAN_RNG */
   sint            i_hbt_limit;      	  /* casovy interval kontroly*/
   sint            i_adr_type;        /* typ adresace bodu na stanicich ringu */ 

} KFG_S_RG_DATA;

//#define KFG_C_ON_REQ        1              /* vysila cidla na pozadani */
//#define KFG_C_ON_CHG        2              /* vysila cidla pri zmene hodnoty */

#define KFG_C_AI_ON_CHANGE	0x1			/* vysilani AI cidla pri zmene hodnoty */
#define KFG_C_DI_ON_CHANGE	0x2			/* vysilani DI cidla pri zmene hodnoty */
#define KFG_C_CT_ON_CHANGE	0x4			/* vysilani CT cidla pri zmene hodnoty */

/**************************************************************************/
/*                   Alarmni meze a data pro vyhodnoceni                  */
/**************************************************************************/

#ifdef _CNT_VERSION_

typedef struct                  /* ................... pro analogove vstupy */
{
	sint         i_ss_period;          /* Interval pro alarm SS */
	sint         i_roc_period;         /* Interval pro ROC */    
	sint         i_roc;                /* Limit pro alarm ROC  */
	sint         i_rezerva;
	float        f_high_high;          /* high-high value alarm limit */
	float        f_high;               /* high value alarm limit */ 
	float        f_low;                /* low value alarm limit */
	float        f_low_low;            /* low-low value alarm limit*/

	PntId        i_feedback_id;        /* Id bodu zpetne vazby */
	sint         i_feedback_timeout;   /* Timeout pro feedback */

} KFG_S_ANLIM; 

#elif defined _KFG_NASOBEK_                     /* VVUU */

typedef struct                  /* ................... pro analogove vstupy */
{
	sint         i_ss_period;          /* Interval pro alarm SS */
	sint         i_roc_period;         /* Interval pro ROC */    
	float        f_konstanta;          /* konstanta - nasobitel */
	float        f_high_high;          /* high-high value alarm limit */
	float        f_high;               /* high value alarm limit */ 
	float        f_low;                /* low value alarm limit */
	float        f_low_low;            /* low-low value alarm limit*/

	PntId        i_feedback_id;        /* Id bodu zpetne vazby */
	sint         i_feedback_timeout;   /* Timeout pro feedback */

} KFG_S_ANLIM; 

#else                                   /* stara verze - V32 */

typedef struct                  /* ................... pro analogove vstupy */
{
	sint         i_ss_period;          /* Interval pro alarm SS */
	sint         i_roc_period;         /* Interval pro ROC */    
	float        i_roc;                /* Limit pro alarm ROC  */
	float        f_high_high;          /* high-high value alarm limit */
	float        f_high;               /* high value alarm limit */ 
	float        f_low;                /* low value alarm limit */
	float        f_low_low;            /* low-low value alarm limit*/

	PntId        i_feedback_id;        /* Id bodu zpetne vazby */
	sint         i_feedback_timeout;   /* Timeout pro feedback */

} KFG_S_ANLIM; 

#endif
   
typedef struct                  /* ................... pro digitalni vstupy */
{
     sint          i_ss_period;          /* Interval pro alarm SS */
     sint          i_roc_period;         /* Interval pro ROC */    
     sint          i_roc;                /* Limit pro alarm ROC  */
     sint          i_inv_polarita;       /* Inverzni hodnota signalu */
     PntId         i_feedback_id;        /* Id bodu zpetne vazby */
     sint          i_feedback_timeout;   /* Timeout pro feedback */

} KFG_S_DGLIM;

typedef struct                  /* ................... pro citacove vstupy */
{
     lint         j_cnt_period;         /* Interval pro nulovani - v sec */
     jtime        t_cnt_set;            /* cas pro 1. spusteni */    
     float        f_high_high;          /* high-high value alarm limit */
     float        f_high;               /* high value alarm limit */ 
     float        f_low;     	        /* low value alarm limit */
     float        f_low_low;            /* low-low value alarm limit */ 
     float        f_max;                /* maximalni povolena hodnota citace */
     float        f_min;                /* pocatecni hodnota citace */
     float        f_factor;             /* faktor nasobeni */

} KFG_S_CTLIM;

typedef struct                  /* ................... pro telefony */
{
	DWORD			dw_rezerva_flags;
	char			c_tf_id [GC_MAX_TF_ID + 1];	

} KFG_S_TF_DATA;

typedef struct                  /* ...... alarm limits and activation flags */
{
    sintU         i_alarm_flag;       /* alarmy nakonfigurovane na bode */ 
    byte          b_ack_req;          /* alarm acknowledge request :
                                         -  GC_ALR_ACK_REQ_YES
           								 - 	GC_ALR_ACK_REQ_NO          */
    byte          b_priority;         /* alarm priority                 */

    union
    {
        KFG_S_ANLIM    e_ana_lim;        /* analogove body           */
        KFG_S_DGLIM    e_dig_lim;        /* digitalni body           */
        KFG_S_CTLIM    e_cnt_lim;        /* citacove body            */
        KFG_S_TF_DATA  e_tf_lim;         /* telefon			         */
        KFG_S_OS_DATA  e_os_lim;         /* stanice OS               */
        KFG_S_RG_DATA  e_rng_lim;        /* ring                     */ 

    } e_limit;

} KFG_S_PNT_ALR;

/**************************************************************************/
/*                     Adresace bodu                                      */
/**************************************************************************/

enum _i_kfg_adr_type_						// typ adresace bodu na stanici 
{
KFG_ADR_TYPE_SIMPLE = 1,						
KFG_ADR_TYPE_MB_BYTE,						// ModBus, bytova adresa 
KFG_ADR_TYPE_MB_WORD,						// ModBus, word adresa 
KFG_ADR_TYPE_DDE_BYTE,
KFG_ADR_TYPE_DDE_WORD,
KFG_ADR_TYPE_DDE_DWORD,
KFG_ADR_TYPE_KEYSTR,
KFG_ADR_TYPE_KEY_INT,
KFG_ADR_TYPE_KEY_INT64,
KFG_ADR_TYPE_LINEARIZED,					// linearizace dat na stanici dalsim predpisem (napr. simatic DRT protokol)
KFG_ADR_TYPE_MAX_VALID = 10,
};

/* ------------------  typy prommenych s komplexni adresou ---------------- */
 
enum _kfg_ca_var_types {

	KFG_KAVT_UNDEF,
												// -- promenne vazane na konkretni registry
	
	KFG_KAVT_DI,								// delka = 1 (binarni vstup)
	KFG_KAVT_COIL,						       // delka = 1 (binarni vystup)
	KFG_KAVT_AI,								// delka v bitech urcuje citlivost prevodniku (neprepocteny an.vstup)

												// -- promenne vazane na 'hold' registry
	KFG_KAVT_BIT_FIELD = 9,					// delka v bitech
	KFG_KAVT_BOOL,								// delka v bitech
	KFG_KAVT_INTEGER,							// delka v bytech
	KFG_KAVT_REAL,								// delka v bytech 
	KFG_KAVT_RREAL,							// delka v bytech, inverzni poradi bytes 
	KFG_KAVT_RINTEGER,							// inverzni poradi bytes
	KFG_KAVT_UINTEGER,							// unsigned integer
	KFG_KAVT_RUINTEGER,						// unsigned integer, inverzni poradi bytes

												// -- specificke promenne dle aplikace
	KFG_KAVT_SDC = 21,							// cidlo SD dopravniku
	KFG_KAVT_POLOHA_SD					 		// poloha APD

};	

#define     KFG_MAX_KAVT		/*(itemsof (_kfg_ca_var_types)) */	   	  22	

/* ------------------------------------------------------------------------ */

#define     KFG_MAX_KA_LEN             25       // delka pole pro formatovanou komplexni adresu
#define     KFG_MAX_KA_FIXLEN          15       // delka pole pro komplexni adresu v pevnem formatu
#define     KFG_MAX_KS_LEN	(KFG_MAX_KA_FIXLEN-1)  // delka 	jedineceneho retezce ekvivalentniho adrese 

typedef struct                                  /* komplexni adresa */
{
   byte			b_type;                  /* typ promenne KFG_KAVT_... */
   byte			b_len;                   /* delka (pocet bytes/bits dle typu) */
   byte			b_offset;                /* bitovy offset */
   byte			b_data_block;				/* # datoveho bloku */
   sintU		i_value_adr;             /* adresa hodnoty */
   sintU		i_status_adr;				/* adresa stavoveho slova */
   lintU		j_status_mask;				/* maska stavoveho slova */			
   lintU		j_status_value;			/* pozadovana hodnota stavoveho slova */			
		
} KFG_S_COMPLEX_ADR;

typedef struct                                  /* jedinecny retezec */
{
   char			c_key [KFG_MAX_KS_LEN+1];   /* jedinecny retezec ekvivalentni adrese bodu */
		
} KFG_S_KEYSTR_ADR;

#include <_pack1.h>

typedef union                           		// doplnkova udaje adresy
{
	KFG_S_COMPLEX_ADR   	e_ca;					// komplexni adresa 
	KFG_S_KEYSTR_ADR		e_ksa;					// key_string adresa 
	int						i_key;					// key-integer	adresa 
	_uint64					iu64_key;				// key-integer-64 adresa 	

} KFG_S_KATT;

#include <_packpop.h>

/* --- po uprave: !!!
#ifndef _DYNAMIC_TRANS_TAB_
  #include <_packpop.h>
#endif 
--- */

typedef struct 
{
   byte				b_adr_type;					/* typ adresace bodu na parent bodu */

   union
   { 
		sint		i_adr;							/* jednoducha adresa v souvislem I/O poli */
		sint		i_tp_rel;						/* rel. poradi bodu v tran. tabulce stanice,
											   	   ! platny udaj jen v commonu */
   } e_u;
    
   KFG_S_KATT		e_ua;

} KFG_S_PNT_ADR;

/* --- po uprave:
#ifdef _DYNAMIC_TRANS_TAB_
  #include <_packpop.h>
#endif 
--- */

/**************************************************************************/
/*                     Popisu bodu v konfiguraci                          */
/**************************************************************************/

typedef struct 
{
   sword           w_pnt_type;                 /* Typ bodu + zdroj informace
                                                  	   resp. podtyp sys. bodu 
                                                       (zpusob zakodovani totozny s bity 
                                                       (GC_PNT_TYP_MASK |GC_DAT_SOURCE_MASK) stavoveho slova RDP) */
   sint            i_tech_type;                  	/* Cislo technol. type  */
   sint            i_pnt_group;                  	/* Skupina resp. AOI    */
   char            c_pnt_name [GC_MAX_NAME_LEN + 1];/* Jmeno bodu  */
   char            c_alr_txt [GC_MAX_DESC_LEN+1];	/* Alarmni text bodu */
   PntId           i_parent_id;                  	/* OS id pro bod, ring ID pro OS, cislo portu pro ring */
   KFG_S_PNT_ADR   e_pnt_adr;                    	/* adresa bodu na OS, adresa OS na ringu, adresa ringu na portu */
   KFG_S_PNT_ALR   e_alarm;                      	/* parametry alarmu bodu */

   jtime           t_time;                       	/* Cas vzniku (aktualizace) bodu */
   sint            i_usr_id;                     	/* Id uzivatele provadejiciho kfg zasah */  

} KFG_S_PNT_DATA;


#ifdef _DYNAMIC_TRANS_TAB_

 typedef struct 
 {
	PntId			 	i_pnt_id;
	KFG_S_PNT_DATA		e_pnt_data;
	char				c_pnt_cmnt [GC_MAX_PNT_CMNT_LEN + 1];
 } KFG_S_PNT_REC;
 
/* --- zrusit ---
#else


 typedef struct 
 {
	PntId			 i_pnt_id;
   char				 c_pnt_cmnt [GC_MAX_PNT_CMNT_LEN + 1];

 } KFG_S_PNT_CMNT;
-----*/

#endif

/*************************************************************************/
/*            Popis technologickeho typu cidla                           */
/*************************************************************************/

typedef struct
{
	char		c_ttype_name[GC_MAX_DESC_LEN+1];		// popis technologickeho typu 
	sword		w_tpnt_type;          				// horni bity dle stavoveho slova

	float		f_high_high;							// high-high value alarm limit 
	float		f_high;									// high value alarm limit
	float		f_low; 									// low value alarm limit 
	float		f_low_low;								// low_low value alarm limit 

	union
	{
		sint	i_dig_alarm;							// pro digitalni body - alarmy 01 nebo 10 
		sint	i_process_flag;						// prepocivat hodnotu AI/AO ano/ne 
	} e_u;

	float		f_max_range;							// rozsah mereni cidla do
	float		f_min_range;							// rozsah mereni cidla od

	float		f_techn_max;							// technologicka horni mez 
	float		f_techn_min;							// technologicka dolni mez 

	union
	{
		float	f_value_deadband;						// pasmo necitlivosti pro analogy 
		float	f_factor;								// default konstanta pro citace 
		sint	i_inv_polarita;						// obracena polarita bin. signalu 
		DWORD	dw_flags;								// vlastnosti dle typu telefon		
	}e_val_proc;
    
	char		c_units [GC_MAX_NAME_LEN+1];			// jednotky 

} KFG_S_TECH_TYPE;

#ifdef _DYNAMIC_TRANS_TAB_
typedef struct
{
	int						i_ttype_id;
	KFG_S_TECH_TYPE		e_ttype_data;

} KFG_S_TTYPE_REC;
#endif

/*************************************************************************/
/*            Popis komunikace                                           */
/*************************************************************************/

/*PROTOCOL DESCRIPTION */

 typedef struct 
 {
       char          c_leading_char_value;     /*hodnota uvodniho znaku*/
       char          c_end_char_value;         /*hodnota koncoveho znaku*/
       char          c_stuff_char_usage;       /*pouziva/nepouziva stuff*/
       char          c_stuff_char_value;       /*hodnota stuff*/
       char          c_checksum_usage;         /*pouziva/nepouziva kontrolni
                                                 soucet*/   
       sint          i_checksum_id;            /*funkce pro vypocet kontrol.
                                                 souctu*/  
       sint          i_checksum_len;           /*delka pouzivaneho kontolniho
                                                 souctu v bytech*/  
       sint          i_mess_max_len;           /*maximalni delka zpravy*/ 

 } KFG_S_PROTOCOL;

/* Popis nastaveni komunikacniho portu Index Port_Id */

typedef struct 
{
	sint		i_max_channel;				// max. pocet kanalu
	sint		i_max_os;					// max. pocet stanic na kanale
	int			i_baud_rate;             // baudova rychlost
	sint		i_parity;                // suda, licha, zadna
	sint		i_stop_bits;             // pocet stop bitu 
	sint		i_data_bits;             // pocet datovych bitu
	char		c_port_addr[4];          // adresa vekt. preruseni
	sint		i_comm_type;             // zpusob komunikace
	sint		i_protocol_id;           // identifikace protokolu na portu   1 .. GC_MAX_PROT

 } KFG_S_PORT;

/* Zpusob komunikace*/

#define KFGC_SIMP     0      /* dotaz-odpoved*/
#define KFGC_FDUP     1      /* plny duplex*/    
#define KFGC_FILE     2      /* komunikace pres soubor */  
#define KFGC_MODEM    3      /* komunikace pres modem - nenastavuje se via stty */     
#define KFGC_HSIM     4      /* dotaz-odpoved (simplex), rizeni signaly RTS, DTR */
#define KFGC_TCP		5		// tcp protokol
#define KFGC_UDP		6		// udp protokol


#define KFGC_NUL      0      /* bez parity*/
#define KFGC_ODD      1      /* licha parita*/
#define KFGC_EVE      2      /* suda parita*/

/*************************************************************************/
/*                       Translacni tabulky                              */
/*************************************************************************/

#ifdef _DYNAMIC_TRANS_TAB_				// --------------------

typedef struct 
{
    PntId    i_pnt_id;                   // ID bodu - OS
    PntId    i_pnt_index;                // index do tran. tabulek cidel, pro ring index do RING_DATA

} KFG_S_TRAN_OS;

typedef struct                           // translacni tabulka kanalu a stanic pro dynamicke translacni tabulky
{
	sint			i_max_channel;				
	sint			i_max_os;	

	KFG_S_TRAN_OS	e_os [1];

} KFG_S_TRAN_CHANNEL;

typedef struct                           // index-tabulka pro rychly pristup k translacni tabulce kanalu a OS via index kanalu
{
	KFG_S_TRAN_CHANNEL		*e_port_params;	// parametry portu (pocet kanalu a stanic na kanale)
	KFG_S_TRAN_OS			*e_os_list;		// seznam stanic na kanale

} KFG_S_TRAN_INDEX;

typedef struct								/* translacni tabulka  cidel */
{
	PntId  i_pnt_id [1];

} KFG_S_TRAN_TPNT;         

typedef struct                          /* doplnkova translacni tabulka cidel */
{
    KFG_S_KATT	e_ua [1]; 					// uplna komplexni adresa bodu
                                            
} KFG_S_TRAN_KATT;

#else										// --------------------

typedef struct                           /* staticka translacni tabulka stanic */
{
    PntId    i_pnt_id;                   /* ID bodu - ringu, OS*/
    PntId    i_pnt_index;                /* pro OS index do tran. tabulek
                                            cidel a index do OS_DATA,
                                            pro ring index do RING_DATA*/
                                            
} KFG_S_TRAN_RNG;

typedef PntId   KFG_TRAN_TPNT;          /* translacni tabulka  cidel   */

#endif										// ---------------------

/*************************************************************************/
/*                  Skupiny bodu (pracoviste resp. AOI)                  */
/*************************************************************************/

typedef struct
{
	sint			i_parent_id;
	char			c_group_name [GC_MAX_WKP_NAME_LEN+1];

} KFG_S_AOI;

#ifdef _DYNAMIC_TRANS_TAB_
typedef struct
{
	int				i_aoi_id;
	KFG_S_AOI		e_aoi_data;

} KFG_S_AOI_REC;
#endif

/*************************************************************************
 *  			Konstanty pro konfiguraci								 *
 *************************************************************************/

#define KFG_C_TIMEOUT              120        // timeout cekani na 
											  // odpoved od GN_KFG00

/************************************************************************
 *       	Konstanty pro zobrazovani skupin bodu - kfg00_read_pnt      *
 ************************************************************************/

#define KFG_ANAL             0x0001      /* pouze analogove body  */
#define KFG_DIG              0x0002      /* pouze digitalni body  */ 
#define KFG_DER              0x0003      /* pouze derivovane body */
#define KFG_SYST             0x0004      /* pouze systemove body  */
#define KFG_TECH             0x0005      /* vsechny technologicke - mimo syst */ 
#define KFG_VYPMISTO         0x0006      /* vypinaci mista */ 
#define KFG_DIG_OUT          0x0007      /* binarni vystupy */ 
#define KFG_CNT              0x0008      /* citace */           
#define KFG_ANA_OUT          0x0009      /* analogove vystupy */ 

/*----- tyto konstanty musi byt navzajem bitove disjunktni a unsigned !! */

#define KFG_SELECT_MASK      0xf000
#define KFG_SELECT_TTYPE     0x1000      // vyber podle tech. typu
#define KFG_SELECT_AOI       0x2000      // vyber podle aoi
#define KFG_SELECT_PARENT    0x4000      // vyber podle rodice ( OS )
#define KFG_SHORT_PNT_LIST   0x8000      // jen cislo a jmeno bodu   

/**************************************************************************/
/*                      Specificke vlastnosti stanice                     */
/**************************************************************************/

#define KFG_OS_PROP_SPEC_ADDR       0x0001     // specialni rezim adresace (specificky dle typu stanice)
#define KFG_OS_PROP_OUTPUT_MASTER   0x0010     // pri najezdu Scady se aktualizuji stavy AO, DO dle stanice  
#define KFG_OS_PROP_LOAD_HISTORY		0x0020     // pri najezdu Scady a obnoveni komunikace se doplnuje zpetne historie snimanych bodu

/**************************************************************************/
/*                      Mapovani vzdalenych portu                         */
/**************************************************************************/

enum _virtual_port_types_
{
KFG_C_MAPPED_PORT_IGNORED,					// 0 = ignorovan
KFG_C_MAPPED_PORT_LOCAL,						// 1 = mistni nebo vzdaleny QNX port rizeny lokalne tj. z MS (tzn. setup je provaden na MS)
KFG_C_MAPPED_PORT_REMOTE,						// 2 = mistni nebo vzdaleny QNX port jehoz komunikacni server je umisten na stejnem uzlu jako tento port
KFG_C_MAPPED_PORT_AUTONOMY,					// 3 = mistni nebo vzdaleny QNX port s automatickym nastavenim pri najezdu pocitace (bez setup via stty v kom00)
KFG_C_MAPPED_PORT_TCP,						// 4 = vzdaleny TCP port
KFG_C_MAPPED_PORT_UDP,						// 5 = vzdaleny UDP port
KFG_C_MAPPED_PORT_INVALID						// neplatny typ
};


typedef struct
{
	sint	i_virt_port_id;             /* virtual port number (MS) */
	sint	i_port_type;

	int		i_node;                     /* (remote) node id / tcp adresa [byte0.byte1.byte2.byte3] */
	int		i_port_id;                  /* real port number on (remote) node / tcp_port_number */
    
} KFG_S_MAP_PORT;

/**************************************************************************/
/*                      Stavy pracovnich stanic                           */
/**************************************************************************/

typedef struct
{
	PntId				i_disc_id;										   		// obsaz. prostor disk
	PntId				i_logged_id;											// status prihlaseni
	PntId				i_memory_id;											// obsaz. pameti
	PntId				i_cpu_temp_id;											// teplota CPU
	PntId				i_rezerva [6];										
	
} KFG_S_WS_STAT_LIST;

enum _i_ws_status_pnt_types_
{
PNT_ID_IS_NOT_FOUND,
PNT_ID_IS_DISC_STATUS,
PNT_ID_IS_NODE_STATUS,
PNT_ID_IS_MEMORY_STATUS,
PNT_ID_IS_CPU_TEMP,

PNT_ID_IS_INVALID,		// 1. neplatny 
};

/**************************************************************************/
/*                      Dynamicke plneni commonu                          */
/**************************************************************************/

typedef struct													// skutecne pocty polozek (administrator limit)
{
	int								i_max_pnt;
	int								i_max_ttype;
	int								i_max_aoi;	
	int								i_max_nodes;
	int								i_kom_devices;					// sirka pasma pro indexy bodu vyhrazeneho pro kanaly a OS	
	int								i_max_roc_ctrl;					// max. pocet bodu pro sledovani Rate of Change
	int								i_max_ss_ctrl;					// max. pocet bodu pro sledovani Steady State
	int								i_max_fb_ctrl;					// max. pocet bodu se sledovanim zpetnovazebni hodnoty
	int								i_max_counter_ctrl;				// max. pocet bodu - citacu se spec. zpracovanim via kom moduly

} KFG_S_ADMIN_LIMITS;

typedef struct
{
	KFG_S_ADMIN_LIMITS				e_admin_limits;

	int								i_max_protocol;
	int								i_prot_serialized_len;			// celkova delka dat
	KFG_S_PROTOCOL					**e_cfg_prot;     				// popis komunikace s OS 
	char							**c_kom_task_mbx;				// jmena schranek mezi kom04 a uziv. ulohami protokolu
	char							**c_kom_in_mbx;					// jmena schranek mezi uziv. protokolem a vstupni ulohou na portu
	char							**c_prot_cmnt;					// komentare/popisy protokolu

	int								i_max_com;
	int								i_com_serialized_len;			// celkova delka dat
	KFG_S_PORT						**e_cfg_com;      				// popis nastaveni komunik. portu
	char							**c_kom_out_mbx;				// jmena schranek mezi uziv. protokolem a vystupni ulohou na portu
	char							**c_kom_in_port;				// jmena spoustenych uloh zajistujicich vstup z portu
	char							**c_kom_out_port;				// jmena spoustenych uloh zajistujicich vystup na port
	char							**c_com_cmnt;					// komentare/popisy portu

	int								i_max_ostype;		
	int								i_ostype_serialized_len;		// celkova delka dat
	KFG_S_OS_TYPE					**e_cfg_ostype;					// popisy typu OS
	KFG_S_OSTYPE_ADV_SPECIFIC		**e_adv_specific;

	int								i_max_ws;
	int								i_ws_stat_serialized_len;		// celkova delka dat
	union
	{
		KFG_S_WS_STAT_LIST			**e_ws_stat_list;
		PntId						**i_ws_stat_pnt_id;
	};
	
} KFG_S_KOM_OVERRIDE;

typedef struct
{
	int								i_max_pnt;
	int								i_max_ttype;
	int								i_max_aoi;	
	int								i_max_nodes;
	int								i_kom_devices;					// sirka pasma pro indexy bodu vyhrazeneho pro kanaly a OS	
	int								i_max_roc_ctrl;					// max. pocet bodu pro sledovani Rate of Change
	int								i_max_ss_ctrl;					// max. pocet bodu pro sledovani Steady State
	int								i_max_fb_ctrl;					// max. pocet bodu se sledovanim zpetnovazebni hodnoty
	int								i_max_counter_ctrl;				// max. pocet bodu - citacu se spec. zpracovanim via kom moduly
		
//	KFG_S_WS_STAT_LIST				**e_ws_stat_list;

} KFG_S_KFG_OVERRIDE;

/***************************************************************************/

#define _KFG00_
#endif
