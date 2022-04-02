
#ifndef TELEPATHYTRANSFERPLUGIN_H
#define TELEPATHYTRANSFERPLUGIN_H
#include "transferplugininterface.h"
#include <QObject>

class Q_DECL_EXPORT TelepathyTransferPlugin : public QObject, public TransferPluginInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "net.telepathy.transfer-plugin")
    Q_INTERFACES(TransferPluginInterface)
public:
    TelepathyTransferPlugin();
    ~TelepathyTransferPlugin();

    MediaTransferInterface * transferObject();
    QString pluginId() const;
};

#endif // TELEPATHYTRANSFERPLUGIN_H
