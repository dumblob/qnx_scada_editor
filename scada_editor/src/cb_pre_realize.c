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

#include "global_vars.h"

/* provided by PhAB */
extern char **_argv;
extern int _argc;

extern struct scada_editor_global_vars_s scada_editor_global_vars;

int cb_pre_realize(PtWidget_t *link_instance, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo)
{
  /* eliminate 'unreferenced' warnings */
  link_instance = link_instance, apinfo = apinfo, cbinfo = cbinfo;

  /* conversion script path */
  scada_editor_global_vars.arg_conversion_script = "src2xml.sh";

  int i;
  for (i = 1; i < _argc; ++i)
  {
    if (strcmp(_argv[i], "-p") == 0)
    {
      if (_argv[i +1] == NULL)
      {
        fprintf(stderr, "ERROR: Empty script path given.\n");
        return Pt_END;
      }

      scada_editor_global_vars.arg_conversion_script = _argv[i +1];
      break;
    }
  }

  printf("Executable \"%s\" chosen for eventual src to XML conversion.\n",
      scada_editor_global_vars.arg_conversion_script);

  return Pt_CONTINUE;
}
