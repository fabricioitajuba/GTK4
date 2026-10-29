/*
 leitura em arquivo
*/

#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>

// A struct deve ser exatamente igual à do programa anterior
typedef struct {
    int id;
    char nome[100];
    int idade;
} Pessoa;

// Estrutura para gerenciar os widgets da tela de busca
typedef struct {
    GtkWidget *entry_id;
    GtkWidget *lbl_resultado_nome;
    GtkWidget *lbl_resultado_idade;
} BuscaWidgets;

// Função de callback acionada ao clicar em "Buscar"
static void on_buscar_clicked(GtkButton *btn, gpointer user_data) {
    BuscaWidgets *widgets = (BuscaWidgets *)user_data;

    // Obtém o ID digitado pelo usuário
    const char *id_texto = gtk_editable_get_text(GTK_EDITABLE(widgets->entry_id));
    int id_busca = atoi(id_texto);

    if (id_busca <= 0) {
        gtk_label_set_text(GTK_LABEL(widgets->lbl_resultado_nome), "ID inválido! Digite um número maior que 0.");
        gtk_label_set_text(GTK_LABEL(widgets->lbl_resultado_idade), "");
        return;
    }

    // Abre o arquivo binário apenas para leitura
    FILE *arquivo = fopen("pessoas.dat", "rb");
    if (arquivo == NULL) {
        gtk_label_set_text(GTK_LABEL(widgets->lbl_resultado_nome), "Erro: Arquivo 'pessoas.dat' não encontrado.");
        gtk_label_set_text(GTK_LABEL(widgets->lbl_resultado_idade), "");
        return;
    }

    // Verifica se o ID solicitado existe dentro dos limites do tamanho do arquivo
    fseek(arquivo, 0, SEEK_END);
    long tamanho_arquivo = ftell(arquivo);
    int total_registros = tamanho_arquivo / sizeof(Pessoa);

    if (id_busca > total_registros) {
        gtk_label_set_text(GTK_LABEL(widgets->lbl_resultado_nome), "ID não encontrado no arquivo.");
        gtk_label_set_text(GTK_LABEL(widgets->lbl_resultado_idade), "");
        fclose(arquivo);
        return;
    }

    // Calcula a posição do registro. Ex: ID 1 está na posição 0, ID 2 na posição 1...
    long posicao = (id_busca - 1) * sizeof(Pessoa);
    
    // Move o ponteiro do arquivo direto para o registro desejado
    fseek(arquivo, posicao, SEEK_SET);

    Pessoa p;
    // Lê a estrutura correspondente
    if (fread(&p, sizeof(Pessoa), 1, arquivo) == 1 && p.id == id_busca) {
        char buffer_nome[150];
        char buffer_idade[50];

        g_snprintf(buffer_nome, sizeof(buffer_nome), "Nome: %s", p.nome);
        g_snprintf(buffer_idade, sizeof(buffer_idade), "Idade: %d anos", p.idade);

        // Atualiza a tela com os dados encontrados
        gtk_label_set_text(GTK_LABEL(widgets->lbl_resultado_nome), buffer_nome);
        gtk_label_set_text(GTK_LABEL(widgets->lbl_resultado_idade), buffer_idade);
    } else {
        gtk_label_set_text(GTK_LABEL(widgets->lbl_resultado_nome), "Erro ao ler o registro.");
        gtk_label_set_text(GTK_LABEL(widgets->lbl_resultado_idade), "");
    }

    fclose(arquivo);
}

static void on_activate(GtkApplication *app, gpointer user_data) {
    BuscaWidgets *widgets = g_new0(BuscaWidgets, 1);

    // Janela Principal
    GtkWidget *window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Consultar Registro por ID");
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 280);

    // Container Principal Vertical
    GtkWidget *main_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 12);
    gtk_widget_set_margin_top(main_box, 15);
    gtk_widget_set_margin_bottom(main_box, 15);
    gtk_widget_set_margin_start(main_box, 15);
    gtk_widget_set_margin_end(main_box, 15);
    gtk_window_set_child(GTK_WINDOW(window), main_box);

    // Campo de Entrada do ID
    GtkWidget *lbl_instrucao = gtk_label_new("Digite o número de ID:");
    gtk_widget_set_halign(lbl_instrucao, GTK_ALIGN_START);
    gtk_box_append(GTK_BOX(main_box), lbl_instrucao);

    widgets->entry_id = gtk_entry_new();
    gtk_box_append(GTK_BOX(main_box), widgets->entry_id);

    // Separador visual
    GtkWidget *separator = gtk_separator_new(GTK_ORIENTATION_HORIZONTAL);
    gtk_box_append(GTK_BOX(main_box), separator);

    // Labels para exibição do Resultado da Busca
    widgets->lbl_resultado_nome = gtk_label_new("Aguardando busca...");
    gtk_widget_set_halign(widgets->lbl_resultado_nome, GTK_ALIGN_CENTER);
    gtk_widget_add_css_class(widgets->lbl_resultado_nome, "title-4");
    gtk_box_append(GTK_BOX(main_box), widgets->lbl_resultado_nome);

    widgets->lbl_resultado_idade = gtk_label_new("");
    gtk_widget_set_halign(widgets->lbl_resultado_idade, GTK_ALIGN_CENTER);
    gtk_box_append(GTK_BOX(main_box), widgets->lbl_resultado_idade);

    // Box Horizontal Inferior para os Botões
    GtkWidget *button_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_widget_set_halign(button_box, GTK_ALIGN_END);
    gtk_widget_set_margin_top(button_box, 15);
    gtk_box_append(GTK_BOX(main_box), button_box);

    // Botão Sair
    GtkWidget *btn_sair = gtk_button_new_with_label("Sair");
    g_signal_connect_swapped(btn_sair, "clicked", G_CALLBACK(gtk_window_destroy), window);
    gtk_box_append(GTK_BOX(button_box), btn_sair);

    // Botão Buscar
    GtkWidget *btn_buscar = gtk_button_new_with_label("Buscar ID");
    gtk_widget_add_css_class(btn_buscar, "suggested-action");
    g_signal_connect(btn_buscar, "clicked", G_CALLBACK(on_buscar_clicked), widgets);
    gtk_box_append(GTK_BOX(button_box), btn_buscar);

    // Liberação de memória ao fechar a janela
    g_signal_connect_swapped(window, "destroy", G_CALLBACK(g_free), widgets);

    gtk_window_present(GTK_WINDOW(window));
}

int main(int argc, char **argv) {
    GtkApplication *app;
    int status;

    app = gtk_application_new("org.exemplo.busca_id", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(on_activate), NULL);

    status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);

    return status;
}
