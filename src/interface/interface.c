#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "../io/image.h"
#include "../main.h"

static char *choosecharacter(char *filename, char *text);

GtkWidget *image_glob;
GtkWidget *label;

char *filename_glob;

void outputocr(char *text)
{
    GtkWidget *dialog, *label, *contentarea;
    GtkDialogFlags flags;

    flags = GTK_DIALOG_DESTROY_WITH_PARENT;
    dialog = gtk_dialog_new_with_buttons ("crOCRodile",
        (GtkWindow *) gtk_window_new (GTK_WINDOW_POPUP),
        flags,
        "OK",
        GTK_RESPONSE_ACCEPT,
        NULL);
    contentarea = gtk_dialog_get_content_area (GTK_DIALOG (dialog));
    label = gtk_label_new(text);
    gtk_container_add(GTK_CONTAINER (contentarea), label);
    g_signal_connect_swapped (dialog, "response",
                              G_CALLBACK (gtk_widget_destroy), dialog);
    gtk_widget_show_all(dialog);
}
void characterdisplayed(GtkEntry *entry)
{
    if (gtk_entry_get_text_length(entry) > 1);
    else
    {
        printf("%s", gtk_entry_get_text(entry));
        fflush(stdout);
    }
}

char *choosecharacter(char *filename, char *text)
{
    GtkWidget *dialog, *imagedisplay, *image, *entry, *contentarea, *label;
    GtkDialogFlags flags;

    flags = GTK_DIALOG_DESTROY_WITH_PARENT;
    dialog = gtk_dialog_new_with_buttons ("crOCRodile",
        (GtkWindow *) gtk_window_new (GTK_WINDOW_POPUP),
        flags,
        NULL);
    contentarea = gtk_dialog_get_content_area (GTK_DIALOG (dialog));
    label = gtk_label_new(text);
    entry = gtk_entry_new_with_buffer(
        gtk_entry_buffer_new("Character displayed", 19));
    imagedisplay = gtk_frame_new("");
    image = gtk_image_new();
    gtk_container_add(GTK_CONTAINER (contentarea), imagedisplay);
    gtk_container_add(GTK_CONTAINER (imagedisplay), image);
    gtk_container_add(GTK_CONTAINER (contentarea), label);
    gtk_container_add(GTK_CONTAINER (contentarea), entry);
    GdkPixbuf *img = img_load(filename);
    img = gdk_pixbuf_scale_simple(img, 960, 540, GDK_INTERP_TILES);
    gtk_image_set_from_pixbuf((GtkImage *) image, img);
    g_signal_connect_swapped (dialog, "response",
        G_CALLBACK (characterdisplayed), entry);
    gtk_widget_show_all(dialog);


}
void loading(char state)
{
    GtkWidget *dialog, *spinner, *content_area;
    GtkDialogFlags flags;

    flags = GTK_DIALOG_DESTROY_WITH_PARENT;
    dialog = gtk_dialog_new_with_buttons ("Font",
        (GtkWindow *) gtk_window_new(GTK_WINDOW_POPUP),
        flags,
        NULL);
    content_area = gtk_dialog_get_content_area (GTK_DIALOG (dialog));
    spinner = gtk_spinner_new();
    gtk_container_add (GTK_CONTAINER (content_area), spinner);
    if (state)
        gtk_spinner_start((GtkSpinner *) spinner);
    else
        gtk_spinner_stop((GtkSpinner *) spinner);
    gtk_widget_show_all(dialog);
}

void printfont(GtkEntry *entry)
{
    if (gtk_entry_get_text_length(entry) > 16);
    else
    {
        printf("%s", gtk_entry_get_text(entry));
        fflush(stdout);
    }
}
void reload(char *path)
{
    GdkPixbuf *img = img_load(path);
    gtk_image_set_from_pixbuf((GtkImage *) image_glob, img);
}

void show_lines(GtkWidget *widget)
{
    if (gtk_toggle_button_get_active((GtkToggleButton *) widget))
        set_line(true);
    else
        set_line(false);
}

void show_character(GtkWidget *widget)
{
    if (gtk_toggle_button_get_active((GtkToggleButton *) widget))
        set_char(true);
    else
        set_char(false);
}

void detect()
{
    /*choosecharacter("/home/jivaros/Pictures/Screenshot "
        "from 2016-10-29 23-28-01.png", "lolmd\ndz");*/
    run(1, &filename_glob);
}

void binarization()
{
    //outputocr("jesusitonpereeejiqjdqzidjqzidqzijdq\ndzqdqz");
    binarize_stuff(filename_glob);
}

