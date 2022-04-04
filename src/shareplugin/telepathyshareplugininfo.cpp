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

#include <QDebug>
#include <QLocale>
#include "telepathyshareplugininfo.h"

TelepathySharePluginInfo::TelepathySharePluginInfo()
{
    qDebug() << "created plugin info";
}

TelepathySharePluginInfo::~TelepathySharePluginInfo()
{

}

QList<SharingMethodInfo> TelepathySharePluginInfo::info() const
{
    return m_infoList;
}

void TelepathySharePluginInfo::query()
{
    SharingMethodInfo info;
    QStringList capabilities;

    // Display name for plugin
    info.setDisplayName(qtTrId("Telepathy file transfer"));

    // Method ID is a unique identifier for this plugin. It is used to identify which share plugin should be
    // used for starting the sharing.
    info.setMethodId(QLatin1String("Telepathy-Share-Method-ID"));

    // Path to the Sharing UI which this plugin provides.
    info.setShareUIPath(QLatin1String("/usr/share/nemo-transferengine/plugins/sharing/ShareTelepathyList.qml"));

    info.setMethodIcon(QLatin1String("/usr/share/themes/sailfish-default/meegotouch/z1.75/icons/icon-m-service-xmpp.png"));

    // Pass information about supported mime types
    capabilities << QLatin1String("*");
    info.setCapabilities(capabilities);

    m_infoList << info;

    // Let the world know that this plugin is ready
    emit infoReady();
}
