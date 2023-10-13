#ifndef SINGLEEDGE_H
#define SINGLEEDGE_H

#include "node.h"
#include "edge.h"
#include "math.h"


class SingleEdge : public Edge { // Edge from from one node to another.
public:
    SingleEdge(Node* sourceNode, Node* destNode, QString label);

    Node* destNode() const;

    void adjust() override;

    enum { Type = UserType + 3 };
    int type() const override;

    bool isLoopEdge() const override;
    void setAngle(double newAngle);

protected:
    void paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*) override;

private:
    Node* dest;
    QPointF sourcePoint;
    QPointF destPoint;

    QPointF bezierPoint;
    double angle = 0; // To calculate bezierPoint
};

#endif // SINGLEEDGE_H
