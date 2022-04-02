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

#include "telepathyshareplugin.h"
#include "telepathyshareplugininfo.h"
#include <QtPlugin>

TelepathySharePlugin::TelepathySharePlugin()
{
}

TelepathySharePlugin::~TelepathySharePlugin()
{
}

QString TelepathySharePlugin::pluginId() const
{
    return "Telepathy-Share-Method-ID";
}

SharingPluginInfo* TelepathySharePlugin::infoObject()
{
    return new TelepathySharePluginInfo;
}
