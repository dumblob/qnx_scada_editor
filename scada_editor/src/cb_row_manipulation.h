/* Jan Pacner xpacne00@stud.fit.vutbr.cz 2012-08-11 07:51:28 CEST */

/* Standard headers */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>

/* Local headers */
#include "ablibs.h"
#include "abimport.h"
#include "proto.h"

#include "dataloader.h"

#define ROWS_TO_ADD 1

int cb_add_row_before(PtWidget_t *, ApInfo_t *, PtCallbackInfo_t *);
int cb_add_row_after (PtWidget_t *, ApInfo_t *, PtCallbackInfo_t *);
int cb_rem_row       (PtWidget_t *, ApInfo_t *, PtCallbackInfo_t *);
