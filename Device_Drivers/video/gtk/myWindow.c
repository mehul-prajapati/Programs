#include <gtk/gtk.h>
#include <stdio.h>


int main(int argc, char *argv[])
{
	GtkWidget *pWindow;

	gtk_init(&argc, &argv);

	printf("###### 1\n");

	pWindow = gtk_window_new(GTK_WINDOW_TOPLEVEL);

        printf("###### 2\n");

	g_signal_connect (pWindow, "destroy", G_CALLBACK (gtk_main_quit), NULL);

	gtk_widget_show(pWindow);

        printf("###### 3\n");

	gtk_main();

	return 0;
}
