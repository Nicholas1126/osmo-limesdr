EXTRA_DIST = osmosgsn-usermanual.adoc \
    osmosgsn-usermanual-docinfo.xml \
    osmosgsn-vty-reference.xml \
    chapters \
    vty

if BUILD_MANUALS
  ASCIIDOC = osmosgsn-usermanual.adoc
  ASCIIDOC_DEPS = $(srcdir)/chapters/*.adoc
  include $(OSMO_GSM_MANUALS_DIR)/build/Makefile.asciidoc.inc

  VTY_REFERENCE = osmosgsn-vty-reference.xml
  include $(OSMO_GSM_MANUALS_DIR)/build/Makefile.vty-reference.inc

  include $(OSMO_GSM_MANUALS_DIR)/build/Makefile.common.inc
endif
