
#include "telepathytransferplugin.h"
#include "telepathyshareuploader.h"
#include <QtPlugin>

TelepathyTransferPlugin::TelepathyTransferPlugin()
{
    qDebug() << "created transfer plugin";
}

TelepathyTransferPlugin::~TelepathyTransferPlugin()
{
}

MediaTransferInterface* TelepathyTransferPlugin::transferObject()
{
    return new TelepathyShareUploader;
}

QString TelepathyTransferPlugin::pluginId() const
{
    return "Telepathy-Share-Method-ID";
}
