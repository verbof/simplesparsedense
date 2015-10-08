#############################################################################
# Makefile for building: bin/sparsedense
# Generated by qmake (2.01a) (Qt 4.6.3) on: Wed Jul 8 13:23:40 2015
# Project:  sparsedense.pro
# Template: app
# Command: /usr/bin/qmake -unix -o Makefile sparsedense.pro
#############################################################################

####### Compiler, tools and options

CC            = /opt/cray/craype/2.2.1/bin/CC
CXX           = /opt/cray/craype/2.2.1/bin/CC
DEFINES       = 
CFLAGS        = -m64 -pipe -O2 -fmessage-length=0 -O2 -Wall -D_FORTIFY_SOURCE=2 -fstack-protector -funwind-tables -fasynchronous-unwind-tables -g -Wall -W $(DEFINES)
CXXFLAGS      = -m64 -pipe -O2 -fmessage-length=0 -O2 -Wall -D_FORTIFY_SOURCE=2 -fstack-protector -funwind-tables -fasynchronous-unwind-tables -g -Wall -W $(DEFINES)
INCPATH       = -I/usr/share/qt4/mkspecs/default -I. -Isrc
LINK          = /opt/cray/craype/2.2.1/bin/CC
LFLAGS        = -dynamic -Wl,-O1
LIBS          = $(SUBLIBS)   -L${HOME}/Libraries/linuxAMD64 -lpardiso500-GNU481-X86-64 -L${MKLROOT}/lib/intel64/ -lmkl_lapack95_lp64 -lmkl_gf_lp64 -lmkl_sequential -lmkl_core -lgfortran -lgomp ${INTEL_PATH}.${INTEL_MINOR_VERSION}/compiler/lib/intel64/libirc.a 
AR            = ar cqs
RANLIB        = 
QMAKE         = /usr/bin/qmake
TAR           = tar -cf
COMPRESS      = gzip -9f
COPY          = cp -f
SED           = sed
COPY_FILE     = $(COPY)
COPY_DIR      = $(COPY) -r
STRIP         = strip
INSTALL_FILE  = install -m 644 -p
INSTALL_DIR   = $(COPY_DIR)
INSTALL_PROGRAM = install -m 755 -p
DEL_FILE      = rm -f
SYMLINK       = ln -f -s
DEL_DIR       = rmdir
MOVE          = mv -f
CHK_DIR_EXISTS= test -d
MKDIR         = mkdir -p

####### Output directory

OBJECTS_DIR   = obj/

####### Files

SOURCES       = src/CSRdouble.cpp \
		src/CSRcomplex.cpp \
		src/RealMath.cpp \
		src/ParDiSO.cpp \
		src/tools.cpp \
		src/schur.cpp \
		src/IO.cpp \
		src/readinput.cpp \
		src/readdist.cpp \
		src/main.cpp 
OBJECTS       = obj/CSRdouble.o \
		obj/CSRcomplex.o \
		obj/RealMath.o \
		obj/ParDiSO.o \
		obj/tools.o \
		obj/schur.o \
		obj/IO.o \
		obj/readinput.o \
		obj/readdist.o \
		obj/main.o
DIST          = /usr/share/qt4/mkspecs/common/g++.conf \
		/usr/share/qt4/mkspecs/common/unix.conf \
		/usr/share/qt4/mkspecs/common/linux.conf \
		/usr/share/qt4/mkspecs/qconfig.pri \
		/usr/share/qt4/mkspecs/features/qt_functions.prf \
		/usr/share/qt4/mkspecs/features/qt_config.prf \
		/usr/share/qt4/mkspecs/features/exclusive_builds.prf \
		/usr/share/qt4/mkspecs/features/default_pre.prf \
		/usr/share/qt4/mkspecs/features/release.prf \
		/usr/share/qt4/mkspecs/features/default_post.prf \
		/usr/share/qt4/mkspecs/features/warn_on.prf \
		/usr/share/qt4/mkspecs/features/resources.prf \
		/usr/share/qt4/mkspecs/features/uic.prf \
		/usr/share/qt4/mkspecs/features/yacc.prf \
		/usr/share/qt4/mkspecs/features/lex.prf \
		/usr/share/qt4/mkspecs/features/include_source_dir.prf \
		sparsedense.pro
QMAKE_TARGET  = sparsedense
DESTDIR       = bin/
TARGET        = bin/sparsedense

first: all
####### Implicit rules

.SUFFIXES: .o .c .cpp .cc .cxx .C

.cpp.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.cc.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.cxx.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.C.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.c.o:
	$(CC) -c $(CFLAGS) $(INCPATH) -o "$@" "$<"

####### Build rules

all: Makefile $(TARGET)

$(TARGET):  $(OBJECTS)  
	@$(CHK_DIR_EXISTS) bin/ || $(MKDIR) bin/ 
	$(LINK) $(LFLAGS) -o $(TARGET) $(OBJECTS) $(OBJCOMP) $(LIBS)

