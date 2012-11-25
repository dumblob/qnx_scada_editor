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
int open_file(PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo)
{
  /* eliminate 'unreferenced' warnings */
  widget = widget, apinfo = apinfo, cbinfo = cbinfo;

  int dataf = -1;
  PtFileSelectionInfo_t datafile;

  dataf = showFileSelector(&datafile, "Select data file...", "Open");

  if (dataf != -1)
  {
    freeAllMemory();

    if (scada_ed_global_vars.filepath != NULL)
      free(scada_ed_global_vars.filepath);

    /* datafile.path is on stack (defined as path[SOME_MAX]) */
    scada_ed_global_vars.filepath = (char*)malloc(sizeof(datafile.path));

    if (scada_ed_global_vars.filepath == NULL)
      PtExit(EXIT_FAILURE);

    strcpy(scada_ed_global_vars.filepath, datafile.path);

    parseFile(scada_ed_global_vars.filepath, NULL);
  }

  return Pt_CONTINUE;
}
