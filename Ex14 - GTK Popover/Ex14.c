/*****************************************************************
 * Ex14 - Popover
 * 
 * Autor: Eng. Fabrício Ribeiro
 * - Para compilar: make
 * - Para limpar a compilação: make clean
 * - Para executar: ./Ex14
 * 
******************************************************************/

#include <gtk/gtk.h>

// Estrutura para podermos acessar a janela e os estados se necessário
typedef struct {
    GtkWidget *popover;
    GtkWidget *list_box;
} AppWidgets;

// Função chamada quando qualquer caixa de seleção muda de estado
static void on_check_toggled(GtkCheckButton *check_button, gpointer user_data) {
    gboolean ativo = gtk_check_button_get_active(check_button);
    
    // Para obter o texto do botão de marcação, precisamos acessar o rótulo interno no GTK4
    // Uma forma segura é buscar o filho do widget se ele foi criado com texto
    GtkWidget *label = gtk_widget_get_first_child(GTK_WIDGET(check_button));
    const char *texto = "Opção";
    
    if (GTK_IS_LABEL(label)) {
        texto = gtk_label_get_text(GTK_LABEL(label));
    }

    g_print("A %s agora está: %s\n", texto, ativo ? "Marcada" : "Desmarcada");
}

static void activate(GtkApplication *app, gpointer user_data) {
    GtkWidget *window;
    GtkWidget *main_box;
    GtkWidget *menu_button;
    GtkWidget *popover;
    GtkWidget *list_box;
    GtkWidget *check;
    
    const char *opcoes[] = {"Opção A", "Opção B", "Opção C", "Opção D"};

    // Cria a janela principal
    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Seleção Múltipla GTK4");
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 200);

    // Caixa principal da janela
    main_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_widget_set_margin_top(main_box, 20);
    gtk_window_set_child(GTK_WINDOW(window), main_box);

    // 1. Cria o MenuButton (um botão feito para abrir popovers)
    menu_button = gtk_menu_button_new();
    gtk_menu_button_set_label(GTK_MENU_BUTTON(menu_button), "Selecionar Opções ▾");
    gtk_widget_set_halign(menu_button, GTK_ALIGN_CENTER);
    gtk_box_append(GTK_BOX(main_box), menu_button);

    // 2. Cria o Popover (a caixa flutuante que vai aparecer abaixo do botão)
    popover = gtk_popover_new();
    gtk_menu_button_set_popover(GTK_MENU_BUTTON(menu_button), popover);

    // 3. Cria a lista interna que ficará dentro do Popover
    list_box = gtk_list_box_new();
    // Importante: desativa a seleção padrão de linhas, pois usaremos os CheckButtons para isso
    gtk_list_box_set_selection_mode(GTK_LIST_BOX(list_box), GTK_SELECTION_NONE);
    gtk_popover_set_child(GTK_POPOVER(popover), list_box);

    // 4. Adiciona os CheckButtons dentro da lista
    for (int i = 0; i < 4; i++) {
        // Cria o botão de marcar com o texto
        check = gtk_check_button_new_with_label(opcoes[i]);
        
        // Estilização simples para dar espaço interno
        gtk_widget_set_margin_start(check, 10);
        gtk_widget_set_margin_end(check, 10);
        gtk_widget_set_margin_top(check, 8);
        gtk_widget_set_margin_bottom(check, 8);

        // Conecta o sinal de clique/mudança
        g_signal_connect(check, "toggled", G_CALLBACK(on_check_toggled), NULL);

        // Insere na lista
        gtk_list_box_append(GTK_LIST_BOX(list_box), check);
    }

    // Exibe a janela
    gtk_window_present(GTK_WINDOW(window));
}

int main(int argc, char **argv) {
    GtkApplication *app;
    int status;

    app = gtk_application_new("org.exemplo.multiplaselecao", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);

    return status;
}
