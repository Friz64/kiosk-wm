#include <X11/Xlib.h>

int main() {
    Display *display = XOpenDisplay(NULL);
    if (!display) return 1;

    Window root = DefaultRootWindow(display);

    XWindowAttributes attr;
    XGetWindowAttributes(display, root, &attr);

    int width = attr.width, height = attr.height;

    // This allows us to receive CreateNotify and ConfigureNotify events.
    XSelectInput(display, root, SubstructureNotifyMask | StructureNotifyMask);

    for(;;) {
        XEvent ev;
        XNextEvent(display, &ev);

        if (ev.type == CreateNotify) {
            // MoveResize all created windows.
            XMoveResizeWindow(display, ev.xcreatewindow.window, 0, 0, width, height);
        } else if (ev.type == ConfigureNotify) {
            XConfigureEvent ce = ev.xconfigure;
            if (ce.window == root) {
                width = ce.width;
                height = ce.height;

                Window root_return;
                Window parent_return;
                Window* children_return = NULL;
                unsigned int nchildren;
                XQueryTree(display, root, &root_return, &parent_return,
                           &children_return, &nchildren);
                if (children_return) {
                  for (int i = 0; i < nchildren; i++) {
                        Window child = children_return[i];
                        XMoveResizeWindow(display, child, 0, 0, width, height);
                    }
  
                    XFree(children_return);
                }
            } else {
                // We may also need to catch windows that move or resize themselves.
                // A lot of applications resize their windows immediately after creating them.

                // Only MoveResize if it's not correct already.
                if (
                    ce.x != 0
                    || ce.y != 0
                    || ce.width != width
                    || ce.height != height
                ) {
                    XMoveResizeWindow(display, ce.window, 0, 0, width, height);
                }
            }
        }
    }

    return 0;
}
