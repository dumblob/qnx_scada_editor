/* Define header for application - AppBuilder 2.03  */

/* 'base' Window link */
extern const int ABN_base;
#define ABW_base                             AbGetABW( ABN_base )
extern const int ABN_tree_wgt;
#define ABW_tree_wgt                         AbGetABW( ABN_tree_wgt )
extern const int ABN_table_pane;
#define ABW_table_pane                       AbGetABW( ABN_table_pane )
extern const int ABN_bm_file;
#define ABW_bm_file                          AbGetABW( ABN_bm_file )
extern const int ABN_b_open;
#define ABW_b_open                           AbGetABW( ABN_b_open )
extern const int ABN_b_save;
#define ABW_b_save                           AbGetABW( ABN_b_save )
extern const int ABN_b_cut;
#define ABW_b_cut                            AbGetABW( ABN_b_cut )
extern const int ABN_b_copy;
#define ABW_b_copy                           AbGetABW( ABN_b_copy )
extern const int ABN_b_paste;
#define ABW_b_paste                          AbGetABW( ABN_b_paste )

#define AbGetABW( n ) ( AbWidgets[ n ].wgt )

/* 'file_menu' Menu link */
extern const int ABN_file_menu;
extern const int ABN_m_new;
extern const int ABN_m_open;
extern const int ABN_m_save;
extern const int ABN_m_save_as;
extern const int ABN_m_quit;

#define AB_OPTIONS "s:x:y:h:w:S:"
