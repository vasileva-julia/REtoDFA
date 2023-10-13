#include "mainwindow.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLineEdit>
#include "node.h"
#include "edge.h"
#include "loopedge.h"
#include "singleedge.h"
#include "dfa.h"
#include "syntaxtree.h"
#include "alphabet.h"
#include "revalidator.h"


MainWindow::MainWindow(QWidget* parent)
    : QWidget(parent), scene(new QGraphicsScene(this))
    , view(new QGraphicsView(scene)), button(new QPushButton("build DFA", this))
    , lineEdit(new QLineEdit(this)) {

    setWindowTitle(tr("RE to DFA"));
    resize(600, 500);
    view->setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
    view->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    view->setRenderHint(QPainter::Antialiasing);

    QHBoxLayout* layout = new QHBoxLayout;
    layout->addWidget(lineEdit);
    layout->addWidget(button);

    QVBoxLayout* vlayout = new QVBoxLayout;
    vlayout->addLayout(layout);
    vlayout->addWidget(view);
    setLayout(vlayout);

    lineEdit->setClearButtonEnabled(true);
    lineEdit->setPlaceholderText("enter a regular expression");
    lineEdit->setValidator(new ReValidator(lineEdit));

    connect(button, SIGNAL(clicked(bool)), this, SLOT(lineEditingFinished()));
    connect(lineEdit, SIGNAL(textEdited(QString)), this, SLOT(inputChanged()));
}

void MainWindow::inputChanged() {
    QPalette palette;
    palette.setColor(QPalette::Base,Qt::white);
    palette.setColor(QPalette::Text,Qt::black);
    lineEdit->setPalette(palette);
}

void MainWindow::lineEditingFinished() {
    if (lineEdit->hasAcceptableInput()) {
        scene->clear();
        // build dfa
        SyntaxTree tree(lineEdit->text().toStdString());
        DFA dfa(tree);
        // draw states
        std::map<std::string, Node*> map;
        int i = -5 * dfa.size();
        for (std::string name : dfa.getStates()) {
            Node* node = new Node(QString::fromStdString(name));
            if (dfa.isFinal(name))
                node->makeFinal();
            if (dfa.isInitial(name))
                node->makeInitial();
            scene->addItem(node);
            node->setPos(i, i * 2);
            i += 10;
            map[name] = node;
        }
        // draw edges
        for (std::string from : dfa.getStates()) {
            for (char c : dfa.getAlphabet()) {
                if (dfa.hasTrans(from, c)) {
                    std::string to = dfa.getTrans(from, c);
                    Edge* edge = map[from]->addEdge(map[to],
                            QString::fromStdString(std::string(1, c)));
                    if (edge != nullptr)
                        scene->addItem(edge);
                }
            }
        }
    } else {
        QPalette palette;
        palette.setColor(QPalette::Base,Qt::yellow);
        palette.setColor(QPalette::Text,Qt::black);
        lineEdit->setPalette(palette);
    }
}
