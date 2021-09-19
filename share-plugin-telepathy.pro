TEMPLATE = lib
TARGET = $$qtLibraryTarget(share-plugin-telepathy)
CONFIG += plugin

CONFIG += link_pkgconfig
PKGCONFIG += nemotransferengine-qt5
PKGCONFIG += TelepathyQt5
PKGCONFIG += Qt5DBus

# Input
HEADERS += \
    src/telepathyshareplugininfo.h \
    src/telepathyshareuploader.h \
    src/telepathyshareplugin.h \
    src/telepathyshareclienthandler.h

SOURCES += \
    src/telepathyshareplugininfo.cpp \
    src/telepathyshareuploader.cpp \
    src/telepathyshareplugin.cpp \
    src/telepathyshareclienthandler.cpp

OTHER_FILES +=

shareui.files = qml/ShareTelepathyList.qml
shareui.path = $$[QT_INSTALL_PREFIX]/share/nemo-transferengine/plugins

target.path = $$[QT_INSTALL_LIBS]/nemo-transferengine/plugins
INSTALLS += target shareui

DISTFILES += target shareui \
    rpm/share-plugin-telepathy.yaml \
    qml/ShareTelepathyList.qml
