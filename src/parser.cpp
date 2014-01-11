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

#include "parser.h"

#include <QtCore/QDateTime>
#include <QtCore/QUrl>
#include <QtGui/QTextDocument>
#include <qt-json/json.h>

#include "linkobject.h"
#include "commentobject.h"
#include "subredditobject.h"

QString unescapeHtml(const QString &html)
{
    QTextDocument document;
    document.setHtml(html);
    QString unescaped = document.toPlainText();
    unescaped.remove("<!-- SC_OFF -->").remove("<!-- SC_ON -->");
    unescaped.remove("<div class=\"md\">").remove("</div>");
    return unescaped;
}

// reddit escape '<', '>' and '&' to '&lt;', '&gt;' and '&amp;' respectively for raw markdown text
QString unescapeMarkdown(QString markdown)
{
    markdown.replace("&lt;", "<");
    markdown.replace("&gt;", ">");
    markdown.replace("&amp;", "&");
    return markdown;
}

QList<LinkObject> Parser::parseLinkList(const QByteArray &json)
{
    bool ok;
    const QVariant root = QtJson::parse(QString::fromUtf8(json), ok);

    Q_ASSERT_X(ok, Q_FUNC_INFO, "Error parsing JSON");

    const QVariantList linkListJson = root.toMap().value("data").toMap().value("children").toList();

    QList<LinkObject> linkList;
    foreach (const QVariant &linkObjectJson, linkListJson) {
        LinkObject link;

        const QVariantMap linkMapJson = linkObjectJson.toMap().value("data").toMap();
        link.setFullname(linkMapJson.value("name").toString());
        link.setAuthor(linkMapJson.value("author").toString());
        link.setCreated(QDateTime::fromTime_t(linkMapJson.value("created_utc").toInt()));
        link.setSubreddit(linkMapJson.value("subreddit").toString());
        link.setScore(linkMapJson.value("score").toInt());
        if (!linkMapJson.value("likes").isNull())
            link.setLikes(linkMapJson.value("likes").toBool() ? 1 : -1);
        link.setCommentsCount(linkMapJson.value("num_comments").toInt());
        link.setTitle(linkMapJson.value("title").toString());
        link.setDomain(linkMapJson.value("domain").toString());

        QString thumbnail = linkMapJson.value("thumbnail").toString();
        if (thumbnail.startsWith("http"))
            link.setThumbnailUrl(QUrl(thumbnail));
        link.setText(unescapeHtml(linkMapJson.value("selftext_html").toString()));
        link.setRawText(unescapeMarkdown(linkMapJson.value("selftext").toString()));
        link.setPermalink(linkMapJson.value("permalink").toString());
        link.setUrl(QUrl(linkMapJson.value("url").toString()));
        link.setDistinguished(linkMapJson.value("distinguished").toString());
        link.setSticky(linkMapJson.value("stickied").toBool());
        link.setNSFW(linkMapJson.value("over_18").toBool());

        linkList.append(link);
    }

    return linkList;
}

// Private
QList<CommentObject> parseCommentListingJson(const QVariantMap &json, const QString &linkAuthor, int depth)
{
    Q_ASSERT(json.value("kind") == "Listing");

    QList<CommentObject> commentList;

    const QVariantList childrenList = json.value("data").toMap().value("children").toList();
    foreach (const QVariant &commentJson, childrenList) {
        if (commentJson.toMap().value("kind") != "t1")
            continue;

        const QVariantMap commentMap = commentJson.toMap().value("data").toMap();

        CommentObject comment;
        comment.setFullname(commentMap.value("name").toString());
        comment.setAuthor(commentMap.value("author").toString());
        comment.setBody(unescapeHtml(commentMap.value("body_html").toString()));
        comment.setRawBody(unescapeMarkdown(commentMap.value("body").toString()));
        int upvotes = commentMap.value("ups").toInt();
        int downvotes = commentMap.value("downs").toInt();
        comment.setScore(upvotes - downvotes);
        if (!commentMap.value("likes").isNull())
            comment.setLikes(commentMap.value("likes").toBool() ? 1 : -1);
        comment.setCreated(QDateTime::fromTime_t(commentMap.value("created_utc").toInt()));
        if (commentMap.value("edited").toBool() != false)
            comment.setEdited(QDateTime::fromTime_t(commentMap.value("edited").toInt()));
        comment.setDistinguished(commentMap.value("distinguished").toString());
        comment.setDepth(depth);
        comment.setSubmitter(comment.author() == linkAuthor);
        comment.setScoreHidden(commentMap.value("score_hidden").toBool());

        commentList.append(comment);

        if (commentMap.value("replies").type() == QVariant::Map)
            commentList.append(parseCommentListingJson(commentMap.value("replies").toMap(), linkAuthor, depth + 1));
    }

    return commentList;
}

