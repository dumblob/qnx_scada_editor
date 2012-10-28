/*
 * Jan Pacner (xpacne00@stud.fit.vutbr.cz)
 * 2012-10-27 20:16:33 CEST
 */

#ifndef GLOBAL_VARS_H
#define GLOBAL_VARS_H

#include "xml_func.h"  /* t_variable_list */

struct scada_editor_global_vars_s {
  char *arg_conversion_script;
  char *filepath;
  char *viewpath;
  t_variable_list *l_head;
};

#endif