void learn_gui(GtkWidget *widget, gpointer data)
{
    GtkWidget *dialog, *entry, *content_area;
    GtkDialogFlags flags;
    const gchar *text = "Font";

    // Create the widgets
    flags = GTK_DIALOG_DESTROY_WITH_PARENT;
    dialog = gtk_dialog_new_with_buttons ("Font",
                                          data,
                                          flags,
                                          NULL);
    content_area = gtk_dialog_get_content_area (GTK_DIALOG (dialog));
    entry = gtk_entry_new_with_buffer (gtk_entry_buffer_new (text, 4));
    g_signal_connect_swapped (dialog,
        "response", G_CALLBACK (printfont), entry);
    gtk_container_add (GTK_CONTAINER (content_area), entry);
    gtk_widget_show_all(dialog);
}

void auto_rotate_gui()
{
    auto_rotate(filename_glob);
}

void chooser(GtkWidget *widget, gpointer *data)
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
        filename_glob = filename;
        GdkPixbuf *img = img_load(filename);
        gtk_image_set_from_pixbuf((GtkImage *) data, img);
    }
    gtk_widget_destroy (dialog);
}

void setText(char *text)
{
    gtk_label_set_text(label, text);
}

void activate(GtkApplication *app, gpointer userdata)
{
    GtkWidget *window, *button, *grid, *gridthereturn,
              *image, *imagedisplay, *checkbutton,
              *scrolledwindow;

    /* create a new window, and set its title */

    window = gtk_application_window_new(app);
    gtk_window_set_title (GTK_WINDOW (window), "crOCRodile");
    gtk_container_set_border_width (GTK_CONTAINER (window), 0);

    /* Here we construct the container that is going to pack our buttons  */
    grid = gtk_grid_new ();
    gridthereturn = gtk_grid_new();

    /* Pack the container in the window */

    /* creating an imagedisplay and a scrollbar
     * then placing it to the right of the buttons */

    image = gtk_image_new();
    imagedisplay = gtk_frame_new("");
    gtk_container_add(GTK_CONTAINER(imagedisplay), image);
    scrolledwindow = gtk_scrolled_window_new
        (gtk_adjustment_new(0, 0, 250, 250, 250, 250),
         gtk_adjustment_new(0, 0, 250, 250, 250, 250));
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
    g_signal_connect (button, "clicked", G_CALLBACK (learn_gui), window);

    /* Place the learn button in the gric cell (0, 2) and make it fill
     * just 1 cell horizontally and vertically (ie no spanning)
     */

    gtk_grid_attach (GTK_GRID (grid), button, 0, 2000, 1, 5);

    button = gtk_button_new_with_label("Binarization");
    gtk_grid_attach (GTK_GRID (grid), button, 0, 3000, 1, 5);
    g_signal_connect (button, "clicked", G_CALLBACK(binarization), NULL);

    button = gtk_button_new_with_label("AutoRotate");
    gtk_grid_attach (GTK_GRID (grid), button, 0, 4000, 1, 5);
    g_signal_connect (button, "clicked", G_CALLBACK (auto_rotate_gui), NULL);

    /* creating show character check button */

    checkbutton = gtk_check_button_new_with_label("Show Character");
    gtk_grid_attach (GTK_GRID (grid), checkbutton, 0, 5000, 1, 5);
    g_signal_connect (checkbutton,
        "toggled",
        G_CALLBACK (show_character), NULL);

    /* creating show lines check button */

    checkbutton = gtk_check_button_new_with_label("Show Lines");
    gtk_grid_attach (GTK_GRID (grid), checkbutton, 0, 6000, 1, 5);
    g_signal_connect (checkbutton, "toggled", G_CALLBACK (show_lines), NULL);

    gtk_grid_attach (GTK_GRID (gridthereturn), grid, 0, 0, 1, 18);
    gtk_grid_attach (GTK_GRID (gridthereturn), scrolledwindow, 1, 0, 1, 1);

    label = gtk_label_new("lolmdr\ndqzdzq\ndqzdzq\ndqz\ndz");
    gtk_grid_attach (GTK_GRID (grid), label, 0, 90000, 1, 5);

    /* Now that we are done packing our widgets, we show them all in
     * one go, by calling gtk_widget_show_all() on the window.
     * This call recusrively calls gtk_widget_show() on all widgets
     * that are contained in the window, directly or indirectly
     */

    gtk_widget_show_all (window);
    image_glob = image;
}

void gui_start()
{
    GtkApplication  *app;
    int status;

    app = gtk_application_new("org.gtk.example", G_APPLICATION_FLAGS_NONE);
    g_signal_connect (app, "activate", G_CALLBACK (activate), NULL);
    status = g_application_run (G_APPLICATION (app), 0, NULL);
    g_object_unref (app);

    if(status)
        printf("Something went wrong in the GUI");
}