Makefile: sparsedense.pro  /usr/share/qt4/mkspecs/default/qmake.conf /usr/share/qt4/mkspecs/common/g++.conf \
		/usr/share/qt4/mkspecs/common/unix.conf \
		/usr/share/qt4/mkspecs/common/linux.conf \
		/usr/share/qt4/mkspecs/qconfig.pri \
		/usr/share/qt4/mkspecs/features/qt_functions.prf \
		/usr/share/qt4/mkspecs/features/qt_config.prf \
		/usr/share/qt4/mkspecs/features/exclusive_builds.prf \
		/usr/share/qt4/mkspecs/features/default_pre.prf \
		/usr/share/qt4/mkspecs/features/release.prf \
		/usr/share/qt4/mkspecs/features/default_post.prf \
		/usr/share/qt4/mkspecs/features/warn_on.prf \
		/usr/share/qt4/mkspecs/features/resources.prf \
		/usr/share/qt4/mkspecs/features/uic.prf \
		/usr/share/qt4/mkspecs/features/yacc.prf \
		/usr/share/qt4/mkspecs/features/lex.prf \
		/usr/share/qt4/mkspecs/features/include_source_dir.prf
	$(QMAKE) -unix -o Makefile sparsedense.pro
/usr/share/qt4/mkspecs/common/g++.conf:
/usr/share/qt4/mkspecs/common/unix.conf:
/usr/share/qt4/mkspecs/common/linux.conf:
/usr/share/qt4/mkspecs/qconfig.pri:
/usr/share/qt4/mkspecs/features/qt_functions.prf:
/usr/share/qt4/mkspecs/features/qt_config.prf:
/usr/share/qt4/mkspecs/features/exclusive_builds.prf:
/usr/share/qt4/mkspecs/features/default_pre.prf:
/usr/share/qt4/mkspecs/features/release.prf:
/usr/share/qt4/mkspecs/features/default_post.prf:
/usr/share/qt4/mkspecs/features/warn_on.prf:
/usr/share/qt4/mkspecs/features/resources.prf:
/usr/share/qt4/mkspecs/features/uic.prf:
/usr/share/qt4/mkspecs/features/yacc.prf:
/usr/share/qt4/mkspecs/features/lex.prf:
/usr/share/qt4/mkspecs/features/include_source_dir.prf:
qmake:  FORCE
	@$(QMAKE) -unix -o Makefile sparsedense.pro

dist: 
	@$(CHK_DIR_EXISTS) obj/sparsedense1.0.0 || $(MKDIR) obj/sparsedense1.0.0 
	$(COPY_FILE) --parents $(SOURCES) $(DIST) obj/sparsedense1.0.0/ && (cd `dirname obj/sparsedense1.0.0` && $(TAR) sparsedense1.0.0.tar sparsedense1.0.0 && $(COMPRESS) sparsedense1.0.0.tar) && $(MOVE) `dirname obj/sparsedense1.0.0`/sparsedense1.0.0.tar.gz . && $(DEL_FILE) -r obj/sparsedense1.0.0


clean:compiler_clean 
	-$(DEL_FILE) $(OBJECTS)
	-$(DEL_FILE) *~ core *.core


####### Sub-libraries

distclean: clean
	-$(DEL_FILE) $(TARGET) 
	-$(DEL_FILE) Makefile


check: first

compiler_rcc_make_all:
compiler_rcc_clean:
compiler_uic_make_all:
compiler_uic_clean:
compiler_image_collection_make_all: qmake_image_collection.cpp
compiler_image_collection_clean:
	-$(DEL_FILE) qmake_image_collection.cpp
compiler_yacc_decl_make_all:
compiler_yacc_decl_clean:
compiler_yacc_impl_make_all:
compiler_yacc_impl_clean:
compiler_lex_make_all:
compiler_lex_clean:
compiler_clean: 

####### Compile

obj/CSRdouble.o: src/CSRdouble.cpp src/CSRdouble.hpp \
		src/config.hpp \
		src/searchingsorting.hpp
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o obj/CSRdouble.o src/CSRdouble.cpp

obj/CSRcomplex.o: src/CSRcomplex.cpp src/CSRcomplex.hpp \
		src/config.hpp \
		src/searchingsorting.hpp
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o obj/CSRcomplex.o src/CSRcomplex.cpp

obj/RealMath.o: src/RealMath.cpp src/config.hpp \
		src/RealMath.hpp \
		src/timing.hpp \
		src/CSRdouble.hpp \
		src/ParDiSO.hpp \
		src/shared_var.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o obj/RealMath.o src/RealMath.cpp

obj/ParDiSO.o: src/ParDiSO.cpp src/ParDiSO.hpp \
		src/config.hpp \
		src/CSRdouble.hpp \
		src/CSRcomplex.hpp \
		src/shared_var.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o obj/ParDiSO.o src/ParDiSO.cpp

obj/tools.o: src/tools.cpp src/CSRdouble.hpp \
		src/config.hpp
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o obj/tools.o src/tools.cpp

obj/schur.o: src/schur.cpp src/shared_var.h \
		src/CSRdouble.hpp \
		src/config.hpp \
		src/ParDiSO.hpp \
		src/RealMath.hpp \
		src/timing.hpp
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o obj/schur.o src/schur.cpp

obj/IO.o: src/IO.cpp src/config.hpp \
		src/IO.hpp
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o obj/IO.o src/IO.cpp

obj/readinput.o: src/readinput.cpp src/shared_var.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o obj/readinput.o src/readinput.cpp

obj/readdist.o: src/readdist.cpp src/shared_var.h \
		src/RealMath.hpp \
		src/CSRdouble.hpp \
		src/config.hpp
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o obj/readdist.o src/readdist.cpp

obj/main.o: src/main.cpp src/shared_var.h \
		src/config.hpp \
		src/CSRdouble.hpp \
		src/IO.hpp \
		src/ParDiSO.hpp \
		src/RealMath.hpp \
		src/smat.h \
		src/timing.hpp
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o obj/main.o src/main.cpp

####### Install

install:   FORCE

uninstall:   FORCE

FORCE:

