#include "gui.h"
#include "gtk/gtk.h"

#define __OB_PADDING 16
#define __OB_WNDWIDTH 1280
#define __OB_WNDHEIGHT 720

void OBGTKcreateNewWindow(struct obgtk_window* restrict window, GtkApplication* app, const char* const title, const int width, const int height)
{
    window->root = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window->root), title);
    gtk_window_set_default_size(GTK_WINDOW(window->root), width, height);

    window->horlayout = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, __OB_PADDING - 12);
    gtk_widget_set_hexpand(window->horlayout, 1);

    window->verlayout = gtk_box_new(GTK_ORIENTATION_VERTICAL, __OB_PADDING - 12);
    gtk_widget_set_valign(window->verlayout, GTK_ALIGN_END);
}

void __ob_gtk_detailsmetadatapane(struct obgtk_details* restrict details)
{
    details->metadata.root = gtk_box_new(GTK_ORIENTATION_VERTICAL, __OB_PADDING - 12);
    details->metadata.fnamelabel = gtk_label_new("test");
    details->metadata.fextlabel = gtk_label_new(".png");
    details->metadata.fchnlabel = gtk_label_new("4");
    details->metadata.fsizelabel = gtk_label_new("124MB");
}

void __ob_gtk_detailsmetadatapack(struct obgtk_details* restrict details)
{
    gtk_box_append(GTK_BOX(details->metadata.root), details->metadata.fnamelabel);
    gtk_box_append(GTK_BOX(details->metadata.root), details->metadata.fextlabel);
    gtk_box_append(GTK_BOX(details->metadata.root), details->metadata.fchnlabel);
    gtk_box_append(GTK_BOX(details->metadata.root), details->metadata.fsizelabel);
}

void __ob_gtk_detailsfileiopane(struct obgtk_details* restrict details)
{
    details->fileio.root = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, __OB_PADDING - 12);
    details->fileio.fopenbtn = gtk_button_new();
    gtk_button_set_label(GTK_BUTTON(details->fileio.fopenbtn), "Open File");
    gtk_widget_set_hexpand(details->fileio.fopenbtn, 1);
    details->fileio.fclosebtn = gtk_button_new();
    gtk_button_set_label(GTK_BUTTON(details->fileio.fclosebtn), "Close File");
    gtk_widget_set_hexpand(details->fileio.fclosebtn, 1);

    gtk_box_append(GTK_BOX(details->fileio.root), details->fileio.fopenbtn);
    gtk_box_append(GTK_BOX(details->fileio.root), details->fileio.fclosebtn);
}

void OBGTKcreateDetailsPane(struct obgtk_details* restrict details)
{
    details->root = gtk_frame_new("Details");
    details->layout = gtk_box_new(GTK_ORIENTATION_VERTICAL, __OB_PADDING);
    __ob_gtk_detailsmetadatapane(details);
    __ob_gtk_detailsfileiopane(details);
    __ob_gtk_detailsmetadatapack(details);

    gtk_box_append(GTK_BOX(details->layout), details->metadata.root);
    gtk_box_append(GTK_BOX(details->layout), details->fileio.root);

    gtk_widget_set_size_request(details->root, __OB_WNDWIDTH * 0.25, __OB_WNDHEIGHT);
    gtk_frame_set_child(GTK_FRAME(details->root), details->layout);
}

void OBGTKcreateOutputPane(struct obgtk_output* restrict output)
{
    output->root = gtk_frame_new("Output");
    output->layout = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    output->textarea = gtk_text_view_new();
    gtk_text_view_set_editable(GTK_TEXT_VIEW(output->textarea), 0);
    output->progbar = gtk_progress_bar_new();
    gtk_progress_bar_set_text(GTK_PROGRESS_BAR(output->progbar), "Conversion Progress...");
    gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(output->progbar), 0.0);

    gtk_box_append(GTK_BOX(output->layout), output->textarea);
    gtk_box_append(GTK_BOX(output->layout), output->progbar);

    gtk_widget_set_size_request(output->root, __OB_WNDWIDTH * 0.50, __OB_WNDHEIGHT * 0.35);
    gtk_widget_set_size_request(output->textarea, gtk_widget_get_width(output->root), gtk_widget_get_height(output->root)-16);
    gtk_widget_set_vexpand(output->textarea, 1);

    gtk_frame_set_child(GTK_FRAME(output->root), output->layout);
}

void OBGTKcreatePreviewPane(struct obgtk_preview* restrict preview)
{
    preview->root = gtk_frame_new("Preview");
    preview->glview = gtk_gl_area_new();
    gtk_gl_area_set_required_version(GTK_GL_AREA(preview->glview), 4, 6);
    
    gtk_widget_set_valign(preview->glview, GTK_ALIGN_START);
    gtk_widget_set_size_request(preview->glview, __OB_WNDWIDTH * 0.50, __OB_WNDHEIGHT * (1.0 - 0.35));
    
    gtk_frame_set_child(GTK_FRAME(preview->root), preview->glview);
}

void OBGTKcreateHexdumpPane(struct obgtk_hexdump* restrict hexdump)
{
    hexdump->root = gtk_frame_new("Hexdump");
    hexdump->hexlabel = gtk_label_new("");

    gtk_frame_set_child(GTK_FRAME(hexdump->root), hexdump->hexlabel);

    gtk_widget_set_size_request(hexdump->root, __OB_WNDWIDTH * 0.25, __OB_WNDHEIGHT);
}

void OBGTKpack(const struct obgtk_window* restrict window,
               const struct obgtk_details* restrict details,
               const struct obgtk_output* restrict output,
               const struct obgtk_preview* restrict preview,
               const struct obgtk_hexdump* restrict hexdump)
{
    gtk_box_append(GTK_BOX(window->verlayout), preview->root);
    gtk_box_append(GTK_BOX(window->verlayout), output->root);

    gtk_box_append(GTK_BOX(window->horlayout), details->root);
    gtk_box_append(GTK_BOX(window->horlayout), window->verlayout);
    gtk_box_append(GTK_BOX(window->horlayout), hexdump->root);

    gtk_window_set_child(GTK_WINDOW(window->root), window->horlayout);

    gtk_window_present(GTK_WINDOW(window->root));
}