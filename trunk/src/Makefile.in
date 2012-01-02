
SHELL = /bin/sh

SRCDIR = .
XPDFSRCDIR = ../xpdf
XPDFLIBDIR = ../xpdf
GOOSRCDIR = ../goo
GOOLIBDIR = ../goo
FOFISRCDIR = ../fofi
FOFILIBDIR = ../fofi
SPLASHSRCDIR = ../splash
SPLASHLIBDIR = ../splash

CXXFLAGS = -I/usr/local/include/freetype2 -I/usr/local/include -g -O2 -DHAVE_CONFIG_H -DHAVE_DIRENT_H=1  -I.. -DHAVE_REWINDDIR=1 -DHAVE_POPEN=1 -I.. -I$(GOOSRCDIR) -I$(XPDFSRCDIR) -I$(FOFISRCDIR) -I$(SPLASHSRCDIR) -I$(srcdir)           -I/usr/X11R6/include

LDFLAGS = 
FTLIBS = -lfreetype

OTHERLIBS = 

CXX ?= c++

LIBPREFIX = lib
EXE = 

#------------------------------------------------------------------------

.SUFFIXES: .cc

.cc.o:
	$(CXX) $(CXXFLAGS) -c $<

#------------------------------------------------------------------------

CXX_SRC = \
	$(SRCDIR)/pdf2image.cc \
	$(SRCDIR)/ImgOutputDev.cc \
	$(SRCDIR)/XmlFonts.cc \
	$(SRCDIR)/XmlLinks.cc 

#------------------------------------------------------------------------

all: pdf2image$(EXE)

#-------------------------------------------------------------------------

PDF2IMAGE_OBJS = ImgOutputDev.o XmlFonts.o XmlLinks.o \
    pdf2image.o
PDF2IMAGE_LIBS = -L$(GOOLIBDIR) -L$(FOFILIBDIR) -L$(SPLASHLIBDIR) $(FTLIBS) -L$(XPDFLIBDIR) $(OTHERLIBS) -lXpdf -lGoo -lfofi -lsplash -lm

pdf2image$(EXE): $(PDF2IMAGE_OBJS) $(GOOLIBDIR)/$(LIBPREFIX)Goo.a
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o pdf2image$(EXE) $(PDF2IMAGE_OBJS) \
		$(PDF2IMAGE_LIBS)

#-------------------------------------------------------------------------
PDF2IMAGE_WINOBJS = pdf2image.exe ImgOutPutDev.obj  pdf2image.obj  XmlFonts.obj  XmlLinks.obj

clean:
	rm -f $(PDF2IMAGE_OBJS) pdf2image$(EXE)
	rm -f $(PDF2IMAGE_WINOBJS)

#------------------------------------------------------------------------

distdepend:
	cp Makefile.in Makefile.in.bak
	sed '/^#----- dependences -----/q' Makefile.in.bak >Makefile.in
	$(CXX) $(CXXFLAGS) -MM $(CXX_SRC) >>Makefile.in

