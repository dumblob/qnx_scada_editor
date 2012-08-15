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
  formatf = showFileSelector(&formatfile, "Select format file...", "Open");

  if (formatf == -1) return Pt_CONTINUE;

  dataf   = showFileSelector(&datafile,   "Select data file...", "Open");

  if (dataf != -1)
  {
    freeAllMemory();

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
