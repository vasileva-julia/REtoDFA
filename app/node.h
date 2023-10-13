#ifndef NODE_H
#define NODE_H

#include <QGraphicsItem>
#include <QList>
#include <QGraphicsScene>


class Edge;

class Node : public QGraphicsItem {
public:
    Node(QString name);
    qreal getRadius();

    void addEdge(Edge* edge);
    Edge* addEdge(Node* dest, QString label);
    QList<Edge*> edges() const;

    enum { Type = UserType + 1 };
    int type() const override;

    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;

    void makeFinal();
    bool isFinal();
    void makeInitial();
    bool isInitial();

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;

    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;

private:
    QList<Edge*> edgeList;
    static const qreal radius;

    QString name;
    static const QFont font;

    bool final = false;
    bool initial = false;
};

#endif // NODE_H
