## Process this file with automake to produce Makefile.in
SUBDIRS = lib gtp ggsn sgsnemu doc contrib tests

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = libgtp.pc

BUILT_SOURCES = $(top_srcdir)/.version
$(top_srcdir)/.version:
	echo $(VERSION) > $@-t && mv $@-t $@
dist-hook:
	echo $(VERSION) > $(distdir)/.tarball-version

EXTRA_DIST = git-version-gen .version README.md README.FreeBSD README.MacOSX

AM_DISTCHECK_CONFIGURE_FLAGS = \
	--with-systemdsystemunitdir=$$dc_install_base/$(systemdsystemunitdir)

@RELMAKE@
