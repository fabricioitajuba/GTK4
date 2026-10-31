/*
 escrita em arquivo
*/

#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>

// Definição da struct atualizada com o campo ID
typedef struct {
    int id;
    char nome[100];
    int idade;
} Pessoa;

// Estrutura para dar acesso aos campos de texto (Entry) nos callbacks
typedef struct {
    GtkWidget *entry_nome;
    GtkWidget *entry_idade;
} AppWidgets;

// Função que calcula o próximo ID com base no tamanho do arquivo
static int obter_proximo_id() {
    FILE *arquivo = fopen("pessoas.dat", "rb");
    if (arquivo == NULL) {
        // Se o arquivo não existe, este é o primeiro registro
        return 1;
    }

    // Move o ponteiro para o final do arquivo
    fseek(arquivo, 0, SEEK_END);
    // Obtém o tamanho total do arquivo em bytes
    long tamanho_arquivo = ftell(arquivo);
    fclose(arquivo);

    // O próximo ID será o número de registros atuais + 1
    int total_registros = tamanho_arquivo / sizeof(Pessoa);
    return total_registros + 1;
}

// Função de callback acionada ao clicar em "Salvar"
static void on_salvar_clicked(GtkButton *btn, gpointer user_data) {
    AppWidgets *widgets = (AppWidgets *)user_data;

    // Obtém o texto digitado nos campos
    const char *nome_digitado = gtk_editable_get_text(GTK_EDITABLE(widgets->entry_nome));
    const char *idade_digitada = gtk_editable_get_text(GTK_EDITABLE(widgets->entry_idade));

    // Validação simples para não salvar campos vazios
    if (strlen(nome_digitado) == 0 || strlen(idade_digitada) == 0) {
        g_print("Por favor, preencha todos os campos antes de salvar.\n");
        return;
    }

    // Preenche a struct com os dados e o ID automático
    Pessoa p;
    p.id = obter_proximo_id(); // <-- ID gerado automaticamente aqui
    g_snprintf(p.nome, sizeof(p.nome), "%s", nome_digitado);
    p.idade = atoi(idade_digitada);

    // Abre o arquivo binário em modo "append" (adiciona ao final)
    FILE *arquivo = fopen("pessoas.dat", "ab");
    if (arquivo == NULL) {
        g_print("Erro ao abrir o arquivo para escrita.\n");
        return;
    }

    // Grava a struct inteira de uma vez no arquivo binário
    fwrite(&p, sizeof(Pessoa), 1, arquivo);
    fclose(arquivo);

    // Exibe no console a confirmação com o ID gerado
    g_print("Dados salvos com sucesso! [ID: %d] Nome: %s, Idade: %d\n", p.id, p.nome, p.idade);

    // Limpa os campos de texto após salvar
    gtk_editable_set_text(GTK_EDITABLE(widgets->entry_nome), "");
    gtk_editable_set_text(GTK_EDITABLE(widgets->entry_idade), "");
}

// Função de ativação do aplicativo GTK
static void on_activate(GtkApplication *app, gpointer user_data) {
    AppWidgets *widgets = g_new0(AppWidgets, 1);

    // Janela Principal
    GtkWidget *window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Cadastro Binário com ID GTK4");
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 250);

    // Box Principal (Organização Vertical dos elementos)
    GtkWidget *main_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 12);
    gtk_widget_set_margin_top(main_box, 15);
    gtk_widget_set_margin_bottom(main_box, 15);
    gtk_widget_set_margin_start(main_box, 15);
    gtk_widget_set_margin_end(main_box, 15);
    gtk_window_set_child(GTK_WINDOW(window), main_box);

    // --- Campo do Nome ---
    GtkWidget *lbl_nome = gtk_label_new("Digite o nome:");
    gtk_widget_set_halign(lbl_nome, GTK_ALIGN_START);
    gtk_box_append(GTK_BOX(main_box), lbl_nome);

    widgets->entry_nome = gtk_entry_new();
    gtk_box_append(GTK_BOX(main_box), widgets->entry_nome);

    // --- Campo da Idade ---
    GtkWidget *lbl_idade = gtk_label_new("Digite a idade:");
    gtk_widget_set_halign(lbl_idade, GTK_ALIGN_START);
    gtk_box_append(GTK_BOX(main_box), lbl_idade);

    widgets->entry_idade = gtk_entry_new();
    gtk_box_append(GTK_BOX(main_box), widgets->entry_idade);

    // --- Box dos Botões (Organização Horizontal inferior) ---
    GtkWidget *button_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_widget_set_halign(button_box, GTK_ALIGN_END);
    gtk_box_append(GTK_BOX(main_box), button_box);

    // Botão Sair
    GtkWidget *btn_sair = gtk_button_new_with_label("Sair");
    g_signal_connect_swapped(btn_sair, "clicked", G_CALLBACK(gtk_window_destroy), window);
    gtk_box_append(GTK_BOX(button_box), btn_sair);

    // Botão Salvar
    GtkWidget *btn_salvar = gtk_button_new_with_label("Salvar");
    gtk_widget_add_css_class(btn_salvar, "suggested-action");
    g_signal_connect(btn_salvar, "clicked", G_CALLBACK(on_salvar_clicked), widgets);
    gtk_box_append(GTK_BOX(button_box), btn_salvar);

    // Limpeza de memória da estrutura quando a janela fechar
    g_signal_connect_swapped(window, "destroy", G_CALLBACK(g_free), widgets);

    gtk_window_present(GTK_WINDOW(window));
}

int main(int argc, char **argv) {
    GtkApplication *app;
    int status;

    app = gtk_application_new("org.exemplo.cadastro_id", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(on_activate), NULL);

    status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);

    return status;
}
