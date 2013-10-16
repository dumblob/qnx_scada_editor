#include <Pt.h>
#include <assert.h>
#include <stdbool.h>

//FIXME nakreslit obrazek te tabulkove hierarchie widgetu + info, ze
//  PtDivider == vylepseny PtGroup (a tedy ma obdobne moznosti + moznost drag)

struct global_var_s {
  PtWidget_t *some_widget00;
  bool PtMainLoop_entered;
} global_var = {
  .some_widget00 = NULL,
  .PtMainLoop_entered = false
};

#define IF_WGT_CLASS(widget, class) \
  if (PtWidgetIsClass((widget), (class))) printf("class " #class "\n")

void print_class(PtWidget_t *w) {
  IF_WGT_CLASS(w, PtBasic);
  IF_WGT_CLASS(w, PtButton);
  //IF_WGT_CLASS(w, PtClient);
  IF_WGT_CLASS(w, PtComboBox);
  IF_WGT_CLASS(w, PtCompound);
  IF_WGT_CLASS(w, PtContainer);
  IF_WGT_CLASS(w, PtDisjoint);
  IF_WGT_CLASS(w, PtDivider);
  IF_WGT_CLASS(w, PtGroup);
  IF_WGT_CLASS(w, PtLabel);
  IF_WGT_CLASS(w, PtLine);
  IF_WGT_CLASS(w, PtList);
  IF_WGT_CLASS(w, PtMenu);
  IF_WGT_CLASS(w, PtMenuBar);
  IF_WGT_CLASS(w, PtMenuButton);
  //IF_WGT_CLASS(w, PtMultitext);
  IF_WGT_CLASS(w, PtNumeric);
  IF_WGT_CLASS(w, PtNumericFloat);
  IF_WGT_CLASS(w, PtNumericInteger);
  IF_WGT_CLASS(w, PtOnOffButton);
  IF_WGT_CLASS(w, PtOSContainer);
  IF_WGT_CLASS(w, PtPane);
  IF_WGT_CLASS(w, PtPanelGroup);
  IF_WGT_CLASS(w, PtProgress);
  IF_WGT_CLASS(w, PtRaw);
  IF_WGT_CLASS(w, PtRawList);
  IF_WGT_CLASS(w, PtRawTree);
  IF_WGT_CLASS(w, PtRect);
  IF_WGT_CLASS(w, PtRegion);
  IF_WGT_CLASS(w, PtScrollArea);
  //IF_WGT_CLASS(w, PtScrollBar);
  IF_WGT_CLASS(w, PtScrollContainer);
  IF_WGT_CLASS(w, PtSeparator);
  //IF_WGT_CLASS(w, PtServer);
  IF_WGT_CLASS(w, PtSlider);
  IF_WGT_CLASS(w, PtTab);
  IF_WGT_CLASS(w, PtTerminal);
  IF_WGT_CLASS(w, PtText);
  IF_WGT_CLASS(w, PtTimer);
  IF_WGT_CLASS(w, PtToggleButton);
  IF_WGT_CLASS(w, PtToolbar);
  IF_WGT_CLASS(w, PtToolbarGroup);
  IF_WGT_CLASS(w, PtTree);
  IF_WGT_CLASS(w, PtTty);
  IF_WGT_CLASS(w, PtUpDown);
  //IF_WGT_CLASS(w, PtWebClient);
  IF_WGT_CLASS(w, PtWidget);
  IF_WGT_CLASS(w, PtWindow);
//  else {
//    printf("---------------------BAD CLASS!\n");
//  }
  fflush(stdout);
}

int cb_divider_drag00(PtWidget_t *divider, void *d, PtCallbackInfo_t *cbinfo) {
  PtWidget_t *group = d;

  static bool running = false;  // avoid recursive invocation
  if (running) {
    printf("RECURSIVE\n");
    fflush(stdout);
    return Pt_CONTINUE;
  }

  if (cbinfo->reason_subtype == Pt_CB_DIVIDER_SETRESOURCES) {
    printf("divider child is being resized using PtSetResource()\n");
    fflush(stdout);
    return Pt_CONTINUE;
  }

  running = true;

//  PtDividerSizes_t *ds;
//  short int len = 0;
//  if (PtGetResource(global_var.some_widget00,
//        Pt_ARG_DIVIDER_SIZES, &ds, &len)) {
//    printf("FAIL00\n");
//    PtExit(EXIT_FAILURE);
//  }

  PtDividerCallback_t *dvdr_cb = cbinfo->cbdata;
  //divider = PtWidgetParent(dvdr_cb->left);
  //divider = PtValidParent(
  //      PtWidgetParent(dvdr_cb->left),
  //      PtDivider);
//  assert(
//      PtValidParent(
//        dvdr_cb->left,
//        //PtWidgetParent(dvdr_cb->left),
//        PtDivider)
//      //PtWidgetParent(dvdr_cb->left)
//      == divider);
  printf("CLASS divider: ");
  print_class(divider);//FIXME
  printf("CLASS button|text: ");
  print_class(dvdr_cb->left);//FIXME
  fflush(stdout);

  /* find the index of the widget with changed dimension */
  PtWidget_t *tmp = dvdr_cb->left;
  int pos = 0;
  /* this callback is called only when there are at least 2 child widgets
     in PtDivider */
  while ((tmp = PtWidgetBrotherBehind(tmp)) != NULL) pos++;
  printf("00 pos %d\n", pos); fflush(stdout);

  // recalculate the width of all PtDivider brothers in front and behind
  PtWidget_t *row = divider;
  while ((row = PtWidgetBrotherBehind(row)) != NULL) {
    printf("CLASS of row: ");
    print_class(row);//FIXME

    /* last internally added widget in PtDivider is always PtGroup
        containing the user-defined widgets */
    tmp = PtWidgetChildBack(row);
    printf("CLASS group [in divider]: ");
    print_class(tmp);//FIXME

    tmp = PtWidgetChildBack(tmp);
    printf("CLASS of column: ");
    print_class(tmp);//FIXME

    int i = 0;
    for (i = 0; i < pos; i++) {
      printf("for i %d\n", i); fflush(stdout);
      assert(tmp != NULL);
      tmp = PtWidgetBrotherInFront(tmp);
      printf("CLASS of column [for]: ");
      print_class(tmp);//FIXME
    }

    //char buf[31] = {0};
    //PtGetResource(tmp, Pt_ARG_TEXT_STRING, &buf, 30);
    //printf("  &buf \"%s\"\n", buf);
    //char buff[31] = {0};
    //PtGetResource(tmp, Pt_ARG_TEXT_STRING, buff, 30);
    //printf("  buff \"%s\"\n", buff);

    PtSetResource(tmp, Pt_ARG_TEXT_STRING, "XXX", 0);

    PtSetResource(tmp, Pt_ARG_MIN_WIDTH, dvdr_cb->sizes[pos].to, 0);
    PtSetResource(tmp, Pt_ARG_MAX_WIDTH, dvdr_cb->sizes[pos].to, 0);
    printf("  pos %d [pos].to %hd\n\n", pos, dvdr_cb->sizes[pos].to);
    fflush(stdout);
  }
  //row = divider;
  //while ((row = PtWidgetBrotherBehind(row)) != NULL) {
  //  b
  //}

  //printf("len %hd", p->nsizes);
  //printf(" moved %d resized %d\n", p->moved, p->resized);
  //fflush(stdout);
//  if (p->nsizes > 0)
//    printf(" [0].from %hd [0].to %hd\n", p->sizes[0].from, p->sizes[0].to);
//  else
//    printf(" empty\n");
//
//  fflush(stdout);
  running = false;
  return Pt_CONTINUE;
}

int cb_resize00(PtWidget_t *w, void *d, PtCallbackInfo_t *cbinfo) {
  w = w;
  cbinfo = cbinfo;
  d = d;

  static int run = 0;
  if (run < 5) {
    printf("HOWK00\n");
    run++;
    return Pt_CONTINUE;
  }

  /* really nasty hack to overcome the fixed size/geometry issue (the value is unimportant) */
  PhDim_t dim;
  /* unfortunately, this needs to be called each time the width increases
     because one call to this callback extends the width by 1px (disregarding
     the dim variable) */
  if (PtSetResource(global_var.some_widget00, Pt_ARG_DIM, &dim, 0))
      PtExit(EXIT_FAILURE);
  printf("some_widget00 dim w %hd h %hd\n", dim.w, dim.h);

//  PhRect_t rect = {
//    // PhPoint_t
//    .ul = { /* short */ .x = 999, .y = 999 },
//    .lr = { .x = 999, .y = 999 }
//  };
//
//  if (PtSetResource(global_var.some_widget00, Pt_ARG_EXTENT, &rect, 0))
//      PtExit(EXIT_FAILURE);
//  printf("some_widget00 rect (extent) ul.x %hd ul.y %hd lr.x %hd lr.y %hd\n",
//      rect.ul.x,
//      rect.ul.y,
//      rect.lr.x,
//      rect.lr.y);
//
//  PhArea_t area = {
//    // PhPoint_t
//    .pos = { /* short */ .x = 999, .y = 999 },
//    // PhDim_t
//    .size = { .w = 999, .h = 999 }
//  };
//
//  if (PtSetResource(global_var.some_widget00, Pt_ARG_AREA, &area , 0))
//      PtExit(EXIT_FAILURE);
//  printf("some_widget00 area pos.x %hd pos.y %hd size.x %hd size.y %hd\n",
//      area.pos.x,
//      area.pos.y,
//      area.size.w,
//      area.size.h);

  //PtReRealizeWidget( d );  //FIXME segfault
  return Pt_CONTINUE;
}

int main( int argc, char *argv[] ) {
  PtWidget_t *window, *container, *scroll, *group,
             *divider00, *divider01,
             *column00, *column01,
             *row00, *row01;
  PtArg_t args[20];
  int i = 0;

  PhRect_t rect;
  PhPoint_t point;
  PhDim_t dim;

  rect.ul.x = 0; rect.ul.y = 0; // upper left
  rect.lr.x = 9; rect.lr.y = 9; // lower right
  point.x = 0; point.y = 0;
  dim.w = 350; dim.h = 250;
  //PhArea_t x = { .pos = { .x = 10, .y = 10 }, .size = { .w = 10, .h = 10 } };
  //PtSetArg( &args[i++], Pt_ARG_AREA, &x, 0);

  /* Create a window */
  i = 0;
  //PtSetArg( &args[i++], Pt_ARG_POS, &point, 0);
  //PtSetArg( &args[i++], Pt_ARG_DIM, &dim, 0);
  //PtSetArg( &args[i++], Pt_ARG_MIN_WIDTH, 300, 0);
  //PtSetArg( &args[i++], Pt_ARG_MIN_HEIGHT, 250, 0);
  PtSetArg( &args[i++], Pt_ARG_MAX_WIDTH, 350, 0);
  PtSetArg( &args[i++], Pt_ARG_MAX_HEIGHT, 350, 0);
  //PtSetArg( &args[i++], Pt_ARG_RESIZE_FLAGS, Pt_RESIZE_XY_ALWAYS, 0 );
  PtSetArg( &args[i++], Pt_ARG_WINDOW_TITLE, "demo window", 0 );
  if ( NULL == ( window = PtAppInit( NULL, &argc, argv, i, args ) ) ) {
    perror( "PtAppInit()" );
    return 1;
  }

  i = 0;
  PtSetArg( &args[i++], Pt_ARG_ANCHOR_FLAGS,
      Pt_BOTTOM_ANCHORED_BOTTOM | Pt_TOP_ANCHORED_TOP |
      Pt_LEFT_ANCHORED_LEFT     | Pt_RIGHT_ANCHORED_RIGHT,
      Pt_IS_ANCHORED);
  container = PtCreateWidget( PtContainer, window, i, args );

  i = 0;
  //PtSetArg( &args[i++], Pt_ARG_BASIC_FLAGS, Pt_FALSE,
  //    Pt_TOP_OUTLINE | Pt_LEFT_OUTLINE );
      //Pt_ALL_ETCHES | Pt_ALL_OUTLINES | Pt_FLAT_FILL | Pt_ALL_BEVELS );
  //PtSetArg( &args[i++], Pt_ARG_POINTER, 0, 0 );
  //PtSetArg( &args[i++], Pt_ARG_RESIZE_FLAGS, Pt_RESIZE_XY_ALWAYS, 0 );
  //PtSetArg( &args[i++], Pt_ARG_ANCHOR_OFFSETS, &rect, 0 );
//PtSetArg( &args[i++], Pt_ARG_LAYOUT, PtRowLayout, &info );
  PtSetArg( &args[i++], Pt_ARG_ANCHOR_FLAGS,
      Pt_BOTTOM_ANCHORED_BOTTOM | Pt_TOP_ANCHORED_TOP |
      Pt_LEFT_ANCHORED_LEFT     | Pt_RIGHT_ANCHORED_RIGHT,
      Pt_IS_ANCHORED);
  scroll = PtCreateWidget( PtScrollContainer, container, i, args );

  i = 0;
  //PtSetArg( &args[i++], Pt_ARG_RESIZE_FLAGS, Pt_RESIZE_XY_ALWAYS, 0 );
  PtSetArg( &args[i++], Pt_ARG_GROUP_ORIENTATION, Pt_GROUP_VERTICAL, 0 );
  //PtSetArg( &args[i++], Pt_ARG_GROUP_FLAGS, Pt_GROUP_EQUAL_SIZE, 0 );
  //PtSetArg( &args[i++], Pt_ARG_GROUP_FLAGS, Pt_GROUP_EXCLUSIVE, 0 );
  //PtSetArg( &args[i++], Pt_ARG_GROUP_FLAGS, Pt_GROUP_STRETCH_HORIZONTAL, 0 );
  //PtSetArg( &args[i++], Pt_ARG_GROUP_ROWS_COLS, 0, 0 );
  /* TODO exchange PtGroup for PtDivider to gain vertically adjustable
       heigh of items */
  group = PtCreateWidget( PtGroup, scroll, i, args );
  global_var.some_widget00 = group;

  PtCallback_t callbacks_resize[1];
  PtCallback_t callbacks_divider_drag[1];
  callbacks_resize[0].event_f = cb_resize00;
  callbacks_resize[0].data = group;
  callbacks_divider_drag[0].event_f = cb_divider_drag00;
  callbacks_divider_drag[0].data = group;

  i = 0;
  PtSetArg(&args[i++], Pt_CB_RESIZE, callbacks_resize,
      sizeof(callbacks_resize)/sizeof(callbacks_resize[0]));
  PtSetArg(&args[i++], Pt_CB_DIVIDER_DRAG, callbacks_divider_drag,
      sizeof(callbacks_divider_drag)/sizeof(callbacks_divider_drag[0]));
  PtSetArg( &args[i++], Pt_ARG_RESIZE_FLAGS, Pt_RESIZE_XY_ALWAYS, 0 );
  //PtSetArg( &args[i++], Pt_ARG_RESIZE_FLAGS, Pt_FALSE, Pt_RESIZE_XY_AS_REQUIRED );
  //PtSetArg( &args[i++], Pt_ARG_DIVIDER_FLAGS, Pt_TRUE, Pt_DIVIDER_LAST_IS_SPACER );
  //PtSetArg( &args[i++], Pt_ARG_DIVIDER_FLAGS, Pt_FALSE, Pt_DIVIDER_LAST_IS_SPACER );
  //PtSetArg( &args[i++], Pt_ARG_DIVIDER_OFFSET, 40, 0);
  PtSetArg( &args[i++], Pt_ARG_DIVIDER_FLAGS, Pt_FALSE, Pt_DIVIDER_RESIZE_BOTH );
  //PtSetArg( &args[i++], Pt_ARG_DIVIDER_FLAGS, Pt_TRUE, Pt_DIVIDER_NORESIZE);
  //PtSetArg( &args[i++], Pt_ARG_CONTAINER_FLAGS,
  //    Pt_ENABLE_CUA | Pt_ENABLE_CUA_ARROWS | Pt_AUTO_EXTENT, 0 );
  //PtSetArg( &args[i++], Pt_ARG_ANCHOR_FLAGS,
  //    Pt_BOTTOM_ANCHORED_BOTTOM | Pt_TOP_ANCHORED_TOP |
  //    Pt_LEFT_ANCHORED_LEFT     | Pt_RIGHT_ANCHORED_RIGHT,
  //    Pt_IS_ANCHORED);
      //Pt_BOTTOM_IS_ANCHORED | Pt_TOP_IS_ANCHORED |
      //Pt_LEFT_IS_ANCHORED   | Pt_RIGHT_IS_ANCHORED );
  /* invoke CB if the subordinate childs changed geometry themselves */
  PtSetArg( &args[i++], Pt_ARG_FLAGS, Pt_TRUE, Pt_CALLBACKS_ACTIVE );
  //PtSetArg( &args[i++], Pt_ARG_DIVIDER_OFFSET, 30, 0);
  /* disable PtDivider reaction on it's child's position&dimension changes */
  //PtSetArg( &args[i++], Pt_ARG_CONTAINER_FLAGS, Pt_FALSE, Pt_CHILD_MOVED_RESIZED);
  divider00 = PtCreateWidget( PtDivider, group, i, args );

  i = 0;
  PtSetArg(&args[i++], Pt_CB_RESIZE, callbacks_resize,
      sizeof(callbacks_resize)/sizeof(callbacks_resize[0]));
  PtSetArg(&args[i++], Pt_CB_DIVIDER_DRAG, callbacks_divider_drag,
      sizeof(callbacks_divider_drag)/sizeof(callbacks_divider_drag[0]));
  PtSetArg( &args[i++], Pt_ARG_RESIZE_FLAGS, Pt_RESIZE_XY_ALWAYS, 0 );
  PtSetArg( &args[i++], Pt_ARG_DIVIDER_FLAGS, Pt_FALSE, Pt_DIVIDER_RESIZE_BOTH );
  //PtSetArg( &args[i++], Pt_ARG_DIVIDER_FLAGS, Pt_TRUE, Pt_DIVIDER_NORESIZE);
  PtSetArg( &args[i++], Pt_ARG_FLAGS, Pt_TRUE, Pt_CALLBACKS_ACTIVE );
  //PtSetArg( &args[i++], Pt_ARG_DIVIDER_OFFSET, 30, 0);
  //PtSetArg( &args[i++], Pt_ARG_CONTAINER_FLAGS, Pt_FALSE, Pt_CHILD_MOVED_RESIZED);
  divider01 = PtCreateWidget( PtDivider, group, i, args );

  /*
  PhArea_t _area = { .pos = { .x = 15, .y = 15 }, .size = { .w = 15, .h = 15 } };
  if (PtGetResource(parent, Pt_ARG_AREA, &_area, 0))
    PtExit(EXIT_FAILURE);
  printf("area %hu %hu %hu %hu\n",
      _area.pos.x, _area.pos.y, _area.size.w, _area.size.h);
  */

//  i = 0;
//  /*
//  //dim.w = 150; dim.h = 100;
//  //PtSetArg( &args[i++], Pt_ARG_DIM, &dim, 0);
//  PtSetArg( &args[i++], Pt_ARG_WIDTH, 100, 0);
//  //PtSetArg( &args[i++], Pt_ARG_GROUP_FLAGS,
//  //    Pt_GROUP_EQUAL_SIZE | Pt_GROUP_EXCLUSIVE | Pt_GROUP_NO_SELECT_ALLOWED, 0 );
//  //PtSetArg( &args[i++], Pt_ARG_GROUP_FLAGS, Pt_GROUP_STRETCH_HORIZONTAL, 0 );
//  //PtSetArg( &args[i++], Pt_ARG_CELL_HORZ_ALIGN, Pt_GROUP_HORZ_CENTER, 0 );
//  //PtSetArg( &args[i++], Pt_ARG_GROUP_ROWS_COLS, 10, 0 );
//  PtSetArg( &args[i++], Pt_ARG_GROUP_FLAGS, Pt_GROUP_EQUAL_SIZE_HORIZONTAL, 0 );
//  //PtSetArg( &args[i++], Pt_ARG_RESIZE_FLAGS, Pt_RESIZE_XY_AS_REQUIRED, 0 );
//  PtSetArg( &args[i++], Pt_ARG_RESIZE_FLAGS, Pt_RESIZE_XY_ALWAYS, 0 );
//  PtSetArg( &args[i++], Pt_ARG_GROUP_ORIENTATION, Pt_GROUP_VERTICAL, 0 );
//  */
//  /////////////////////
//  PtSetArg( &args[i++], Pt_ARG_CONTAINER_FLAGS,
//      Pt_AUTO_EXTENT | Pt_ENABLE_CUA | Pt_ENABLE_CUA_ARROWS, 0 );
////PtSetArg( &args[i++], Pt_ARG_LAYOUT, PtRowLayout, &info );
//  //PtSetArg( &args[i++], Pt_ARG_MIN_WIDTH, 100, 0 );
//  column00 = PtCreateWidget( PtContainer, scroll, i, args );
//  /*
//  PtRowLayoutInfo_t *_info;
//  if (PtGetResource(column00, Pt_ARG_LAYOUT_INFO, &_info, 0))
//    PtExit(EXIT_FAILURE);
//  assert(_info->type == Pt_LAYOUT_VERTICAL && _info->flags == Pt_ROW_PACK &&
//      _info->margin.ul.y == 0 && _info->h_spacing == 0);
//  */
//
//  i = 0;
//  /*
//  //PtSetArg( &args[i++], Pt_ARG_DIM, &dim, 0);
//  PtSetArg( &args[i++], Pt_ARG_WIDTH, 100, 0);
//  //PtSetArg( &args[i++], Pt_ARG_CONTAINER_FLAGS,
//  //    Pt_AUTO_EXTENT | Pt_ENABLE_CUA | Pt_ENABLE_CUA_ARROWS, 0 );
//  PtSetArg( &args[i++], Pt_ARG_GROUP_FLAGS, Pt_GROUP_EQUAL_SIZE_HORIZONTAL, 0 );
//  PtSetArg( &args[i++], Pt_ARG_RESIZE_FLAGS, Pt_RESIZE_XY_ALWAYS, 0 );
//  PtSetArg( &args[i++], Pt_ARG_GROUP_ORIENTATION, Pt_GROUP_VERTICAL, 0 );
//  */
//  /////////////////////
//  PtSetArg( &args[i++], Pt_ARG_CONTAINER_FLAGS,
//      Pt_AUTO_EXTENT | Pt_ENABLE_CUA | Pt_ENABLE_CUA_ARROWS, 0 );
////PtSetArg( &args[i++], Pt_ARG_LAYOUT, PtRowLayout, &info );
//  column01 = PtCreateWidget( PtContainer, scroll, i, args );
//
//  /*
//  i = 0;
//  //FIXME dummy "resizable" button
//  //PtSetArg( &args[i++], Pt_ARG_TEXT_STRING, "", 0);
//  PtSetArg( &args[i++], Pt_ARG_RESIZE_FLAGS, Pt_RESIZE_XY_ALWAYS, 0 );
//  PtCreateWidget( PtButton, divider, i, args );
//  */
//
//  i = 0;
//  PtSetArg( &args[i++], Pt_ARG_TEXT_STRING, "line00 col00 ---", 0 );
//  //PtSetArg( &args[i++], Pt_ARG_RESIZE_FLAGS, Pt_RESIZE_XY_ALWAYS, 0 );
//  PtCreateWidget( PtButton, divider00, i, args );
//
//  i = 0;
//  PtSetArg( &args[i++], Pt_ARG_TEXT_STRING, "line00 col01", 0 );
//  //PtSetArg( &args[i++], Pt_ARG_RESIZE_FLAGS, Pt_RESIZE_XY_ALWAYS, 0 );
//  PtCreateWidget( PtButton, divider00, i, args );
//
//  i = 0;
//  PtSetArg( &args[i++], Pt_ARG_TEXT_STRING, "line01 col00", 0 );
//  //PtSetArg( &args[i++], Pt_ARG_RESIZE_FLAGS, Pt_RESIZE_XY_ALWAYS, 0 );
//  PtCreateWidget( PtButton, divider01, i, args );
//
//  i = 0;
//  PtSetArg( &args[i++], Pt_ARG_TEXT_STRING, "line01 col01 ______", 0 );
//  //PtSetArg( &args[i++], Pt_ARG_RESIZE_FLAGS, Pt_RESIZE_XY_ALWAYS, 0 );
//  PtCreateWidget( PtButton, divider01, i, args );




  /* row00 buttons */

  row00 = divider00;
  row01 = divider01;

//  PtSetArg(&args[i++], Pt_CB_RESIZE, callbacks_resize,
//      sizeof(callbacks_resize)/sizeof(callbacks_resize[0]));
//  PtSetArg(&args[i++], Pt_CB_DIVIDER_DRAG, callbacks_divider_drag,
//      sizeof(callbacks_divider_drag)/sizeof(callbacks_divider_drag[0]));
  i = 0;
  //dim.w = 30; dim.h = 50;
  //PtSetArg( &args[i++], Pt_ARG_DIM, &dim, 0);
  PtSetArg( &args[i++], Pt_ARG_TEXT_STRING, "T1", 0 );
  //PtSetArg( &args[i++], Pt_ARG_LAYOUT_DATA, &data, PtGridLayout);
  //PtSetArg( &args[i++], Pt_ARG_RESIZE_FLAGS, Pt_RESIZE_XY_AS_REQUIRED, 0 );
  //PtSetArg( &args[i++], Pt_ARG_RESIZE_FLAGS, Pt_RESIZE_XY_ALWAYS, 0 );
  PtSetArg( &args[i++], Pt_ARG_WIDTH, 40, 0);
  //PtSetArg( &args[i++], Pt_ARG_MIN_WIDTH, 40, 0);
  //PtSetArg( &args[i++], Pt_ARG_MAX_WIDTH, 40, 0);
  PtSetArg( &args[i++], Pt_ARG_RESIZE_FLAGS, Pt_RESIZE_XY_ALWAYS, 0 );
  PtCreateWidget( PtText, row00, i, args );

  i = 0;
  //dim.w = 30; dim.h = 50;
  //PtSetArg( &args[i++], Pt_ARG_DIM, &dim, 0);
  PtSetArg( &args[i++], Pt_ARG_TEXT_STRING, "Button 2 (two)", 0);
  //PtSetArg( &args[i++], Pt_ARG_RESIZE_FLAGS, Pt_RESIZE_XY_ALWAYS, 0 );
//  PtSetArg( &args[i++], Pt_ARG_MIN_WIDTH, 30, 0);
//  PtSetArg( &args[i++], Pt_ARG_MAX_WIDTH, 30, 0);
  PtSetArg( &args[i++], Pt_ARG_RESIZE_FLAGS, Pt_RESIZE_XY_ALWAYS, 0 );
  PtCreateWidget( PtButton, row00, i, args );

  i = 0;
  //dim.w = 30; dim.h = 50;
  //PtSetArg( &args[i++], Pt_ARG_DIM, &dim, 0);
  PtSetArg( &args[i++], Pt_ARG_TEXT_STRING, "Butt 3", 0 );
  //PtSetArg( &args[i++], Pt_ARG_RESIZE_FLAGS, Pt_RESIZE_XY_ALWAYS, 0 );
//  PtSetArg( &args[i++], Pt_ARG_MIN_WIDTH, 30, 0);
//  PtSetArg( &args[i++], Pt_ARG_MAX_WIDTH, 30, 0);
  PtSetArg( &args[i++], Pt_ARG_RESIZE_FLAGS, Pt_RESIZE_XY_ALWAYS, 0 );
  PtCreateWidget( PtButton, row00, i, args );

  /* row01 buttons */

  i = 0;
  //dim.w = 30; dim.h = 50;
  //PtSetArg( &args[i++], Pt_ARG_DIM, &dim, 0);
  PtSetArg( &args[i++], Pt_ARG_TEXT_STRING, "B4", 0 );
  //PtSetArg( &args[i++], Pt_ARG_RESIZE_FLAGS, Pt_RESIZE_XY_ALWAYS, 0 );
//  PtSetArg( &args[i++], Pt_ARG_MIN_WIDTH, 30, 0);
//  PtSetArg( &args[i++], Pt_ARG_MAX_WIDTH, 30, 0);
  PtSetArg( &args[i++], Pt_ARG_RESIZE_FLAGS, Pt_RESIZE_XY_ALWAYS, 0 );
  PtCreateWidget( PtButton, row01, i, args );

  i = 0;
  //dim.w = 30; dim.h = 50;
  //PtSetArg( &args[i++], Pt_ARG_DIM, &dim, 0);
  PtSetArg( &args[i++], Pt_ARG_TEXT_STRING, "Butt 5", 0 );
  //PtSetArg( &args[i++], Pt_ARG_RESIZE_FLAGS, Pt_RESIZE_XY_ALWAYS, 0 );
//  PtSetArg( &args[i++], Pt_ARG_MIN_WIDTH, 30, 0);
//  PtSetArg( &args[i++], Pt_ARG_MAX_WIDTH, 30, 0);
  PtSetArg( &args[i++], Pt_ARG_RESIZE_FLAGS, Pt_RESIZE_XY_ALWAYS, 0 );
  PtCreateWidget( PtButton, row01, i, args );

  i = 0;
  //dim.w = 30; dim.h = 50;
  //PtSetArg( &args[i++], Pt_ARG_DIM, &dim, 0);
  PtSetArg( &args[i++], Pt_ARG_TEXT_STRING, "Buttonchen 6", 0 );
  //PtSetArg( &args[i++], Pt_ARG_MAXIMUM_DIM, 30, 0);
//  PtSetArg( &args[i++], Pt_ARG_MIN_WIDTH, 30, 0);
//  PtSetArg( &args[i++], Pt_ARG_MAX_WIDTH, 30, 0);
  PtSetArg( &args[i++], Pt_ARG_RESIZE_FLAGS, Pt_RESIZE_XY_ALWAYS, 0 );
  PtCreateWidget( PtButton, row01, i, args );

printf("HOWK55\n");
  PtRealizeWidget( window );
  global_var.PtMainLoop_entered = true;
printf("HOWK77\n");
  PtMainLoop();

  return 0;
}
