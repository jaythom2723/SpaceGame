#ifndef OBGL_H
#define OBGL_H

#include <gtk-4.0/gtk/gtk.h>

void OBGLonGLAreaRealize(GtkGLArea* self);
gboolean OBGLglAreaRender(GtkGLArea* self, GdkGLContext* context);

#endif