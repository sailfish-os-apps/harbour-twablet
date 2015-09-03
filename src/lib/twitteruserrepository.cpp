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

#include "twitteruserrepository.h"
#include <QtCore/QJsonArray>

TwitterUser TwitterUserRepository::find(const QString &userId) const
{
    List::const_iterator it {std::find_if(std::begin(m_data), std::end(m_data), [userId](const TwitterUser &user) {
        return (user.userId() == userId);
    })};

    if (it != std::end(m_data)) {
        return *it;
    }
    return TwitterUser();
}

void TwitterUserRepository::load(const QJsonObject &json)
{
    const QJsonArray &usersArray {json.value(QLatin1String("users")).toArray()};

    List data;
    for (const QJsonValue &userValue : usersArray) {
        const QJsonObject &user {userValue.toObject()};
        const QString &name {user.value(QLatin1String("name")).toString()};
        const QString &userId {user.value(QLatin1String("userId")).toString()};
        const QString &screenName {user.value(QLatin1String("screenName")).toString()};
        const QByteArray &token {user.value(QLatin1String("token")).toString().toLocal8Bit()};
        const QByteArray &tokenSecret {user.value(QLatin1String("tokenSecret")).toString().toLocal8Bit()};
        if (!name.isEmpty() && !userId.isEmpty()) {
            data.emplace_back(name, userId, screenName, token, tokenSecret);
        }
    }
    m_data = std::move(data);
}

void TwitterUserRepository::save(QJsonObject &json) const
{
    QJsonArray users {};
    for (const TwitterUser &user : m_data) {
        QJsonObject userObject {};
        userObject.insert(QLatin1String("name"), user.name());
        userObject.insert(QLatin1String("userId"), user.userId());
        userObject.insert(QLatin1String("screenName"), user.screenName());
        userObject.insert(QLatin1String("token"), QString::fromLocal8Bit(user.token()));
        userObject.insert(QLatin1String("tokenSecret"), QString::fromLocal8Bit(user.tokenSecret()));
        users.append(userObject);
    }
    json.insert(QLatin1String("users"), users);
}
