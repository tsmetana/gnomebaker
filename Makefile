# Makefile.in generated automatically by automake 1.4-p6 from Makefile.am

# Copyright (C) 1994, 1995-8, 1999, 2001 Free Software Foundation, Inc.
# This Makefile.in is free software; the Free Software Foundation
# gives unlimited permission to copy and/or distribute it,
# with or without modifications, as long as this notice is preserved.

# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY, to the extent permitted by law; without
# even the implied warranty of MERCHANTABILITY or FITNESS FOR A
# PARTICULAR PURPOSE.


SHELL = /bin/sh

srcdir = .
top_srcdir = .

prefix = /usr/local
exec_prefix = ${prefix}

bindir = ${exec_prefix}/bin
sbindir = ${exec_prefix}/sbin
libexecdir = ${exec_prefix}/libexec
datadir = ${prefix}/share
sysconfdir = ${prefix}/etc
sharedstatedir = ${prefix}/com
localstatedir = ${prefix}/var
libdir = ${exec_prefix}/lib
infodir = ${prefix}/info
mandir = ${prefix}/man
includedir = ${prefix}/include
oldincludedir = /usr/include

DESTDIR =

pkgdatadir = $(datadir)/gnomebaker
pkglibdir = $(libdir)/gnomebaker
pkgincludedir = $(includedir)/gnomebaker

top_builddir = .

ACLOCAL = aclocal-1.4
AUTOCONF = autoconf
AUTOMAKE = automake-1.4
AUTOHEADER = autoheader

INSTALL = /usr/bin/install -c
INSTALL_PROGRAM = ${INSTALL} $(AM_INSTALL_PROGRAM_FLAGS)
INSTALL_DATA = ${INSTALL} -m 644
INSTALL_SCRIPT = ${INSTALL}
transform = s,x,x,

