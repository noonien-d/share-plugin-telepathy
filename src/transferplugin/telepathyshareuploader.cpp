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

#include "telepathyshareuploader.h"
#include "mediaitem.h"

#include <QFile>
#include <QImage>
#include <QStandardPaths>
#include <QDir>
#include <QMimeDatabase>

#include <QDebug>

#include <telepathy-qt5/TelepathyQt/ClientRegistrar>
#include <telepathy-qt5/TelepathyQt/AbstractClient>
#include <telepathy-qt5/TelepathyQt/PendingChannelRequest>
#include <telepathy-qt5/TelepathyQt/FileTransferChannelCreationProperties>
#include <telepathy-qt5/TelepathyQt/AccountManager>
#include <telepathy-qt5/TelepathyQt/OutgoingFileTransferChannel>
#include <telepathy-qt5/TelepathyQt/PendingReady>
#include <telepathy-qt5/TelepathyQt/ConnectionFactory>
#include <telepathy-qt5/TelepathyQt/ChannelFactory>
#include <telepathy-qt5/TelepathyQt/ContactFactory>
#include <telepathy-qt5/TelepathyQt/Debug>

#include "telepathyshareclienthandler.h"

TelepathyShareUploader::TelepathyShareUploader (QObject *parent):
    MediaTransferInterface (parent)
{
    qDebug() << "created uploader";

    Tp::registerTypes();
    Tp::enableDebug(true);
    Tp::enableWarnings(true);
}

TelepathyShareUploader::~TelepathyShareUploader ()
{

}

QString TelepathyShareUploader::displayName () const
{
    return qtTrId("Telepathy File Transfer");
}

QUrl TelepathyShareUploader::serviceIcon () const
{
    // Url to the icon which should be shown in the transfer UI
    return QUrl::fromLocalFile("/usr/share/themes/sailfish-default/meegotouch/z1.75/icons/icon-m-service-xmpp.png");
}

bool TelepathyShareUploader::cancelEnabled () const
{
    // Return true if cancelling ongoing upload is supported
    // Return false if cancelling ongoing upload is not supported
    return true;
}

bool TelepathyShareUploader::restartEnabled () const
{
    // Return true, if restart is  supported.
    // Return false, if restart is not supported
    return false;
}


void TelepathyShareUploader::start ()
{
    // This is called by the sharing framework to start sharing
    qDebug() << "start to " << mediaItem()->value(MediaItem::UserData).toMap()["targetid"];

    mTargetId = mediaItem()->value(MediaItem::UserData).toMap()["targetid"].toString();
    mScaleImage = mediaItem()->value(MediaItem::UserData).toMap()["scaleimage"].toBool();
    mFilePath = mediaItem()->value(MediaItem::Url).toString().replace("file://", "");


    const QDBusConnection &dbus = QDBusConnection::sessionBus();
    if (mRegistrar.isNull())
        mRegistrar = Tp::ClientRegistrar::create(dbus);

    mHandler = new TelepathyShareClientHandler(this);
    mRegistrar->registerClient(Tp::AbstractClientPtr(mHandler), "telepathyshareplugin");

    Tp::ChannelFactoryPtr channelFactory = Tp::ChannelFactory::create(
        QDBusConnection::sessionBus());

    Tp::ConnectionFactoryPtr connectionFactory = Tp::ConnectionFactory::create(
        QDBusConnection::sessionBus(), Tp::Connection::FeatureConnected |
        Tp::Connection::FeatureRoster | Tp::Connection::FeatureRosterGroups);

    Tp::ContactFactoryPtr contactFactory = Tp::ContactFactory::create(
        Tp::Contact::FeatureAlias | Tp::Contact::FeatureSimplePresence);

    Tp::AccountFactoryPtr accountFactory = Tp::AccountFactory::create(QDBusConnection::sessionBus());

    mAccountMgr = Tp::AccountManager::create(QDBusConnection::sessionBus(),
            accountFactory,
            connectionFactory,
            channelFactory,
            contactFactory);

    connect(mAccountMgr->becomeReady(Tp::AccountManager::FeatureCore),
        SIGNAL(finished(Tp::PendingOperation *)),
        SLOT(onAccountManagerReady(Tp::PendingOperation *)));

     setStatus(MediaTransferInterface::TransferStarted);
}

void TelepathyShareUploader::cancel ()
{
    // This is called by the sharing framework to cancel on going transfer
    qDebug() << "cancel";
    mFileChannel->cancel ();
}

void TelepathyShareUploader::TelepathySendFile ()
{
    qDebug() << "FT Channel request ...";

    if (((mFilePath.toLower().contains(".jpg")) || (mFilePath.toLower().contains(".png"))) && (mScaleImage))
        ScaleImage();

    QMimeDatabase db;
    QMimeType type = db.mimeTypeForFile(mFilePath);
    qDebug() << "Mime type:" << type.name();

    Tp::FileTransferChannelCreationProperties req(mFilePath, type.name());
    mFileRequest = mAccount->createFileTransfer (mTargetId, req);

    connect(mFileRequest,
        &Tp::PendingChannelRequest::finished,
        this,
        &TelepathyShareUploader::onFiletransferChannelCreated);
}

