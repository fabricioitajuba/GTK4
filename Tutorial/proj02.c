/*
 GTK - Segundo projeto básico (completo)
*/

#include <gtk/gtk.h>

//Callback botão_exit
static void button_exit(GtkWidget *widget, gpointer data){
    GtkWidget *window = GTK_WIDGET(data);
    
    g_print("Botão sair\n");
    gtk_window_destroy(GTK_WINDOW(window));
}

//Callback botão2
static void func_button2(GtkWidget *widget, gpointer data){

    g_print("Botão 2 pressionado\n");

    GtkEntryBuffer *buffer = gtk_entry_get_buffer(GTK_ENTRY(data));
    const char *texto_digitado = gtk_entry_buffer_get_text(buffer);

    g_print("O usuário digitou: %s\n", texto_digitado);  

    gtk_entry_buffer_set_text(buffer, "", -1);//Limpa o texto digitado  
}

//Callback botão1
static void func_button1(GtkWidget *widget, gpointer data){

    g_print("Botão 1 pressionado\n");

    GtkEntryBuffer *buffer = gtk_entry_get_buffer(GTK_ENTRY(data));
    const char *texto_digitado = gtk_entry_buffer_get_text(buffer);

    g_print("O usuário digitou: %s\n", texto_digitado);

    gtk_entry_buffer_set_text(buffer, "", -1);//Limpa o texto digitado
}

//Cria a apliação
static void activate(GtkApplication *app, gpointer user_data){

    GtkWidget *window, *grid, *label1, *label2, *text1, *text2, *button1, *button2, *button3;

    window = gtk_application_window_new(app);
    gtk_window_set_title (GTK_WINDOW(window), "Janela");
    gtk_window_set_default_size (GTK_WINDOW(window), 400, 200);

    grid = gtk_grid_new();
    gtk_window_set_child (GTK_WINDOW(window), grid);

    gtk_grid_set_column_spacing(GTK_GRID(grid), 20);
    gtk_grid_set_row_spacing(GTK_GRID(grid), 20);

    gtk_widget_set_margin_start(grid, 15);
    gtk_widget_set_margin_end(grid, 15);
    gtk_widget_set_margin_top(grid, 15);
    gtk_widget_set_margin_bottom(grid, 15);    


    label1 = gtk_label_new("Label 1");
    gtk_grid_attach(GTK_GRID(grid), label1, 0, 0, 1, 1); //Coluna, Linha, Largura, Altura

    text1 = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(text1), "Digite 1 ...");
    gtk_grid_attach(GTK_GRID(grid), text1, 1, 0, 1, 1);

    button1 = gtk_button_new_with_label("Button 1");
    g_signal_connect(button1, "clicked", G_CALLBACK(func_button1), text1);
    gtk_grid_attach(GTK_GRID(grid), button1, 2, 0, 1, 1);    

    label2 = gtk_label_new("Label 2");
    gtk_grid_attach(GTK_GRID(grid), label2, 0, 1, 1, 1);

    text2 = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(text2), "Digite 2 ...");
    gtk_grid_attach(GTK_GRID(grid), text2, 1, 1, 1, 1);

    button2 = gtk_button_new_with_label("Button 2");
    g_signal_connect(button2, "clicked", G_CALLBACK(func_button2), text2);
    gtk_grid_attach(GTK_GRID (grid), button2, 2, 1, 1, 1);

    button3 = gtk_button_new_with_label("Sair");
    g_signal_connect(button3, "clicked", G_CALLBACK(button_exit), window);
    gtk_grid_attach(GTK_GRID(grid), button3, 2, 2, 1, 1);    


    gtk_window_present (GTK_WINDOW (window));
}

//Programa principal
int main(int argc, char **argv){

    GtkApplication *app;
    int status;

    app = gtk_application_new ("org.gtk.example", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect (app, "activate", G_CALLBACK (activate), NULL);
    status = g_application_run (G_APPLICATION (app), argc, argv);
    g_object_unref (app);

    return status;
}