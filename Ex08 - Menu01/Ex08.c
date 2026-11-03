/*****************************************************************
 * Ex08 - Menus
 * 
 * Autor: Eng. Fabrício Ribeiro
 * - Para compilar: make
 * - Para limpar a compilação: make clean
 * - Para executar: ./Ex08
 * 
******************************************************************/

#include <gtk/gtk.h>

// Função callback que é chamada quando as opções Novo, Abrir ou Salvar são clicadas
static void on_menu_item_activated(GSimpleAction *action, GVariant *parameter, gpointer user_data) {
    const gchar *name = g_action_get_name(G_ACTION(action));
    g_print("Opção selecionada: %s\n", name);
}

// Função callback para a opção Sair
static void on_quit_activated(GSimpleAction *action, GVariant *parameter, gpointer user_data) {
    g_print("Opção selecionada: Sair. Fechando a aplicação...\n");
    GApplication *app = G_APPLICATION(user_data);
    g_application_quit(app);
}

static void activate(GtkApplication *app, gpointer user_data) {
    // 1. Criar a janela principal
    GtkWidget *window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Exemplo Menu GTK4");
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 300);

    // 2. Criar as ações (ações que o menu vai disparar)
    // O prefixo "win." mapeia a ação para a janela atual
    GSimpleAction *act_new = g_simple_action_new("novo", NULL);
    g_signal_connect(act_new, "activate", G_CALLBACK(on_menu_item_activated), NULL);
    g_action_map_add_action(G_ACTION_MAP(window), G_ACTION(act_new));

    GSimpleAction *act_open = g_simple_action_new("abrir", NULL);
    g_signal_connect(act_open, "activate", G_CALLBACK(on_menu_item_activated), NULL);
    g_action_map_add_action(G_ACTION_MAP(window), G_ACTION(act_open));

    GSimpleAction *act_save = g_simple_action_new("salvar", NULL);
    g_signal_connect(act_save, "activate", G_CALLBACK(on_menu_item_activated), NULL);
    g_action_map_add_action(G_ACTION_MAP(window), G_ACTION(act_save));

    GSimpleAction *act_quit = g_simple_action_new("sair", NULL);
    // Passamos o 'app' como argumento para podermos fechá-lo na função do Sair
    g_signal_connect(act_quit, "activate", G_CALLBACK(on_quit_activated), app);
    g_action_map_add_action(G_ACTION_MAP(window), G_ACTION(act_quit));

    // 3. Construir a estrutura do Menu
    GMenu *menu = g_menu_new();
    GMenu *submenu_arquivo = g_menu_new();

    // Adiciona os itens ao submenu "Arquivo" ligando-os às ações "win.nome_da_acao"
    g_menu_append(submenu_arquivo, "Novo", "win.novo");
    g_menu_append(submenu_arquivo, "Abrir", "win.abrir");
    g_menu_append(submenu_arquivo, "Salvar", "win.salvar");
    g_menu_append(submenu_arquivo, "Sair", "win.sair");

    // Adiciona o submenu à barra de menu principal chamada "Arquivo"
    g_menu_append_submenu(menu, "Arquivo", G_MENU_MODEL(submenu_arquivo));

    // 4. Criar o widget da barra de menu e adicionar à janela
    GtkWidget *menu_bar = gtk_popover_menu_bar_new_from_model(G_MENU_MODEL(menu));
    
    // Organizar o layout (Menu no topo e um espaço para o conteúdo embaixo)
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_box_append(GTK_BOX(box), menu_bar);
    
    // Adiciona um label simples apenas para preencher a janela
    GtkWidget *label = gtk_label_new("Conteúdo do aplicativo aqui");
    gtk_widget_set_vexpand(label, TRUE);
    gtk_box_append(GTK_BOX(box), label);

    gtk_window_set_child(GTK_WINDOW(window), box);

    // Libera a memória dos modelos de menu que não são mais necessários diretamente
    g_object_unref(submenu_arquivo);
    g_object_unref(menu);

    gtk_window_present(GTK_WINDOW(window));
}

int main(int argc, char **argv) {
    GtkApplication *app = gtk_application_new("org.gtk.exemplo.menu", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    int status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);
    return status;
}
