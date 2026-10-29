/*
 GTK - Adicionando um Label em uma posição na janela
*/

#include <gtk/gtk.h>

static void app_activate(GApplication *app, gpointer *user_data){

  //Escreve uma mensagem no termnal
  g_print ("Aplicação GTK está ativada!\n");

  //Cria as instâncias
  GtkWidget *window;
  GtkWidget *fixed;
  GtkWidget *label;

  // Cria uma janela
  window = gtk_application_window_new (GTK_APPLICATION (app));
  // Define o título da janela
  gtk_window_set_title (GTK_WINDOW (window), "Janela criada");
  // Define o tamanho da janela
  gtk_window_set_default_size (GTK_WINDOW (window), 400, 300);
  // Define que a janela NÃO pode ser redimensionada
  gtk_window_set_resizable(GTK_WINDOW(window), FALSE);

  //Cria o container de posicionamento fixo
  fixed = gtk_fixed_new();
  gtk_window_set_child(GTK_WINDOW(window), fixed);

  // Cria um label e mostra no centro da janela
  label = gtk_label_new("Texto na posição (20, 50)");
  // Adiciona a label ao fixed especificando as coordenadas X e Y
  // gtk_fixed_put(GtkFixed *fixed, GtkWidget *widget, double x, double y)
  gtk_fixed_put(GTK_FIXED(fixed), label, 20.0, 50.0);  

  //Executa a janela
  gtk_window_present(GTK_WINDOW(window));  

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