/***************************************************************************
 *$Header: /Cvs/v34/include/com_kfg.h,v 5.2 2012/07/20 14:27:40 v34 Exp $
 *
 *                       Disam RT s.r.o.
 *
 *
 *	Projekt			:  SCADA QNX 4,6
 *
 *	Jmeno modulu	:  com_kfg.h
 *
 *	Funkce			:  Definice common oblasti obsahujici konfiguracni data.
 *
 *	Autor			:  V. Kostal 
 *
 **************************************************************************
 *  Verze:   Datum:   Autor:      Popis:
 *  ======   ======   ======      ====== 
 *   2.1    02-08-93   VK         Zakladni verze               
 *   2.2    11-14-94   HCH        Zmena vel. e_cfg_kom GC_MAX_PROT -> GC_MAX_COM)
 *   2.3    26-10-95   BAT        zruseni konfig.tabulek OS, ringu pro cnt_version
 *   2.4    21-04-98   VK         mapovani portu do cfg2 (ICN 016)
 *   3.1    05-05-00   VK         icn 020
 *   5.1    28-07-03   VK         uprava pro QNX 6
 *   5.2    20-07-12   VK         uprava pro dynamicke translacni tabulky
 *
 **************************************************************************/

#ifndef _COM_KFG_
#ifdef __QNXNTO__
  #include <pthread.h>
#endif

#include <g_const.h> 
#include <g_struct.h>
#include <kfg00.h>    

/*******************************************************************************/

#define KOM_MBX_NAME		10
#define KOM_TSK_NAME 		20

#ifdef _DYNAMIC_TRANS_TAB_

/*******************************************************************************/

typedef struct
{
	KFG_S_PROTOCOL		e_cfg_prot; 				    						// popis komunikace s OS 
                          
	char				*c_kom_task_mbx;										// jmeno schranky mezi kom04 a uziv. ulohou protokolu (ze scady)
	char				*c_kom_in_mbx;											// jmeno schranky mezi vstupni ulohou na portu a uziv. protokolem (do scady) 
	char				*c_dll_name;											// dll to runtime-link
	char				*c_prot_cmnt;											// komentar/popis protokolu

} COM_S_PROTOCOL;

typedef struct
{
	KFG_S_PORT			e_cfg_com;					      						// popis nastaveni komunik. portu
	KFG_S_MAP_PORT		e_map_port;											// mapovani virtualnich portu
                          
	char				*c_kom_out_mbx;										// jmeno schranky mezi uziv. protokolem a vystupni ulohou na portu (ze scady)
	char				*c_kom_in_port;										// jmeno spoustene ulohy zajistujici vstup z portu (kom01_x)	
	char				*c_kom_out_port;										// jmeno spoustene ulohy zajistujici vystup na port (kom03_x)
	char				*c_com_cmnt;											// komentar/popis portu

} COM_S_PORT;

typedef struct
{
	int					i_max_roc_ctrl;										// max. pocet bodu pro sledovani Rate of Change
	int					i_max_ss_ctrl;											// max. pocet bodu pro sledovani Steady State
	int					i_max_fb_ctrl;											// max. pocet bodu se sledovanim zpetnovazebni hodnoty
	int					i_max_counter_ctrl;									// max. pocet bodu - citacu se spec. zpracovanim via kom moduly
	int 				i_kom_devices; 										// sirka pasma pro indexy bodu vyhrazeneho pro kanaly a OS	
	int 				i_rezerva [95]; 

} COM_S_SPECIFIC;

typedef struct                   // ....................konfigurace scady
{
	GS_COM_HEADER			e_segment_header;

	pthread_rwlock_t 		e_semaphor;											// semafor pro pristup k datum
	
	int						i_max_pnt;											// skutecny pocet bodu (<= GC_MAX_PNT)
	int						i_max_ttype;										// skutecny pocet techn. typu (<= GC_MAX_TTYPE)
	int						i_max_aoi;											// skutecny pocet AOI (<= GC_MAX_AOI)

	COM_S_SPECIFIC			e_proj_specif;										// pocty omezujici parametry specificke pro dany projekt

	KFG_S_PNT_DATA			*e_cfg_pnt [GC_MAX_PNT];       					// konfiguracni data popisujici bod
	KFG_S_TECH_TYPE			*e_cfg_ttype [GC_MAX_TTYPE];   					// popisy technol. typu cidel 
	KFG_S_AOI				*e_cfg_aoi [GC_MAX_AOI];        					// AOI (pracoviste)

	pthread_mutex_t 		e_cfg_mutex;
	pthread_mutex_t 		e_trt_mutex;
	pthread_mutex_t 		e_aoi_mutex;

	GS_POOL_STACK			e_pnt_stack;
	GS_POOL_STACK			e_ttype_stack;
	GS_POOL_STACK			e_aoi_stack;

	GS_MEM_POOL			e_mem_pool;										// memory pool dynamicke polozky v commonu

} COM_S_KFG;


