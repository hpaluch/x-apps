// xhello.c - most trivial X-Window program - work in progress
// see https://web.archive.org/web/20071018025425/users.actcom.co.il/~choo/lupg/tutorials/xlib-programming/xlib-programming.html
// and: https://github.com/gamedevtech/X11OpenGLWindow

#include <stdlib.h>
#include <stdio.h>

#include <X11/Xlib.h>   /* defines common Xlib functions and structs. */

/* this variable will be used to store the "default" screen of the  */
/* X server. usually an X server has only one screen, so we're only */
/* interested in that screen.                                       */
int screen_num = -1;

/* these variables will store the size of the screen, in pixels.    */
int screen_width = -1;
int screen_height = -1;

/* these variables will be used to store the IDs of the black and white */
/* colors of the given screen. More on this will be explained later.    */
unsigned long white_pixel=0;
unsigned long black_pixel=0;

Window window;

// print macros similar to Linux kernel
#define pr_err(fmt,...) fprintf(stderr, "ERROR: %s:%d " fmt "\n",__func__,__LINE__,##__VA_ARGS__)

int  my_error_handler(Display * d, XErrorEvent * e)
{
	char err[1024];

	XGetErrorText(d, e->error_code, err, sizeof(err));
	pr_err("%s: err=%u: %s", DisplayString(d),e->error_code, err);
	return 0;
}

int main(int argc, char **argv)
{
	int ret = EXIT_FAILURE;
	Display *display = NULL;
	XEvent ev;

	// uses environment DISPLAY if available
	display = XOpenDisplay(NULL);
	if (display == NULL){
		pr_err("XOpenDisplay() failed. DISPLAY=%s", getenv("DISPLAY"));
		goto exit0;
	}
	XSetErrorHandler(my_error_handler);
	printf("Connected fd=%d to X-Server, vendor: '%s', display='%s'\n",
			ConnectionNumber(display), ServerVendor(display), DisplayString(display));
	screen_num = DefaultScreen(display);
	/* find the width of the default screen of our X server, in pixels. */
	screen_width = DisplayWidth(display, screen_num);
	/* find the height of the default screen of our X server, in pixels. */
	screen_height = DisplayHeight(display, screen_num);
	printf("Screen %d has w x h: %d x %d\n", screen_num, screen_width, screen_height);

	/* find the value of a white pixel on this screen. */
	white_pixel = WhitePixel(display, screen_num);
	/* find the value of a black pixel on this screen. */
	black_pixel = BlackPixel(display, screen_num);
	printf("White pixel = %lu, Black pixel = %lu\n", white_pixel, black_pixel);

	window = XCreateSimpleWindow(display, RootWindow(display, screen_num),
			screen_width/2 - screen_width/4, screen_height/2 - screen_height/4,
			screen_width/2, screen_height/2, 1, black_pixel,white_pixel);

	XSelectInput(display, window, ExposureMask | KeyPressMask);

    // Show the window
    XClearWindow(display, window);
    XMapRaised(display, window);

    printf("Press any key (when window has focus) to exit...\n");

    // Enter message loop
    while (1) {
          XNextEvent(display, &ev);
          switch(ev.type){
          case Expose:
        	  printf("TODO Expose - redraw\n");
        	  break;
          case KeyPress:
        	  printf("Key pressed - exiting");
        	  goto exit_loop;
			  break;
          default:
        	  printf("Unknown type=%d - ignored\n", ev.type);
          }
    }
exit_loop:

	ret = EXIT_SUCCESS;
exit2:
	XDestroyWindow(display, window); window = 0;
exit1:
	XCloseDisplay(display); display = NULL;
exit0:
	printf("Exiting %s with ret=%d\n", argv[0], ret);
	return ret;

}
