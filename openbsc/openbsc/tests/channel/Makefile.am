AM_CPPFLAGS = \
	$(all_includes) \
	-I$(top_srcdir)/include \
	$(NULL)

AM_CFLAGS = \
	-Wall \
	-ggdb3 \
	$(LIBOSMOCORE_CFLAGS) \
	$(LIBOSMOGSM_CFLAGS) \
	$(LIBOSMOABIS_CFLAGS) \
	$(NULL)

EXTRA_DIST = \
	channel_test.ok \
	$(NULL)

noinst_PROGRAMS = \
	channel_test \
	$(NULL)

channel_test_SOURCES = \
	channel_test.c \
	$(NULL)

channel_test_LDFLAGS = \
	-Wl,--wrap=paging_request \
	$(NULL)

channel_test_LDADD = \
	$(top_builddir)/src/libmsc/libmsc.a \
	$(top_builddir)/src/libbsc/libbsc.a \
	$(top_builddir)/src/libcommon-cs/libcommon-cs.a \
	$(top_builddir)/src/libcommon/libcommon.a \
	$(LIBOSMOCORE_LIBS) \
	$(LIBOSMOGSM_LIBS) \
	$(LIBOSMOABIS_LIBS) \
	$(LIBCRYPTO_LIBS) \
	-ldbi \
	$(NULL)
