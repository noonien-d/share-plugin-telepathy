TEMPLATE = lib
TARGET = $$qtLibraryTarget(telepathyshareplugin)
CONFIG += plugin

CONFIG += link_pkgconfig
PKGCONFIG += nemotransferengine-qt5

# Input
HEADERS += \
    telepathyshareplugininfo.h \
    telepathyshareplugin.h \

SOURCES += \
    telepathyshareplugininfo.cpp \
    telepathyshareplugin.cpp \

OTHER_FILES +=

shareui.files = ShareTelepathyList.qml
shareui.path = $$[QT_INSTALL_PREFIX]/share/nemo-transferengine/plugins/sharing

target.path = $$[QT_INSTALL_LIBS]/nemo-transferengine/plugins/sharing
INSTALLS += target shareui

DISTFILES += target shareui \
    rpm/share-plugin-telepathy.yaml \
    qml/ShareTelepathyList.qml
