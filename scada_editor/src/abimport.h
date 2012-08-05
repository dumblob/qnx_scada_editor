/* Import (extern) header for application - AppBuilder 2.03  */

#include "abdefine.h"

extern ApWindowLink_t base;
extern ApWidget_t AbWidgets[ 9 ];

extern ApMenuLink_t file_menu;

#ifdef __cplusplus
extern "C" {
#endif
int open_file( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int save_file( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int cut_text( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int copy_text( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int paste_text( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int open_file( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int save_file( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int save_file_as( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int app_exit( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int on_tree_selection( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int new_file( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
#ifdef __cplusplus
}
#endif
