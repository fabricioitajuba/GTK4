/*
 relógio
*/

#include <gtk/gtk.h>

// Estrutura para gerenciar os widgets da interface
typedef struct {
    GtkWidget *label;
    GTimeZone *tz_brasilia;
} AppData;

// Função de callback que atualiza o relógio a cada 1 segundo (1000ms)
static gboolean atualizar_relogio(gpointer data) {
    AppData *app_data = (AppData *)data;
    
    // Obtém o horário atual do sistema aplicando explicitamente o fuso de Brasília
    GDateTime *agora = g_date_time_new_now(app_data->tz_brasilia);
    
    // Formata a string (Hora:Minuto:Segundo)
    // %X formata conforme as configurações locais ou você pode fixar com "%H:%M:%S"
    char *hora_formatada = g_date_time_format(agora, "%H:%M:%S");
    
    // Atualiza o texto da interface gráfica
    gtk_label_set_text(GTK_LABEL(app_data->label), hora_formatada);
    
    // Libera a memória alocada pelas estruturas de tempo do GLib
    g_free(hora_formatada);
    g_date_time_unref(agora);
    
    // Retorna TRUE para manter o timer rodando continuamente
    return TRUE;
}

static void on_activate(GtkApplication *app, gpointer user_data) {
    AppData *app_data = g_new0(AppData, 1);
    
    // Cria o identificador de fuso horário para Brasília (America/Sao_Paulo)
    app_data->tz_brasilia = g_time_zone_new_identifier("America/Sao_Paulo");
    
    // Caso o identificador falhe por falta de dados no S.O., recua para o UTC-3 fixo
    if (!app_data->tz_brasilia) {
        app_data->tz_brasilia = g_time_zone_new_offset(-3 * 3600);
    }

    // Criação da janela principal do GTK4
    GtkWidget *window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Relógio de Brasília (GTK4)");
    gtk_window_set_default_size(GTK_WINDOW(window), 350, 150);

    // Cria o rótulo de texto (Label) inicializando vazio
    app_data->label = gtk_label_new("--:--:--");
    gtk_widget_set_halign(app_data->label, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(app_data->label, GTK_ALIGN_CENTER);
    
    // No GTK4 moderno (4.10+), adicionamos classes CSS diretamente no Widget
    gtk_widget_add_css_class(app_data->label, "title-1");

    // Adiciona o label diretamente como filho da janela
    gtk_window_set_child(GTK_WINDOW(window), app_data->label);

    // Força uma primeira execução imediata para não exibir os traços '--:--:--'
    atualizar_relogio(app_data);

    // Registra o loop para rodar a cada 1000 milissegundos
    g_timeout_add(1000, atualizar_relogio, app_data);

    // Callback de limpeza de memória quando a janela fechar
    g_signal_connect_swapped(window, "destroy", G_CALLBACK(g_free), app_data);

    // Exibe a tela
    gtk_window_present(GTK_WINDOW(window));
}

int main(int argc, char **argv) {
    GtkApplication *app;
    int status;

    app = gtk_application_new("org.exemplo.relogio", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(on_activate), NULL);
    
    status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);

    return status;
}
