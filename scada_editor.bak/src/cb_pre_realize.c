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

extern char **_argv;
extern int _argc;

/* global variable with the conversion script path */
char *arg_conversion_script = "src2xml.sh";

int cb_pre_realize(PtWidget_t *link_instance, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo)
{

  /* eliminate 'unreferenced' warnings */
  link_instance = link_instance, apinfo = apinfo, cbinfo = cbinfo;

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

      arg_conversion_script = _argv[i +1];
      break;
    }
  }

  printf("Will use script \"%s\" for conversion from src format to XML.\n",
      arg_conversion_script);

  return Pt_CONTINUE;
}

