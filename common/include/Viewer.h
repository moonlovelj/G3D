#ifndef __VIEWER_H__
#define __VIEWER_H__


namespace g3dcommon
{
  /**
  * Provides SDL context, window display, and event handling routines.
  * A user application may draw to the window's context by providing
  * a user renderer. The viewer manages other display components such as the
  * zoom views, text OSD, etc. It also takes care of window event handling and
  * event passing, through which the renderer may interact with user inputs.
  */
  class Viewer
  {

  };
}


#endif