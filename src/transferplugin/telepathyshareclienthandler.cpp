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

#include "telepathyshareclienthandler.h"

#include <telepathy-qt5/TelepathyQt/ChannelClassSpec>
#include <telepathy-qt5/TelepathyQt/Channel>
#include <telepathy-qt5/TelepathyQt/Account>
#include <telepathy-qt5/TelepathyQt/PendingOperation>
#include <telepathy-qt5/TelepathyQt/PendingReady>
#include <telepathy-qt5/TelepathyQt/PendingChannelRequest>
#include <telepathy-qt5/TelepathyQt/AbstractClientHandler>
#include <telepathy-qt5/TelepathyQt/Features>
#include <telepathy-qt5/TelepathyQt/Types>
#include <QDebug>

TelepathyShareClientHandler::TelepathyShareClientHandler(TelepathyShareUploader *uploader, QObject *parent) : QObject (parent), Tp::AbstractClientHandler(Tp::ChannelClassSpecList() << Tp::ChannelClassSpec::outgoingFileTransfer())
{
    qDebug() << "Created Handler";

    mUploader = uploader;
}

TelepathyShareClientHandler::~TelepathyShareClientHandler ()
{
    qDebug() << "Destroy Handler";
}

bool TelepathyShareClientHandler::bypassApproval() const
{
    return true;
}

void TelepathyShareClientHandler::handleChannels(const Tp::MethodInvocationContextPtr<> &context, const Tp::AccountPtr &account,
                                     const Tp::ConnectionPtr &connection, const QList<Tp::ChannelPtr> &channels,
                                     const QList<Tp::ChannelRequestPtr> &requestsSatisfied, const QDateTime &userActionTime,
                                     const HandlerInfo &handlerInfo)
{
    qDebug() << "Incoming channel...";

    Q_UNUSED(connection);
    Q_UNUSED(requestsSatisfied);
    Q_UNUSED(userActionTime);
    Q_UNUSED(handlerInfo);

    foreach (const Tp::ChannelPtr &channel, channels) {
        QVariantMap properties = channel->immutableProperties();
        QString targetId = properties.value(TP_QT_IFACE_CHANNEL + QLatin1String(".TargetID")).toString();
        QString localId = account->objectPath();

        if (targetId.isEmpty()) {
            qWarning() << "handleChannels cannot get TargetID for channel";
            continue;
        }

        qDebug() << "Handle channel";

        Tp::Features feats;
        feats.insert(Tp::Channel::FeatureCore);

        mChannel = channel;

        connect(channel->becomeReady(feats), &Tp::PendingReady::finished,
                this, &TelepathyShareClientHandler::onChannelReady);
    }

    context->setFinished();
}

void TelepathyShareClientHandler::onChannelReady(Tp::PendingOperation *op)
{
    if (!op->isValid())
        qDebug() << "Channel NOT ready";
    else
    {
        qDebug() << "Channel ready ID:" << mChannel->targetId() << "type: " << mChannel->channelType();

        qDebug() << "Unhandled channel of type " + mChannel->channelType();

    }
}
