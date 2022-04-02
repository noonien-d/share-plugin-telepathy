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

#ifndef TELEPATHYSHAREUPLOADER_H
#define TELEPATHYSHAREUPLOADER_H

#include "mediatransferinterface.h"
#include <telepathy-qt5/TelepathyQt/Types>
#include <telepathy-qt5/TelepathyQt/OutgoingFileTransferChannel>

namespace Tp {
    class PendingOperation;
    class Connection;
    class PendingChannelRequest;
}

class TelepathyShareClientHandler;

class TelepathyShareUploader : public MediaTransferInterface
{
    Q_OBJECT
public:
    TelepathyShareUploader (QObject *parent = 0);
    ~TelepathyShareUploader ();

    QString displayName () const;
    QUrl serviceIcon () const;
    bool cancelEnabled () const;
    bool restartEnabled () const;

public Q_SLOTS:
    void start();
    void cancel();
    void onAccountReady(Tp::PendingOperation *op);
private:
    void TelepathySendFile ();
    void onFiletransferChannelCreated (Tp::PendingOperation *op);
    void onFiletransferChannelReady (Tp::PendingOperation *op);
    void onFiletransferChannelStateChanged (Tp::FileTransferState state, Tp::FileTransferStateChangeReason reason);
    void onFiletransferChannelTransferredBytesChanged (qulonglong count);
    void onFiletransferChannelClosed (Tp::PendingOperation *op);
    void ScaleImage();

    Tp::AccountManagerPtr mAccountMgr;
    Tp::AccountPtr mAccount;
    Tp::ConnectionPtr mConnection;
    Tp::PendingChannelRequest *mFileRequest;
    Tp::OutgoingFileTransferChannelPtr mFileChannel;
    TelepathyShareClientHandler *mHandler;
    Tp::ClientRegistrarPtr mRegistrar;

    QString mFilePath;
    QString mTargetId;
    bool mScaleImage;

private slots:
    void onAccountManagerReady(Tp::PendingOperation *op);
    void onConnectionChanged(const Tp::ConnectionPtr &conn);

};

#endif // TELEPATHYSHAREUPLOADER_H
