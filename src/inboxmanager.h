/*
    Quickddit - Reddit client for mobile phones
    Copyright (C) 2015  Sander van Grieken

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see [http://www.gnu.org/licenses/].
*/

#ifndef INBOXMANAGER_H
#define INBOXMANAGER_H

#include <QObject>
#include <QNetworkReply>
#include <QTimer>

#include "abstractmanager.h"
#include "messageobject.h"
#include "parser.h"

class InboxManager : public AbstractManager
{
    Q_OBJECT
    Q_PROPERTY(bool hasUnread READ hasUnread NOTIFY hasUnreadChanged)
    Q_PROPERTY(bool enabled READ enabled WRITE setEnabled NOTIFY enabledChanged)
public:
    explicit InboxManager(QObject *parent = 0);

    bool hasUnread();

    bool enabled();
    void setEnabled(bool enabled);

    Q_INVOKABLE void request();
    Q_INVOKABLE void resetTimer();

signals:
    void hasUnreadChanged(bool hasUnread);
    void newUnread(int unreadSinceLastSeen, const QVariantList &messages);
    void enabledChanged(bool enabled);

    void error(const QString& error);

public slots:

private slots:
    void onUnreadReceived(QNetworkReply *reply);
    void onInboxReceived(QNetworkReply *reply);
    void pollTimeout();

private:
    bool m_hasUnread;
    bool m_enabled;
    QTimer* m_pollTimer;

    void setHasUnread(bool hasUnread);

    void filterInbox(Listing<MessageObject> messages);
    void checkNewUnread();
};

#endif // INBOXMANAGER_H
