/*
    Quickddit - Reddit client for mobile phones
    Copyright (C) 2014  Dickson Leong

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

AbstractPage {
    id: aboutPage
    title: "About Quickddit"

    PageHeader {
        anchors { top: parent.top; right: parent.right; left: parent.left }
        title: aboutPage.title
    }

    Text {
        anchors.fill: parent
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        font.pixelSize: constant.fontSizeLarge
        color: constant.colorLight
        wrapMode: Text.Wrap
        text: "Copyright (c) Dickson Leong\nv" + APP_VERSION + " BETA\nApp icon by @andrewzhilin\n" +
              "Licensed under GNU GPLv3+"
    }
}
