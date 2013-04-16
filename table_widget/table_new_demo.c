#include <Pt.h>
#include <assert.h>

int cb_resize00(PtWidget_t *w, void *d, PtCallbackInfo_t *cbinfo) {
  w = w;
  cbinfo = cbinfo;
  static int x = 0;

  //FIXME
  //PtReRealizeWidget( d );
  /*
  PtUnrealizeWidget( d );
  PtRealizeWidget( d );
  */

  return Pt_CONTINUE;
}

int main( int argc, char *argv[] )
{
  PtWidget_t *window, *container, *scroll, *group,
             *divider00, *divider01, *divider_rows,
             *column00, *column01;
  PtArg_t args[20];
  int i = 0;

  PhRect_t rect;
  PhPoint_t point;
  PhDim_t dim;

  rect.ul.x = 0; rect.ul.y = 0; // upper left
  rect.lr.x = 9; rect.lr.y = 9; // lower right
  point.x = 0; point.y = 0;
  dim.w = 250; dim.h = 150;
  //PhArea_t x = { .pos = { .x = 10, .y = 10 }, .size = { .w = 10, .h = 10 } };
  //PtSetArg( &args[i++], Pt_ARG_AREA, &x, 0);

  /* Create a window */
  i = 0;
  //PtSetArg( &args[i++], Pt_ARG_POS, &point, 0);
  //PtSetArg( &args[i++], Pt_ARG_DIM, &dim, 0);
  //PtSetArg( &args[i++], Pt_ARG_MIN_WIDTH, 300, 0);
  //PtSetArg( &args[i++], Pt_ARG_MIN_HEIGHT, 250, 0);
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

  PtRowLayoutInfo_t info;

  /* Set layout type and layout info */
  info.type = Pt_LAYOUT_VERTICAL;
  info.flags = Pt_ROW_PACK; // | Pt_ROW_WRAP;
  info.margin.ul.x = 0;  // upper left
  info.margin.ul.y = 0;
  info.margin.lr.x = 0;  // lower right
  info.margin.lr.y = 0;
  info.h_spacing = 0;
  info.v_spacing = 0;
  info = info;

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
  PtSetArg( &args[i++], Pt_ARG_RESIZE_FLAGS, Pt_RESIZE_XY_ALWAYS, 0 );
  PtSetArg( &args[i++], Pt_ARG_GROUP_ORIENTATION, Pt_GROUP_VERTICAL, 0 );
  //PtSetArg( &args[i++], Pt_ARG_GROUP_FLAGS, Pt_GROUP_EQUAL_SIZE, 0 );
  //PtSetArg( &args[i++], Pt_ARG_GROUP_FLAGS, Pt_GROUP_EXCLUSIVE, 0 );
  PtSetArg( &args[i++], Pt_ARG_GROUP_FLAGS, Pt_GROUP_STRETCH_HORIZONTAL, 0 );
  //PtSetArg( &args[i++], Pt_ARG_GROUP_ROWS_COLS, 0, 0 );
  group = PtCreateWidget( PtGroup, scroll, i, args );

  PtCallback_t callbacks[1];

  i = 0;
  callbacks[0].event_f = cb_resize00;
  callbacks[0].data = group;
  PtSetArg(&args[i++], Pt_CB_RESIZE, callbacks,
      sizeof(callbacks)/sizeof(callbacks[0]));
  PtSetArg( &args[i++], Pt_ARG_RESIZE_FLAGS, Pt_RESIZE_XY_ALWAYS, 0 );
  //PtSetArg( &args[i++], Pt_ARG_RESIZE_FLAGS, Pt_FALSE, Pt_RESIZE_XY_AS_REQUIRED );
  //PtSetArg( &args[i++], Pt_ARG_DIVIDER_FLAGS, Pt_TRUE, Pt_DIVIDER_LAST_IS_SPACER );
  //PtSetArg( &args[i++], Pt_ARG_DIVIDER_FLAGS, Pt_FALSE, Pt_DIVIDER_LAST_IS_SPACER );
  //PtSetArg( &args[i++], Pt_ARG_DIVIDER_OFFSET, 40, 0);
  PtSetArg( &args[i++], Pt_ARG_DIVIDER_FLAGS, Pt_FALSE, Pt_DIVIDER_RESIZE_BOTH );
  //PtSetArg( &args[i++], Pt_ARG_CONTAINER_FLAGS,
  //    Pt_ENABLE_CUA | Pt_ENABLE_CUA_ARROWS | Pt_AUTO_EXTENT, 0 );
  //PtSetArg( &args[i++], Pt_ARG_ANCHOR_FLAGS,
  //    Pt_BOTTOM_ANCHORED_BOTTOM | Pt_TOP_ANCHORED_TOP |
  //    Pt_LEFT_ANCHORED_LEFT     | Pt_RIGHT_ANCHORED_RIGHT,
  //    Pt_IS_ANCHORED);
      //Pt_BOTTOM_IS_ANCHORED | Pt_TOP_IS_ANCHORED |
      //Pt_LEFT_IS_ANCHORED   | Pt_RIGHT_IS_ANCHORED );
  divider00 = PtCreateWidget( PtDivider, group, i, args );

  callbacks[0].event_f = cb_resize00;
  callbacks[0].data = group;
  PtSetArg(&args[i++], Pt_CB_RESIZE, callbacks,
      sizeof(callbacks)/sizeof(callbacks[0]));
  PtSetArg( &args[i++], Pt_ARG_RESIZE_FLAGS, Pt_RESIZE_XY_ALWAYS, 0 );
  PtSetArg( &args[i++], Pt_ARG_DIVIDER_FLAGS, Pt_FALSE, Pt_DIVIDER_RESIZE_BOTH );
  divider01 = PtCreateWidget( PtDivider, group, i, args );

  /*
  PhArea_t _area = { .pos = { .x = 15, .y = 15 }, .size = { .w = 15, .h = 15 } };
  if (PtGetResource(parent, Pt_ARG_AREA, &_area, 0))
    PtExit(EXIT_FAILURE);
  printf("area %hu %hu %hu %hu\n",
      _area.pos.x, _area.pos.y, _area.size.w, _area.size.h);
  */

  i = 0;
  /*
  //dim.w = 150; dim.h = 100;
  //PtSetArg( &args[i++], Pt_ARG_DIM, &dim, 0);
  PtSetArg( &args[i++], Pt_ARG_WIDTH, 100, 0);
  //PtSetArg( &args[i++], Pt_ARG_GROUP_FLAGS,
  //    Pt_GROUP_EQUAL_SIZE | Pt_GROUP_EXCLUSIVE | Pt_GROUP_NO_SELECT_ALLOWED, 0 );
  //PtSetArg( &args[i++], Pt_ARG_GROUP_FLAGS, Pt_GROUP_STRETCH_HORIZONTAL, 0 );
  //PtSetArg( &args[i++], Pt_ARG_CELL_HORZ_ALIGN, Pt_GROUP_HORZ_CENTER, 0 );
  //PtSetArg( &args[i++], Pt_ARG_GROUP_ROWS_COLS, 10, 0 );
  PtSetArg( &args[i++], Pt_ARG_GROUP_FLAGS, Pt_GROUP_EQUAL_SIZE_HORIZONTAL, 0 );
  //PtSetArg( &args[i++], Pt_ARG_RESIZE_FLAGS, Pt_RESIZE_XY_AS_REQUIRED, 0 );
  PtSetArg( &args[i++], Pt_ARG_RESIZE_FLAGS, Pt_RESIZE_XY_ALWAYS, 0 );
  PtSetArg( &args[i++], Pt_ARG_GROUP_ORIENTATION, Pt_GROUP_VERTICAL, 0 );
  */
  /////////////////////
  PtSetArg( &args[i++], Pt_ARG_CONTAINER_FLAGS,
      Pt_AUTO_EXTENT | Pt_ENABLE_CUA | Pt_ENABLE_CUA_ARROWS, 0 );
//PtSetArg( &args[i++], Pt_ARG_LAYOUT, PtRowLayout, &info );
  //PtSetArg( &args[i++], Pt_ARG_MIN_WIDTH, 100, 0 );
  column00 = PtCreateWidget( PtContainer, scroll, i, args );
  PtRowLayoutInfo_t *_info;
  /*
  if (PtGetResource(column00, Pt_ARG_LAYOUT_INFO, &_info, 0))
    PtExit(EXIT_FAILURE);
  assert(_info->type == Pt_LAYOUT_VERTICAL && _info->flags == Pt_ROW_PACK &&
      _info->margin.ul.y == 0 && _info->h_spacing == 0);
  */

  i = 0;
  /*
  //PtSetArg( &args[i++], Pt_ARG_DIM, &dim, 0);
  PtSetArg( &args[i++], Pt_ARG_WIDTH, 100, 0);
  //PtSetArg( &args[i++], Pt_ARG_CONTAINER_FLAGS,
  //    Pt_AUTO_EXTENT | Pt_ENABLE_CUA | Pt_ENABLE_CUA_ARROWS, 0 );
  PtSetArg( &args[i++], Pt_ARG_GROUP_FLAGS, Pt_GROUP_EQUAL_SIZE_HORIZONTAL, 0 );
  PtSetArg( &args[i++], Pt_ARG_RESIZE_FLAGS, Pt_RESIZE_XY_ALWAYS, 0 );
  PtSetArg( &args[i++], Pt_ARG_GROUP_ORIENTATION, Pt_GROUP_VERTICAL, 0 );
  */
  /////////////////////
  PtSetArg( &args[i++], Pt_ARG_CONTAINER_FLAGS,
      Pt_AUTO_EXTENT | Pt_ENABLE_CUA | Pt_ENABLE_CUA_ARROWS, 0 );
//PtSetArg( &args[i++], Pt_ARG_LAYOUT, PtRowLayout, &info );
  column01 = PtCreateWidget( PtContainer, scroll, i, args );

	/*
  i = 0;
  //FIXME dummy "resizable" button
  //PtSetArg( &args[i++], Pt_ARG_TEXT_STRING, "", 0);
  PtSetArg( &args[i++], Pt_ARG_RESIZE_FLAGS, Pt_RESIZE_XY_ALWAYS, 0 );
  PtCreateWidget( PtButton, divider, i, args );
	*/

  /* column00 buttons */

  i = 0;
  PtSetArg( &args[i++], Pt_ARG_TEXT_STRING, "line00 col00 ---", 0 );
  //PtSetArg( &args[i++], Pt_ARG_RESIZE_FLAGS, Pt_RESIZE_XY_ALWAYS, 0 );
  PtCreateWidget( PtButton, divider00, i, args );

  i = 0;
  PtSetArg( &args[i++], Pt_ARG_TEXT_STRING, "line00 col01", 0 );
  //PtSetArg( &args[i++], Pt_ARG_RESIZE_FLAGS, Pt_RESIZE_XY_ALWAYS, 0 );
  PtCreateWidget( PtButton, divider00, i, args );

  i = 0;
  PtSetArg( &args[i++], Pt_ARG_TEXT_STRING, "line01 col00", 0 );
  //PtSetArg( &args[i++], Pt_ARG_RESIZE_FLAGS, Pt_RESIZE_XY_ALWAYS, 0 );
  PtCreateWidget( PtButton, divider01, i, args );

  i = 0;
  PtSetArg( &args[i++], Pt_ARG_TEXT_STRING, "line01 col01 ______", 0 );
  //PtSetArg( &args[i++], Pt_ARG_RESIZE_FLAGS, Pt_RESIZE_XY_ALWAYS, 0 );
  PtCreateWidget( PtButton, divider01, i, args );





  i = 0;
  PtSetArg( &args[i++], Pt_ARG_TEXT_STRING, "T1", 0 );
  //PtSetArg( &args[i++], Pt_ARG_LAYOUT_DATA, &data, PtGridLayout);
  //PtSetArg( &args[i++], Pt_ARG_RESIZE_FLAGS, Pt_RESIZE_XY_AS_REQUIRED, 0 );
  //PtSetArg( &args[i++], Pt_ARG_RESIZE_FLAGS, Pt_RESIZE_XY_ALWAYS, 0 );
//PtSetArg( &args[i++], Pt_ARG_WIDTH, 70, 0);
  PtCreateWidget( PtText, column00, i, args );

  i = 0;
  PtSetArg( &args[i++], Pt_ARG_TEXT_STRING, "Button 2 (two)", 0);
  //PtSetArg( &args[i++], Pt_ARG_RESIZE_FLAGS, Pt_RESIZE_XY_ALWAYS, 0 );
  PtCreateWidget( PtButton, column00, i, args );

  i = 0;
  PtSetArg( &args[i++], Pt_ARG_TEXT_STRING, "Butt 3", 0 );
  //PtSetArg( &args[i++], Pt_ARG_RESIZE_FLAGS, Pt_RESIZE_XY_ALWAYS, 0 );
  PtCreateWidget( PtButton, column00, i, args );

  /* column01 buttons */

  i = 0;
  PtSetArg( &args[i++], Pt_ARG_TEXT_STRING, "B4", 0 );
  //PtSetArg( &args[i++], Pt_ARG_RESIZE_FLAGS, Pt_RESIZE_XY_ALWAYS, 0 );
  PtCreateWidget( PtButton, column01, i, args );

  i = 0;
  PtSetArg( &args[i++], Pt_ARG_TEXT_STRING, "Butt 5", 0 );
  //PtSetArg( &args[i++], Pt_ARG_RESIZE_FLAGS, Pt_RESIZE_XY_ALWAYS, 0 );
  PtCreateWidget( PtButton, column01, i, args );

  i = 0;
  PtSetArg( &args[i++], Pt_ARG_TEXT_STRING, "Buttonchen 6", 0 );
  //PtSetArg( &args[i++], Pt_ARG_RESIZE_FLAGS, Pt_RESIZE_XY_ALWAYS, 0 );
  PtCreateWidget( PtButton, column01, i, args );

  PtRealizeWidget( window );
  PtMainLoop();

  return 0;
}
