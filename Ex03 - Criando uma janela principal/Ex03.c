/*****************************************************************
 * Ex03 - Criando uma janela principal
 * 
 * Autor: Eng. Fabrício Ribeiro
 * - Para compilar: make
 * - Para limpar a compilação: make clean
 * - Para executar: ./Ex03
 * 
******************************************************************/

#include <gtk/gtk.h>

// Função
static void app_activate(GApplication *app, gpointer *user_data){
  
  //Escreve uma mensagem no termnal  
  g_print ("Aplicação GTK está ativada!\n");

  //Cria as instâncias
  GtkWidget *window;

  //Cria uma janela
  window = gtk_application_window_new(GTK_APPLICATION(app));
  // Define o título da janela
  gtk_window_set_title (GTK_WINDOW(window), "Janela criada");
  // Define o tamanho da janela
  gtk_window_set_default_size(GTK_WINDOW(window), 400, 300);
  // Define que a janela NÃO pode ser redimensionada
  gtk_window_set_resizable(GTK_WINDOW(window), FALSE);


  //Executa a janela
  gtk_window_present(GTK_WINDOW(window));   
}

// Programa principal
int main(int argc, char **argv){

  GtkApplication *app;
  int stat;

  app = gtk_application_new("com.github.fabricioitajuba.ex03", G_APPLICATION_DEFAULT_FLAGS);
  g_signal_connect(app, "activate", G_CALLBACK(app_activate), NULL); //Executa um função
  stat = g_application_run(G_APPLICATION (app), argc, argv);
  g_object_unref(app);

  return stat;
}