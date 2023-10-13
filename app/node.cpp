#include "node.h"

#include <QGraphicsScene>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include "edge.h"
#include "loopedge.h"
#include "singleedge.h"


const qreal Node::radius = 17;
const QFont Node::font("Arial", 12, QFont::DemiBold);


Node::Node(QString name) : name(name) {
    setFlag(ItemIsMovable);
    setFlag(ItemSendsGeometryChanges);
    setCacheMode(DeviceCoordinateCache);
    setZValue(1);
}

void Node::makeFinal() {
    final = true;
}

bool Node::isFinal() {
    return final;
}

void Node::makeInitial() {
    initial = true;
}

bool Node::isInitial() {
    return initial;
}

void Node::addEdge(Edge* edge) {
    if (edge != nullptr) {
        edgeList << edge;
        edge->adjust();
    }
}

Edge* Node::addEdge(Node* dest, QString label) {
    bool wasAlready = false;
    for (Edge* edge : std::as_const(edgeList)) {
        if ((dest == this && edge->isLoopEdge()) ||
            (dest != this && !edge->isLoopEdge() && edge->sourceNode() == this
             && qgraphicsitem_cast<SingleEdge*>(edge)->destNode() == dest)) {
            edge->addToLabel(label);
            wasAlready = true;
            break;
        }
    }
    if (!wasAlready) {
        if (dest == this)
            return new LoopEdge(this, label);
        else
            return new SingleEdge(this, dest, label);
    }
    return nullptr;
}

QList<Edge*> Node::edges() const {
    return edgeList;
}

qreal Node::getRadius() {
    return radius;
}

int Node::type() const {
    return Type;
}

QRectF Node::boundingRect() const {
    qreal adjust = 2;
    return QRectF(-radius, -radius, 2 * radius, 2 * radius)
            .normalized()
            .adjusted(-adjust, -adjust, adjust, adjust);
}

QPainterPath Node::shape() const {
    QPainterPath path;
    path.addEllipse(-radius, -radius, 2 * radius, 2 * radius);
    return path;
}

void Node::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget*) {
    if (option->state & QStyle::State_Sunken) {
        if (isInitial())
            painter->setBrush(Qt::darkCyan);
        else
            painter->setBrush(Qt::gray);
    } else {
        if (isInitial())
            painter->setBrush(Qt::cyan);
        else
            painter->setBrush(Qt::lightGray);
    }

    painter->setPen(QPen(Qt::black, 1.2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));

    QRectF rec = QRectF(-radius, -radius, 2 * radius, 2 * radius);
    painter->drawEllipse(rec);
    if (isFinal()) {
        qreal add = radius / 4;
        painter->drawEllipse(rec.adjusted(add, add, -add, -add));
    }
    painter->setFont(font);
    painter->drawText(rec, Qt::AlignCenter, name);
}

QVariant Node::itemChange(GraphicsItemChange change, const QVariant& value) {
    switch (change) {
    case ItemPositionHasChanged:
        for (Edge *edge : std::as_const(edgeList))
            edge->adjust();
        break;
    default:
        break;
    };
    return QGraphicsItem::itemChange(change, value);
}

void Node::mousePressEvent(QGraphicsSceneMouseEvent* event) {
    update();
    QGraphicsItem::mousePressEvent(event);
}

void Node::mouseReleaseEvent(QGraphicsSceneMouseEvent* event) {
    update();
    QGraphicsItem::mouseReleaseEvent(event);
}
