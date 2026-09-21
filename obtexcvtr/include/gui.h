#ifndef OB_TEX_CVTR_GUI_H
#define OB_TEX_CVTR_GUI_H

#include <gtk-4.0/gtk/gtk.h>

struct obgtk_window {
    GtkWidget* root;
    GtkWidget* horlayout;
    GtkWidget* verlayout;
};

struct obgtk_details {
    GtkWidget* root;
    GtkWidget* layout;
    struct {
        GtkWidget* root;
        GtkWidget* fnamelabel;
        GtkWidget* fextlabel;
        GtkWidget* fchnlabel;
        GtkWidget* fsizelabel;
    } metadata;
    struct {
        GtkWidget* root;
        GtkWidget* fopenbtn;
        GtkWidget* fclosebtn;
    } fileio;
};

struct obgtk_output {
    GtkWidget* root;
    GtkWidget* layout;
    GtkWidget* textarea;
    GtkWidget* progbar;
};

struct obgtk_preview {
    GtkWidget* root;
    GtkWidget* glview;
};

// TODO: split in half for a before and after?
// TODO: maybe a tab system?
struct obgtk_hexdump {
    GtkWidget* root;
    GtkWidget* hexlabel;
};

void OBGTKcreateNewWindow(struct obgtk_window* restrict window, GtkApplication* app, const char* const title, const int width, const int height);
void OBGTKcreateDetailsPane(struct obgtk_details* restrict details);
void OBGTKcreateOutputPane(struct obgtk_output* restrict output);
void OBGTKcreatePreviewPane(struct obgtk_preview* restrict preview);
void OBGTKcreateHexdumpPane(struct obgtk_hexdump* restrict hexdump);
void OBGTKpack(const struct obgtk_window* restrict window,
               const struct obgtk_details* restrict details,
               const struct obgtk_output* restrict output,
               const struct obgtk_preview* restrict preview,
               const struct obgtk_hexdump* restrict hexdump);

#endif