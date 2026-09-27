#include "gio/gio.h"
#include "gui.h"

#include <stdio.h>
#include <stdint.h>

#include <gtk-4.0/gtk/gtk.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb-image/stb_image.h>

extern GFile* file;
extern GFileInfo* finfo;
extern int width;
extern int height;
extern int nrChannels;
extern struct obgtk_window window;

void file_open_dialog_close(GObject* source_object,
                            GAsyncResult* res,
                            gpointer data)
{
    GtkFileDialog* dialog = GTK_FILE_DIALOG(source_object);
    GError* error = NULL;

    file = gtk_file_dialog_open_finish(dialog, res, &error);
    if (file == NULL)
    {
        if (g_error_matches(error, GTK_DIALOG_ERROR, GTK_DIALOG_ERROR_DISMISSED) != 1)
        {
            GtkAlertDialog* alert;
            alert = gtk_alert_dialog_new("Something went wrong!");
            gtk_alert_dialog_set_detail(alert, error->message);
            gtk_alert_dialog_show(alert, GTK_WINDOW(window.root));
            g_object_unref(alert);
        }
        g_error_free(error);
        return;
    }
    finfo = g_file_query_info(file, "standard::*", G_FILE_QUERY_INFO_NONE, NULL, NULL);

    data = stbi_load(g_file_get_path(file), &width, &height, &nrChannels, 4);
    OBGTKupdateWindowOpen(file);

    printf("%p", data);

    g_object_unref(file);
}

// TODO: prototype fclosebtn functionality

void fopenbtn_click(void)
{
    if (file != NULL)
        return;

    GtkFileDialog* dialog = gtk_file_dialog_new();
    GtkFileFilter* filter = gtk_file_filter_new();
    gtk_file_filter_add_suffix(filter, "obtf");
    gtk_file_filter_set_name(filter, "OBTF");
   
    GListStore* list = g_list_store_new(GTK_TYPE_FILE_FILTER);
    g_list_store_append(list, filter);

    gtk_file_dialog_set_filters(dialog, G_LIST_MODEL(list));

    gtk_file_dialog_open(dialog, GTK_WINDOW(window.root), NULL, file_open_dialog_close, NULL);

    g_object_unref(filter);
    g_object_unref(list);
    g_object_unref(dialog);
}

void fclosebtn_click(void)
{
    if (file == NULL)
        return;

    file = NULL;
}