NORMAL_INSTALL = :
PRE_INSTALL = :
POST_INSTALL = :
NORMAL_UNINSTALL = :
PRE_UNINSTALL = :
POST_UNINSTALL = :
host_alias = 
host_triplet = i686-pc-linux-gnu
AR = ar
AS = @AS@
BUILD_INCLUDED_LIBINTL = @BUILD_INCLUDED_LIBINTL@
CATALOGS =  pt_BR.gmo
CATOBJEXT = .gmo
CC = gcc
CFLAGS = 
CXX = g++
CXXCPP = g++ -E
DATADIRNAME = share
DLLTOOL = @DLLTOOL@
ECHO = echo
EGREP = grep -E
EXEEXT = 
F77 = 
GCJ = @GCJ@
GCJFLAGS = @GCJFLAGS@
GENCAT = @GENCAT@
GETTEXT_PACKAGE = gnomebaker
GLIBC21 = @GLIBC21@
GMOFILES =  pt_BR.gmo
GMSGFMT = /usr/bin/msgfmt
GNOME_CFLAGS = -DORBIT2=1 -pthread -DXTHREADS -I/usr/include/libgnomeui-2.0 -I/usr/include/libgnome-2.0 -I/usr/include/libgnomecanvas-2.0 -I/usr/include/gtk-2.0 -I/usr/include/libart-2.0 -I/usr/include/gconf/2 -I/usr/include/libbonoboui-2.0 -I/usr/include/glib-2.0 -I/usr/lib/glib-2.0/include -I/usr/include/orbit-2.0 -I/usr/include/libbonobo-2.0 -I/usr/include/gnome-vfs-2.0 -I/usr/lib/gnome-vfs-2.0/include -I/usr/include/bonobo-activation-2.0 -I/usr/include/pango-1.0 -I/usr/include/freetype2 -I/usr/lib/gtk-2.0/include -I/usr/X11R6/include -I/usr/include/atk-1.0 -I/usr/include/libxml2 -I/usr/include/libglade-2.0  
GNOME_LIBS = -Wl,--export-dynamic -pthread -L/usr/X11R6/lib -lgnomeui-2 -lSM -lICE -lbonoboui-2 -lgnomecanvas-2 -lgnome-2 -lpopt -lart_lgpl_2 -lpangoft2-1.0 -lgnomevfs-2 -lbonobo-2 -lgconf-2 -lbonobo-activation -lORBit-2 -lgthread-2.0 -lglade-2.0 -lgtk-x11-2.0 -lxml2 -lpthread -lz -lgdk-x11-2.0 -latk-1.0 -lgdk_pixbuf-2.0 -lpangoxft-1.0 -lpangox-1.0 -lpango-1.0 -lgobject-2.0 -lgmodule-2.0 -ldl -lglib-2.0 -lvorbisfile -lvorbis -lm -logg  
HAVE_ASPRINTF = @HAVE_ASPRINTF@
HAVE_LIB = @HAVE_LIB@
HAVE_POSIX_PRINTF = @HAVE_POSIX_PRINTF@
HAVE_SNPRINTF = @HAVE_SNPRINTF@
HAVE_WPRINTF = @HAVE_WPRINTF@
INSTOBJEXT = .mo
INTLBISON = @INTLBISON@
INTLLIBS = 
INTLOBJS = @INTLOBJS@
INTL_LIBTOOL_SUFFIX_PREFIX = @INTL_LIBTOOL_SUFFIX_PREFIX@
LIB = @LIB@
LIBICONV = @LIBICONV@
LIBINTL = @LIBINTL@
LIBTOOL = $(SHELL) $(top_builddir)/libtool
LN_S = ln -s
LTLIB = @LTLIB@
LTLIBICONV = @LTLIBICONV@
LTLIBINTL = @LTLIBINTL@
MAKEINFO = /home/luke/gnomebaker/missing makeinfo
MKINSTALLDIRS = ./mkinstalldirs
NO_PREFIX_PACKAGE_DATA_DIR = share
NO_PREFIX_PACKAGE_DOC_DIR = doc/gnomebaker
NO_PREFIX_PACKAGE_HELP_DIR = share/gnome/help/gnomebaker
NO_PREFIX_PACKAGE_MENU_DIR = share/gnome/apps
NO_PREFIX_PACKAGE_PIXMAPS_DIR = share/gnomebaker
OBJDUMP = @OBJDUMP@
OBJEXT = o
PACKAGE = gnomebaker
PACKAGE_DATA_DIR = /usr/local/share
PACKAGE_DOC_DIR = /usr/local/doc/gnomebaker
PACKAGE_HELP_DIR = /usr/local/share/gnome/help/gnomebaker
PACKAGE_MENU_DIR = /usr/local/share/gnome/apps
PACKAGE_PIXMAPS_DIR = /usr/local/share/gnomebaker
PKG_CONFIG = /usr/bin/pkg-config
POFILES =  pt_BR.po
POSUB = po
PO_IN_DATADIR_FALSE = 
PO_IN_DATADIR_TRUE = 
RANLIB = ranlib
RC = @RC@
STRIP = strip
USE_INCLUDED_LIBINTL = @USE_INCLUDED_LIBINTL@
USE_NLS = yes
VERSION = 0.2.1

SUBDIRS = intl po macros src pixmaps

gnomebakerdocdir = ${prefix}/doc/gnomebaker
gnomebakerdoc_DATA =  	README 	COPYING 	AUTHORS 	ChangeLog 	INSTALL 	NEWS 	TODO 	ABOUT-NLS


#gnomemenudir = $(prefix)/share/gnome/apps/Applications
gnomemenudir = /usr/share/applications
gnomemenu_DATA = gnomebaker.desktop

gnomebaker_glade_filedir = $(prefix)/share/gnomebaker
gnomebaker_glade_file_DATA = gnomebaker.glade

EXTRA_DIST = $(gnomebakerdoc_DATA) $(gnomebaker_glade_file_DATA) $(gnomemenu_DATA)
ACLOCAL_M4 = $(top_srcdir)/aclocal.m4
mkinstalldirs = $(SHELL) $(top_srcdir)/mkinstalldirs
CONFIG_HEADER = config.h
CONFIG_CLEAN_FILES =  gnomebaker.desktop
DATA =  $(gnomebaker_glade_file_DATA) $(gnomebakerdoc_DATA) \
$(gnomemenu_DATA)

DIST_COMMON =  README ./stamp-h.in ABOUT-NLS AUTHORS COPYING ChangeLog \
INSTALL Makefile.am Makefile.in NEWS TODO acconfig.h acinclude.m4 \
aclocal.m4 config.guess config.h.in config.sub configure configure.in \
gnomebaker.desktop.in install-sh ltmain.sh missing mkinstalldirs


