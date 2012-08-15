/* Link header for application - AppBuilder 2.03  */

extern ApContext_t AbContext;

ApWindowLink_t base = {
	"base.wgtw",
	&AbContext,
	AbLinks_base, 0, 12
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
	13, 5, 6
	};

static ApItem_t ApItems_src_menu[ 3 ] = {
	{ 1, 1, 0, NULL, 0, "import_src", "Import", NULL },
	{ 1, 1, 0, NULL, 0, "export_src", "Export", NULL },
	{ 0, 0, NULL, NULL, 0, NULL, NULL, NULL } };

ApMenuLink_t src_menu = {
	"src_menu",
	"",
	NULL,
	NULL,
	2,
	ApItems_src_menu,
	& AbContext,
	AbLinks_src_menu,
	20, 2, 2
	};

