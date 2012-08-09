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
#include "open_file.h"

char *filepath = NULL;
char *viewpath = NULL;

int open_file(PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo)
{
  /* eliminate 'unreferenced' warnings */
  widget = widget, apinfo = apinfo, cbinfo = cbinfo;

  int dataf = -1;
  int formatf = -1;
  PtFileSelectionInfo_t datafile;
  PtFileSelectionInfo_t formatfile;
  formatf = showFileSelector(&formatfile, "Select format file...");

  if (formatf == -1) return Pt_CONTINUE;

  dataf   = showFileSelector(&datafile,   "Select data file...");

  if (dataf != -1)
  {
    if (filepath != NULL) free(filepath);

    if (viewpath != NULL) free(viewpath);

    filepath = (char*) malloc(sizeof(datafile.path));
    viewpath = (char*) malloc(sizeof(formatfile.path));
    strcpy(filepath, datafile.path);
    strcpy(viewpath, formatfile.path);
    parseFile(datafile.path, formatfile.path);
  }

  return Pt_CONTINUE;
}

int showFileSelector(PtFileSelectionInfo_t *info, const char *title)
{
  memset(info, 0x0, sizeof(PtFileSelectionInfo_t));
  info->num_args = 0;

  int k = PtFileSelection(
      NULL,  /* parent */
      NULL,  /* pos */
      title, /* title */
      "~",   /* root_dir, tilde is the home directory specified by $HOME */
      NULL,  /* file_spec filter */
      NULL,  /* label of btn1, the Open button, default is "Open" */
      NULL,  /* label of btn2, the Cancel button, default is "Cancel" */
      NULL,  /* Pt_ARG_FS_FORMAT resource of the PtFileSel widget, default "nsd" */
      info,  /* PtFileSelectionInfo_t *info structure, must be specified */
      Pt_FSR_SHOW_HIDDEN | Pt_FSR_NO_FCHECK /* PtFileSelection flags */
      );

  if (k != -1 && info->ret != Pt_FSDIALOG_BTN1) return -1;

  return k;
}
