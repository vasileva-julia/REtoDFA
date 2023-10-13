#include "loopedge.h"

#include <QPainter>
#include <QFontMetrics>
#include <math.h>


LoopEdge::LoopEdge(Node* sourceNode, QString label)
    : Edge(sourceNode, label) {
    source->addEdge(this);
    adjust();
}

int LoopEdge::type() const {
    return Type;
}

bool LoopEdge::isLoopEdge() const {
    return true;
}

void LoopEdge::adjust() {
    if (!source)
        return;
    prepareGeometryChange();

    sourcePoint = mapFromItem(source, 0, 0);
    QRectF rect = QRectF(sourcePoint,
                         sourcePoint + 2 * nodeRadius * QPointF(1, -1));

    // Path
    path = QPainterPath();
    path.moveTo(rect.center());
    path.arcMoveTo(rect, 90);
    path.arcTo(rect, 90, -270);

    arrowPolygon = makeArrowPolygon(sourcePoint + QPointF(0, -nodeRadius), 2 * M_PI / 5);

    // Bounding rect
    labelPoint = sourcePoint + QPointF(1, -1) * 1.7 * nodeRadius;
    QFontMetrics fm(font);
    QRectF labelRect = fm.boundingRect(label);
    labelRect.moveBottomLeft(labelPoint);
    boundingRectangle = path.boundingRect().united(labelRect)
            .united(arrowPolygon.boundingRect());

    return;
}

void LoopEdge::paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*) {
    if (!source)
        return;
    paintEdge(painter);
}
