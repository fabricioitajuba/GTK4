/*****************************************************************
 * Ex10 - Menus
 * 
 * Autor: Eng. Fabrício Ribeiro
 * - Para compilar: make
 * - Para limpar a compilação: make clean
 * - Para executar: ./Ex10
 * 
******************************************************************/

#include <gtk/gtk.h>

// --- Funções do Menu Arquivo ---

static void on_novo_activated(GSimpleAction *action, GVariant *parameter, gpointer user_data) {
    g_print("Opção selecionada: Novo -> Criando um novo arquivo...\n");
}

static void on_abrir_activated(GSimpleAction *action, GVariant *parameter, gpointer user_data) {
    g_print("Opção selecionada: Abrir -> Abrindo o seletor de arquivos...\n");
}

static void on_salvar_activated(GSimpleAction *action, GVariant *parameter, gpointer user_data) {
    g_print("Opção selecionada: Salvar -> Gravando alterações no disco...\n");
}

static void on_sair_activated(GSimpleAction *action, GVariant *parameter, gpointer user_data) {
    g_print("Opção selecionada: Sair -> Fechando a aplicação...\n");
    GApplication *app = G_APPLICATION(user_data);
    g_application_quit(app);
}

// --- Funções do Menu Editar ---

static void on_copiar_activated(GSimpleAction *action, GVariant *parameter, gpointer user_data) {
    g_print("Opção selecionada: Copiar -> Conteúdo copiado para a área de transferência.\n");
}

static void on_colar_activated(GSimpleAction *action, GVariant *parameter, gpointer user_data) {
    g_print("Opção selecionada: Colar -> Conteúdo colado da área de transferência.\n");
}

// --- Função Principal de Ativação da Interface ---

static void activate(GtkApplication *app, gpointer user_data) {
    // 1. Criar a janela principal
    GtkWidget *window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Menu GTK4 com Atalhos Alt");
    gtk_window_set_default_size(GTK_WINDOW(window), 450, 350);

    // 2. Criar e registrar as Ações (Menu Arquivo)
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

    // Criar e registrar as Ações (Menu Editar)
    GSimpleAction *act_copy = g_simple_action_new("copiar", NULL);
    g_signal_connect(act_copy, "activate", G_CALLBACK(on_copiar_activated), NULL);
    g_action_map_add_action(G_ACTION_MAP(window), G_ACTION(act_copy));

    GSimpleAction *act_paste = g_simple_action_new("colar", NULL);
    g_signal_connect(act_paste, "activate", G_CALLBACK(on_colar_activated), NULL);
    g_action_map_add_action(G_ACTION_MAP(window), G_ACTION(act_paste));


    // 3. Construir a estrutura do Menu usando Mnemonics (o caractere '_')
    GMenu *menu_bar_model = g_menu_new();

    // Criando o Submenu Arquivo (Note o '_A' de _Arquivo e '_S' de _Sair)
    GMenu *submenu_arquivo = g_menu_new();
    g_menu_append(submenu_arquivo, "_Novo", "win.novo");
    g_menu_append(submenu_arquivo, "_Abrir", "win.abrir");
    g_menu_append(submenu_arquivo, "Sa_lvar", "win.salvar");
    g_menu_append(submenu_arquivo, "_Sair", "win.sair");
    g_menu_append_submenu(menu_bar_model, "_Arquivo", G_MENU_MODEL(submenu_arquivo));

    // Criando o Submenu Editar (Note o '_E' de _Editar e '_C' de _Copiar)
    GMenu *submenu_editar = g_menu_new();
    g_menu_append(submenu_editar, "_Copiar", "win.copiar");
    g_menu_append(submenu_editar, "C_olar", "win.colar"); // 'o' vira o atalho pois 'C' já foi usado em Copiar
    g_menu_append_submenu(menu_bar_model, "_Editar", G_MENU_MODEL(submenu_editar));


    // 4. Criar o widget da barra de menu e estruturar o layout
    GtkWidget *menu_bar = gtk_popover_menu_bar_new_from_model(G_MENU_MODEL(menu_bar_model));
    
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_box_append(GTK_BOX(box), menu_bar);
    
    GtkWidget *label = gtk_label_new("Pressione 'Alt' para ver as letras de atalho sublinhadas!");
    gtk_widget_set_vexpand(label, TRUE);
    gtk_box_append(GTK_BOX(box), label);

    gtk_window_set_child(GTK_WINDOW(window), box);

    // Liberar memória dos modelos de menu
    g_object_unref(submenu_arquivo);
    g_object_unref(submenu_editar);
    g_object_unref(menu_bar_model);

    gtk_window_present(GTK_WINDOW(window));
}

int main(int argc, char **argv) {
    GtkApplication *app = gtk_application_new("org.gtk.exemplo.menu.atalhos", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    int status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);
    return status;
}
