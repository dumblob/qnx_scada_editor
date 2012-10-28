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
#include "global_vars.h"

extern struct scada_editor_global_vars_s scada_editor_global_vars;

int save_file( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	if (scada_editor_global_vars.filepath == NULL)
  {
		return save_file_as(widget, apinfo, cbinfo);
	}

	save_data();

	return Pt_CONTINUE;
}
