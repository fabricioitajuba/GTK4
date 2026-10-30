/*
 GTK - Criando Menu com mais opções
*/

#include <gtk/gtk.h>

// Funções de retorno (callbacks) para cada item do menu
static void on_novo_activated(GSimpleAction *action, GVariant *parameter, gpointer user_data) {
    g_print("Ação 'Novo' ativada.\n");
}

static void on_abrir_activated(GSimpleAction *action, GVariant *parameter, gpointer user_data) {
    g_print("Ação 'Abrir' ativada.\n");
}

static void on_salvar_activated(GSimpleAction *action, GVariant *parameter, gpointer user_data) {
    g_print("Ação 'Salvar' ativada.\n");
}

static void on_sair_activated(GSimpleAction *action, GVariant *parameter, gpointer user_data) {
    g_print("Fechando o aplicativo...\n");
    // Fecha a janela principal que foi passada como user_data
    GtkWindow *window = GTK_WINDOW(user_data);
    gtk_window_destroy(window);
}

static void activate(GtkApplication *app, gpointer user_data) {
    // 1. Criar a janela principal
    GtkWidget *window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Exemplo de Menu GTK4");
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 300);

    // 2. Criar o modelo do Menu usando GMenu
    GMenu *menu_barra = g_menu_new();
    GMenu *menu_arquivo = g_menu_new();

    // Adicionar os itens ao menu Arquivo (Nome visual, nome_da_acao)
    // O prefixo "win." indica que a ação pertence à janela (window)
    g_menu_append(menu_arquivo, "Novo", "win.novo");
    g_menu_append(menu_arquivo, "Abrir", "win.abrir");
    g_menu_append(menu_arquivo, "Salvar", "win.salvar");
    g_menu_append(menu_arquivo, "Sair", "win.sair");

    // Adicionar o menu Arquivo como um submenu da barra de menus principal
    g_menu_append_submenu(menu_barra, "Arquivo", G_MENU_MODEL(menu_arquivo));

    // 3. Criar a barra de menu na interface (Popover Menu Bar)
    GtkWidget *menu_bar_widget = gtk_popover_menu_bar_new_from_model(G_MENU_MODEL(menu_barra));

    // 4. Criar um layout para colocar o menu no topo e o conteúdo abaixo
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_box_append(GTK_BOX(box), menu_bar_widget);

    // Adicionar uma label simples no centro para preencher espaço
    GtkWidget *label = gtk_label_new("Conteúdo do Aplicativo");
    gtk_widget_set_vexpand(label, TRUE);
    gtk_box_append(GTK_BOX(box), label);

    gtk_window_set_child(GTK_WINDOW(window), box);

    // 5. Mapear as ações e conectar as funções de callback à janela
    const GActionEntry entries[] = {
        { "novo", on_novo_activated, NULL, NULL, NULL },
        { "abrir", on_abrir_activated, NULL, NULL, NULL },
        { "salvar", on_salvar_activated, NULL, NULL, NULL },
        { "sair", on_sair_activated, NULL, NULL, NULL }
    };

    g_action_map_add_action_entries(G_ACTION_MAP(window), entries, G_N_ELEMENTS(entries), window);

    // 6. Exibir a janela
    gtk_window_present(GTK_WINDOW(window));

    // Liberar a memória dos modelos de menu (o widget agora cuida deles)
    g_object_unref(menu_barra);
    g_object_unref(menu_arquivo);
}

int main(int argc, char **argv) {
    GtkApplication *app = gtk_application_new("org.gtk.exemplo.menu", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    int status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);
    return status;
}
