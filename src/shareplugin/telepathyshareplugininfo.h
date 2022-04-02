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

#ifndef TELEPATHYSHAREPLUGININFO_H
#define TELEPATHYSHAREPLUGININFO_H

#include "sharingplugininfo.h"

class TelepathySharePluginInfo : public SharingPluginInfo
{
    Q_OBJECT
public:
    TelepathySharePluginInfo();
    ~TelepathySharePluginInfo();

    QList<SharingMethodInfo> info() const;
    void query();
private:
    QList<SharingMethodInfo> m_infoList;
};

#endif // TELEPATHYSHAREPLUGININFO_H
