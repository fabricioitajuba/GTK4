/*
 GTK - Escrevendo uma mensagem no terminal
*/

#include <gtk/gtk.h>

static void app_activate(GApplication *app, gpointer *user_data){
  
  //Escreve uma mensagem no termnal
  g_print ("Aplicação GTK está ativada!\n");
}

int main(int argc, char **argv){
  
  GtkApplication *app;
  int stat;

  app = gtk_application_new ("com.github.ToshioCP.pr2", G_APPLICATION_DEFAULT_FLAGS);
  g_signal_connect (app, "activate", G_CALLBACK (app_activate), NULL);
  stat = g_application_run (G_APPLICATION (app), argc, argv);
  g_object_unref (app);
  return stat;
}