/*
Arduino uno
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
        g_print("Conexão serial com o Arduino Uno finalizada.\n");
    }
}

// Callback do botão Sair e do fechamento da janela
static void ao_clicar_sair(GtkWidget *widget, gpointer user_data) {
    AppData *data = (AppData *)user_data;
    
    // 1. Finaliza o timeout do GTK para parar de tentar ler
    if (data->timeout_id > 0) {
        g_source_remove(data->timeout_id);
    }
    
    // 2. Fecha a conexão serial antes de fechar a janela
    fechar_serial(data);
    
    // 3. Fecha o programa
    g_print("Saindo do programa...\n");
    exit(0);
}

// Função para autodetectar e conectar à porta do Arduino Uno
int detectar_e_conectar_serial() {
    // Portas mais comuns para o Arduino Uno (ttyACM) e clones (ttyUSB)
    const char *portas[] = {"/dev/ttyACM0", "/dev/ttyACM1", "/dev/ttyUSB0", "/dev/ttyUSB1"};
    int fd = -1;

    for (int i = 0; i < 4; i++) {
        g_print("Verificando se o Arduino Uno está na porta: %s...\n", portas[i]);
        fd = open(portas[i], O_RDWR | O_NOCTTY | O_NDELAY);
        
        if (fd != -1) {
            g_print("Arduino Uno detectado e conectado em: %s\n", portas[i]);
            
            // Configuração da serial (9600 baud, 8 bits de dados, sem paridade, 1 stop bit)
            struct termios options;
            tcgetattr(fd, &options);
            cfsetispeed(&options, B9600);
            cfsetospeed(&options, B9600);
            
            options.c_cflag |= (CLOCAL | CREAD);
            options.c_cflag &= ~PARENB;
            options.c_cflag &= ~CSTOPB;
            options.c_cflag &= ~CSIZE;
            options.c_cflag |= CS8;
            
            // Modo canônico: processa linha por linha (ideal para o Serial.println do Uno)
            options.c_lflag |= ICANON; 
            tcsetattr(fd, TCSANOW, &options);
            
            return fd; 
        }
    }
    return -1; // Nenhum dispositivo encontrado nas portas listadas
}

// Função executada periodicamente pelo GTK para ler os dados enviados pelo Uno
static gboolean ler_serial_timeout(gpointer user_data) {
    AppData *data = (AppData *)user_data;
    if (data->serial_fd < 0) return TRUE;

    char buffer[256];
    memset(buffer, 0, sizeof(buffer));

    // O_NDELAY garante que o read não vai travar a interface se o Uno não tiver enviado nada ainda
    int bytes_lidos = read(data->serial_fd, buffer, sizeof(buffer) - 1);
    
    if (bytes_lidos > 0) {
        // Remove as quebras de linha (\r e \n) enviadas pelo Serial.println() do Uno
        buffer[strcspn(buffer, "\r\n")] = 0; 
        
        // Só atualiza a interface se houver um texto válido limpo
        if (strlen(buffer) > 0) {
            char texto_final[300];
            snprintf(texto_final, sizeof(texto_final), "Valor do Arduino Uno: %s", buffer);
            gtk_label_set_text(GTK_LABEL(data->label_dados), texto_final);
        }
    }

    return TRUE; // Retorna TRUE para manter o timer ativo
}

// Inicialização da interface GTK4
static void activate(GtkApplication *app, gpointer user_data) {
    AppData *data = (AppData *)user_data;

    // Criar a janela principal
    GtkWidget *window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Monitor Serial - Arduino Uno");
    gtk_window_set_default_size(GTK_WINDOW(window), 450, 200);

    // Layout Vertical
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 20);
    gtk_widget_set_margin_top(box, 25);
    gtk_widget_set_margin_bottom(box, 25);
    gtk_widget_set_margin_start(box, 25);
    gtk_widget_set_margin_end(box, 25);
    gtk_window_set_child(GTK_WINDOW(window), box);

    // Tenta realizar a conexão antes de desenhar os elementos
    data->serial_fd = detectar_e_conectar_serial();
    
    // Label para exibir o número incrementado
    if (data->serial_fd != -1) {
        data->label_dados = gtk_label_new("Conectado! Aguardando primeiro incremento...");
        // Verifica a porta a cada 100 milissegundos
        data->timeout_id = g_timeout_add(100, ler_serial_timeout, data);
    } else {
        data->label_dados = gtk_label_new("Erro: Nenhum Arduino Uno foi encontrado.\nVerifique o cabo USB.");
    }
    gtk_box_append(GTK_BOX(box), data->label_dados);

    // Botão Desconectar e Sair
    GtkWidget *btn_sair = gtk_button_new_with_label("Encerrar Conexão e Sair");
    gtk_widget_set_halign(btn_sair, GTK_ALIGN_CENTER);
    g_signal_connect(btn_sair, "clicked", G_CALLBACK(ao_clicar_sair), data);
    gtk_box_append(GTK_BOX(box), btn_sair);

    // Caso o usuário feche diretamente no "X" da janela, também desconecta a serial com segurança
    g_signal_connect(window, "destroy", G_CALLBACK(ao_clicar_sair), data);

    gtk_window_present(GTK_WINDOW(window));
}

int main(int argc, char **argv) {
    GtkApplication *app;
    int status;
    AppData data = { .serial_fd = -1, .timeout_id = 0, .label_dados = NULL };

    app = gtk_application_new("org.gtk.arduinouno.monitor", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(activate), &data);
    status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);

    return status;
}
