#ifndef COMMENTMODEL_H
#define COMMENTMODEL_H

#include <QtCore/QAbstractListModel>

#include "commentobject.h"

class CommentModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum Roles {
        FullnameRole = Qt::UserRole,
        AuthorRole,
        BodyRole,
        ScoreRole,
        LikesRole,
        CreatedRole,
        DepthRole,
        IsScoreHiddenRole
    };

    explicit CommentModel(QObject *parent = 0);

    int rowCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;

    int count() const;
    QString lastFullName() const;

    void append(const QList<CommentObject> &commentList);
    void clear();

    Q_INVOKABLE int getParentIndex(int index) const;

private:
    QList<CommentObject> m_commentList;
};

#endif // COMMENTMODEL_H
