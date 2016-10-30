#include <gtk/gtk.h>

#include "../io/image.h"

static void chooser(GtkWidget *widget, gpointer data)
{
    GtkWidget *windowimg;
    GtkWidget *dialog;
    GtkWidget *image;
    gint res;

    windowimg = gtk_application_window_new(gtk_window_get_application(data));
    gtk_window_set_title (GTK_WINDOW (windowimg), "Windowimg");
    gtk_window_set_default_size (GTK_WINDOW (windowimg), 200, 200);

    dialog = gtk_file_chooser_dialog_new ("Open File",
                                          (GtkWindow *) data,
                                          GTK_FILE_CHOOSER_ACTION_OPEN,
                                          "_Cancel",
                                          GTK_RESPONSE_CANCEL,
                                          "_Open",
                                          GTK_RESPONSE_ACCEPT,
                                          NULL);
    res = gtk_dialog_run (GTK_DIALOG (dialog));
    if (res == GTK_RESPONSE_ACCEPT)
    {
        char *filename;
        GtkFileChooser *chooser = GTK_FILE_CHOOSER (dialog);
        filename = gtk_file_chooser_get_filename (chooser);
        printf("%s\n", filename);
        GdkPixbuf *img = img_load(filename);
        img = gdk_pixbuf_scale_simple(img, 960, 540, GDK_INTERP_TILES);
        image = gtk_image_new_from_pixbuf(img);
    }
    else
        image = gtk_image_new();
    gtk_container_add (GTK_CONTAINER (windowimg), image);
    gtk_widget_destroy (dialog);
    gtk_widget_show_all (windowimg);
}
static void 
activate(GtkApplication *app, 
         gpointer userdata)
{
    GtkWidget *window; 
    GtkWidget *button;
    GtkWidget *button_box;

    window = gtk_application_window_new(app);
    gtk_window_set_title (GTK_WINDOW (window), "Window");
    gtk_window_set_default_size (GTK_WINDOW (window), 200, 200);

    button_box = gtk_button_box_new (GTK_ORIENTATION_HORIZONTAL);
    gtk_container_add (GTK_CONTAINER (window), button_box);

    button = gtk_button_new_with_label("Load Image");
    g_signal_connect(button, "clicked", G_CALLBACK (chooser), window);
    g_signal_connect_swapped (button, "clicked", G_CALLBACK (gtk_widget_destroy), window);
    gtk_container_add (GTK_CONTAINER (button_box), button);
    gtk_widget_show_all (window);
}

int main(int argc, char **argv)
{
    GtkApplication  *app;
    int status;

    app = gtk_application_new("org.gtk.example", G_APPLICATION_FLAGS_NONE);
    g_signal_connect(app, "activate", G_CALLBACK (activate), NULL);
    status = g_application_run(G_APPLICATION (app), argc, argv);
    g_object_unref (app);

    return status; 
}
