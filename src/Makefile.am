## reduced version of gimp-plugin-template/src/Makefile.am
## then extended with OpenCV

bin_PROGRAMS = gimp-plugin-engine-template

bindir = $(GIMP_LIBDIR)/plug-ins

gimp_plugin_engine_template_SOURCES = \
	main.c		\
	render.c	\
	render.h	\
	adaptOpenCV.cpp \
	adaptOpenCV.hpp

AM_CPPFLAGS =\
	-I$(top_srcdir)		\
	@GIMP_CFLAGS@		\
	-I$(includedir)

## OPENCV_LIBS is optional
LDADD = $(GIMP_LIBS) $(OPENCV_LIBS)

