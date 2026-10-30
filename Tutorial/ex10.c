/*
 GTK - Criando Menu com mais opções
*/

#include <gtk/gtk.h>

// --- Callbacks do Menu Arquivo ---
static void on_novo_activated(GSimpleAction *action, GVariant *parameter, gpointer user_data) {
    g_print("Ação 'Novo' ativada.\n");
    // Força o foco de volta para a janela, fechando o menu popover
    gtk_window_present(GTK_WINDOW(user_data));
}

static void on_abrir_activated(GSimpleAction *action, GVariant *parameter, gpointer user_data) {
    g_print("Ação 'Abrir' ativada.\n");
    gtk_window_present(GTK_WINDOW(user_data));
}

static void on_salvar_activated(GSimpleAction *action, GVariant *parameter, gpointer user_data) {
    g_print("Ação 'Salvar' ativada.\n");
    gtk_window_present(GTK_WINDOW(user_data));
}

static void on_sair_activated(GSimpleAction *action, GVariant *parameter, gpointer user_data) {
    g_print("Fechando o aplicativo...\n");
    GtkWindow *window = GTK_WINDOW(user_data);
    gtk_window_destroy(window);
}

// --- Callbacks do Menu Editar ---
static void on_copiar_activated(GSimpleAction *action, GVariant *parameter, gpointer user_data) {
    g_print("Ação 'Copiar' ativada.\n");
    gtk_window_present(GTK_WINDOW(user_data));
}

static void on_colar_activated(GSimpleAction *action, GVariant *parameter, gpointer user_data) {
    g_print("Ação 'Colar' ativada.\n");
    gtk_window_present(GTK_WINDOW(user_data));
}

static void on_alterar_activated(GSimpleAction *action, GVariant *parameter, gpointer user_data) {
    g_print("Ação 'Alterar' ativada.\n");
    gtk_window_present(GTK_WINDOW(user_data));
}

static void activate(GtkApplication *app, gpointer user_data) {

    GtkWidget *window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Exemplo de Menus GTK4");
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 300);

    GMenu *menu_barra = g_menu_new();

    // Menu Arquivo
    GMenu *menu_arquivo = g_menu_new();
    g_menu_append(menu_arquivo, "Novo", "win.novo");
    g_menu_append(menu_arquivo, "Abrir", "win.abrir");
    g_menu_append(menu_arquivo, "Salvar", "win.salvar");
    g_menu_append(menu_arquivo, "Sair", "win.sair");
    g_menu_append_submenu(menu_barra, "Arquivo", G_MENU_MODEL(menu_arquivo));

    // Menu Editar
    GMenu *menu_editar = g_menu_new();
    g_menu_append(menu_editar, "Copiar", "win.copiar");
    g_menu_append(menu_editar, "Colar", "win.colar");
    g_menu_append(menu_editar, "Alterar", "win.alterar");
    g_menu_append_submenu(menu_barra, "Editar", G_MENU_MODEL(menu_editar));

    GtkWidget *menu_bar_widget = gtk_popover_menu_bar_new_from_model(G_MENU_MODEL(menu_barra));

    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_box_append(GTK_BOX(box), menu_bar_widget);

    GtkWidget *label = gtk_label_new("Conteúdo do Aplicativo");
    gtk_widget_set_vexpand(label, TRUE);
    gtk_box_append(GTK_BOX(box), label);

    gtk_window_set_child(GTK_WINDOW(window), box);

    // Passamos a 'window' como o último argumento (user_data) para que os 
    // callbacks consigam chamá-la no gtk_window_present
    const GActionEntry entries[] = {
        { "novo", on_novo_activated, NULL, NULL, NULL },
        { "abrir", on_abrir_activated, NULL, NULL, NULL },
        { "salvar", on_salvar_activated, NULL, NULL, NULL },
        { "sair", on_sair_activated, NULL, NULL, NULL },
        { "copiar", on_copiar_activated, NULL, NULL, NULL },
        { "colar", on_colar_activated, NULL, NULL, NULL },
        { "alterar", on_alterar_activated, NULL, NULL, NULL }
    };

    g_action_map_add_action_entries(G_ACTION_MAP(window), entries, G_N_ELEMENTS(entries), window);

    gtk_window_present(GTK_WINDOW(window));

    g_object_unref(menu_barra);
    g_object_unref(menu_arquivo);
    g_object_unref(menu_editar);
}

int main(int argc, char **argv) {
  
    GtkApplication *app = gtk_application_new("org.gtk.exemplo.menu", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    int status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);
    return status;
}
