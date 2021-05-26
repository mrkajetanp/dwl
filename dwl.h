#ifndef DWL_H_
#define DWL_H_

#include <wlr/render/wlr_renderer.h>
#include <wlr/types/wlr_compositor.h>
#include <wlr/types/wlr_cursor.h>
#include <wlr/types/wlr_data_control_v1.h>
#include <wlr/types/wlr_data_device.h>
#include <wlr/types/wlr_export_dmabuf_v1.h>
#include <wlr/types/wlr_gamma_control_v1.h>
#include <wlr/types/wlr_input_device.h>
#include <wlr/types/wlr_idle.h>
#include <wlr/types/wlr_layer_shell_v1.h>
#include <wlr/types/wlr_keyboard.h>
#include <wlr/types/wlr_matrix.h>
#include <wlr/types/wlr_output.h>
#include <wlr/types/wlr_output_layout.h>
#include <wlr/types/wlr_output_management_v1.h>
#include <wlr/types/wlr_pointer.h>
#include <wlr/types/wlr_primary_selection.h>
#include <wlr/types/wlr_primary_selection_v1.h>
#include <wlr/types/wlr_screencopy_v1.h>
#include <wlr/types/wlr_server_decoration.h>
#include <wlr/types/wlr_seat.h>
#include <wlr/types/wlr_viewporter.h>
#include <wlr/types/wlr_virtual_keyboard_v1.h>
#include <wlr/types/wlr_xcursor_manager.h>
#include <wlr/types/wlr_xdg_decoration_v1.h>
#include <wlr/types/wlr_xdg_output_v1.h>
#include <wlr/types/wlr_xdg_shell.h>
#include <wlr/backend/libinput.h>
#include <wlr/util/log.h>
#include <xkbcommon/xkbcommon.h>
#ifdef XWAYLAND
#include <X11/Xlib.h>
#include <wlr/xwayland.h>
#endif

/* enums */
enum { CurNormal, CurMove, CurResize }; /* cursor */
#ifdef XWAYLAND
enum { NetWMWindowTypeDialog, NetWMWindowTypeSplash, NetWMWindowTypeToolbar,
	NetWMWindowTypeUtility, NetLast }; /* EWMH atoms */
enum { XDGShell, X11Managed, X11Unmanaged }; /* client types */
#endif

typedef union {
	int i;
	unsigned int ui;
	float f;
	const void *v;
} Arg;

typedef struct {
	unsigned int mod;
	unsigned int button;
	void (*func)(const Arg *);
	const Arg arg;
} Button;

typedef struct Monitor Monitor;

typedef struct {
	struct wl_list link;
	struct wl_list flink;
	struct wl_list slink;
	union {
		struct wlr_xdg_surface *xdg;
		struct wlr_xwayland_surface *xwayland;
	} surface;
	struct wl_listener commit;
	struct wl_listener map;
	struct wl_listener unmap;
	struct wl_listener destroy;
	struct wl_listener set_title;
	struct wl_listener fullscreen;
	struct wlr_box geom;  /* layout-relative, includes border */
	Monitor *mon;
#ifdef XWAYLAND
	unsigned int type;
	struct wl_listener activate;
	struct wl_listener configure;
#endif
	int bw;
	unsigned int tags;
	int isfloating;
	uint32_t resize; /* configure serial of a pending resize */
	int prevx;
	int prevy;
	int prevwidth;
	int prevheight;
	int isfullscreen;
} Client;

typedef struct {
	struct wl_listener request_mode;
	struct wl_listener destroy;
} Decoration;

typedef struct {
	uint32_t mod;
	xkb_keysym_t keysym;
	void (*func)(const Arg *);
	const Arg arg;
} Key;

typedef struct {
	struct wl_list link;
	struct wlr_input_device *device;

	struct wl_listener modifiers;
	struct wl_listener key;
	struct wl_listener destroy;
} Keyboard;

typedef struct {
	struct wlr_layer_surface_v1 *layer_surface;
	struct wl_list link;

	struct wl_listener destroy;
	struct wl_listener map;
	struct wl_listener unmap;
	struct wl_listener surface_commit;

	struct wlr_box geo;
	enum zwlr_layer_shell_v1_layer layer;
} LayerSurface;

typedef struct {
	uint32_t singular_anchor;
	uint32_t anchor_triplet;
	int *positive_axis;
	int *negative_axis;
	int margin;
} Edge;

typedef struct {
	const char *id;
	const char *title;
	unsigned int tags;
	int isfloating;
	int monitor;
} Rule;

typedef struct {
	const char *symbol;
	void (*arrange)(Monitor *);
} Layout;

struct Monitor {
	struct wl_list link;
	struct wlr_output *wlr_output;
	struct wl_listener frame;
	struct wl_listener destroy;
	struct wlr_box m;      /* monitor area, layout-relative */
	struct wlr_box w;      /* window area, layout-relative */
	struct wl_list layers[4]; // LayerSurface::link
	const Layout *lt[2];
	unsigned int seltags;
	unsigned int sellt;
	unsigned int tagset[2];
	double mfact;
	int nmaster;
};

typedef struct {
	const char *name;
	float mfact;
	int nmaster;
	float scale;
	const Layout *lt;
	enum wl_output_transform rr;
	int x;
	int y;
} MonitorRule;

/* Used to move all of the data necessary to render a surface from the top-level
 * frame handler to the per-surface render function. */
struct render_data {
	struct wlr_output *output;
	struct timespec *when;
	int x, y; /* layout-relative */
};

static void tile(Monitor *m);
static void monocle(Monitor *m);

#endif /* ifndef DWL_H */










