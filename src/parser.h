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

#ifndef PARSER_H
#define PARSER_H

#include <QtCore/QList>
#include <QtCore/QPair>
#include <QtCore/QByteArray>

class LinkObject;
class CommentObject;
class SubredditObject;

namespace Parser
{
QList<LinkObject> parseLinkList(const QByteArray &json);

QList<CommentObject> parseCommentList(const QByteArray &json);
CommentObject parseNewComment(const QByteArray &json);

SubredditObject parseSubreddit(const QByteArray &json);
QList<SubredditObject> parseSubredditList(const QByteArray &json);

QList< QPair<QString, QString> > parseImgurImages(const QByteArray &json);
}

#endif // PARSER_H
