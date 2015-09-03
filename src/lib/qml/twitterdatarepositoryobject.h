/*
 * Copyright (C) 2014 Lucien XU <sfietkonstantin@free.fr>
 *
 * You may use this file under the terms of the BSD license as follows:
 *
 * "Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in
 *     the documentation and/or other materials provided with the
 *     distribution.
 *   * The names of its contributors may not be used to endorse or promote
 *     products derived from this software without specific prior written
 *     permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
 */

#ifndef TWITTERDATAREPOSITORYOBJECT_H
#define TWITTERDATAREPOSITORYOBJECT_H

#include <QtCore/QObject>
#include "loadsavemanager.h"
#include "twitteruserrepository.h"
#include "layoutrepository.h"
#include "twittertweetcentralrepository.h"

class TwitterDataRepositoryObject : public QObject
{
    Q_OBJECT
public:
    explicit TwitterDataRepositoryObject(QObject *parent = 0);
    TwitterUserRepository & users();
    LayoutRepository & layouts();
    TwitterTweetRepository & tweets(const Layout &layout);
public slots:
    // Users
    void addUser(const QString &name, const QString &userId, const QString &screenName,
                 const QString &token, const QString &tokenSecret);
    void updateUserName(int index, const QString &name);
    void removeUser(int index);
    // Layouts
    void addLayout(const QString &name, int userIndex, int queryType, const QVariantMap &arguments);
    void updateLayoutName(int index, const QString &name);
    void updateLayoutUnread(int index, int unread);
    void removeLayout(int index);
    void refresh();
private:
    class LayoutComparator
    {
    public:
        bool operator()(const Layout &first, const Layout &second) const;
    };
    LoadSaveManager m_loadSaveManager {};
    TwitterUserRepository m_users {};
    LayoutRepository m_layouts {};
    TwitterTweetCentralRepository m_tweetsCentralRepository {};
    std::map<Layout, TwitterTweetRepository, LayoutComparator> m_tweetRepositories {};
};

#endif // TWITTERDATAREPOSITORYOBJECT_H