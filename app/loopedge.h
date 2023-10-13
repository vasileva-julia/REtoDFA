#ifndef LOOPEDGE_H
#define LOOPEDGE_H

#include <string>
#include "node.h"
#include "edge.h"


class LoopEdge : public Edge {  // Edge from a node to itself.
public:
    LoopEdge(Node* sourceNode, QString label);

    void adjust();

    enum { Type = UserType + 4 };
    int type() const override;

    bool isLoopEdge() const override;

protected:
    void paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*) override;

private:
    QPointF sourcePoint;
};

#endif // LOOPEDGE_H
