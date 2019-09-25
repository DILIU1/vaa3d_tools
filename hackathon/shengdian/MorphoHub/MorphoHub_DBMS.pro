
TEMPLATE	= lib
CONFIG	+= qt plugin warn_off
#CONFIG	+= x86_64
VAA3DPATH = ../../../../v3d_external
INCLUDEPATH	+= $$VAA3DPATH/v3d_main/basic_c_fun

HEADERS	+= MorphoHub_DBMS_plugin.h \
    MainWindow/morphohub_mainwindow.h \
    MainDialog/maindialog.h \
    BasicInfo.h
SOURCES	+= MorphoHub_DBMS_plugin.cpp \
    MainWindow/morphohub_mainwindow.cpp \
    MainDialog/maindialog.cpp
SOURCES	+= $$VAA3DPATH/v3d_main/basic_c_fun/v3d_message.cpp

TARGET	= $$qtLibraryTarget(MorphoHub_DBMS)
DESTDIR	= $$VAA3DPATH/bin/plugins/neuron_utilities/MorphoHub_DBMS/

OTHER_FILES +=
