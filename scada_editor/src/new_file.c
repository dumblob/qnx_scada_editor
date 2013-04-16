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

#include "dataloader.h"
#include "filepicker.h"
#include "free_memory.h"
#include "global_vars.h"

extern struct scada_ed_global_vars_s scada_ed_global_vars;

/** uses scada_ed_global_vars */
int new_file(PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo)
{
  /* eliminate 'unreferenced' warnings */
  widget = widget, apinfo = apinfo, cbinfo = cbinfo;

  int formatf = -1;

  PtFileSelectionInfo_t formatfile;
  formatf = showFileSelector(&formatfile, "Select format file...", "Select");

  if (formatf == -1)
  {
    return Pt_CONTINUE;
  }
  else
  {
    freeAllMemory();

    if (scada_ed_global_vars.viewpath != NULL)
      free(scada_ed_global_vars.viewpath);

    if ((scada_ed_global_vars.viewpath =
          (char*)malloc(sizeof(formatfile.path))) == NULL)
      PtExit(EXIT_FAILURE);

    strcpy(scada_ed_global_vars.viewpath, formatfile.path);
    parseFile(NULL, formatfile.path);
  }

  return Pt_CONTINUE;
}
