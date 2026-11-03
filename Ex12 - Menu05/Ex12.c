/*****************************************************************
 * Ex12 - Menus
 * 
 * Autor: Eng. Fabrício Ribeiro
 * - Para compilar: make
 * - Para limpar a compilação: make clean
 * - Para executar: ./Ex12
 * 
******************************************************************/

#include <gtk/gtk.h>

// Estrutura para passar dados personalizados para os callbacks se necessário
typedef struct{
    GtkWindow *main_window;
} AppWindowData;

// Callback acionado quando o arquivo é selecionado ou o diálogo é fechado
static void on_file_open_finish(GObject *source_object, GAsyncResult *res, gpointer user_data){
    GtkFileDialog *dialog = GTK_FILE_DIALOG(source_object);
    GError *error = NULL;
    
    // Obtém o arquivo selecionado
    GFile *file = gtk_file_dialog_open_finish(dialog, res, &error);
    
    if (error) {
        // Trata o erro ou cancelamento (ex: usuário clicou em Fechar/Cancelar)
        g_print("Seleção de arquivo cancelada ou falhou: %s\n", error->message);
        g_clear_error(&error);
        return;
    }

    if (file){
        // Obtém o caminho do arquivo (ponteiro para string)
        char *path = g_file_get_path(file);
        g_print("Arquivo selecionado com sucesso: %s\n", path);
        
        // --- O PONTEIRO DO ARQUIVO ESTÁ AQUI ---
        // Você pode usar o ponteiro 'path' ou o objeto 'file' aqui para ler o arquivo.
        
        // Libera a memória do caminho retornado
        g_free(path);
        // Libera a referência do GFile
        g_object_unref(file);
    }
}

// Action: Callback para a opção "Abrir" do menu
static void on_open_action_activated(GSimpleAction *action, GVariant *parameter, gpointer user_data){
    GtkWindow *parent_window = GTK_WINDOW(user_data);
    
    // Cria o novo diálogo de arquivos do GTK4
    GtkFileDialog *file_dialog = gtk_file_dialog_new();
    gtk_file_dialog_set_title(file_dialog, "Abrir Arquivo");
    
    // Abre a janela de diálogo de forma assíncrona (nativo do sistema, mostra unidades/pastas)
    // Ela já vem por padrão com os botões "Abrir" e "Cancelar/Fechar"
    gtk_file_dialog_open(file_dialog, parent_window, NULL, on_file_open_finish, NULL);
    
    // Libera a referência local já que o diálogo gerencia seu próprio ciclo de vida assíncrono
    g_object_unref(file_dialog);
}

// Action: Callback para a opção "Sair" do menu
static void on_quit_action_activated(GSimpleAction *action, GVariant *parameter, gpointer user_data){
    GApplication *app = G_APPLICATION(user_data);
    // Fecha o programa com segurança
    g_application_quit(app);
}

// Callback principal da aplicação GTK
static void on_app_activate(GApplication *app, gpointer user_data){

    // Cria a janela principal
    GtkWidget *window = gtk_application_window_new(GTK_APPLICATION(app));
    gtk_window_set_title(GTK_WINDOW(window), "Exemplo Menu GTK4");
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 300);

    // 1. Criação correta da estrutura do Menu para o GTK4
    GMenu *menu_bar = g_menu_new();
    GMenu *menu_arquivo = g_menu_new();

    // Adiciona os itens ao menu "Arquivo"
    g_menu_append(menu_arquivo, "Abrir", "app.open");
    g_menu_append(menu_arquivo, "Sair", "app.quit");
    
    // Adiciona o menu "Arquivo" como um submenu da barra de menus principal
    g_menu_append_submenu(menu_bar, "Arquivo", G_MENU_MODEL(menu_arquivo));

    // Associa a barra de menus ao aplicativo e ativa a exibição na janela
    gtk_application_set_menubar(GTK_APPLICATION(app), G_MENU_MODEL(menu_bar));
    gtk_application_window_set_show_menubar(GTK_APPLICATION_WINDOW(window), TRUE);

    // 2. Criação das Ações (Actions)
    GSimpleAction *open_action = g_simple_action_new("open", NULL);
    g_signal_connect(open_action, "activate", G_CALLBACK(on_open_action_activated), window);
    g_action_map_add_action(G_ACTION_MAP(app), G_ACTION(open_action));

    GSimpleAction *quit_action = g_simple_action_new("quit", NULL);
    g_signal_connect(quit_action, "activate", G_CALLBACK(on_quit_action_activated), app);
    g_action_map_add_action(G_ACTION_MAP(app), G_ACTION(quit_action));

    // Libera a memória local dos menus
    g_object_unref(menu_arquivo);
    g_object_unref(menu_bar);

    // Exibe a janela principal
    gtk_window_present(GTK_WINDOW(window));
}


int main(int argc, char **argv) {

    GtkApplication *app = gtk_application_new("org.exemplo.menuapp", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(on_app_activate), NULL);
    int status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);
    
    return status;
}
