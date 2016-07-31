/* share-plugin-telepathy - a share provider for Sailfish OS.
 * Copyright (C) 2016  Ferdinand Stehle
 *
 * This program is free software; you can redistribute it and/or modify it under the
 * terms of the GNU General Public License as published by the Free Software Foundation;
 * either version 3 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with this program;
 * if not, see <http://www.gnu.org/licenses/>. */

#ifndef TELEPATHYSHAREPLUGIN_H
#define TELEPATHYSHAREPLUGIN_H

#include "transferplugininterface.h"
#include <QObject>

class Q_DECL_EXPORT TelepathySharePlugin : public QObject, public TransferPluginInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "fi.lahdemaki.transfer.plugin.telepathy-share-plugin")
    Q_INTERFACES(TransferPluginInterface)

public:
    TelepathySharePlugin();
    ~TelepathySharePlugin();

    MediaTransferInterface * transferObject();
    TransferPluginInfo *infoObject();
    QString pluginId() const;
    bool enabled() const ;

};

#endif // TELEPATHYSHAREPLUGIN_H
