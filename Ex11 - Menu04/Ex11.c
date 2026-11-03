/*****************************************************************
 * Ex11 - Menus
 * 
 * Autor: Eng. Fabrício Ribeiro
 * - Para compilar: make
 * - Para limpar a compilação: make clean
 * - Para executar: ./Ex11
 * 
******************************************************************/

#include <gtk/gtk.h>

// --- Declaração das funções para que possam ser referenciadas livremente ---
static void on_abrir_activated(GSimpleAction *action, GVariant *parameter, gpointer user_data);
static void on_salvar_activated(GSimpleAction *action, GVariant *parameter, gpointer user_data);

// --- Função para fechar a janela secundária ao clicar em Sair ---
static void on_fechar_janela_secundaria(GSimpleAction *action, GVariant *parameter, gpointer user_data) {
    g_print("Opção selecionada: Sair da janela secundária -> Fechando esta janela...\n");
    GtkWidget *window = GTK_WIDGET(user_data);
    gtk_window_destroy(GTK_WINDOW(window));
}

// --- Função que cria e abre a Segunda Janela ---
static void on_novo_activated(GSimpleAction *action, GVariant *parameter, gpointer user_data) {
    g_print("Opção selecionada: Novo -> Abrindo uma segunda janela...\n");
    
    // Recupera a instância do GtkApplication enviada via user_data
    GtkApplication *app = GTK_APPLICATION(user_data);

    // 1. Criar a janela secundária
    GtkWidget *sub_window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(sub_window), "Segunda Janela");
    gtk_window_set_default_size(GTK_WINDOW(sub_window), 350, 250);

    // 2. Criar as ações específicas desta nova janela
    GSimpleAction *act_open = g_simple_action_new("abrir", NULL);
    g_signal_connect(act_open, "activate", G_CALLBACK(on_abrir_activated), NULL);
    g_action_map_add_action(G_ACTION_MAP(sub_window), G_ACTION(act_open));

    GSimpleAction *act_save = g_simple_action_new("salvar", NULL);
    g_signal_connect(act_save, "activate", G_CALLBACK(on_salvar_activated), NULL);
    g_action_map_add_action(G_ACTION_MAP(sub_window), G_ACTION(act_save));

    GSimpleAction *act_close = g_simple_action_new("sair_sub", NULL);
    // Passamos a própria sub_window como parâmetro para podermos destruí-la
    g_signal_connect(act_close, "activate", G_CALLBACK(on_fechar_janela_secundaria), sub_window);
    g_action_map_add_action(G_ACTION_MAP(sub_window), G_ACTION(act_close));

    // 3. Montar a barra de menu da segunda janela (Abrir, Salvar, Sair)
    GMenu *menu_bar_model = g_menu_new();
    GMenu *submenu_arquivo = g_menu_new();
    
    g_menu_append(submenu_arquivo, "_Abrir", "win.abrir");
    g_menu_append(submenu_arquivo, "_Salvar", "win.salvar");
    g_menu_append(submenu_arquivo, "_Sair", "win.sair_sub"); // Aponta para a ação de fechar a sub-janela
    g_menu_append_submenu(menu_bar_model, "_Arquivo", G_MENU_MODEL(submenu_arquivo));

    // 4. Montar o layout da segunda janela
    GtkWidget *menu_bar = gtk_popover_menu_bar_new_from_model(G_MENU_MODEL(menu_bar_model));
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_box_append(GTK_BOX(box), menu_bar);
    
    GtkWidget *label = gtk_label_new("Esta é a segunda janela!");
    gtk_widget_set_vexpand(label, TRUE);
    gtk_box_append(GTK_BOX(box), label);

    gtk_window_set_child(GTK_WINDOW(sub_window), box);

    // Liberar referências do menu
    g_object_unref(submenu_arquivo);
    g_object_unref(menu_bar_model);

    // Mostrar a nova janela na tela
    gtk_window_present(GTK_WINDOW(sub_window));
}

// --- Funções Restantes do Menu Principal ---

static void on_abrir_activated(GSimpleAction *action, GVariant *parameter, gpointer user_data) {
    g_print("Opção selecionada: Abrir -> Abrindo o seletor de arquivos...\n");
}

static void on_salvar_activated(GSimpleAction *action, GVariant *parameter, gpointer user_data) {
    g_print("Opção selecionada: Salvar -> Gravando alterações no disco...\n");
}

