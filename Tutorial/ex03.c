/*
 GTK - Criando uma janela
*/

#include <gtk/gtk.h>

static void app_activate(GApplication *app, gpointer *user_data){

  //Escreve uma mensagem no termnal
  g_print ("Aplicação GTK está ativada!\n");

  //Cria as instâncias
  GtkWidget *win;

  //Cria uma janela
  win = gtk_application_window_new (GTK_APPLICATION (app));
  // Define o título da janela
  gtk_window_set_title (GTK_WINDOW (win), "Janela criada");
  // Define o tamanho da janela
  gtk_window_set_default_size (GTK_WINDOW (win), 400, 300);
  // Define que a janela NÃO pode ser redimensionada
  gtk_window_set_resizable(GTK_WINDOW(win), FALSE);


  //Executa a janela
  gtk_window_present (GTK_WINDOW (win));  

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