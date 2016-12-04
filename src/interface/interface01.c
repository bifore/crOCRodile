# include <gtk/gtk.h>
# include <stdio.h>
# include <stdlib.h>

# include "../io/image.c"

GdkPixbuf *img_load(const char *name);

static void show_lines(GtkWidget *widget, gpointer data)
{
}

static void show_character(GtkWidget *widget, gpointer data)
{
}

static void detect(GtkWidget *widget, gpointer data)
{
}

static void learn(GtkWidget *widget, gpointer data)
{
}

static void chooser(GtkWidget *widget, gpointer *data)
{
    GtkWidget *image;
    GtkWidget *dialog;
    gint res;


    dialog = gtk_file_chooser_dialog_new ("Open File",
                                          (GtkWindow *) gtk_window_new(GTK_WINDOW_POPUP),
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
        gtk_image_set_from_pixbuf((GtkImage *) data, img);
    }
    gtk_widget_destroy (dialog);
}

static void 
activate(GtkApplication *app, 
         gpointer userdata)
{
    GtkWidget *window; 
    GtkWidget *button;
    GtkWidget *grid;
    GtkWidget *image;
    GtkWidget *imagedisplay;
    GtkWidget *switchh;
    GtkWidget *label;

    /* create a new window, and set its title */

    window = gtk_application_window_new(app);
    gtk_window_set_title (GTK_WINDOW (window), "Window");
    gtk_container_set_border_width (GTK_CONTAINER (window), 50);

    /* Here we construct the container that is going to pack our buttons  */
    grid = gtk_grid_new ();

    /* Pack the container in the window */  
 
    /* creating image and placing it to the right of the buttons */

    image = gtk_image_new();
    imagedisplay = gtk_frame_new("");
    gtk_container_add(GTK_CONTAINER(imagedisplay), image);
    gtk_grid_attach (GTK_GRID (grid), imagedisplay, 2, 0, 20, 20);


    /* creating button load */
    
    button = gtk_button_new_with_label("Load Image");
    g_signal_connect(button, "clicked", G_CALLBACK (chooser), image);

    /* Place the load button in the grid cell (0,0), and make it fill
     * just 1 cell horizontally and vertically (ie no spanning)
     */
    
    gtk_grid_attach (GTK_GRID (grid), button, 0, 1, 1, 1);

    /* creating button detect */

    button = gtk_button_new_with_label ("Detect");
    g_signal_connect (button, "clicked", G_CALLBACK (detect), NULL);

    /* Place the detect button in the grid cell (0, 1), and make it fill
     * just 1 cell horizontally and vertically (ie no spanning)
     */

    gtk_grid_attach (GTK_GRID (grid), button, 0, 5, 1, 1);

    /* creating button learn */

    button = gtk_button_new_with_label ("Learn");
    g_signal_connect (button, "clicked", G_CALLBACK (learn), NULL);

    /* Place the learn button in the gric cell (0, 2) and make it fill
     * just 1 cell horizontally and vertically (ie no spanning)
     */

    gtk_grid_attach (GTK_GRID (grid), button, 0, 9, 1, 1);
    gtk_container_add (GTK_CONTAINER (window), grid);

    /* creating show character switchh */
    
    switchh = gtk_switch_new();
    gtk_grid_attach (GTK_GRID (grid), switchh, 0, 13, 1, 1);
    g_signal_connect (button, "clicked", G_CALLBACK (show_character), NULL);

    /* creating a label "show character" */

    label = gtk_label_new("Show Character");
    gtk_grid_attach (GTK_GRID (grid), label, 0, 14, 1, 1);

    /* creating show lines switch */

    switchh = gtk_switch_new();
    gtk_grid_attach (GTK_GRID (grid), switchh, 0, 18, 1, 1);
    g_signal_connect (button, "clicked", G_CALLBACK (show_lines), NULL);

    /* creating a label "show lines" */

    label = gtk_label_new("Show Lines");
    gtk_grid_attach (GTK_GRID (grid), label, 0, 19, 1, 1);

    /* Now that we are done packing our widgets, we show them all in 
     * one go, by calling gtk_widget_show_all() on the window.
     * This call recusrively calls gtk_widget_show() on all widgets
     * that are contained in the window, directly or indirectly 
     */

    gtk_widget_show_all (window);
}

int main(int argc, 
         char **argv)
{
    GtkApplication  *app;
    int status;

    app = gtk_application_new("org.gtk.example", G_APPLICATION_FLAGS_NONE);
    g_signal_connect (app, "activate", G_CALLBACK (activate), NULL);
    status = g_application_run (G_APPLICATION (app), argc, argv);
    g_object_unref (app);

    return status;
}
