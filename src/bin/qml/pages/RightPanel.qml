/*
 * Copyright (C) 2015 Lucien XU <sfietkonstantin@free.fr>
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

import QtQuick 2.0
import Sailfish.Silica 1.0
import harbour.twablet 1.0

Item {
    id: container
    property bool open: false
    property Page mainPage
    anchors.top: parent.top; anchors.bottom: parent.bottom
    visible: false

    function openUser(id, accountUserId, pushMode) {
        var params = {
            userId: id,
            accountUserId: accountUserId,
            panel: container
        }

        var page = _open(Qt.resolvedUrl("UserPage.qml"), params, pushMode)
        page.load()
    }
    function openSearch(query, accountUserId, pushMode) {
        var parameters = {q: query, result_type: "recent"}
        var params = {
            title: query,
            type: QueryType.Search,
            parameters: parameters,
            accountUserId: accountUserId,
            panel: container
        }

        _open(Qt.resolvedUrl("TweetsPage.qml"), params, pushMode)
    }
    function openFavorites(userId, screenName, accountUserId, pushMode) {
        var parameters = {user_id: userId}
        var params = {
            title: qsTr("%1's favorites").arg(screenName),
            type: QueryType.Favorites,
            parameters: parameters,
            accountUserId: accountUserId,
            panel: container
        }

        _open(Qt.resolvedUrl("TweetsPage.qml"), params, pushMode)
    }
    function openUserTimeline(userId, screenName, accountUserId, pushMode) {
        var parameters = {user_id: userId}
        var params = {
            title: qsTr("Tweets from %1").arg(screenName),
            type: QueryType.UserTimeline,
            parameters: parameters,
            accountUserId: accountUserId,
            panel: container
        }

        _open(Qt.resolvedUrl("TweetsPage.qml"), params, pushMode)
    }

    function openFriends(userId, screenName, accountUserId, pushMode) {
        var parameters = {"user_id": userId}
        var params = {
            title: qsTr("%1 is following").arg(screenName),
            type: QueryType.Friends,
            parameters: parameters,
            accountUserId: accountUserId,
            panel: container
        }

        _open(Qt.resolvedUrl("UsersPage.qml"), params, pushMode)
    }

    function openFollowers(userId, screenName, accountUserId, pushMode) {
        var parameters = {"user_id": userId}
        var params = {
            title: qsTr("%1's followers").arg(screenName),
            type: QueryType.Followers,
            parameters: parameters,
            accountUserId: accountUserId,
            panel: container
        }

        _open(Qt.resolvedUrl("UsersPage.qml"), params, pushMode)
    }

    function openTweet(tweetId, retweetId, accountUserId, pushMode) {
        var params = {
            tweetId: tweetId,
            retweetId: retweetId,
            accountUserId: accountUserId,
            panel: container
        }
        var page = _open(Qt.resolvedUrl("TweetPage.qml"), params, pushMode)
        page.load()
    }

    function openSubscribedLists(userId, screenName, accountUserId, pushMode) {
        var parameters = {user_id: userId}
        var params = {
            title: qsTr("%1's subscribed lists").arg(screenName),
            type: QueryType.Subscriptions,
            parameters: parameters,
            accountUserId: accountUserId,
            panel: container
        }
        _open(Qt.resolvedUrl("ListPage.qml"), params, pushMode)
    }

    function openListedLists(userId, screenName, accountUserId, pushMode) {
        var parameters = {user_id: userId}
        var params = {
            title: qsTr("%1 listed in").arg(screenName),
            type: QueryType.Memberships,
            parameters: parameters,
            accountUserId: accountUserId,
            panel: container
        }
        _open(Qt.resolvedUrl("ListPage.qml"), params, pushMode)
    }

    function openImageBrowser(tweet, accountUserId) {
        var params = {
            tweet: tweet,
            accountUserId: accountUserId,
            panel: container
        }

        pageStack.push(Qt.resolvedUrl("ImageBrowser.qml"), params)
    }

    function _open(page, args, pushMode) {
        if (typeof(pushMode) === 'undefined') {
            pushMode = Info.Push
        }

        if (Screen.sizeCategory === Screen.Large) {
            if (pushMode === Info.Clear) {
                panelPageStack.clear()
            }
            container.open = true
            args.width = container.width
            args.height = container.height
            args.orientation = Orientation.Portrait
            args.allowedOrientations = Orientation.Portrait
            if (pushMode === Info.Replace) {
                pageStack.pop(mainPage)
            }
            return panelPageStack.push(page, args)
        } else {
            if (pushMode === Info.Replace) {
                return pageStack.replace(page, args)
            } else {
                return pageStack.push(page, args)
            }
        }
    }

    // Background
    Rectangle {
        gradient: Gradient {
            GradientStop { position: 0.0; color: Theme.rgba(Theme.highlightColor, 0.15) }
            GradientStop { position: 1.0; color: Theme.rgba(Theme.highlightColor, 0.3) }
        }
    }

    MouseArea {
        id: mouse
        property bool moving: mouse.drag.active
        property real xMin: container.parent.width - container.width
        property real xMax: container.parent.width
        property int threshold: Theme.itemSizeSmall
        enabled: panelPageStack.depth == 1
        function close(active) {
            var delta = container.x - mouse.xMin
            if (!active && delta >  threshold) {
                container.open = false
            }
        }
        anchors.fill: parent
        drag {
            target: container
            minimumX: mouse.xMin
            maximumX: mouse.xMax
            axis: Drag.XAxis
            filterChildren: true
            onActiveChanged: mouse.close(drag.active)
        }

        MouseArea {
            anchors.horizontalCenter: parent.left
            anchors.top: parent.top; anchors.bottom: parent.bottom
            width: Theme.itemSizeSmall

            Rectangle {
                anchors.top: parent.top; anchors.bottom: parent.bottom
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.horizontalCenterOffset: -width / 2
                width: Theme.paddingSmall
                color: Theme.highlightColor
            }
            drag {
                target: container
                minimumX: mouse.xMin
                maximumX: mouse.xMax
                axis: Drag.XAxis
                filterChildren: true
                onActiveChanged: mouse.close(drag.active)
            }
        }


        PageStack {
            id: panelPageStack
            anchors.fill: parent
            clip: true

            function _overridePageIndicator() {
                if (_pageStackIndicator && _pageStackIndicator.parent != panelPageStack) {
                    _pageStackIndicator.parent = panelPageStack
                }
            }

            onBackNavigationChanged: {
                if (backNavigation) {
                    _overridePageIndicator()
                }
            }
            onForwardNavigationChanged: {
                if (forwardNavigation) {
                    _overridePageIndicator()
                }
            }
        }
    }

    state: "closed"
    states: [
        State {
            name: "closed"; when: !container.open
            PropertyChanges {
                target: container
                x: mouse.xMax
            }
        },
        State {
            name: "open"; when: container.open
            PropertyChanges {
                target: container
                x: mouse.xMin
            }
        }
    ]
    transitions: [
        Transition {
            to: "open"
            SequentialAnimation {
                PropertyAction {
                    target: container
                    property: "visible"
                    value: true
                }
                NumberAnimation {
                    target: container
                    property: "x"
                    duration: 300; easing.type: Easing.OutQuad
                }
            }
        },
        Transition {
            to: "closed"
            SequentialAnimation {
                NumberAnimation {
                    target: container
                    property: "x"
                    duration: 300; easing.type: Easing.OutQuad
                }
                PropertyAction {
                    target: container
                    property: "visible"
                    value: false
                }
                ScriptAction {
                    script: panelPageStack.clear()
                }
            }
        }
    ]
}
