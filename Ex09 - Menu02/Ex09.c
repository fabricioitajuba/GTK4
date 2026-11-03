/*****************************************************************
 * Ex09 - Menus
 * 
 * Autor: Eng. Fabrício Ribeiro
 * - Para compilar: make
 * - Para limpar a compilação: make clean
 * - Para executar: ./Ex09
 * 
******************************************************************/

#include <gtk/gtk.h>

// --- Funções Callback Dedicadas para cada Opção do Menu ---

static void on_novo_activated(GSimpleAction *action, GVariant *parameter, gpointer user_data) {
    g_print("Opção selecionada: Novo -> Criando um novo arquivo...\n");
    // Adicione a lógica para criar um novo documento aqui
}

static void on_abrir_activated(GSimpleAction *action, GVariant *parameter, gpointer user_data) {
    g_print("Opção selecionada: Abrir -> Abrindo o seletor de arquivos...\n");
    // Adicione a lógica para abrir um arquivo aqui
}

static void on_salvar_activated(GSimpleAction *action, GVariant *parameter, gpointer user_data) {
    g_print("Opção selecionada: Salvar -> Gravando alterações no disco...\n");
    // Adicione a lógica para salvar o arquivo aqui
}

static void on_sair_activated(GSimpleAction *action, GVariant *parameter, gpointer user_data) {
    g_print("Opção selecionada: Sair -> Fechando a aplicação...\n");
    GApplication *app = G_APPLICATION(user_data);
    g_application_quit(app);
}

// --- Função Principal de Ativação da Interface ---

static void activate(GtkApplication *app, gpointer user_data) {
    // 1. Criar a janela principal
    GtkWidget *window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Exemplo Menu GTK4 Modulado");
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 300);

    // 2. Criar as ações e conectá-las às suas respectivas funções
    GSimpleAction *act_new = g_simple_action_new("novo", NULL);
    g_signal_connect(act_new, "activate", G_CALLBACK(on_novo_activated), NULL);
    g_action_map_add_action(G_ACTION_MAP(window), G_ACTION(act_new));

    GSimpleAction *act_open = g_simple_action_new("abrir", NULL);
    g_signal_connect(act_open, "activate", G_CALLBACK(on_abrir_activated), NULL);
    g_action_map_add_action(G_ACTION_MAP(window), G_ACTION(act_open));

    GSimpleAction *act_save = g_simple_action_new("salvar", NULL);
    g_signal_connect(act_save, "activate", G_CALLBACK(on_salvar_activated), NULL);
    g_action_map_add_action(G_ACTION_MAP(window), G_ACTION(act_save));

    GSimpleAction *act_quit = g_simple_action_new("sair", NULL);
    g_signal_connect(act_quit, "activate", G_CALLBACK(on_sair_activated), app);
    g_action_map_add_action(G_ACTION_MAP(window), G_ACTION(act_quit));

    // 3. Construir a estrutura do Menu
    GMenu *menu = g_menu_new();
    GMenu *submenu_arquivo = g_menu_new();

    g_menu_append(submenu_arquivo, "Novo", "win.novo");
    g_menu_append(submenu_arquivo, "Abrir", "win.abrir");
    g_menu_append(submenu_arquivo, "Salvar", "win.salvar");
    g_menu_append(submenu_arquivo, "Sair", "win.sair");

    g_menu_append_submenu(menu, "Arquivo", G_MENU_MODEL(submenu_arquivo));

    // 4. Criar o widget da barra de menu e estruturar o layout
    GtkWidget *menu_bar = gtk_popover_menu_bar_new_from_model(G_MENU_MODEL(menu));
    
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_box_append(GTK_BOX(box), menu_bar);
    
    GtkWidget *label = gtk_label_new("Funções separadas com sucesso!");
    gtk_widget_set_vexpand(label, TRUE);
    gtk_box_append(GTK_BOX(box), label);

    gtk_window_set_child(GTK_WINDOW(window), box);

    // Liberar memória dos modelos de menu
    g_object_unref(submenu_arquivo);
    g_object_unref(menu);

    gtk_window_present(GTK_WINDOW(window));
}

int main(int argc, char **argv) {
    GtkApplication *app = gtk_application_new("org.gtk.exemplo.menu.separado", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    int status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);
    return status;
}
