#include "filepicker.h"

int showFileSelector(PtFileSelectionInfo_t *info, const char *title, const char *action_button_name)
{
  memset(info, 0x0, sizeof(PtFileSelectionInfo_t));
  info->num_args = 0;

  int k = PtFileSelection(
      NULL,  /* parent */
      NULL,  /* pos */
      title, /* title */
      "~",   /* root_dir, tilde is the home directory specified by $HOME */
      NULL,  /* file_spec filter */
      action_button_name,  /* label of btn1, the Open button, default is "Open" */
      NULL,  /* label of btn2, the Cancel button, default is "Cancel" */
      NULL,  /* Pt_ARG_FS_FORMAT resource of the PtFileSel widget, default "nsd" */
      info,  /* PtFileSelectionInfo_t *info structure, must be specified */
      Pt_FSR_SHOW_HIDDEN | Pt_FSR_NO_FCHECK /* PtFileSelection flags */
      );

  if (k != -1 && info->ret != Pt_FSDIALOG_BTN1) return -1;

  return k;
}
