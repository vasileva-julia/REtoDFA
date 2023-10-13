#include "edge.h"

#include "math.h"
#include <QFontMetrics>
#include <QPolygonF>
#include <QPainter>


const qreal Edge::arrowSize = 11;
const qreal Edge::arrowAngle = M_PI / 12;
const QFont Edge::font("Arial", 12, QFont::DemiBold);

Edge::Edge(Node* sourceNode, QString label)
    : source(sourceNode),
      nodeRadius(sourceNode->getRadius()),
      label(label) {
    setAcceptedMouseButtons(Qt::NoButton);
}

Node* Edge::sourceNode() const {
    return source;
}

int Edge::type() const {
        return Type;
}

void Edge::addToLabel(QString str) {
    label += ", " + str;
    adjust();
}

QRectF Edge::boundingRect() const {
    qreal penWidth = 1;
    qreal extra = (penWidth + arrowSize) / 2.0;

    return boundingRectangle
            .normalized().adjusted(-extra, -extra, extra, extra);
}

QPolygonF Edge::makeArrowPolygon(QPointF point, qreal angle) const {
    QPointF arrowP1 = point + QPointF(std::sin(angle + M_PI / 2 - arrowAngle),
                                      std::cos(angle + M_PI / 2 - arrowAngle)) * arrowSize;
    QPointF arrowP2 = point + QPointF(std::sin(angle + M_PI / 2 + arrowAngle),
                                      std::cos(angle + M_PI / 2 + arrowAngle)) * arrowSize;
    return QPolygonF({point, arrowP1, arrowP2});
}

void Edge::paintEdge(QPainter* painter) {
    if (path.elementCount() > 0) {
        // Edge
        painter->setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
        painter->drawPath(path);
        // Arrow
        painter->setBrush(Qt::black);
        painter->drawPolygon(arrowPolygon);
        // Text
        painter->setFont(font);
        painter->drawText(labelPoint, label);
    }
}
