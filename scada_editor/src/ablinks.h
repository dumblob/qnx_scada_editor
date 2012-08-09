/* Link header for application - AppBuilder 2.03  */

extern ApContext_t AbContext;

ApWindowLink_t base = {
	"base.wgtw",
	&AbContext,
	AbLinks_base, 0, 10
	};

static ApItem_t ApItems_file_menu[ 7 ] = {
	{ 1, 1, 0, NULL, 0, "m_new", "New file", NULL },
	{ 1, 1, 0, NULL, 0, "m_open", "Open", NULL },
	{ 1, 1, 0, NULL, 0, "m_save", "Save", NULL },
	{ 1, 1, 0, NULL, 0, "m_save_as", "Save As", NULL },
	{ 1, 16, 0, NULL, 4, "", "", NULL },
	{ 1, 1, 0, NULL, 0, "m_quit", "Quit", NULL },
	{ 0, 0, NULL, NULL, 0, NULL, NULL, NULL } };

ApMenuLink_t file_menu = {
	"file_menu",
	"",
	NULL,
	NULL,
	2,
	ApItems_file_menu,
	& AbContext,
	AbLinks_file_menu,
	12, 5, 6
	};

