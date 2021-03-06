/*
    Quickddit - Reddit client for mobile phones
    Copyright (C) 2015-2017  Sander van Grieken

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

import QtQuick 2.0
import Sailfish.Silica 1.0
import harbour.quickddit.Core 1.0

AbstractPage {
    id: newLinkPage
    title: editPost === "" ? qsTr("New Post") : qsTr("Edit Post")

    property string subreddit
    property string editPost: ""
    property alias text: linkDescription.text
    property alias postTitle: linkTitle.text
    property QtObject linkManager

    function submit() {
        if (editPost === "") {
            console.log("submitting post...");
            linkManager.submit(subreddit, "", "", linkTitle.text, selfLinkSwitch.checked ? "" : linkUrl.text, linkDescription.text);
        } else {
            console.log("saving post...");
            linkManager.editLinkText(editPost, text);
        }
    }

    Flickable {
        id: flickable
        anchors.fill: parent
        contentHeight: mainContentColumn.height

        Column {
            id: mainContentColumn
            width: parent.width
            spacing: constant.paddingMedium

            QuickdditPageHeader { title: newLinkPage.title }

            Label {
                anchors {right: parent.right; rightMargin: Theme.paddingLarge }
                text: "/r/" + subreddit
                font.pixelSize: constant.fontSizeXSmall
                color: Theme.highlightColor
            }

            TextField {
                id: linkTitle
                anchors { left: parent.left; right: parent.right }
                placeholderText: qsTr("Post Title")
                labelVisible: false
                enabled: editPost === ""
            }

            TextSwitch {
                id: selfLinkSwitch
                visible: aboutSubredditManager.submissionType === AboutSubredditManager.Any && editPost === ""
                text: qsTr("Self Post")
                checked: true
            }

            TextField {
                id: linkUrl
                anchors { left: parent.left; right: parent.right }
                placeholderText: qsTr("Post URL")
                enabled: !selfLinkSwitch.checked
                visible: enabled
                labelVisible: false
                inputMethodHints: Qt.ImhNoAutoUppercase | Qt.ImhNoPredictiveText
                validator: RegExpValidator { regExp: /^https?:\/\/.+/ }
                errorHighlight: activeFocus && !acceptableInput
            }

            TextArea {
                id: linkDescription
                anchors { left: parent.left; right: parent.right }
                placeholderText: qsTr("Post Text")
                enabled: selfLinkSwitch.checked
                visible: enabled
                height: Math.max(implicitHeight, Theme.itemSizeLarge * 3)
            }

            Button {
                text: editPost === "" ? qsTr("Submit") : qsTr("Save")
                anchors.horizontalCenter: parent.horizontalCenter
                enabled: (editPost != "" || linkTitle.text.length > 0) /* official limits? */
                         && ((selfLinkSwitch.checked && linkDescription.text.length > 0) || linkUrl.acceptableInput)
                onClicked: submit()
            }
        }
    }

    AboutSubredditManager {
        id: aboutSubredditManager
        manager: quickdditManager
        subreddit: newLinkPage.subreddit
        onError: infoBanner.warning(errorString)
        onDataChanged: {
            if (submissionType === AboutSubredditManager.Link)
                selfLinkSwitch.checked = false
        }
    }

    Component.onCompleted: aboutSubredditManager.refresh();
}