DISTFILES = $(DIST_COMMON) $(SOURCES) $(HEADERS) $(TEXINFOS) $(EXTRA_DIST)

TAR = tar
GZIP_ENV = --best
all: all-redirect
.SUFFIXES:
$(srcdir)/Makefile.in: Makefile.am $(top_srcdir)/configure.in $(ACLOCAL_M4) 
	cd $(top_srcdir) && $(AUTOMAKE) --gnu Makefile

Makefile: $(srcdir)/Makefile.in  $(top_builddir)/config.status $(BUILT_SOURCES)
	cd $(top_builddir) \
	  && CONFIG_FILES=$@ CONFIG_HEADERS= $(SHELL) ./config.status

$(ACLOCAL_M4):  configure.in  acinclude.m4
	cd $(srcdir) && $(ACLOCAL)

config.status: $(srcdir)/configure $(CONFIG_STATUS_DEPENDENCIES)
	$(SHELL) ./config.status --recheck
$(srcdir)/configure: $(srcdir)/configure.in $(ACLOCAL_M4) $(CONFIGURE_DEPENDENCIES)
	cd $(srcdir) && $(AUTOCONF)

config.h: stamp-h
	@if test ! -f $@; then \
		rm -f stamp-h; \
		$(MAKE) stamp-h; \
	else :; fi
stamp-h: $(srcdir)/config.h.in $(top_builddir)/config.status
	cd $(top_builddir) \
	  && CONFIG_FILES= CONFIG_HEADERS=config.h \
	     $(SHELL) ./config.status
	@echo timestamp > stamp-h 2> /dev/null
$(srcdir)/config.h.in: $(srcdir)/stamp-h.in
	@if test ! -f $@; then \
		rm -f $(srcdir)/stamp-h.in; \
		$(MAKE) $(srcdir)/stamp-h.in; \
	else :; fi
$(srcdir)/stamp-h.in: $(top_srcdir)/configure.in $(ACLOCAL_M4) acconfig.h
	cd $(top_srcdir) && $(AUTOHEADER)
	@echo timestamp > $(srcdir)/stamp-h.in 2> /dev/null

mostlyclean-hdr:

clean-hdr:

distclean-hdr:
	-rm -f config.h

maintainer-clean-hdr:
gnomebaker.desktop: $(top_builddir)/config.status gnomebaker.desktop.in
	cd $(top_builddir) && CONFIG_FILES=$@ CONFIG_HEADERS= $(SHELL) ./config.status

install-gnomebaker_glade_fileDATA: $(gnomebaker_glade_file_DATA)
	@$(NORMAL_INSTALL)
	$(mkinstalldirs) $(DESTDIR)$(gnomebaker_glade_filedir)
	@list='$(gnomebaker_glade_file_DATA)'; for p in $$list; do \
	  if test -f $(srcdir)/$$p; then \
	    echo " $(INSTALL_DATA) $(srcdir)/$$p $(DESTDIR)$(gnomebaker_glade_filedir)/$$p"; \
	    $(INSTALL_DATA) $(srcdir)/$$p $(DESTDIR)$(gnomebaker_glade_filedir)/$$p; \
	  else if test -f $$p; then \
	    echo " $(INSTALL_DATA) $$p $(DESTDIR)$(gnomebaker_glade_filedir)/$$p"; \
	    $(INSTALL_DATA) $$p $(DESTDIR)$(gnomebaker_glade_filedir)/$$p; \
	  fi; fi; \
	done

uninstall-gnomebaker_glade_fileDATA:
	@$(NORMAL_UNINSTALL)
	list='$(gnomebaker_glade_file_DATA)'; for p in $$list; do \
	  rm -f $(DESTDIR)$(gnomebaker_glade_filedir)/$$p; \
	done

install-gnomebakerdocDATA: $(gnomebakerdoc_DATA)
	@$(NORMAL_INSTALL)
	$(mkinstalldirs) $(DESTDIR)$(gnomebakerdocdir)
	@list='$(gnomebakerdoc_DATA)'; for p in $$list; do \
	  if test -f $(srcdir)/$$p; then \
	    echo " $(INSTALL_DATA) $(srcdir)/$$p $(DESTDIR)$(gnomebakerdocdir)/$$p"; \
	    $(INSTALL_DATA) $(srcdir)/$$p $(DESTDIR)$(gnomebakerdocdir)/$$p; \
	  else if test -f $$p; then \
	    echo " $(INSTALL_DATA) $$p $(DESTDIR)$(gnomebakerdocdir)/$$p"; \
	    $(INSTALL_DATA) $$p $(DESTDIR)$(gnomebakerdocdir)/$$p; \
	  fi; fi; \
	done

