/*
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Library General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */
 /*
 * File: burn.h
 * Copyright: luke_biddell@yahoo.com
 * Created on: Tue Jan 28 22:12:09 2003
 */

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include "gnomebaker.h"
#include <gnome.h>
#include <glade/glade.h>
#include "splashdlg.h"
#include "gbcommon.h"
#include "devices.h"
#include "media.h"
#include "preferences.h"
#include <libintl.h>
#include <locale.h>
#include <gst/gst.h>
#include "gblibnotify.h"

const gchar *glade_file;
gboolean show_trace = FALSE;

static GOptionEntry entries[] =
{
    { "trace-on", 0, 0, G_OPTION_ARG_NONE, &show_trace, N_("Show tracing information (useful when debugging)"), 0 },
    { NULL }
};

void backend_init()
{
		// Get current backend
		enum backend b = preferences_get_int(GB_BACKEND);
		
		/* Check if Backend in GConf is still valid */
		if (!backend_is_backend_supported(b))
		{
			// Backend is not there anymore... try other backends
			switch (b) 
			{
				case BACKEND_WODIM:
					if (backend_is_backend_supported(BACKEND_CDRECORD))					
						preferences_set_int(GB_BACKEND, BACKEND_CDRECORD);
						break;
				case BACKEND_CDRECORD:
					if (backend_is_backend_supported(BACKEND_WODIM))					
						preferences_set_int(GB_BACKEND, BACKEND_WODIM);
						break;
				default:
					printf("ERROR: No supported backend found.\n");
					exit(-1);
			}
		}
}

gint
main(gint argc, gchar *argv[])
{	
	GError *error = NULL;
	GOptionContext *context = g_option_context_new(_(" - GNOME CD/DVD burning application"));
  if (!g_thread_supported ()) g_thread_init(NULL);
	/* add main entries */
	g_option_context_add_main_entries(context, entries, GETTEXT_PACKAGE);
	/* recognise gtk/gdk/gstreamer options */
	g_option_context_add_group(context, gtk_get_option_group(TRUE));
#ifdef GST_010
    g_option_context_add_group(context, gst_init_get_option_group());
#endif
	/* ignore unknown options */
	g_option_context_set_ignore_unknown_options(context, TRUE);
	g_option_context_parse(context, &argc, &argv, &error);
	g_option_context_free(context);
    if(error != NULL) g_error_free(error);

#ifdef ENABLE_NLS
    setlocale(LC_ALL,"");
    bindtextdomain (GETTEXT_PACKAGE, GNOMELOCALEDIR);
    bind_textdomain_codeset (GETTEXT_PACKAGE, "UTF-8");
    textdomain (GETTEXT_PACKAGE);
#endif

#ifdef GST_010
    struct poptOption* options = NULL;
#else
    gdk_threads_init();

	struct poptOption options[] =
	{
    	{NULL, '\0', POPT_ARG_INCLUDE_TABLE, NULL, 0, "GStreamer", NULL},
    	POPT_TABLEEND
	};
	/* init GStreamer and GNOME using the GStreamer popt tables */
	options[0].arg = (void*) gst_init_get_popt_table ();
#endif

    GnomeProgram *prog = gnome_program_init ("gnomebaker", PACKAGE_VERSION, LIBGNOMEUI_MODULE, argc, argv,
            GNOME_PARAM_POPT_TABLE, options, GNOME_PARAM_APP_DATADIR, DATADIR, NULL);
	glade_gnome_init();

	glade_file = gnome_program_locate_file(NULL, GNOME_FILE_DOMAIN_APP_DATADIR,
			"gnomebaker/gnomebaker.glade", FALSE, NULL);

	g_set_application_name (_("GnomeBaker"));
	gtk_window_set_default_icon_name ("gnomebaker-48");

#if !defined(__linux__)
	GtkWidget *dlg = splashdlg_new();
#endif

	while(g_main_context_pending(NULL))
		g_main_context_iteration(NULL, TRUE);

    gblibnotify_init("GnomeBaker");

    gbcommon_init();

#if !defined(__linux__)
    splashdlg_set_text(_("Loading preferences..."));
#endif
    preferences_init();

#if !defined(__linux__)
    splashdlg_set_text(_("Detecting devices..."));
#endif
    devices_init();

#if !defined(__linux__)
    splashdlg_set_text(_("Registering gstreamer plugins..."));
#endif
    media_init();

#if !defined(__linux__)
    splashdlg_set_text(_("Loading GUI..."));
#endif
	GtkWidget *app = gnomebaker_new();

#if !defined(__linux__)
	splashdlg_delete(dlg);
#endif

	backend_init();
	
    gdk_threads_enter();
	gtk_main();
	gdk_threads_leave();
    gnomebaker_delete(app);
    g_object_unref(prog);
    prog = NULL;
    gblibnotify_clear();
	return 0;
}



