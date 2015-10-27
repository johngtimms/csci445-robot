

#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>

g_signal_connect(G_OBJECT(MAIN_WINDOW),"key_press_event",G_CALLBACK(on_key_press), NULL);

gboolean on_key_press(GtkWidget *widget, GdkEventKey *event, gpointer user_data ){

	if(event->state & GDK_SHIFT_MASK){
	printf("SHIFT pressed\n");
	}
	else if( event->state &GDK_CONTROL_MASK){
	printf("CTRL pressed\n");
	}







}