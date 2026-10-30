/*
 GTK - Soma entre 2 números
*/
#include <gtk/gtk.h>

// Estrutura para transportar os widgets até a função de callback
typedef struct {
    GtkWidget *entry;
    GtkWidget *label;
} AppWidgets;

// Função chamada quando o botão é clicado
static void on_button_clicked(GtkButton *button, gpointer user_data){

    AppWidgets *widgets = (AppWidgets *)user_data;

    // Recupera o buffer de texto da caixa de entrada
    GtkEntryBuffer *buffer = gtk_entry_get_buffer(GTK_ENTRY(widgets->entry));
    const char *texto = gtk_entry_buffer_get_text(buffer);

    // Define o texto recuperado como o conteúdo do label
    gtk_label_set_text(GTK_LABEL(widgets->label), texto);
}

// Função de ativação do aplicativo
static void activate(GtkApplication *app, gpointer user_data){

    // Aloca memória para a estrutura dos widgets
    AppWidgets *widgets = g_new(AppWidgets, 1);

    // Cria a janela principal
    GtkWidget *window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Exemplo GTK4");
    gtk_window_set_default_size(GTK_WINDOW(window), 300, 200);

    // Cria uma caixa vertical (GtkBox) com espaçamento de 10 pixels entre os elementos
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_widget_set_margin_top(box, 20);
    gtk_widget_set_margin_bottom(box, 20);
    gtk_widget_set_margin_start(box, 20);
    gtk_widget_set_margin_end(box, 20);
    
    // Define a caixa como o widget filho da janela
    gtk_window_set_child(GTK_WINDOW(window), box);

    // Cria os componentes visuais
    widgets->entry = gtk_entry_new();
    widgets->label = gtk_label_new("O texto aparecerá aqui");
    GtkWidget *button = gtk_button_new_with_label("Enviar Texto");

    // Adiciona os widgets na caixa vertical
    gtk_box_append(GTK_BOX(box), widgets->entry);
    gtk_box_append(GTK_BOX(box), button);
    gtk_box_append(GTK_BOX(box), widgets->label);

    // Conecta o sinal de clique do botão à função de callback
    g_signal_connect(button, "clicked", G_CALLBACK(on_button_clicked), widgets);

    // Libera a memória da estrutura automaticamente quando a janela for destruída
    g_object_set_data_full(G_OBJECT(window), "app-widgets", widgets, g_free);

    // Exibe a janela
    gtk_window_present(GTK_WINDOW(window));
}

int main(int argc, char **argv){
    GtkApplication *app;
    int status;

    // Cria uma nova aplicação GTK
    app = gtk_application_new("org.exemplo.gtk4", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    
    // Executa a aplicação
    status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);

    return status;
}
