/*
 GTK - Soma entre 2 números
*/
#include <gtk/gtk.h>
#include <stdlib.h>

typedef struct {
    GtkWidget *entrada1;
    GtkWidget *entrada2;
    GtkWidget *label_valor_soma; // Agora aponta apenas para o valor numérico
} WidgetsSoma;

static void on_botao_somar_clicked(GtkButton *button, gpointer user_data){

    WidgetsSoma *widgets = (WidgetsSoma *)user_data;

    GtkEntryBuffer *buffer1 = gtk_entry_get_buffer(GTK_ENTRY(widgets->entrada1));
    GtkEntryBuffer *buffer2 = gtk_entry_get_buffer(GTK_ENTRY(widgets->entrada2));

    const char *texto1 = gtk_entry_buffer_get_text(buffer1);
    const char *texto2 = gtk_entry_buffer_get_text(buffer2);

    double num1 = atof(texto1);
    double num2 = atof(texto2);
    double resultado = num1 + num2;

    // Formata apenas o número resultante
    char resultado_texto[64];
    g_snprintf(resultado_texto, sizeof(resultado_texto), "%.2f", resultado);

    // Atualiza o valor numérico com destaque em negrito
    char markup_texto[128];
    g_snprintf(markup_texto, sizeof(markup_texto), "<span size='large' weight='bold'>%s</span>", resultado_texto);
    gtk_label_set_markup(GTK_LABEL(widgets->label_valor_soma), markup_texto);
}

static void activate(GtkApplication *app, gpointer user_data){

    GtkWidget *window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Calculadora de Soma - GTK4");
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 220);

    GtkWidget *box_principal = gtk_box_new(GTK_ORIENTATION_VERTICAL, 15);
    gtk_widget_set_margin_top(box_principal, 20);
    gtk_widget_set_margin_bottom(box_principal, 20);
    gtk_widget_set_margin_start(box_principal, 20);
    gtk_widget_set_margin_end(box_principal, 20);
    gtk_window_set_child(GTK_WINDOW(window), box_principal);

    // Criar o GtkGrid para alinhar tudo simetricamente
    GtkWidget *grid = gtk_grid_new();
    gtk_grid_set_column_spacing(GTK_GRID(grid), 15); // Espaço horizontal entre colunas
    gtk_grid_set_row_spacing(GTK_GRID(grid), 12);    // Espaço vertical entre linhas
    gtk_box_append(GTK_BOX(box_principal), grid);

    WidgetsSoma *widgets = g_new(WidgetsSoma, 1);

    // --- LINHA 0: Primeiro Número ---
    GtkWidget *label_num1 = gtk_label_new("Digite o primeiro número:");
    gtk_widget_set_halign(label_num1, GTK_ALIGN_START);
    
    widgets->entrada1 = gtk_entry_new();
    gtk_widget_set_hexpand(widgets->entrada1, TRUE);

    gtk_grid_attach(GTK_GRID(grid), label_num1, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), widgets->entrada1, 1, 0, 1, 1);

    // --- LINHA 1: Segundo Número ---
    GtkWidget *label_num2 = gtk_label_new("Digite o segundo número:");
    gtk_widget_set_halign(label_num2, GTK_ALIGN_START);

    widgets->entrada2 = gtk_entry_new();
    gtk_widget_set_hexpand(widgets->entrada2, TRUE);

    gtk_grid_attach(GTK_GRID(grid), label_num2, 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), widgets->entrada2, 1, 1, 1, 1);

    // --- LINHA 2: Botão Somar ---
    GtkWidget *botao_somar = gtk_button_new_with_label("Somar");
    // Alinha ao centro horizontalmente para que o botão não estique por toda a tela
    gtk_widget_set_halign(botao_somar, GTK_ALIGN_CENTER);
    // gtk_grid_attach(grid, widget, coluna, linha, largura_em_colunas, altura_em_linhas)
    // Fazemos o botão ocupar a linha 2 inteira unindo as duas colunas (largura = 2)
    //gtk_grid_attach(GTK_GRID(grid), botao_somar, 0, 2, 2, 1);
    gtk_grid_attach(GTK_GRID(grid), botao_somar, 1, 2, 1, 1); // Colocamos o botão na coluna 1 (direita), linha 2, ocupando apenas 1 coluna de largura

    // --- LINHA 3: Resultado Lado a Lado ---
    GtkWidget *label_resultado_fixo = gtk_label_new("Resultado:");
    gtk_widget_set_halign(label_resultado_fixo, GTK_ALIGN_START);
    // Aplica negrito apenas na palavra "Resultado:"
    gtk_label_set_markup(GTK_LABEL(label_resultado_fixo), "<span size='large' weight='bold'>Resultado:</span>");

    widgets->label_valor_soma = gtk_label_new("");
    gtk_widget_set_halign(widgets->label_valor_soma, GTK_ALIGN_START);
    gtk_label_set_markup(GTK_LABEL(widgets->label_valor_soma), "<span size='large' weight='bold'>0.00</span>");

    gtk_grid_attach(GTK_GRID(grid), label_resultado_fixo, 0, 3, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), widgets->label_valor_soma, 1, 3, 1, 1);

    g_signal_connect(botao_somar, "clicked", G_CALLBACK(on_botao_somar_clicked), widgets);
    g_signal_connect_swapped(window, "destroy", G_CALLBACK(g_free), widgets);

    gtk_window_present(GTK_WINDOW(window));
}

int main(int argc, char **argv) {

    GtkApplication *app = gtk_application_new("org.gtk.exemplo.soma.grid2", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    int status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);

    return status;
}

