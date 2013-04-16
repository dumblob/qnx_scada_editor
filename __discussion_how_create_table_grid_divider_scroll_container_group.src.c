#include <Pt.h>

int main( int argc, char *argv[] )
{
  PtWidget_t *window, *container, *grid, *divider, *column00, *column01;
  PtArg_t args[20];
  int i = 0;

  /* Create a window */
  i = 0;
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
  PtSetArg( &args[i++], Pt_ARG_ANCHOR_FLAGS,
      Pt_BOTTOM_ANCHORED_BOTTOM | Pt_TOP_ANCHORED_TOP |
      Pt_LEFT_ANCHORED_LEFT     | Pt_RIGHT_ANCHORED_RIGHT,
      Pt_IS_ANCHORED);
  grid = PtCreateWidget( PtScrollContainer, container, i, args );

  i = 0;
  PtSetArg( &args[i++], Pt_ARG_RESIZE_FLAGS, Pt_RESIZE_XY_ALWAYS, 0 );
  PtSetArg( &args[i++], Pt_ARG_DIVIDER_FLAGS, Pt_FALSE, Pt_DIVIDER_RESIZE_BOTH );
  //PtSetArg( &args[i++], Pt_ARG_CONTAINER_FLAGS,
  //    Pt_ENABLE_CUA | Pt_ENABLE_CUA_ARROWS | Pt_AUTO_EXTENT, 0 );
  divider = PtCreateWidget( PtDivider, grid, i, args );

  /* Set layout type and layout info */
  PtRowLayoutInfo_t info;
  info.type = Pt_LAYOUT_VERTICAL;
  info.flags = 0;  // all widgets will have the same size
  info.margin.ul.x = 0;  // upper left
  info.margin.ul.y = 0;
  info.margin.lr.x = 0;  // lower right
  info.margin.lr.y = 0;
  info.h_spacing = 0;
  info.v_spacing = 0;

  i = 0;
  PtSetArg( &args[i++], Pt_ARG_CONTAINER_FLAGS,
      Pt_AUTO_EXTENT | Pt_ENABLE_CUA | Pt_ENABLE_CUA_ARROWS, 0 );
  PtSetArg( &args[i++], Pt_ARG_LAYOUT, PtRowLayout, &info );
  column00 = PtCreateWidget( PtContainer, divider, i, args );

  i = 0;
  PtSetArg( &args[i++], Pt_ARG_CONTAINER_FLAGS,
      Pt_AUTO_EXTENT | Pt_ENABLE_CUA | Pt_ENABLE_CUA_ARROWS, 0 );
  PtSetArg( &args[i++], Pt_ARG_LAYOUT, PtRowLayout, &info );
  column01 = PtCreateWidget( PtContainer, divider, i, args );

  /* column00 buttons */

  i = 0;
  PtSetArg( &args[i++], Pt_ARG_TEXT_STRING, "T1", 0 );
  //PtSetArg( &args[i++], Pt_ARG_RESIZE_FLAGS, Pt_RESIZE_XY_ALWAYS, 0 );
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
