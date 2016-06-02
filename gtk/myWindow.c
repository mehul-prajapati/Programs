//====================================================
//======== @INCLUDES =================================
//====================================================
#include <gtk/gtk.h> 

int expose_event ( GtkWidget *widget, GdkEventExpose *event ); 
//=====================================================
//========= @FUNCTIONS ================================
//=====================================================
int main (int argc, char *argv[ ]) 
{ 
  	GtkWidget *p ; 

	/* Initialize GTK library */
 	gtk_init (&argc, &argv) ; 

	/* Create a top level window */
	p = gtk_window_new (GTK_WINDOW_TOPLEVEL) ;
	
	/* Title */
	gtk_window_set_title (p , "Sample Window") ; 

	/* Register sighandler - cleanup & shutdown the app */
	g_signal_connect (p, "destroy", gtk_main_quit, NULL) ; 

	/* Resize window - height & width */
	gtk_widget_set_size_request ( p, 500, 500 ) ; 

	/* Display window on screen */
	gtk_widget_show ( p ) ; 

	/* Wait for events */
	gtk_main( );


	return 0 ; 
} 

