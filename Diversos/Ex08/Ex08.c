/*
 Usando o timer
*/
#include <gtk/gtk.h>

// Estrutura para passar dados para o callback do timer
typedef struct {
    GtkWidget *label;
    int contador;
} AppData;

// Função de callback que é chamada a cada 1 segundo (1000ms)
static gboolean atualizar_contador(gpointer data) {
    AppData *app_data = (AppData *)data;
    
    // Incrementa o contador
    app_data->contador++;
    
    // Atualiza o texto do label
    char texto[32];
    g_snprintf(texto, sizeof(texto), "Segundos: %d", app_data->contador);
    gtk_label_set_text(GTK_LABEL(app_data->label), texto);
    
    // Retornar TRUE mantém o timer ativo. Retornar FALSE desativa o timer.
    return TRUE;
}

// Função de ativação do aplicativo GTK
static void on_activate(GtkApplication *app, gpointer user_data) {
    // Aloca a estrutura de dados (usando g_new0 para limpar a memória)
    AppData *app_data = g_new0(AppData, 1);
    app_data->contador = 0;

    // Cria a janela principal
    GtkWidget *window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Contador GTK4");
    gtk_window_set_default_size(GTK_WINDOW(window), 300, 200);

    // Cria o widget de texto (Label) com tamanho de fonte aumentado
    app_data->label = gtk_label_new("Segundos: 0");
    gtk_widget_set_halign(app_data->label, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(app_data->label, GTK_ALIGN_CENTER);
    
    // Adiciona uma estilização CSS simples para aumentar a fonte (opcional)
    GtkStyleContext *context = gtk_widget_get_style_context(app_data->label);
    gtk_style_context_add_class(context, "title-1");

    // Adiciona o label à janela
    gtk_window_set_child(GTK_WINDOW(window), app_data->label);

    // Configura o timer para rodar a cada 1000 milissegundos (1 segundo)
    g_timeout_add(1000, atualizar_contador, app_data);

    // Libera a memória da estrutura quando a janela for destruída
    g_signal_connect_swapped(window, "destroy", G_CALLBACK(g_free), app_data);

    // Exibe a janela
    gtk_window_present(GTK_WINDOW(window));
}

int main(int argc, char **argv) {
    GtkApplication *app;
    int status;

    // Cria uma nova aplicação GTK
    app = gtk_application_new("org.exemplo.contador", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(on_activate), NULL);
    
    // Executa a aplicação
    status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);

    return status;
}
