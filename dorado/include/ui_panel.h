/* Front panel for the SDL frontend. See src/ui_panel.c for why it exists.
 *
 * The panel occupies a band across the top of the window; the guest display
 * is drawn below it. Everything a host control used to steal from the guest
 * (Cmd/Ctrl+V, Cmd/Ctrl+Q, F1) is a button here instead, and controls the
 * real machine had but a windowed user could not reach -- the boot button,
 * the green status LED -- are here for the first time.
 */

#ifndef DORADO_UI_PANEL_H
#define DORADO_UI_PANEL_H

#include <SDL.h>

#include "machine.h"
#include "microui.h"

/* Height of the panel band, in window pixels. The guest framebuffer is
 * blitted below it, so the frontend must subtract this when mapping a click
 * back to a guest coordinate. */
#define DORADO_UI_HEIGHT 30

typedef enum dorado_ui_action {
    DORADO_UI_NONE = 0,
    DORADO_UI_PAUSE,     /* toggle run/pause                                */
    DORADO_UI_BOOT,      /* press the machine's boot button                 */
    DORADO_UI_PASTE,     /* host clipboard -> guest, as paced keystrokes    */
    DORADO_UI_SAVE,      /* write a snapshot                                */
    DORADO_UI_ADDFILE    /* prompt for a file to add to the served tree     */
} dorado_ui_action;

/* Frontend-side state the panel displays but does not own. */
typedef struct dorado_ui_status {
    int         paused;
    double      speed_ratio;   /* emulated Dorado seconds per CPU second   */
    const char *world;         /* short name of the loaded world           */
    char        message[128];  /* last thing that happened, shown to the user */
} dorado_ui_status;

int  dorado_ui_init(SDL_Renderer *ren);
void dorado_ui_shutdown(void);
int  dorado_ui_height(void);

/* Returns 1 if the panel consumed the event (so it must NOT also reach the
 * guest -- a click on Boot must not land in the viewer underneath). */
int  dorado_ui_handle_event(const SDL_Event *e);

dorado_ui_action dorado_ui_frame(const dorado_machine_panel *p,
                                 const dorado_ui_status *st,
                                 int window_w);

/* An indicator with a caption, laid out like any other widget. */
void dorado_ui_lamp(mu_Context *ctx, const char *name, int on,
                    int r, int g, int b);

#endif /* DORADO_UI_PANEL_H */
