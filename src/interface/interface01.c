# include <gtk/gtk.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>

# include "../io/image.c"

GdkPixbuf *img_load(const char *name);

GtkWidget *image_glob;


static void characterdisplayed(GtkEntry *entry)
{
    if (gtk_entry_get_text_length(entry) > 1);
    else
    {
        printf("%s", gtk_entry_get_text(entry));
        fflush(stdout);
    }
}

static char *choosecharacter(char *filename)
{
    GtkWidget *dialog, *imagedisplay, *image, *entry, *contentarea;
    GtkDialogFlags flags;

    flags = GTK_DIALOG_DESTROY_WITH_PARENT;
    dialog = gtk_dialog_new_with_buttons ("jsaisap",
                                          (GtkWindow *) gtk_window_new (GTK_WINDOW_POPUP),
                                          flags,
                                          NULL);
    contentarea = gtk_dialog_get_content_area (GTK_DIALOG (dialog));
    entry = gtk_entry_new_with_buffer(gtk_entry_buffer_new("Character displayed", 19));
    imagedisplay = gtk_frame_new("");
    image = gtk_image_new();;
    gtk_container_add(GTK_CONTAINER(imagedisplay), image);
    gtk_container_add(GTK_CONTAINER (contentarea), imagedisplay);
    gtk_container_add(GTK_CONTAINER (contentarea), entry);
    GdkPixbuf *img = img_load(filename);
    img = gdk_pixbuf_scale_simple(img, 960, 540, GDK_INTERP_TILES);
    gtk_image_set_from_pixbuf((GtkImage *) image, img);
    g_signal_connect_swapped (dialog, "response", G_CALLBACK (characterdisplayed), entry); 
    gtk_widget_show_all(dialog);


}
static void loading()
{
    GtkWidget *dialog, *pb, *content_area;
    GtkDialogFlags flags;

    flags = GTK_DIALOG_DESTROY_WITH_PARENT;
    dialog = gtk_dialog_new_with_buttons ("Font", 
                                          (GtkWindow *) gtk_window_new(GTK_WINDOW_POPUP),
                                          flags,
                                          NULL);
    content_area = gtk_dialog_get_content_area (GTK_DIALOG (dialog));
    pb = gtk_progress_bar_new();
    gtk_container_add (GTK_CONTAINER (content_area), pb);
    gtk_progress_bar_pulse((GtkProgressBar *) pb);
    gtk_progress_bar_set_fraction((GtkProgressBar *) pb, 0.2);
    gtk_progress_bar_set_fraction((GtkProgressBar *) pb, 0.4);
    gtk_progress_bar_set_fraction((GtkProgressBar *) pb, 0.6);
    gtk_progress_bar_set_fraction((GtkProgressBar *) pb, 0.8);
    gtk_progress_bar_set_fraction((GtkProgressBar *) pb, 0.9);
    gtk_widget_show_all(dialog);
    if (gtk_progress_bar_get_fraction((GtkProgressBar *) pb) == 1.0)
        gtk_widget_destroy(dialog); 
}

static void printfont(GtkEntry *entry)
{
    printf("%s", gtk_entry_get_text(entry));
    fflush(stdout);
}
static void reload(char *path)
{
    GdkPixbuf *img = img_load(path);
    gtk_image_set_from_pixbuf((GtkImage *) image_glob, img);
}

static void show_lines(GtkWidget *widget, gpointer data)
{
    if (gtk_toggle_button_get_active((GtkToggleButton *) widget))
    {
        printf ("LINES\n");
    }
}

static void show_character(GtkWidget *widget, gpointer data)
{
    if (gtk_toggle_button_get_active((GtkToggleButton *) widget))
    {
        printf ("CHARACTER\n");
    }
}

static void detect(GtkWidget *widget, gpointer data)
{
    choosecharacter("/home/jivaros/Pictures/Screenshot from 2016-10-29 23-28-01.png");
}

static void learn(GtkWidget *widget, gpointer data)
{
    GtkWidget *dialog, *entry, *content_area;
    GtkDialogFlags flags;
    const gchar *text = "Font";
    const gchar *ok = "_OK";
    gint res;

    // Create the widgets
    flags = GTK_DIALOG_DESTROY_WITH_PARENT;
    dialog = gtk_dialog_new_with_buttons ("Font", 
                                          data,
                                          flags,
                                          NULL);
    content_area = gtk_dialog_get_content_area (GTK_DIALOG (dialog));
    entry = gtk_entry_new_with_buffer (gtk_entry_buffer_new (text, 4));
    g_signal_connect_swapped (dialog, "response", G_CALLBACK (printfont), entry);
    gtk_container_add (GTK_CONTAINER (content_area), entry);
    gtk_widget_show_all(dialog);
}


