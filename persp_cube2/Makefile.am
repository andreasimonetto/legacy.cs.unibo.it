## Process this file with automake to produce Makefile.in

SUBDIRS = src po 

persp_cube2docdir = ${prefix}/doc/persp_cube2
persp_cube2doc_DATA = \
	README\
	COPYING\
	AUTHORS\
	ChangeLog\
	INSTALL\
	NEWS

EXTRA_DIST = $(persp_cube2doc_DATA) \
	intltool-merge\
	intltool-extract\
	intltool-update \
	mfiles/club71.m \
	mfiles/cube.m \
	mfiles/escher_turtles.m \
	intltool-extract.in \
	intltool-merge.in \
	intltool-update.in \
	mfiles/mannequin.m

# Copy all the spec files. Of cource, only one is actually used.
dist-hook:
	for specfile in *.spec; do \
		if test -f $$specfile; then \
			cp -p $$specfile $(distdir); \
		fi \
	done
