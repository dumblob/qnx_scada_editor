
/* abmain.c */

/* app_exit.c */
int app_exit ( PtWidget_t *widget , ApInfo_t *apinfo , PtCallbackInfo_t *cbinfo );

/* copy_text.c */
int copy_text ( PtWidget_t *widget , ApInfo_t *apinfo , PtCallbackInfo_t *cbinfo );

/* cut_text.c */
int cut_text ( PtWidget_t *widget , ApInfo_t *apinfo , PtCallbackInfo_t *cbinfo );

/* dataloader.c */
int parseFile ( char *filename , char *viewname );
void loadViewAndData ( void );

void init ( void );
void destroy ( void );

/* datasaver.c */
void save_data ( void );
void generateXMLfromTree ( void );


/* new_file.c */
int new_file ( PtWidget_t *widget , ApInfo_t *apinfo , PtCallbackInfo_t *cbinfo );

/* on_tree_selection.c */
int on_tree_selection ( PtWidget_t *widget , ApInfo_t *apinfo , PtCallbackInfo_t *cbinfo );

/* open_file.c */
int open_file ( PtWidget_t *widget , ApInfo_t *apinfo , PtCallbackInfo_t *cbinfo );
int showFileSelector ( PtFileSelectionInfo_t *info , const char *title );

/* paste_text.c */
int paste_text ( PtWidget_t *widget , ApInfo_t *apinfo , PtCallbackInfo_t *cbinfo );

/* save_file.c */
int save_file ( PtWidget_t *widget , ApInfo_t *apinfo , PtCallbackInfo_t *cbinfo );

/* save_file_as.c */
int save_file_as ( PtWidget_t *widget , ApInfo_t *apinfo , PtCallbackInfo_t *cbinfo );

/* table.c */
int tblDestructionCB ( PtWidget_t *w , void *data , PtCallbackInfo_t *cbinfo );
PtWidget_t *tblInit ( PtWidget_t *parent , PhPoint_t pos , PhDim_t dim );
int tblSetSize ( PtWidget_t *scrollCon , int cols , int rows );
int tblLastIndex ( PtWidget_t *scrollCon , short row );
int tblSetDim ( PtWidget_t *scrollCon , PhPoint_t pos , PhDim_t dim );
int tblExeOnCellArea ( PtWidget_t *scrollCon , int tlx , int tly , int brx , int bry , PtWidgetClassRef_t *class , int n_args , PtArg_t const *args );
int tblGetCellResource ( PtWidget_t *scrollCon , int col , int row , long type , void *value , long len );
int tblSetColWidth ( PtWidget_t *scrollCon , int col , int size );
int tblSetRowHeight ( PtWidget_t *scrollCon , int row , int size );