uninstall-gnomebakerdocDATA:
	@$(NORMAL_UNINSTALL)
	list='$(gnomebakerdoc_DATA)'; for p in $$list; do \
	  rm -f $(DESTDIR)$(gnomebakerdocdir)/$$p; \
	done

install-gnomemenuDATA: $(gnomemenu_DATA)
	@$(NORMAL_INSTALL)
	$(mkinstalldirs) $(DESTDIR)$(gnomemenudir)
	@list='$(gnomemenu_DATA)'; for p in $$list; do \
	  if test -f $(srcdir)/$$p; then \
	    echo " $(INSTALL_DATA) $(srcdir)/$$p $(DESTDIR)$(gnomemenudir)/$$p"; \
	    $(INSTALL_DATA) $(srcdir)/$$p $(DESTDIR)$(gnomemenudir)/$$p; \
	  else if test -f $$p; then \
	    echo " $(INSTALL_DATA) $$p $(DESTDIR)$(gnomemenudir)/$$p"; \
	    $(INSTALL_DATA) $$p $(DESTDIR)$(gnomemenudir)/$$p; \
	  fi; fi; \
	done

uninstall-gnomemenuDATA:
	@$(NORMAL_UNINSTALL)
	list='$(gnomemenu_DATA)'; for p in $$list; do \
	  rm -f $(DESTDIR)$(gnomemenudir)/$$p; \
	done

# This directory's subdirectories are mostly independent; you can cd
# into them and run `make' without going through this Makefile.
# To change the values of `make' variables: instead of editing Makefiles,
# (1) if the variable is set in `config.status', edit `config.status'
#     (which will cause the Makefiles to be regenerated when you run `make');
# (2) otherwise, pass the desired values on the `make' command line.



all-recursive install-data-recursive install-exec-recursive \
installdirs-recursive install-recursive uninstall-recursive  \
check-recursive installcheck-recursive info-recursive dvi-recursive:
	@set fnord $(MAKEFLAGS); amf=$$2; \
	dot_seen=no; \
	target=`echo $@ | sed s/-recursive//`; \
	list='$(SUBDIRS)'; for subdir in $$list; do \
	  echo "Making $$target in $$subdir"; \
	  if test "$$subdir" = "."; then \
	    dot_seen=yes; \
	    local_target="$$target-am"; \
	  else \
	    local_target="$$target"; \
	  fi; \
	  (cd $$subdir && $(MAKE) $(AM_MAKEFLAGS) $$local_target) \
	   || case "$$amf" in *=*) exit 1;; *k*) fail=yes;; *) exit 1;; esac; \
	done; \
	if test "$$dot_seen" = "no"; then \
	  $(MAKE) $(AM_MAKEFLAGS) "$$target-am" || exit 1; \
	fi; test -z "$$fail"

mostlyclean-recursive clean-recursive distclean-recursive \
maintainer-clean-recursive:
	@set fnord $(MAKEFLAGS); amf=$$2; \
	dot_seen=no; \
	rev=''; list='$(SUBDIRS)'; for subdir in $$list; do \
	  rev="$$subdir $$rev"; \
	  test "$$subdir" != "." || dot_seen=yes; \
	done; \
	test "$$dot_seen" = "no" && rev=". $$rev"; \
	target=`echo $@ | sed s/-recursive//`; \
	for subdir in $$rev; do \
	  echo "Making $$target in $$subdir"; \
	  if test "$$subdir" = "."; then \
	    local_target="$$target-am"; \
	  else \
	    local_target="$$target"; \
	  fi; \
	  (cd $$subdir && $(MAKE) $(AM_MAKEFLAGS) $$local_target) \
	   || case "$$amf" in *=*) exit 1;; *k*) fail=yes;; *) exit 1;; esac; \
	done && test -z "$$fail"
tags-recursive:
	list='$(SUBDIRS)'; for subdir in $$list; do \
	  test "$$subdir" = . || (cd $$subdir && $(MAKE) $(AM_MAKEFLAGS) tags); \
	done

