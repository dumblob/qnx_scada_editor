/* Event header for application - AppBuilder 2.03  */

static const ApEventLink_t AbApplLinks[] = {
	{ 3, 0, 0L, 0L, 0L, &base, NULL, NULL, 0, NULL, 0, 0, 0, 0, },
	{ 0 }
	};

static const ApEventLink_t AbLinks_base[] = {
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "tree_wgt", 23022, on_tree_selection, 0, 0, 0, 0, },
	{ 5, 0, 0L, 0L, 0L, &file_menu, NULL, "bm_file", 2007, NULL, 0, 1, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "b_open", 2007, open_file, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "b_save", 2007, save_file, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "b_cut", 2007, cut_text, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "b_copy", 2007, copy_text, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "b_paste", 2007, paste_text, 0, 0, 0, 0, },
	{ 0 }
	};

static const ApEventLink_t AbLinks_file_menu[] = {
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "m_new", 2009, new_file, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "m_open", 2009, open_file, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "m_save", 2009, save_file, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "m_save_as", 2009, save_file_as, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "m_quit", 2009, app_exit, 0, 0, 0, 0, },
	{ 0 }
	};

const char ApOptions[] = AB_OPTIONS;
