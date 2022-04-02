TEMPLATE = lib
TARGET = $$qtLibraryTarget(telepathytransferplugin)
CONFIG += plugin
DEPENDPATH += .

CONFIG += link_pkgconfig
PKGCONFIG += nemotransferengine-qt5
PKGCONFIG += TelepathyQt5
PKGCONFIG += Qt5DBus

# Input
HEADERS += \
    telepathyshareuploader.h \
    telepathytransferplugin.h \
    telepathyshareclienthandler.h

SOURCES += \
    telepathyshareuploader.cpp \
    telepathytransferplugin.cpp \
    telepathyshareclienthandler.cpp

OTHER_FILES +=

target.path = $$[QT_INSTALL_LIBS]/nemo-transferengine/plugins/transfer
INSTALLS += target

DISTFILES += target
