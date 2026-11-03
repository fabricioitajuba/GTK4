/*****************************************************************
 * Ex13 - List Box
 * 
 * Autor: Eng. Fabrício Ribeiro
 * - Para compilar: make
 * - Para limpar a compilação: make clean
 * - Para executar: ./Ex13
 * 
******************************************************************/

#include <gtk/gtk.h>

// Função chamada quando o usuário altera a opção selecionada
static void on_dropdown_changed(GtkDropDown *dropdown, GParamSpec *pspec, gpointer user_data) {
    // Obtém o índice do item selecionado
    guint selected = gtk_drop_down_get_selected(dropdown);
    
    // Obtém o modelo de dados (GtkStringList) associado
    GListModel *model = gtk_drop_down_get_model(dropdown);
    
    // Obtém a string correspondente à posição selecionada
    const char *text = gtk_string_list_get_string(GTK_STRING_LIST(model), selected);
    
    g_print("Opção selecionada: %s (Índice: %d)\n", text, selected);
}

static void activate(GtkApplication *app, gpointer user_data) {
    GtkWidget *window;
    GtkWidget *box;
    GtkWidget *dropdown;
    
    // Lista de opções que estarão dentro da caixa
    const char *opcoes[] = {"Opção 1", "Opção 2", "Opção 3", "Opção 4", NULL};

    // Cria a janela principal
    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Exemplo GtkDropDown GTK4");
    gtk_window_set_default_size(GTK_WINDOW(window), 300, 150);

    // Cria uma caixa organizadora (GtkBox) com margens
    box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_widget_set_margin_top(box, 20);
    gtk_widget_set_margin_bottom(box, 20);
    gtk_widget_set_margin_start(box, 20);
    gtk_widget_set_margin_end(box, 20);
    gtk_window_set_child(GTK_WINDOW(window), box);

    // Cria o modelo de strings do GTK4 a partir do nosso array
    GtkStringList *string_list = gtk_string_list_new(opcoes);

    // Cria o widget DropDown passando o modelo de strings criado
    dropdown = gtk_drop_down_new(G_LIST_MODEL(string_list), NULL);
    
    // Centraliza e ajusta o tamanho do dropdown na janela
    gtk_widget_set_halign(dropdown, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(dropdown, GTK_ALIGN_START);

    // Adiciona o dropdown dentro da nossa caixa vertical
    gtk_box_append(GTK_BOX(box), dropdown);

    // Conecta o sinal para detectar quando a seleção muda
    // Nota: "selected" é uma propriedade, por isso usamos 'notify::selected'
    g_signal_connect(dropdown, "notify::selected", G_CALLBACK(on_dropdown_changed), NULL);

    // Exibe a janela
    gtk_window_present(GTK_WINDOW(window));
}

int main(int argc, char **argv) {
    GtkApplication *app;
    int status;

    app = gtk_application_new("org.exemplo.dropdown", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);

    return status;
}