static void chooser(GtkWidget *widget, gpointer *data)
{
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
    GtkWidget *gridthereturn;
    GtkWidget *image;
    GtkWidget *imagedisplay;
    GtkWidget *checkbutton;
    GtkWidget *scrolledwindow;

    /* create a new window, and set its title */

    window = gtk_application_window_new(app);
    gtk_window_set_title (GTK_WINDOW (window), "crOCRodile");
    gtk_container_set_border_width (GTK_CONTAINER (window), 0);

    /* Here we construct the container that is going to pack our buttons  */
    grid = gtk_grid_new ();
    gridthereturn = gtk_grid_new();

    /* Pack the container in the window */  
 
    /* creating an imagedisplay and a scrollbar then placing it to the right of the buttons */

    image = gtk_image_new();
    imagedisplay = gtk_frame_new("");
    gtk_container_add(GTK_CONTAINER(imagedisplay), image);
    scrolledwindow = gtk_scrolled_window_new(gtk_adjustment_new(0, 0, 250, 250, 250, 250), gtk_adjustment_new(0, 0, 250, 250, 250, 250));
    gtk_widget_set_size_request(scrolledwindow, 1200, 650);
    gtk_container_add(GTK_CONTAINER(window), gridthereturn);
    gtk_container_add(GTK_CONTAINER(scrolledwindow), imagedisplay);
    

    /* creating button load */
    
    button = gtk_button_new_with_label("Load Image");
    g_signal_connect(button, "clicked", G_CALLBACK (chooser), image);

    /* Place the load button in the grid cell (0,0), and make it fill
     * just 1 cell horizontally and vertically (ie no spanning)
     */
    
    gtk_grid_attach (GTK_GRID (grid), button, 0, 1, 1, 5);

    /* creating button detect */

    button = gtk_button_new_with_label ("Detect");
    g_signal_connect (button, "clicked", G_CALLBACK (detect), NULL);

    /* Place the detect button in the grid cell (0, 1), and make it fill
     * just 1 cell horizontally and vertically (ie no spanning)
     */

    gtk_grid_attach (GTK_GRID (grid), button, 0, 1000, 1, 5);

    /* creating button learn */

    button = gtk_button_new_with_label ("Learn");
    g_signal_connect (button, "clicked", G_CALLBACK (learn), window);

    /* Place the learn button in the gric cell (0, 2) and make it fill
     * just 1 cell horizontally and vertically (ie no spanning)
     */

    gtk_grid_attach (GTK_GRID (grid), button, 0, 2000, 1, 5);
    
    button = gtk_button_new_with_label("Binarization");
    gtk_grid_attach (GTK_GRID (grid), button, 0, 3000, 1, 5);

    button = gtk_button_new_with_label("AutoRotate");
    gtk_grid_attach (GTK_GRID (grid), button, 0, 4000, 1, 5);
    g_signal_connect (button, "clicked", G_CALLBACK (loading), NULL); 

    /* creating show character check button */
    
    checkbutton = gtk_check_button_new_with_label("Show Character");
    gtk_grid_attach (GTK_GRID (grid), checkbutton, 0, 5000, 1, 5);
    g_signal_connect (checkbutton, "toggled", G_CALLBACK (show_character), NULL);

    /* creating show lines check button */

    checkbutton = gtk_check_button_new_with_label("Show Lines");
    gtk_grid_attach (GTK_GRID (grid), checkbutton, 0, 6000, 1, 5);
    g_signal_connect (checkbutton, "toggled", G_CALLBACK (show_lines), NULL);
    
    gtk_grid_attach (GTK_GRID (gridthereturn), grid, 0, 0, 1, 18);
    gtk_grid_attach (GTK_GRID (gridthereturn), scrolledwindow, 1, 0, 1, 1);

    /* Now that we are done packing our widgets, we show them all in 
     * one go, by calling gtk_widget_show_all() on the window.
     * This call recusrively calls gtk_widget_show() on all widgets
     * that are contained in the window, directly or indirectly 
     */

    gtk_widget_show_all (window);
    image_glob = image;
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
