/* Define header for application - AppBuilder 2.03  */

#include "abvars.h"

/* 'base' Window link */
#define ABW_base                             AbGetABW( ABN_base )
#define ABW_table_pane                       AbGetABW( ABN_table_pane )
#define ABW_tree_wgt                         AbGetABW( ABN_tree_wgt )
#define ABW_bm_file                          AbGetABW( ABN_bm_file )
#define ABW_b_rename_item                    AbGetABW( ABN_b_rename_item )
#define ABW_b_add_item_before                AbGetABW( ABN_b_add_item_before )
#define ABW_b_add_item_after                 AbGetABW( ABN_b_add_item_after )
#define ABW_b_remove_item                    AbGetABW( ABN_b_remove_item )
#define ABW_b_cut                            AbGetABW( ABN_b_cut )
#define ABW_b_copy                           AbGetABW( ABN_b_copy )
#define ABW_b_paste                          AbGetABW( ABN_b_paste )
#define ABW_b_add_row_before                 AbGetABW( ABN_b_add_row_before )
#define ABW_b_add_row_after                  AbGetABW( ABN_b_add_row_after )
#define ABW_b_rem_row                        AbGetABW( ABN_b_rem_row )

#define AbGetABW( n ) ( AbWidgets[ n ].wgt )

/* 'file_menu' Menu link */

#define AB_OPTIONS "s:x:y:h:w:S:"
