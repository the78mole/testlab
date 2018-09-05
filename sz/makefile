

#****************************************************************************
#
#  Program    : SPACE - testprogram
#
#****************************************************************************
#
#  Company    : SZ-Testsysteme, Wasserburger-Str. 44, 8201 Amerang
#
#  Author     : Kirstin Mentzel, Artmann Volker
#
#  System 300 : HP-UX S300 B.09.03, C/ANSI C  Internal Revision: 72.2
#  System 700 : HP-UX A.09.03., HP92453-01 A.09.69 HP C Compiler
#  System sun : SunOS 5.3, cc.c 7.178 93/11/15
#
#****************************************************************************
#
#  Project    : testprogram
#
#  Task       : received all start messages and run the test functions
#
#*****************************************************************************

PROGRAM_NAME	= testlab

HDRS		=

EXTHDRS		=

SRCSC		= testlab.c



#
# including additional CFLAGS for 'normal'
#
NORMAL_CFLAGS     =
NORMAL_CFLAGS_HP  =
NORMAL_CFLAGS_300 =
NORMAL_CFLAGS_700 =
NORMAL_CFLAGS_SUN =




#
# including additional CFLAGS for 'debug' and 'szDebug'
#
DEBUG_CFLAGS      =
DEBUG_CFLAGS_HP   =
DEBUG_CFLAGS_300  =
DEBUG_CFLAGS_700  =
DEBUG_CFLAGS_SUN  =



#
# including additional LDFLAGS for 'normal'
#
NORMAL_LDFLAGS     =
NORMAL_LDFLAGS_HP  =
NORMAL_LDFLAGS_300 =
NORMAL_LDFLAGS_700 =
NORMAL_LDFLAGS_SUN = 



#
# including additional LDFLAGS for 'debug' and 'szDebug'
#
DEBUG_LDFLAGS     =
DEBUG_LDFLAGS_HP  =
DEBUG_LDFLAGS_300 =
DEBUG_LDFLAGS_700 =
DEBUG_LDFLAGS_SUN = 



#
# for temporary overriding already used libs
# (it`s entered at the beginning of the library-list)
#
TEMP_LIBS       =					\
$(HOME)/sz/progs/prober/enhancedSerial.$(MODEL).deb.o	\
                  $(SZDIR)/lib/rs232Driver.o





#
# including additional libraries for 'normal'
#
NORMAL_LIBS     =
NORMAL_LIBS_HP  =
NORMAL_LIBS_300 =
NORMAL_LIBS_700 =
NORMAL_LIBS_SUN = 



#
# including additional libraries for 'debug' and 'szDebug'
#
DEBUG_LIBS     =
DEBUG_LIBS_HP  =
DEBUG_LIBS_300 =
DEBUG_LIBS_700 =
DEBUG_LIBS_SUN = 



include $(SZDIR)/config/tp.${MODEL}.mk



#
# Possible targets:
# normal, debug, szDebug install clobber myClobber
#
all:            normal

myClobber:;

install:;

