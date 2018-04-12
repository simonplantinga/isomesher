#############################################################################
# Makefile for building: 3d-dynamic
# Generated by qmake (1.07a) (Qt 3.3.3) on: Tue Jan 17 13:15:50 2006
# Project:  3d-dynamic.pro
# Template: app
# Command: $(QMAKE) -o Makefile 3d-dynamic.pro
#############################################################################

####### Compiler, tools and options

CC       = gcc
CXX      = g++
LEX      = flex
YACC     = yacc
CFLAGS   = -pipe -Wall -W -O2  -DQT_NO_DEBUG -DQT_SHARED -DQT_THREAD_SUPPORT
CXXFLAGS = -pipe -Wall -W -O2  -DQT_NO_DEBUG -DQT_SHARED -DQT_THREAD_SUPPORT
LEXFLAGS = 
YACCFLAGS= -d
INCPATH  = -I/usr/share/qt3/mkspecs/default -I. -I$(FIDIR)/include -I/home/scratch/libqgl/libQGLViewer\-2.1.1\-3 -I/usr/include/qt3
LINK     = g++
LFLAGS   = 
LIBS     = $(SUBLIBS) -L/usr/share/qt3/lib -L/usr/X11R6/lib -L$(FIDIR)/lib -Wl,-rpath=$(FIDIR)/lib -lfi -lieee -L/home/scratch/libqgl/libQGLViewer\-2.1.1\-3/QGLViewer -lQGLViewer -lqt-mt -lXext -lX11 -lm
AR       = ar cqs
RANLIB   = 
MOC      = /usr/share/qt3/bin/moc
UIC      = /usr/share/qt3/bin/uic
QMAKE    = qmake
TAR      = tar -cf
GZIP     = gzip -9f
COPY     = cp -f
COPY_FILE= $(COPY)
COPY_DIR = $(COPY) -r
INSTALL_FILE= $(COPY_FILE)
INSTALL_DIR = $(COPY_DIR)
DEL_FILE = rm -f
SYMLINK  = ln -sf
DEL_DIR  = rmdir
MOVE     = mv -f
CHK_DIR_EXISTS= test -d
MKDIR    = mkdir -p

####### Output directory

OBJECTS_DIR = ./

####### Files

HEADERS = surfacewin.h \
		surface.h \
		testfunc.h \
		octree.h \
		approximate.h \
		tetramesh.h
SOURCES = main.cpp \
		surfacewin.cpp \
		octree.cpp \
		approximate.cpp \
		tetramesh.cpp
OBJECTS = main.o \
		surfacewin.o \
		octree.o \
		approximate.o \
		tetramesh.o
FORMS = 
UICDECLS = 
UICIMPLS = 
SRCMOC   = moc_surfacewin.cpp
OBJMOC = moc_surfacewin.o
DIST	   = 3d-dynamic.pro
QMAKE_TARGET = 3d-dynamic
DESTDIR  = 
TARGET   = 3d-dynamic

first: all
####### Implicit rules

.SUFFIXES: .c .o .cpp .cc .cxx .C

.cpp.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $@ $<

.cc.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $@ $<

.cxx.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $@ $<

.C.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $@ $<

.c.o:
	$(CC) -c $(CFLAGS) $(INCPATH) -o $@ $<

####### Build rules

all: Makefile $(TARGET)

$(TARGET):  $(UICDECLS) $(OBJECTS) $(OBJMOC)  
	$(LINK) $(LFLAGS) -o $(TARGET) $(OBJECTS) $(OBJMOC) $(OBJCOMP) $(LIBS)

mocables: $(SRCMOC)
uicables: $(UICDECLS) $(UICIMPLS)

$(MOC): 
	( cd $(QTDIR)/src/moc && $(MAKE) )

Makefile: 3d-dynamic.pro  /usr/share/qt3/mkspecs/default/qmake.conf /usr/share/qt3/lib/libqt-mt.prl
	$(QMAKE) -o Makefile 3d-dynamic.pro
qmake: 
	@$(QMAKE) -o Makefile 3d-dynamic.pro

dist: 
	@mkdir -p .tmp/3d-dynamic && $(COPY_FILE) --parents $(SOURCES) $(HEADERS) $(FORMS) $(DIST) .tmp/3d-dynamic/ && ( cd `dirname .tmp/3d-dynamic` && $(TAR) 3d-dynamic.tar 3d-dynamic && $(GZIP) 3d-dynamic.tar ) && $(MOVE) `dirname .tmp/3d-dynamic`/3d-dynamic.tar.gz . && $(DEL_FILE) -r .tmp/3d-dynamic

mocclean:
	-$(DEL_FILE) $(OBJMOC)
	-$(DEL_FILE) $(SRCMOC)

uiclean:

yaccclean:
lexclean:
clean: mocclean
	-$(DEL_FILE) $(OBJECTS)
	-$(DEL_FILE) *~ core *.core


####### Sub-libraries

distclean: clean
	-$(DEL_FILE) $(TARGET) $(TARGET)


FORCE:

####### Compile

main.o: main.cpp surfacewin.h \
		testfunc.h \
		approximate.h \
		octree.h \
		surface.h \
		tetramesh.h

surfacewin.o: surfacewin.cpp surfacewin.h \
		approximate.h \
		octree.h \
		surface.h \
		tetramesh.h

octree.o: octree.cpp octree.h

approximate.o: approximate.cpp approximate.h \
		octree.h \
		surface.h \
		tetramesh.h

tetramesh.o: tetramesh.cpp tetramesh.h \
		surface.h

moc_surfacewin.o: moc_surfacewin.cpp  surfacewin.h approximate.h \
		octree.h \
		surface.h \
		tetramesh.h

moc_surfacewin.cpp: $(MOC) surfacewin.h
	$(MOC) surfacewin.h -o moc_surfacewin.cpp

####### Install

install:  

uninstall:  

