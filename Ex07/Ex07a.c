/*
Arduino nano
*/
#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <errno.h>

// Estrutura para armazenar os dados do app
typedef struct {
    int serial_fd;
    GtkWidget *label_dados;
    guint timeout_id;
} AppData;

// Função para fechar a porta serial com segurança
void fechar_serial(AppData *data) {
    if (data->serial_fd >= 0) {
        close(data->serial_fd);
        data->serial_fd = -1;
        g_print("Conexão serial finalizada com sucesso.\n");
    }
}

// Callback do botão Sair
static void ao_clicar_sair(GtkWidget *widget, gpointer user_data) {
    AppData *data = (AppData *)user_data;
    
    // 1. Finaliza o timeout do GTK para parar de ler
    if (data->timeout_id > 0) {
        g_source_remove(data->timeout_id);
    }
    
    // 2. Fecha a conexão serial antes de fechar a janela
    fechar_serial(data);
    
    // 3. Fecha o programa
    g_print("Saindo do programa...\n");
    exit(0);
}

// Função para autodetectar e conectar à primeira porta serial disponível (Linux)
int detectar_e_conectar_serial() {
    // Lista de prefixos comuns para Arduino no Linux
    const char *portas[] = {"/dev/ttyUSB0", "/dev/ttyUSB1", "/dev/ttyACM0", "/dev/ttyACM1"};
    int fd = -1;

    for (int i = 0; i < 4; i++) {
        g_print("Tentando detectar porta: %s...\n", portas[i]);
        fd = open(portas[i], O_RDWR | O_NOCTTY | O_NDELAY);
        
        if (fd != -1) {
            g_print("Conectado com sucesso em: %s\n", portas[i]);
            
            // Configuração básica da serial (9600 baud, 8 bits, sem paridade, 1 stop bit)
            struct termios options;
            tcgetattr(fd, &options);
            cfsetispeed(&options, B9600);
            cfsetospeed(&options, B9600);
            options.c_cflag |= (CLOCAL | CREAD);
            options.c_cflag &= ~PARENB;
            options.c_cflag &= ~CSTOPB;
            options.c_cflag &= ~CSIZE;
            options.c_cflag |= CS8;
            options.c_lflag |= ICANON; // Modo canônico (lê linha por linha)
            tcsetattr(fd, TCSANOW, &options);
            
            return fd; // Retorna o descritor do arquivo da porta conectada
        }
    }
    return -1; // Nenhuma porta encontrada
}

// Função executada periodicamente pelo GTK para ler a serial sem travar a interface
static gboolean ler_serial_timeout(gpointer user_data) {
    AppData *data = (AppData *)user_data;
    if (data->serial_fd < 0) return TRUE;

    char buffer[256];
    memset(buffer, 0, sizeof(buffer));

    // Como usamos O_NDELAY, read não trava se não houver dados
    int bytes_lidos = read(data->serial_fd, buffer, sizeof(buffer) - 1);
    
    if (bytes_lidos > 0) {
        // Remove quebras de linha para exibição limpa
        buffer[strcspn(buffer, "\r\n")] = 0; 
        
        // Atualiza a interface gráfica com o número recebido
        char texto_final[300];
        snprintf(texto_final, sizeof(texto_final), "Último dado recebido: %s", buffer);
        gtk_label_set_text(GTK_LABEL(data->label_dados), texto_final);
    }

    return TRUE; // Retorna TRUE para continuar executando o timeout
}

// Ativação do app GTK4
static void activate(GtkApplication *app, gpointer user_data) {
    AppData *data = (AppData *)user_data;

    // Criar a janela principal
    GtkWidget *window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Monitor Serial Arduino Nano");
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 200);

    // Criar um container vertical
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 15);
    gtk_widget_set_margin_top(box, 20);
    gtk_widget_set_margin_bottom(box, 20);
    gtk_widget_set_margin_start(box, 20);
    gtk_widget_set_margin_end(box, 20);
    gtk_window_set_child(GTK_WINDOW(window), box);

    // Tentar conectar na inicialização
    data->serial_fd = detectar_e_conectar_serial();
    
    // Label de status/dados
    if (data->serial_fd != -1) {
        data->label_dados = gtk_label_new("Aguardando dados do Arduino...");
        // Configura o GTK para rodar nossa função de leitura a cada 100ms
        data->timeout_id = g_timeout_add(100, ler_serial_timeout, data);
    } else {
        data->label_dados = gtk_label_new("Erro: Nenhum Arduino Nano detectado!");
    }
    gtk_box_append(GTK_BOX(box), data->label_dados);

    // Botão Sair
    GtkWidget *btn_sair = gtk_button_new_with_label("Fechar Conexão e Sair");
    gtk_widget_set_halign(btn_sair, GTK_ALIGN_CENTER);
    g_signal_connect(btn_sair, "clicked", G_CALLBACK(ao_clicar_sair), data);
    gtk_box_append(GTK_BOX(box), btn_sair);

    // Garante fechamento limpo se o usuário fechar no "X" da janela
    g_signal_connect(window, "destroy", G_CALLBACK(ao_clicar_sair), data);

    gtk_window_present(GTK_WINDOW(window));
}

int main(int argc, char **argv) {
    GtkApplication *app;
    int status;
    AppData data = { .serial_fd = -1, .timeout_id = 0, .label_dados = NULL };

    app = gtk_application_new("org.gtk.arduino.monitor", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(activate), &data);
    status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);

    return status;
}