void TelepathyShareUploader::ScaleImage ()
{
    QImage image (mFilePath);

    QDir dir(QStandardPaths::writableLocation(QStandardPaths::TempLocation));

    if (!dir.exists())
        dir.mkpath(".");

    QString localurl = dir.absolutePath() + QDir::separator() + QUrl(mFilePath).fileName();

    if (!QFile(localurl).exists())
    {
        int w = image.width();
        int h = image.height();

        if ((w > 1100) || (h > 1100))
        {
            if (w > h)
            {
                h = (float)h / (float)w * 1024.0;
                w = 1024;
            }
            else
            {
                w = (float)w / (float)h * 1024.0;
                h = 1024;
            }
        }

        QImage scaled = image.scaled (w, h, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
        scaled.save(localurl, 0, 90);
    }

    qDebug() << "Scaled image is " << localurl;
    mFilePath = localurl;
}

void TelepathyShareUploader::onFiletransferChannelCreated (Tp::PendingOperation *op)
{
    Q_UNUSED(op);

    if (!mFileRequest->isValid())
    {
        qWarning() << "FT Channel creation failed: " << mFileRequest->errorName () << "\n" << mFileRequest->errorMessage();
    }
    else
    {
        qDebug() << "FT Channel creation finished";

        mFileChannel =  Tp::OutgoingFileTransferChannel::create (mConnection,
                                                                mFileRequest->channelRequest()->channel()->objectPath(),
                                                                QVariantMap());

        connect (mFileChannel.data(), &Tp::OutgoingFileTransferChannel::stateChanged,
                 this, &TelepathyShareUploader::onFiletransferChannelStateChanged);

        Tp::Features feats;
        feats.insert(Tp::FileTransferChannel::FeatureCore);

        connect (mFileChannel->becomeReady(feats), &Tp::PendingReady::finished,
                 this, &TelepathyShareUploader::onFiletransferChannelReady);

        connect (mFileChannel.data(), &Tp::OutgoingFileTransferChannel::stateChanged,
                 this, &TelepathyShareUploader::onFiletransferChannelStateChanged);

        connect (mFileChannel.data(), &Tp::OutgoingFileTransferChannel::transferredBytesChanged,
                 this, &TelepathyShareUploader::onFiletransferChannelTransferredBytesChanged);
    }
}

void TelepathyShareUploader::onFiletransferChannelReady (Tp::PendingOperation *op)
{
    if (op->isValid())
    {
        qDebug() << "FT Channel ready";
        mFileChannel->provideFile(new QFile(mFilePath));
    }
    else
    {
        qWarning() << "FT Channel NOT ready";
    }
}

void TelepathyShareUploader::onFiletransferChannelClosed (Tp::PendingOperation *op)
{
    Q_UNUSED(op);

    qDebug() << "FT Channel closed";

    setStatus(MediaTransferInterface::TransferFinished);

    if (!mRegistrar.isNull())
    {
        mRegistrar->unregisterClient(Tp::AbstractClientPtr(mHandler));
        mHandler = NULL;
    }

}

void TelepathyShareUploader::onFiletransferChannelStateChanged (Tp::FileTransferState state, Tp::FileTransferStateChangeReason reason)
{
    QString str;

    switch (state)
    {
        case Tp::FileTransferStateNone:
            str = "None";
            break;
        case Tp::FileTransferStatePending:
            str = "Pending";
            break;
        case Tp::FileTransferStateAccepted:
            str = "Accepted";
            break;
        case Tp::FileTransferStateOpen:
            str = "Open";
                break;
        case Tp::FileTransferStateCompleted:
            str = "Completed";

            connect (mFileChannel->requestClose(), &Tp::PendingOperation::finished,
                     this, &TelepathyShareUploader::onFiletransferChannelClosed);

            break;
        case Tp::FileTransferStateCancelled:
            str = "Cancelled";
            setStatus(MediaTransferInterface::TransferInterrupted);
            break;
        default:
            str = "Invalid";
    }

    qDebug() << "FT Channel state changed: " << str << " (" << state << ") Reason: ("<< reason << ")";
}

void TelepathyShareUploader::onFiletransferChannelTransferredBytesChanged (qulonglong count)
{
    qDebug() << "FT Channel bytes changed: " << count << " Bytes";

    setProgress ((qreal)count / (qreal)QFile(mFilePath).size());
}

void TelepathyShareUploader::onAccountManagerReady(Tp::PendingOperation *op)
{
    if (!op->isValid())
    {
        qWarning() << "AccountManager NOT ready";
    }
    else
    {
        qDebug() << "AccountManager ready";

        foreach (Tp::AccountPtr a, mAccountMgr->allAccounts())
        {
            if (a->protocolName () == "jabber")
            {
                qDebug() << "Become ready: " << a->protocolName () << " : " << a->uniqueIdentifier ();

                mAccount = a;

                connect(mAccount->becomeReady(Tp::Account::FeatureCore),
                    &Tp::PendingReady::finished,
                    this,
                    &TelepathyShareUploader::onAccountReady);

                break;
            }
        }
    }
}

void TelepathyShareUploader::onAccountReady(Tp::PendingOperation *op)
{
    if (op->isError())
    {
        qWarning() << "Account cannot become ready - " << op->errorName() << '-' << op->errorMessage();
    }
    else
    {
        qDebug() << "Account ready";

        connect(mAccount.data(),
            &Tp::Account::connectionChanged,
            this,
            &TelepathyShareUploader::onConnectionChanged);

        if (mAccount->connection().isNull())
            qWarning() << "The account given has no Connection. Please set it online to continue.";
        else
            onConnectionChanged(mAccount->connection());
    }
}

void TelepathyShareUploader::onConnectionChanged(const Tp::ConnectionPtr &conn)
{
    qDebug() << "Connection changed";

    if (conn)
    {
        if (mConnection)
        {
            //unsetConnection();
        }
        mConnection = conn;

        TelepathySendFile();
    }
    else
    {
        mConnection.reset();
    }
}
