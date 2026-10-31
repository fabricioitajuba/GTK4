/*****************************************************************
 * Ex01 - Iniciando uma aplicação
 * 
 * Autor: Eng. Fabrício Ribeiro
 * - Para compilar: make
 * - Para limpar a compilação: make clean
 * - Para executar: ./Ex01
 * 
******************************************************************/
 
#include <gtk/gtk.h>

// Programa principal
int main(int argc, char **argv){

  GtkApplication *app;
  int stat;

  app = gtk_application_new("com.github.fabricioitajuba.ex01", G_APPLICATION_DEFAULT_FLAGS);
  stat = g_application_run(G_APPLICATION(app), argc, argv);
  g_object_unref(app);

  return stat;  
}