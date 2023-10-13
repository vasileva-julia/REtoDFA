#ifndef EDGE_H
#define EDGE_H

#include<QGraphicsItem>
#include "node.h"
#include "math.h"


class Edge : public QGraphicsItem {
public:
    Edge(Node* sourceNode, QString label);

    Node* sourceNode() const;

    virtual void adjust() = 0;
    QRectF boundingRect() const;

    enum { Type = UserType + 2 };
    virtual int type() const override;

    virtual bool isLoopEdge() const = 0;
    void addToLabel(QString str);

protected:
    QPolygonF makeArrowPolygon(QPointF point, qreal angle) const;
    void paintEdge(QPainter* painter);

    Node* source;
    const qreal nodeRadius;

    static const QFont font; // Font for the label.
    QString label;
    QPointF labelPoint; // Beginning of the label.

    QPainterPath path; // Path for drawing line or arc of the edge.
    QRectF boundingRectangle;

    QPolygonF arrowPolygon;
    static const qreal arrowSize;
    static const qreal arrowAngle;
};

#endif // EDGE_H
