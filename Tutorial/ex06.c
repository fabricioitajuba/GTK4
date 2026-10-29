/*
 GTK - Adicionando um botões
*/

#include <gtk/gtk.h>

//Função de callback do botão2
static void click_exit(GtkButton *btn, GtkWindow *window){
  g_print ("Fechando a aplicação!\n");
  gtk_window_destroy(window);
}

//Função de callback do botão1
static void click_cb(GtkButton *btn){
  g_print ("Botão pressionado!\n");
}

//Função para criar a janela
static void app_activate(GApplication *app, gpointer *user_data){

  //Escreve uma mensagem no termnal
  g_print ("Aplicação GTK está ativada!\n");

  //Cria as instâncias
  GtkWidget *window;
  GtkWidget *fixed;
  GtkWidget *label;
  GtkWidget *button1;
  GtkWidget *button2;

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

  // Cria um botão1
  button1 = gtk_button_new_with_label("Clique Aqui");
  // Define o tamanho do botão (largura = 150 pixels, altura = 50 pixels)
  // gtk_widget_set_size_request(GtkWidget *widget, int width, int height)
  gtk_widget_set_size_request(button1, 70, 40);  
  g_signal_connect (button1, "clicked", G_CALLBACK(click_cb), NULL);
  // Posiciona o botão em um local fixo
  gtk_fixed_put(GTK_FIXED(fixed), button1, 220.0, 40.0);

  // Cria um botão2
  button2 = gtk_button_new_with_label("Sair");
  gtk_widget_set_size_request(button2, 70, 40);  
  g_signal_connect (button2, "clicked", G_CALLBACK(click_exit), window);
  gtk_fixed_put(GTK_FIXED(fixed), button2, 220.0, 90.0);


  //Executa a janela
  gtk_window_present(GTK_WINDOW(window));  

}

//Programa principal
int main(int argc, char **argv){

  GtkApplication *app;
  int stat;

  app = gtk_application_new ("com.github.ToshioCP.pr2", G_APPLICATION_DEFAULT_FLAGS);
  g_signal_connect (app, "activate", G_CALLBACK (app_activate), NULL);
  stat = g_application_run (G_APPLICATION (app), argc, argv);
  g_object_unref (app);

  return stat;
}