#ifndef __RENDERER_H__
#define __RENDERER_H__

#include <string>

namespace g3dcommon
{

  /**
   * Abstract renderer definition.
   * The abstract class defines a general framework for user-space renderers.
   * User space renderers can have different routines for drawing, and the
   * viewer will call the render function to display the output of user space
   * renderers. The viewer will also forward events such as buffer resizes to
   * the user space renderer for it to respond. It will also pass events that it
   * does not know how to handle to the renderer so that the renderer can define
   * its own control keybindings, etc.
   */
  class Renderer {
  public:

    /**
     * Virtual Destructor.
     * Each renderer implementation should define its own destructor
     * that takes care of freeing the resources that it uses.
     */
    virtual ~Renderer() { }

    /**
     * Initialize the renderer.
     * A renderer may have some initialization work to do before it is ready
     * to be used. The viewer will call the init function before using the
     * renderer in drawing.
     */
    virtual void Init() = 0;

    /**
     * Draw content.
     * Renderers are free to define their own routines for drawing to the
     * context. The viewer calls this function on every frame update.
     */
    virtual void Render() = 0;

    /**
     * Respond to buffer resize.
     * The viewer will inform the renderer of a context resize by calling
     * this function. The renderer has complete freedom to handle resizing,
     * and a good renderer implementation should handle resizes properly.
     * \param w The new width of the context
     * \param h The new height of the context
     */
    virtual void Resize(size_t w, size_t h) = 0;

    /*
    * Set render target and width and height of the render target.
    */
    virtual void SetRenderTarget(unsigned char* target, size_t width, size_t height) = 0;


    /**
     * Respond to cursor events.
     * The viewer itself does not really care about the cursor but it will take
     * the GLFW cursor events and forward the ones that matter to  the renderer.
     * The arguments are defined in screen space coordinates ( (0,0) at top
     * left corner of the window and (w,h) at the bottom right corner.
     * \param x the x coordinate of the cursor
     * \param y the y coordinate of the cursor
     */
    virtual void CursorEvent(float x, float y) { }

    /**
     * Respond to zoom event.
     * Like cursor events, the viewer itself does not care about the mouse wheel
     * either, but it will take the GLFW wheel events and forward them directly
     * to the renderer.
     * \param offset_x Scroll offset in x direction
     * \param offset_y Scroll offset in y direction
     */
    virtual void ScrollEvent(float offset_x, float offset_y) { }

    /**
     * Respond to mouse click event.
     * The viewer will always forward mouse click events to the renderer.
     * \param key The key that spawned the event. The mapping between the
     *        key values and the mouse buttons are given by the macros defined
     *        at the top of this file.
     * \param event The type of event. Possible values are 0, 1 and 2, which
     *        corresponds to the events defined in macros.
     * \param mods if any modifier keys are held down at the time of the event
     *        modifiers are defined in macros.
     */
    virtual void MouseEvent(int key, int event, unsigned char mods) { }

    /**
     * Respond to keyboard event.
     * The viewer will always forward key events to the renderer.
     * \param key The key that spawned the event. ASCII numbers are used for
     *        letter characters. Non-letter keys are selectively supported
     *        and are defined in macros.
     * \param event The type of event. Possible values are 0, 1 and 2, which
     *        corresponds to the events defined in macros.
     * \param mods if any modifier keys are held down at the time of the event
     *        modifiers are defined in macros.
     */
    virtual void KeyboardEvent(int key, int event, unsigned char mods) { }

    /**
     * Respond to character event.
     * The viewer will always forward character events to the renderer.
     * Note that whereas keyboard_event provides information about which
     * physical keys were pressed, char_event provides the final character
     * input produced by the operating system.  For instance, on macOS
     * the user might type an umlaut over a u by hitting option-u followed
     * by the u key.  In this case, this callback will provide the final
     * letter ü, rather than the sequence of key presses that generated it.
     * \codepoint Unicode code for character input by the user.
     */
    virtual void CharEvent(unsigned int codepoint) { }

    /**
     * Internal -
     * The viewer will tell the renderer if the screen is in HDPI mode.
     */
    void useHdpiRenederTarget() { useHdpi = true; }

  protected:
    // If the render target is using HIDPI.
    bool useHdpi; 

  };

}

#endif
