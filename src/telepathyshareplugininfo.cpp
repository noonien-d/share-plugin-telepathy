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
    : m_ready(false)
{
    qDebug() << "created plugin info";
}

TelepathySharePluginInfo::~TelepathySharePluginInfo()
{

}

QList<TransferMethodInfo> TelepathySharePluginInfo::info() const
{
    return m_infoList;
}

void TelepathySharePluginInfo::query()
{
    TransferMethodInfo info;

    // Display name for plugin
    info.displayName     = qtTrId("Telepathy file transfer");

    // Method ID is a unique identifier for this plugin. It is used to identify which share plugin should be
    // used for starting the sharing.
    info.methodId        = QLatin1String("share-plugin-telepathy");

    // Path to the Sharing UI which this plugin provides.
    info.shareUIPath     = QLatin1String("/usr/share/nemo-transferengine/plugins/ShareTelepathyList.qml");

    // Pass information about supported mime types
    info.capabilitities    << QLatin1String("text/x-url")
                           << QLatin1String("text/vcard")
                           << QLatin1String("audio")
                           << QLatin1String("image/jpeg")
                           << QLatin1String("image/png");


    m_infoList << info;

    // Let the world know that this plugin is ready
    m_ready = true;
    emit infoReady();
}

bool TelepathySharePluginInfo::ready() const
{
    return m_ready;
}