tags: TAGS

ID: $(HEADERS) $(SOURCES) $(LISP)
	list='$(SOURCES) $(HEADERS)'; \
	unique=`for i in $$list; do echo $$i; done | \
	  awk '    { files[$$0] = 1; } \
	       END { for (i in files) print i; }'`; \
	here=`pwd` && cd $(srcdir) \
	  && mkid -f$$here/ID $$unique $(LISP)

TAGS: tags-recursive $(HEADERS) $(SOURCES) config.h.in $(TAGS_DEPENDENCIES) $(LISP)
	tags=; \
	here=`pwd`; \
	list='$(SUBDIRS)'; for subdir in $$list; do \
   if test "$$subdir" = .; then :; else \
	    test -f $$subdir/TAGS && tags="$$tags -i $$here/$$subdir/TAGS"; \
   fi; \
	done; \
	list='$(SOURCES) $(HEADERS)'; \
	unique=`for i in $$list; do echo $$i; done | \
	  awk '    { files[$$0] = 1; } \
	       END { for (i in files) print i; }'`; \
	test -z "$(ETAGS_ARGS)config.h.in$$unique$(LISP)$$tags" \
	  || (cd $(srcdir) && etags -o $$here/TAGS $(ETAGS_ARGS) $$tags config.h.in $$unique $(LISP))

mostlyclean-tags:

clean-tags:

distclean-tags:
	-rm -f TAGS ID

maintainer-clean-tags:

distdir = $(PACKAGE)-$(VERSION)
top_distdir = $(distdir)

# This target untars the dist file and tries a VPATH configuration.  Then
# it guarantees that the distribution is self-contained by making another
# tarfile.
distcheck: dist
	-rm -rf $(distdir)
	GZIP=$(GZIP_ENV) $(TAR) zxf $(distdir).tar.gz
	mkdir $(distdir)/=build
	mkdir $(distdir)/=inst
	dc_install_base=`cd $(distdir)/=inst && pwd`; \
	cd $(distdir)/=build \
	  && ../configure --with-included-gettext --srcdir=.. --prefix=$$dc_install_base \
	  && $(MAKE) $(AM_MAKEFLAGS) \
	  && $(MAKE) $(AM_MAKEFLAGS) dvi \
	  && $(MAKE) $(AM_MAKEFLAGS) check \
	  && $(MAKE) $(AM_MAKEFLAGS) install \
	  && $(MAKE) $(AM_MAKEFLAGS) installcheck \
	  && $(MAKE) $(AM_MAKEFLAGS) dist
	-rm -rf $(distdir)
	@banner="$(distdir).tar.gz is ready for distribution"; \
	dashes=`echo "$$banner" | sed s/./=/g`; \
	echo "$$dashes"; \
	echo "$$banner"; \
	echo "$$dashes"
dist: distdir
	-chmod -R a+r $(distdir)
	GZIP=$(GZIP_ENV) $(TAR) chozf $(distdir).tar.gz $(distdir)
	-rm -rf $(distdir)
dist-all: distdir
	-chmod -R a+r $(distdir)
	GZIP=$(GZIP_ENV) $(TAR) chozf $(distdir).tar.gz $(distdir)
	-rm -rf $(distdir)
distdir: $(DISTFILES)
	-rm -rf $(distdir)
	mkdir $(distdir)
	-chmod 777 $(distdir)
	here=`cd $(top_builddir) && pwd`; \
	top_distdir=`cd $(distdir) && pwd`; \
	distdir=`cd $(distdir) && pwd`; \
	cd $(top_srcdir) \
	  && $(AUTOMAKE) --include-deps --build-dir=$$here --srcdir-name=$(top_srcdir) --output-dir=$$top_distdir --gnu Makefile
	@for file in $(DISTFILES); do \
	  d=$(srcdir); \
	  if test -d $$d/$$file; then \
	    cp -pr $$d/$$file $(distdir)/$$file; \
	  else \
	    test -f $(distdir)/$$file \
	    || ln $$d/$$file $(distdir)/$$file 2> /dev/null \
	    || cp -p $$d/$$file $(distdir)/$$file || :; \
	  fi; \
	done
	for subdir in $(SUBDIRS); do \
	  if test "$$subdir" = .; then :; else \
	    test -d $(distdir)/$$subdir \
	    || mkdir $(distdir)/$$subdir \
	    || exit 1; \
	    chmod 777 $(distdir)/$$subdir; \
	    (cd $$subdir && $(MAKE) $(AM_MAKEFLAGS) top_distdir=../$(distdir) distdir=../$(distdir)/$$subdir distdir) \
	      || exit 1; \
	  fi; \
	done
	$(MAKE) $(AM_MAKEFLAGS) top_distdir="$(top_distdir)" distdir="$(distdir)" dist-hook
