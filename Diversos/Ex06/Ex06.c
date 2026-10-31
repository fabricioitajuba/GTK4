#include <gtk/gtk.h>

typedef struct {
    GtkWidget *entry;
    GtkWidget *label;
} WidgetsApp;

static void on_button_clicked(GtkButton *button, gpointer user_data) {
    WidgetsApp *app_widgets = (WidgetsApp *)user_data;
    GtkEntryBuffer *buffer = gtk_entry_get_buffer(GTK_ENTRY(app_widgets->entry));
    const char *texto = gtk_entry_buffer_get_text(buffer);
    gtk_label_set_text(GTK_LABEL(app_widgets->label), texto);
}

static void activate(GtkApplication *app, gpointer user_data) {
    GtkWidget *window;
    GtkWidget *box;
    GtkWidget *button_transferir;
    GtkWidget *button_sair; // <-- Novo botão
    
    WidgetsApp *app_widgets = g_new(WidgetsApp, 1);

    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Exemplo GTK4");
    gtk_window_set_default_size(GTK_WINDOW(window), 300, 250);

    box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_widget_set_margin_top(box, 20);
    gtk_widget_set_margin_bottom(box, 20);
    gtk_widget_set_margin_start(box, 20);
    gtk_widget_set_margin_end(box, 20);
    gtk_window_set_child(GTK_WINDOW(window), box);

    app_widgets->entry = gtk_entry_new();
    gtk_box_append(GTK_BOX(box), app_widgets->entry);

    button_transferir = gtk_button_new_with_label("Transferir Texto");
    gtk_box_append(GTK_BOX(box), button_transferir);

    app_widgets->label = gtk_label_new("O texto aparecerá aqui");
    gtk_box_append(GTK_BOX(box), app_widgets->label);

    // 1. Cria o botão de sair
    button_sair = gtk_button_new_with_label("Sair");
    gtk_box_append(GTK_BOX(box), button_sair);

    // Conecta o botão de transferir texto
    g_signal_connect(button_transferir, "clicked", G_CALLBACK(on_button_clicked), app_widgets);

    // 2. Conecta o botão de sair diretamente para destruir a janela principal
    g_signal_connect_swapped(button_sair, "clicked", G_CALLBACK(gtk_window_destroy), window);

    g_object_set_data_full(G_OBJECT(window), "app_data", app_widgets, g_free);

    gtk_window_present(GTK_WINDOW(window));
}

int main(int argc, char **argv) {
    GtkApplication *app;
    int status;

    app = gtk_application_new("org.exemplo.gtk4", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    
    status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);

    return status;
}
