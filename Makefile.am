ACLOCAL_AMFLAGS = -I m4
SUBDIRS = src tests doc

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = ggen.pc

README: README.md
	@PANDOC@ -s -t plain $< -o $@

README.python: README.python.md
	@PANDOC@ -s -t plain $< -o $@

all-local: README README.python

clean-local:
	-rm README README.python

EXTRA_DIST = autogen.sh version.sh ChangeLog ggen.pc