info-am:
info: info-recursive
dvi-am:
dvi: dvi-recursive
check-am: all-am
check: check-recursive
installcheck-am:
installcheck: installcheck-recursive
all-recursive-am: config.h
	$(MAKE) $(AM_MAKEFLAGS) all-recursive

install-exec-am:
install-exec: install-exec-recursive

install-data-am: install-gnomebaker_glade_fileDATA \
		install-gnomebakerdocDATA install-gnomemenuDATA
install-data: install-data-recursive

install-am: all-am
	@$(MAKE) $(AM_MAKEFLAGS) install-exec-am install-data-am
install: install-recursive
uninstall-am: uninstall-gnomebaker_glade_fileDATA \
		uninstall-gnomebakerdocDATA uninstall-gnomemenuDATA
uninstall: uninstall-recursive
all-am: Makefile $(DATA) config.h
all-redirect: all-recursive-am
install-strip:
	$(MAKE) $(AM_MAKEFLAGS) AM_INSTALL_PROGRAM_FLAGS=-s install
installdirs: installdirs-recursive
installdirs-am:
	$(mkinstalldirs)  $(DESTDIR)$(gnomebaker_glade_filedir) \
		$(DESTDIR)$(gnomebakerdocdir) $(DESTDIR)$(gnomemenudir)


mostlyclean-generic:

clean-generic:

distclean-generic:
	-rm -f Makefile $(CONFIG_CLEAN_FILES)
	-rm -f config.cache config.log stamp-h stamp-h[0-9]*

maintainer-clean-generic:
mostlyclean-am:  mostlyclean-hdr mostlyclean-tags mostlyclean-generic

mostlyclean: mostlyclean-recursive

clean-am:  clean-hdr clean-tags clean-generic mostlyclean-am

clean: clean-recursive

distclean-am:  distclean-hdr distclean-tags distclean-generic clean-am
	-rm -f libtool

distclean: distclean-recursive
	-rm -f config.status

maintainer-clean-am:  maintainer-clean-hdr maintainer-clean-tags \
		maintainer-clean-generic distclean-am
	@echo "This command is intended for maintainers to use;"
	@echo "it deletes files that may require special tools to rebuild."

maintainer-clean: maintainer-clean-recursive
	-rm -f config.status

.PHONY: mostlyclean-hdr distclean-hdr clean-hdr maintainer-clean-hdr \
uninstall-gnomebaker_glade_fileDATA install-gnomebaker_glade_fileDATA \
uninstall-gnomebakerdocDATA install-gnomebakerdocDATA \
uninstall-gnomemenuDATA install-gnomemenuDATA install-data-recursive \
uninstall-data-recursive install-exec-recursive \
uninstall-exec-recursive installdirs-recursive uninstalldirs-recursive \
all-recursive check-recursive installcheck-recursive info-recursive \
dvi-recursive mostlyclean-recursive distclean-recursive clean-recursive \
maintainer-clean-recursive tags tags-recursive mostlyclean-tags \
distclean-tags clean-tags maintainer-clean-tags distdir info-am info \
dvi-am dvi check check-am installcheck-am installcheck all-recursive-am \
install-exec-am install-exec install-data-am install-data install-am \
install uninstall-am uninstall all-redirect all-am all installdirs-am \
installdirs mostlyclean-generic distclean-generic clean-generic \
maintainer-clean-generic clean mostlyclean distclean maintainer-clean


# Copy all the spec files. Of cource, only one is actually used.
dist-hook:
	for specfile in *.spec; do \
		if test -f $$specfile; then \
			cp -p $$specfile $(distdir); \
		fi \
	done

# Tell versions [3.59,3.63) of GNU make to not export all variables.
# Otherwise a system limit (for SysV at least) may be exceeded.
.NOEXPORT: