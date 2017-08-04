#-------------------------------------------------
#
# Project created by QtCreator 2017-04-20T09:05:20
#
#-------------------------------------------------

#-------------------------------------------------------------------------#
# Common code paths                                                       #
#-------------------------------------------------------------------------#
COMMON_SRC    = $${PWD}/FMI_common/common
COMMONGUI_SRC = $${PWD}/FMI_common/common_gui

#-------------------------------------------------------------------------#
# Libraries' sources and paths                                            #
#-------------------------------------------------------------------------#
unix{
    QUAZIP_SRC = /usr/include/quazip5/
    FMIPP_LIB  = $${PWD}/../../../ThirdParty/build/fmipp-build
}

win32-g++{
    DEFINES      += QUAZIP_STATIC
    QUAZIP_SRC    = $${PWD}/../../../ThirdParty/src/quazip-0.7.3/quazip
    QUAZIP_LIB    = $${PWD}/../../../ThirdParty/build-win/quazip-build
    FMIPP_LIB     = $${PWD}/../../../ThirdParty/build-win/fmipp-build
    SUNDIALS_LIB  = $${PWD}/../../../ThirdParty/build-win/sundials-build
}

FMIPP_SRC  = $${PWD}/../../../ThirdParty/src/fmipp-code
#-------------------------------------------------------------------------#

QT          += core gui xml charts datavisualization printsupport svg
RESOURCES    = resources.qrc internal.qrc
BINARY_NAME  = "$$cat(binary_name)"
TARGET       = $${BINARY_NAME}
TEMPLATE     = app

qtHaveModule(opengl): QT += opengl

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS USE_SUNDIALS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# ---------------- #
# Icon for Windows #
# ---------------- #
win32-g++: RC_ICONS += appico.ico

#------------------------------- #
# For cross-compilation of FMI++ #
#------------------------------- #
win32-g++: DEFINES += MINGW

SOURCES += main.cpp\
           mainwindow.cpp \
           commonsimint.cpp \
           svgviewer.cpp \
           about.cpp \
           $${COMMON_SRC}/opt_project.cpp \
           $${COMMON_SRC}/simulation.cpp \
           $${COMMON_SRC}/common.cpp \
           $${COMMON_SRC}/formats/libcsv.c \
           $${COMMON_SRC}/formats/read_csv.c \
           $${COMMON_SRC}/formats/read_matlab4.c \
           $${COMMON_SRC}/formats/write_matlab4.c \
           $${COMMON_SRC}/opt_problem.cpp \
           $${COMMONGUI_SRC}/commonapp.cpp \
           $${COMMONGUI_SRC}/brushconf.cpp \
           $${COMMONGUI_SRC}/graphconfig.cpp \
           $${COMMONGUI_SRC}/penconfig.cpp \
           $${COMMONGUI_SRC}/seriesconfig.cpp \
           $${COMMONGUI_SRC}/integrator.cpp \
           $${COMMONGUI_SRC}/outputs.cpp \
           $${COMMONGUI_SRC}/plotgraphs.cpp

HEADERS  += mainwindow.h \
            confapp.h \
            commonsimint.h \
            svgviewer.h \
            about.h \
            $${COMMON_SRC}/simulation.h \
            $${COMMON_SRC}/common.h \
            $${COMMON_SRC}/formats/libcsv.h \
            $${COMMON_SRC}/formats/read_csv.h \
            $${COMMON_SRC}/formats/read_matlab4.h \
            $${COMMON_SRC}/formats/write_matlab4.h \
            $${COMMON_SRC}/warnings_off.h \
            $${COMMON_SRC}/warnings_on.h \
            $${COMMON_SRC}/opt_problem.h \
            $${COMMONGUI_SRC}/commonapp.h \
            $${COMMONGUI_SRC}/brushconf.h \
            $${COMMONGUI_SRC}/graphconfig.h \
            $${COMMONGUI_SRC}/penconfig.h \
            $${COMMONGUI_SRC}/seriesconfig.h \
            $${COMMONGUI_SRC}/integrator.h \
            $${COMMONGUI_SRC}/outputs.h \
            $${COMMONGUI_SRC}/plotgraphs.h

INCLUDEPATH += $${COMMON_SRC} \    # Common
               $${COMMONGUI_SRC} \ # Common_gui
               $${QUAZIP_SRC} \    # Quazip
               $${FMIPP_SRC}       # FMI++

win32-g++{
    LIBS += -L$${FMIPP_LIB}/ -lfmippim \                             # FMI++
            -L$${FMIPP_LIB}/ -lfmippex \                             # FMI++
            -L$${QUAZIP_LIB}/release/ -lquazip \                     # Quazip
            -L$${SUNDIALS_LIB}/src/cvode/ -lsundials_cvode \         # Sundials
            -L$${SUNDIALS_LIB}/src/nvec_ser/ -lsundials_nvecserial \ # Sundials
}

unix{
    LIBS += -L$${FMIPP_LIB} -lfmippim \ # FMI++
            -L$${FMIPP_LIB} -lfmippex \ # FMI++
            -ldl \                      # libdl
            -lquazip5 \                 # Quazip
            -lsundials_cvode \          # Sundials
            -lsundials_nvecserial \     # Sundials
}

FORMS    += mainwindow.ui \
            svgviewer.ui \
            about.ui \
            $${COMMONGUI_SRC}/brushconf.ui \
            $${COMMONGUI_SRC}/graphconfig.ui \
            $${COMMONGUI_SRC}/penconfig.ui \
            $${COMMONGUI_SRC}/seriesconfig.ui \
            $${COMMONGUI_SRC}/integrator.ui \
            $${COMMONGUI_SRC}/outputs.ui \
            $${COMMONGUI_SRC}/plotgraphs.ui

# Binary for Linux
# -----------------------------
QMAKE_RPATHDIR += $${FMIPP_LIB}
