NAME=TuioClient
CSYM=TuioClient

CS_INCLUDE_DIR=/home/andres/src/csound5/H

current: cs_linux
UNAME = $(shell uname)

# ----------------------- NT -----------------------

cs_nt: $(NAME).dll

.SUFFIXES: .dll

PDNTCFLAGS = /W0 /DNT /DPD /nologo /D "WIN32" /EHsc

NTOBJECTS = TuioWrapper.obj TuioCsound.obj ./TUIO/TuioClient.obj ./TUIO/TuioTime.obj \
        ./oscpack/osc/OscTypes.obj ./oscpack/osc/OscReceivedElements.obj ./oscpack/osc/OscPrintReceivedElements.obj \
        ./oscpack/ip/win32/NetworkingUtils.obj ./oscpack/ip/win32/UdpSocket.obj

FINALOBJECTS = TuioClient.obj TuioWrapper.obj TuioCsound.obj TuioTime.obj \
        OscTypes.obj OscReceivedElements.obj OscPrintReceivedElements.obj \
        NetworkingUtils.obj UdpSocket.obj

VC="C:\Program Files\Microsoft Visual Studio 8\VC"
VCLIB="C:\Program Files\Microsoft Visual Studio 8\VC\PlatformSDK\lib"

PDNTINCLUDE = /I./TUIO /I./oscpack /I. /I..\..\src /I$(VC)\include

PDNTLDIR = $(VC)\lib
PDNTLIB = $(PDNTLDIR)\libcmt.lib \
        $(PDNTLDIR)\oldnames.lib \
        $(VCLIB)\kernel32.lib \
        $(VCLIB)\ws2_32.lib \
         $(VCLIB)\winmm.lib \
        C:\pd\bin\pd.lib

.cpp.obj:
        cl $(PDNTCFLAGS) $(PDNTINCLUDE) /c $*.cpp

$(NAME).dll: $(NTOBJECTS)
	link /dll /export:$(CSYM)_setup $(FINALOBJECTS) $(PDNTLIB)

# ----------------------- IRIX 5.x -----------------------

cs_irix5: $(NAME).cs_irix5

.SUFFIXES: .cs_irix5

SGICFLAGS5 = -o32 -DPD -DUNIX -DIRIX -O2

SGIINCLUDE =  -I./TUIO

.c.cs_irix5:
	$(CC) $(SGICFLAGS5) $(SGIINCLUDE) -o $*.o -c $*.c
	ld -elf -shared -rdata_shared -o $*.cs_irix5 $*.o
	rm $*.o

# ----------------------- IRIX 6.x -----------------------

cs_irix6: $(NAME).cs_irix6

.SUFFIXES: .cs_irix6

SGICFLAGS6 = -n32 -DPD -DUNIX -DIRIX -DN32 -woff 1080,1064,1185 \
	-OPT:roundoff=3 -OPT:IEEE_arithmetic=3 -OPT:cray_ivdep=true \
	-Ofast=ip32

.c.cs_irix6:
	$(CC) $(SGICFLAGS6) $(SGIINCLUDE) -o $*.o -c $*.c
	ld -n32 -IPA -shared -rdata_shared -o $*.cs_irix6 $*.o
	rm $*.o

# ----------------------- LINUX i386 -----------------------

cs_linux: $(NAME).so

.SUFFIXES: .so

OBJECTS = TuioWrapper.o TuioCsound.o ./TUIO/TuioTime.o ./TUIO/TuioClient.o \
	./oscpack/osc/OscTypes.o ./oscpack/osc/OscReceivedElements.o ./oscpack/osc/OscPrintReceivedElements.o \
	./oscpack/ip/posix/NetworkingUtils.o ./oscpack/ip/posix/UdpSocket.o

CXXFLAGS = -DOSC_HOST_LITTLE_ENDIAN -DUSE_DOUBLE -DPD -O2 -funroll-loops -fomit-frame-pointer -fPIC \
	$(CFLAGS) -I. -I./TUIO -I./oscpack -I$(CS_INCLUDE_DIR)

$(NAME).so: $(OBJECTS)
	$(CXX) -shared -o ../lib$(NAME).so $(OBJECTS)  -lc  -lm


# ----------------------- Mac OSX -----------------------

cs_darwin: $(NAME).cs_darwin

.SUFFIXES: .cs_darwin_o

OBJECTS = TuioWrapper.o TuioCsound.o ./TUIO/TuioTime.o ./TUIO/TuioClient.o \
	./oscpack/osc/OscTypes.o ./oscpack/osc/OscReceivedElements.o ./oscpack/osc/OscPrintReceivedElements.o \
	./oscpack/ip/posix/NetworkingUtils.o ./oscpack/ip/posix/UdpSocket.o

ifeq ($(UNAME),Darwin)
	CXXFLAGS = -DPD -g -O2 -arch i386 -arch ppc -funroll-loops -fomit-frame-pointer -I. -I./TUIO -I./oscpack -mmacosx-version-min=10.4
endif

$(NAME).cs_darwin: $(OBJECTS)
	$(CXX)  -bundle -undefined suppress -flat_namespace -o ../$(NAME).cs_darwin $(OBJECTS) -lc -lm -arch i386 -arch ppc

# ----------------------------------------------------------

clean:
	rm -f $(OBJECTS) *.cs_* so_locations
