#include <gtk/gtk.h>
#include <webkit2/webkit2.h>


static void destroyWindowCb(GtkWidget* widget, GtkWidget* window);
static gboolean closeWebViewCb(WebKitWebView* webView, GtkWidget* window);

WebKitWebView *webView;
GtkWidget *main_window;
GtkWidget *g_menu_label;
GtkWidget *g_cmd_label;
GtkWidget *grid;

gboolean key_event (GtkWidget* widget, GdkEventKey *event, gpointer data){
	static char buf[256];
	static int len= 0;
	
	#define NONE 0
	#define INSERT 1
	static int flag=NONE;

	// prevent double input
	static int two = 0;
	if(two ==1){
		two = 0;
		return false;
	}
	two++;

	if (event->keyval == GDK_KEY_Escape){
		// clear the string
		len = 0;
		for(int i=0;i<256;i++)
			buf[i]=0;
		flag = NONE;
		return false;
	}

	if (event->keyval == GDK_KEY_Return){
		if ( buf[0] == ':' && buf[1] == 'q')
			gtk_main_quit();
		// clear the string
		len = 0;
		for(int i=0;i<256;i++)
			buf[i]=0;
		flag = NONE;
	}
	if( event->keyval == ':'){
		flag =INSERT;
	}

	if (len < 256){
	if (event->keyval < 255){
		buf[len] = event->keyval;
		len++;
	}}

	if( event->keyval == 'o' && flag == NONE){
		webkit_web_view_load_uri(webView, "https://duckduckgo.com/");
	}
	if( event->keyval == 'r' && flag == NONE){
		webkit_web_view_reload (webView);
	}

	if( event->keyval == 'H' && flag == NONE){
		webkit_web_view_go_back (webView);
	}
	if( event->keyval == 'L' && flag == NONE){
		webkit_web_view_go_forward (webView);
	}


	gtk_label_set_text(GTK_LABEL(g_cmd_label), buf);
	gtk_window_set_title (GTK_WINDOW(main_window),"hoge");


	//gtk_window_resize(GTK_WINDOW(main_window), 100,100);
	return false;
}

int main(int argc, char* argv[])
{
    // Initialize GTK+
    gtk_init(&argc, &argv);

	// init main window
	main_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	//gtk_window_set_position(GTK_WINDOW(main_window), GTK_WIN_POS_CENTER);
	gtk_window_set_position(GTK_WINDOW(main_window), GTK_WIN_POS_MOUSE);
    gtk_window_set_default_size(GTK_WINDOW(main_window), 800, 600);
	gtk_window_set_title (GTK_WINDOW(main_window),"title");

	gtk_widget_add_events(main_window, GDK_KEY_PRESS_MASK);


	// init cmd label	
	g_menu_label =  gtk_label_new("newtab");
	g_cmd_label = gtk_label_new("");




	// init web stuff
	webView = WEBKIT_WEB_VIEW( webkit_web_view_new());
	webkit_web_view_load_uri(webView, "file:///home/k/share/http/index.html");

	// connection to signal
    //gtk_container_add(GTK_CONTAINER(main_window), GTK_WIDGET(webView));

    g_signal_connect(main_window, "destroy", G_CALLBACK(destroyWindowCb), NULL);
    g_signal_connect(webView, "close", G_CALLBACK(closeWebViewCb), main_window);
    g_signal_connect(G_OBJECT(main_window), "key_press_event", G_CALLBACK(key_event), main_window);


	// init grid
	grid = gtk_grid_new();
	

	gtk_widget_set_hexpand ((GtkWidget*)webView, TRUE);
	//gtk_widget_set_halign (web, GTK_ALIGN_FILL);
	gtk_widget_set_vexpand ((GtkWidget*)webView, TRUE);
	//gtk_widget_set_valign (web, GTK_ALIGN_FILL);

	gtk_grid_attach (GTK_GRID (grid), g_menu_label, 0, 0, 17, 1);
	gtk_grid_attach (GTK_GRID (grid), g_cmd_label,  0, 16, 17, 1);
	gtk_grid_attach (GTK_GRID (grid), (GtkWidget*)webView, 0, 1, 17, 15);

    //gtk_widget_grab_focus(GTK_WIDGET(webView));

	gtk_container_add(GTK_CONTAINER(main_window) , grid);
	gtk_widget_show(grid);
    gtk_widget_show_all(main_window);

    gtk_main();

    return 0;
}


static void destroyWindowCb(GtkWidget* widget, GtkWidget* window)
{
    gtk_main_quit();
}

static gboolean closeWebViewCb(WebKitWebView* webView, GtkWidget* window)
{
    gtk_widget_destroy(window);
    return TRUE;
}