static void on_sair_principal_activated(GSimpleAction *action, GVariant *parameter, gpointer user_data) {
    g_print("Opção selecionada: Sair do app principal -> Encerrando a aplicação inteira...\n");
    GApplication *app = G_APPLICATION(user_data);
    g_application_quit(app);
}

static void on_copiar_activated(GSimpleAction *action, GVariant *parameter, gpointer user_data) {
    g_print("Opção selecionada: Copiar -> Conteúdo copiado.\n");
}

static void on_colar_activated(GSimpleAction *action, GVariant *parameter, gpointer user_data) {
    g_print("Opção selecionada: Colar -> Conteúdo colado.\n");
}

// --- Ativação da Janela Principal ---

static void activate(GtkApplication *app, gpointer user_data) {
    GtkWidget *window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Janela Principal - GTK4 Menu");
    gtk_window_set_default_size(GTK_WINDOW(window), 450, 350);

    // Registrando ações da janela principal (Passamos 'app' no Novo e no Sair)
    GSimpleAction *act_new = g_simple_action_new("novo", NULL);
    g_signal_connect(act_new, "activate", G_CALLBACK(on_novo_activated), app);
    g_action_map_add_action(G_ACTION_MAP(window), G_ACTION(act_new));

    GSimpleAction *act_open = g_simple_action_new("abrir", NULL);
    g_signal_connect(act_open, "activate", G_CALLBACK(on_abrir_activated), NULL);
    g_action_map_add_action(G_ACTION_MAP(window), G_ACTION(act_open));

    GSimpleAction *act_save = g_simple_action_new("salvar", NULL);
    g_signal_connect(act_save, "activate", G_CALLBACK(on_salvar_activated), NULL);
    g_action_map_add_action(G_ACTION_MAP(window), G_ACTION(act_save));

    GSimpleAction *act_quit = g_simple_action_new("sair", NULL);
    g_signal_connect(act_quit, "activate", G_CALLBACK(on_sair_principal_activated), app);
    g_action_map_add_action(G_ACTION_MAP(window), G_ACTION(act_quit));

    GSimpleAction *act_copy = g_simple_action_new("copiar", NULL);
    g_signal_connect(act_copy, "activate", G_CALLBACK(on_copiar_activated), NULL);
    g_action_map_add_action(G_ACTION_MAP(window), G_ACTION(act_copy));

    GSimpleAction *act_paste = g_simple_action_new("colar", NULL);
    g_signal_connect(act_paste, "activate", G_CALLBACK(on_colar_activated), NULL);
    g_action_map_add_action(G_ACTION_MAP(window), G_ACTION(act_paste));

    // Montando a barra de menus principal
    GMenu *menu_bar_model = g_menu_new();

    GMenu *submenu_arquivo = g_menu_new();
    g_menu_append(submenu_arquivo, "_Novo", "win.novo");
    g_menu_append(submenu_arquivo, "_Abrir", "win.abrir");
    g_menu_append(submenu_arquivo, "_Salvar", "win.salvar");
    g_menu_append(submenu_arquivo, "_Sair", "win.sair");
    g_menu_append_submenu(menu_bar_model, "_Arquivo", G_MENU_MODEL(submenu_arquivo));

    GMenu *submenu_editar = g_menu_new();
    g_menu_append(submenu_editar, "_Copiar", "win.copiar");
    g_menu_append(submenu_editar, "C_olar", "win.colar");
    g_menu_append_submenu(menu_bar_model, "_Editar", G_MENU_MODEL(submenu_editar));

    GtkWidget *menu_bar = gtk_popover_menu_bar_new_from_model(G_MENU_MODEL(menu_bar_model));
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_box_append(GTK_BOX(box), menu_bar);
    
    GtkWidget *label = gtk_label_new("Clique em Arquivo -> Novo para abrir a segunda janela.");
    gtk_widget_set_vexpand(label, TRUE);
    gtk_box_append(GTK_BOX(box), label);

    gtk_window_set_child(GTK_WINDOW(window), box);

    g_object_unref(submenu_arquivo);
    g_object_unref(submenu_editar);
    g_object_unref(menu_bar_model);

    gtk_window_present(GTK_WINDOW(window));
}

int main(int argc, char **argv) {
    // CORRIGIDO: Removido o espaço em branco do ID do aplicativo
    GtkApplication *app = gtk_application_new("org.gtk.exemplo.multi.janelas", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    int status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);
    return status;
}