typedef struct                   // ....................konfigurace komunikaci 
{
	GS_COM_HEADER		e_segment_header;
	
	pthread_rwlock_t 	e_semaphor;											// semafor pro pristup k datum

	sint				i_max_com;												// max. povoleny pocet portu (<= GC_MAX_COM)
	sint				i_max_protocol;										// max. povoleny pocet protokolu (<= GC_MAX_PROT)
	sint				i_max_ostype;											// max. povoleny pocet typu kontroleru (<= GC_MAX_OS_TYPE)
	sint				i_max_nodes;											// max. povoleny pocet uzlu systemu (operatorskych stanic) (<= GC_MAX_NODES)
	sint				i_max_os;												// max. povoleny / teoreticky pocet kontroleru (<= GC_GMAX_OS)
	sint				i_max_channel;											// max. povoleny / teoreticky pocet kanalu 
	
	COM_S_PROTOCOL		*e_comm_prot [GC_MAX_PROT];							// --- popis protokolu
	
	COM_S_PORT			*e_comm_com [GC_MAX_COM];								// --- popis komunikacnich portu 
	
	// --- typy OS (kontroleru)
	
	KFG_S_OS_TYPE		*e_cfg_ostype [GC_MAX_OS_TYPE];						// popisy typu OS
	
	// --- seznamy systemovych bodu - status operatorskych stanic
	
	KFG_S_WS_STAT_LIST *e_ws_status_list [GC_MAX_NODES];

	// --- dynamicke translacni tabulky
	
//	KFG_S_POOL_STACK	e_released_stack;										// uvolnene pool segmenty z translacnich tabulek

	KFG_S_TRAN_CHANNEL	 *e_tran_os [GC_MAX_COM];								// translacni tab. kanalu a OS
	KFG_S_TRAN_INDEX	 *e_tran_os_index;										// index pro translacni tab. kanalu a OS

	GS_MEM_POOL		 *e_tran_tp [GC_GMAX_OS];								// translacni tab. cidel na OS (-> KFG_S_TRAN_TPNT	)
	GS_MEM_POOL		 *e_katt [GC_GMAX_OS];								// doplnkova transl. tab bodu (-> KFG_S_TRAN_KATT)

	GS_MEM_POOL		e_mem_pool;											// memory pool dynamicke polozky v commonu

} COM_S_KFG2;

/*******************************************************************************/

#else

/*******************************************************************************/

 typedef struct                   /*....................konfigurace bodu */
 {
    KFG_S_PNT_DATA      e_cfg_pnt [GC_MAX_PNT];       /* konfiguracni data 
                                                         popisujici bod    */
    KFG_S_TRAN_RNG      e_tran_os[GC_MAX_COM][GC_MAX_RING][GC_MAX_OS+1];
                                                      /* translacni  tab. OS*/
    KFG_TRAN_TPNT       i_tran_tp[GC_GMAX_OS][GC_MAX_OS_PNT];
                                                      /* translacni tab.
                                                         cidel na OS  */ 
    KFG_S_KATT          e_katt [GC_GMAX_OS][GC_MAX_OS_PNT];
													  /* doplnkova transl. tab bodu */

    KFG_S_AOI           e_cfg_aoi[GC_MAX_AOI];        /* AOI (pracoviste) */

#ifdef __QNXNTO__									// ---- specif. QNX 6
	pthread_mutex_t 		e_cfg_mutex;
	pthread_mutex_t 		e_trt_mutex;
	pthread_mutex_t 		e_aoi_mutex;
#endif

 } COM_S_KFG;


typedef struct                   /*....................konfigurace typu bodu */
{

    KFG_S_TECH_TYPE     e_cfg_ttype [GC_MAX_TTYPE];   /* popisy technol. typu 
                                                         cidel             */
    KFG_S_OS_TYPE       e_cfg_ostype [GC_MAX_OS_TYPE];/* popisy typu OS    */

    KFG_S_PROTOCOL      e_cfg_prot [GC_MAX_PROT];     /* popis komunikace 
                                                         s OS              */
    KFG_S_PORT          e_cfg_kom [GC_MAX_COM];      /* popis nastaveni  
                                                         komunik. portu    */
    byte                b_port_map_valid;             /* mapa platna */
    KFG_S_MAP_PORT      e_map_port [GC_MAX_COM];      /* map. vzdalenych portu */

} COM_S_KFG2;

/*******************************************************************************/

#endif

/*************************************************************************
 *************************************************************************/

#define _COM_KFG_
#endif
