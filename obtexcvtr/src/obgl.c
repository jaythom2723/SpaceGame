#include "obgl.h"

#include "gui.h"

#include <gtk-4.0/gtk/gtk.h>
#include <GL/gl.h>

extern struct obgtk_preview preview;

void OBGLonGLContextCreation(void)
{
    // TODO: prototype
}

void OBGLonGLAreaRealize(void)
{
    gtk_gl_area_make_current(GTK_GL_AREA(preview.root));
    if (gtk_gl_area_get_error(GTK_GL_AREA(preview.root)) != NULL)
        return;

    
}

void OBGLglAreaRender(void)
{
    glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}