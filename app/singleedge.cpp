#include "singleedge.h"

#include <QPainter>
#include <math.h>
#include <QFontMetrics>
#include <QPainterPath>


SingleEdge::SingleEdge(Node* sourceNode, Node* destNode, QString label)
    : Edge(sourceNode, label), dest(destNode), angle(0) {
    source->addEdge(this);
    dest->addEdge(this);
    for (Edge* edge : source->edges()) {
        if (edge->sourceNode() == dest && !edge->isLoopEdge()) {
            SingleEdge* se = static_cast<SingleEdge*>(edge);
            se->setAngle(M_PI / 18);
            setAngle(M_PI / 18);
        }
    }
    adjust();
}

int SingleEdge::type() const {
    return Type;
}

bool SingleEdge::isLoopEdge() const {
    return false;
}

void SingleEdge::setAngle(double new_angle) {
    angle = new_angle;
    adjust();
}

Node* SingleEdge::destNode() const {
    return dest;
}

void SingleEdge::adjust() {
    if (!source || !dest)
        return;

    prepareGeometryChange();

    sourcePoint = mapFromItem(source, 0, 0);
    destPoint = mapFromItem(dest, 0, 0);

    QLineF line(sourcePoint, destPoint);
    qreal length = line.length();
    double lineAngle = std::atan2(-line.dy(), line.dx());
    double len = length / 2 / std::cos(angle);
    bezierPoint = sourcePoint
                + QPointF(std::sin(lineAngle + (M_PI / 2 - angle)) * len,
                          std::cos(lineAngle + (M_PI / 2 - angle)) * len);

    QLineF line1(sourcePoint, bezierPoint);
    sourcePoint += QPointF(line1.dx(), line1.dy()) * nodeRadius / line1.length();
    QLineF line2(destPoint, bezierPoint);
    destPoint += QPointF(line2.dx(), line2.dy()) * nodeRadius / line2.length();

    // Label
    if (angle == 0) {
        lineAngle += M_PI;
        labelPoint = line.center();
    } else {
        QLineF line1(destPoint, bezierPoint);
        labelPoint = destPoint + QPointF(line1.dx(), line1.dy()) / 2;
        lineAngle = std::atan2(-line1.dy(), line1.dx());
    }

    arrowPolygon = makeArrowPolygon(destPoint, lineAngle);

    // Path
    path = QPainterPath();
    if (length > 2 * nodeRadius) {
        if (angle == 0) {
            path.moveTo(sourcePoint);
            path.lineTo(destPoint);
        } else {
            path.moveTo(sourcePoint);
            path.quadTo(bezierPoint, destPoint);
        }
    }

    // Bounding rect
    QFontMetrics fm(font);
    QRectF labelRect = fm.boundingRect(label);
    labelRect.moveBottomLeft(labelPoint);
    boundingRectangle = path.boundingRect().united(labelRect)
            .united(arrowPolygon.boundingRect());
}

void SingleEdge::paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*) {
    if (!source || !dest)
        return;

    QLineF line(sourcePoint, destPoint);
    if (qFuzzyCompare(line.length(), qreal(0.)))
        return;

    paintEdge(painter);
}