QList<CommentObject> Parser::parseCommentList(const QByteArray &json)
{
    bool ok;
    const QVariantList root = QtJson::parse(QString::fromUtf8(json), ok).toList();

    Q_ASSERT_X(ok, Q_FUNC_INFO, "Error parsing JSON");

    // Longest. Expression. Ever.
    const QString linkAuthor = root.first().toMap().value("data").toMap()
            .value("children").toList().first().toMap().value("data").toMap()
            .value("author").toString();

    return parseCommentListingJson(root.last().toMap(), linkAuthor, 0);
}

CommentObject Parser::parseNewComment(const QByteArray &json)
{
    bool ok;
    const QVariant root = QtJson::parse(QString::fromUtf8(json), ok);

    Q_ASSERT_X(ok, Q_FUNC_INFO, "Error parsing JSON");

    const QVariantMap commentMap = root.toMap().value("json").toMap().value("data").toMap()
            .value("things").toList().first().toMap().value("data").toMap();

    CommentObject comment;
    comment.setFullname(commentMap.value("name").toString());
    comment.setAuthor(commentMap.value("author").toString());
    comment.setBody(unescapeHtml(commentMap.value("body_html").toString()));
    comment.setRawBody(unescapeMarkdown(commentMap.value("body").toString()));
    int upvotes = commentMap.value("ups").toInt();
    int downvotes = commentMap.value("downs").toInt();
    comment.setScore(upvotes - downvotes);
    if (!commentMap.value("likes").isNull())
        comment.setLikes(commentMap.value("likes").toBool() ? 1 : -1);
    comment.setCreated(QDateTime::fromTime_t(commentMap.value("created_utc").toInt()));
    if (commentMap.value("edited").toBool() != false)
        comment.setEdited(QDateTime::fromTime_t(commentMap.value("edited").toInt()));
    comment.setDistinguished(commentMap.value("distinguished").toString());
    comment.setScoreHidden(commentMap.value("score_hidden").toBool());

    return comment;
}

// Private
SubredditObject parseSubredditThing(const QVariantMap &subredditThing)
{
    Q_ASSERT(subredditThing.value("kind").toString() == "t5");

    const QVariantMap data = subredditThing.value("data").toMap();

    SubredditObject subreddit;
    subreddit.setFullname(data.value("name").toString());
    subreddit.setDisplayName(data.value("display_name").toString());
    subreddit.setUrl(data.value("url").toString());
    subreddit.setHeaderImageUrl(QUrl(data.value("header_img").toString()));
    subreddit.setShortDescription(data.value("public_description").toString());
    subreddit.setLongDescription(unescapeHtml(data.value("description_html").toString()));
    subreddit.setSubscribers(data.value("subscribers").toInt());
    subreddit.setActiveUsers(data.value("accounts_active").toInt());
    subreddit.setNSFW(data.value("over18").toBool());
    subreddit.setSubscribed(data.value("user_is_subscriber").toBool());
    return subreddit;
}

SubredditObject Parser::parseSubreddit(const QByteArray &json)
{
    bool ok;
    const QVariant root = QtJson::parse(json, ok);

    Q_ASSERT_X(ok, Q_FUNC_INFO, "Error parsing JSON");

    return parseSubredditThing(root.toMap());
}

QList<SubredditObject> Parser::parseSubredditList(const QByteArray &json)
{
    bool ok;
    const QVariant root = QtJson::parse(json, ok);

    Q_ASSERT_X(ok, Q_FUNC_INFO, "Error parsing JSON");

    const QVariantList subredditListJson = root.toMap().value("data").toMap().value("children").toList();

    QList<SubredditObject> subredditList;
    foreach (const QVariant &subredditJson, subredditListJson) {
        subredditList.append(parseSubredditThing(subredditJson.toMap()));
    }

    return subredditList;
}

// Private
QPair<QString, QString> parseImgurImageMap(const QVariantMap &imageMap)
{
    const QString imageUrl = imageMap.value("link").toString();
    int insertThumbIndex = imageUrl.lastIndexOf('.');

    QString mainUrl;
    if (imageMap.value("animated").toBool())
        mainUrl = imageUrl;
    else
        mainUrl = QString(imageUrl).insert(insertThumbIndex, 'h');

    QString thumbUrl = QString(imageUrl).insert(insertThumbIndex, 'b');

    return qMakePair(mainUrl, thumbUrl);
}

QList< QPair<QString, QString> > Parser::parseImgurImages(const QByteArray &json)
{
    bool ok;
    const QVariant root = QtJson::parse(json, ok);

    Q_ASSERT_X(ok, Q_FUNC_INFO, "Error parsing JSON");

    QList< QPair<QString, QString> > imageAndThumbUrlList;

    const QVariant data = root.toMap().value("data");

    if (data.type() == QVariant::Map) {
        imageAndThumbUrlList.append(parseImgurImageMap(data.toMap()));
    } else if (data.type() == QVariant::List) {
        foreach (const QVariant &imageJson, data.toList()) {
            imageAndThumbUrlList.append(parseImgurImageMap(imageJson.toMap()));
        }
    } else {
        qCritical("Parser::parseImgurImages(): Invalid JSON");
    }

    return imageAndThumbUrlList;
}
