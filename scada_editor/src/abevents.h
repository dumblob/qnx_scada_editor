/* Event header for application - AppBuilder 2.03  */

static const ApEventLink_t AbApplLinks[] = {
	{ 3, 0, 0L, 0L, 0L, &base, NULL, NULL, 0, cb_pre_realize, 0, 9, 0, 0, },
	{ 0 }
	};

static const ApEventLink_t AbLinks_base[] = {
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "base", 1009, cb_destroyed_base, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "tree_wgt", 23022, on_tree_selection, 0, 0, 0, 0, },
	{ 5, 0, 0L, 0L, 0L, &file_menu, NULL, "bm_file", 2007, NULL, 0, 1, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "b_rename_item", 2007, renameTreeItem, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "b_add_item_before", 2007, addTreeItemBefore, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "b_add_item_after", 2007, addTreeItemAfter, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "b_remove_item", 2007, removeTreeItem, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "b_cut", 2007, cut_text, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "b_copy", 2007, copy_text, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "b_paste", 2007, paste_text, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "b_add_row_before", 2007, cb_add_row_before, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "b_add_row_after", 2007, cb_add_row_after, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "b_rem_row", 2007, cb_rem_row, 0, 0, 0, 0, },
	{ 0 }
	};

static const ApEventLink_t AbLinks_file_menu[] = {
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "m_new", 2009, new_file, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "m_open", 2009, open_file, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "m_save", 2009, save_file, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "m_save_as", 2009, save_file_as, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "export_src", 2009, export_src, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "m_quit", 2009, app_exit, 0, 0, 0, 0, },
	{ 0 }
	};

const char ApOptions[] = AB_OPTIONS;
