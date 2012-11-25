/* Y o u r   D e s c r i p t i o n                       */
/*                            AppBuilder Photon Code Lib */
/*                                         Version 2.03  */

/* Standard headers */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

/* Local headers */
#include "ablibs.h"
#include "abimport.h"
#include "proto.h"

#include "datasaver.h"
#include "filepicker.h"
#include "global_vars.h"

extern struct scada_ed_global_vars_s scada_ed_global_vars;

/** uses scada_ed_global_vars */
int save_file_as(PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo)
{
  /* eliminate 'unreferenced' warnings */
  widget = widget, apinfo = apinfo, cbinfo = cbinfo;

  PtFileSelectionInfo_t info;
  int k;
  memset(&info, 0x0, sizeof(PtFileSelectionInfo_t));
  info.num_args = 0;

  k = showFileSelector(&info, "Save As...", "Save");

  if (k) return Pt_CONTINUE;

  if (info.ret == Pt_FSDIALOG_BTN1)
  {
    if (scada_ed_global_vars.filepath != NULL)
      free(scada_ed_global_vars.filepath);

    scada_ed_global_vars.filepath = (char *)malloc(sizeof(info.path));
    strcpy(scada_ed_global_vars.filepath, info.path);
    save_data();
  }

  return Pt_CONTINUE;
}
