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

#ifndef TELEPATHYSHARECLIENTHANDLER_H
#define TELEPATHYSHARECLIENTHANDLER_H

#include "telepathyshareuploader.h"
#include <telepathy-qt5/TelepathyQt/AbstractClientHandler>
#include <telepathy-qt5/TelepathyQt/ClientRegistrar>
#include <telepathy-qt5/TelepathyQt/Types>

class TelepathyShareClientHandler : public QObject, public Tp::AbstractClientHandler
{
    Q_OBJECT
public:
    explicit TelepathyShareClientHandler (TelepathyShareUploader* uploader, QObject *parent = 0);
    ~TelepathyShareClientHandler();

    virtual bool bypassApproval() const;
    virtual void handleChannels(const Tp::MethodInvocationContextPtr<> &context, const Tp::AccountPtr &account,
                                const Tp::ConnectionPtr &connection, const QList<Tp::ChannelPtr> &channels,
                                const QList<Tp::ChannelRequestPtr> &requestsSatisfied, const QDateTime &userActionTime,
                                const HandlerInfo &handlerInfo);

private Q_SLOTS:
    void onChannelReady(Tp::PendingOperation *op);

private:
    Tp::ChannelPtr mChannel;
    TelepathyShareUploader *mUploader;
};

#endif // TELEPATHYSHARECLIENTHANDLER_H
