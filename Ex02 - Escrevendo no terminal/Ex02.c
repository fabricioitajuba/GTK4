/*****************************************************************
 * Ex02 - Escrevendo no terminal
 * 
 * Autor: Eng. Fabrício Ribeiro
 * - Para compilar: make
 * - Para limpar a compilação: make clean
 * - Para executar: ./Ex02
 * 
******************************************************************/

#include <gtk/gtk.h>

// Função
static void app_activate(GApplication *app, gpointer *user_data){
  
  //Escreve uma mensagem no termnal  
  g_print ("Aplicação GTK está ativada!\n");
}

// Programa principal
int main(int argc, char **argv){

  GtkApplication *app;
  int stat;

  app = gtk_application_new("com.github.fabricioitajuba.ex02", G_APPLICATION_DEFAULT_FLAGS);
  g_signal_connect(app, "activate", G_CALLBACK(app_activate), NULL); //Executa um função
  stat = g_application_run(G_APPLICATION(app), argc, argv);
  g_object_unref(app);

  return stat;
}