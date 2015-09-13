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

#include "mentionstimelinequeryhandler.h"
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonArray>
#include <QtCore/QJsonObject>
#include "tweet.h"

MentionsTimelineQueryHandler::MentionsTimelineQueryHandler()
{
}

void MentionsTimelineQueryHandler::createRequest(QString &path, std::map<QString, QString> &parameters) const
{
    path = QLatin1String("statuses/mentions_timeline.json");
    parameters.insert({QLatin1String("count"), QString::number(200)});
    parameters.insert({QLatin1String("trim_user"), QLatin1String("false")});
    parameters.insert({QLatin1String("include_entities"), QLatin1String("true")});
    if (!m_sinceId.isEmpty()) {
        parameters.insert({QLatin1String("since_id"), m_sinceId});
    }
}

bool MentionsTimelineQueryHandler::treatReply(const QByteArray &data, std::vector<Tweet> &items,
                                              QString &errorMessage, Placement &placement)
{
    QJsonParseError error {-1, QJsonParseError::NoError};
    QJsonDocument document {QJsonDocument::fromJson(data, &error)};
    if (error.error != QJsonParseError::NoError) {
        errorMessage = error.errorString();
        placement = Discard;
        return false;
    }

    QJsonArray tweets (document.array());
    items.reserve(tweets.size());
    for (const QJsonValue &tweet : tweets) {
        if (tweet.isObject()) {
            items.emplace_back(tweet.toObject());
        }
    }

    if (!items.empty()) {
        m_sinceId = std::begin(items)->id();
    }

    placement = Prepend;
    return true;
}
