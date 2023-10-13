#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QWidget>
#include <QPushButton>
#include <QWidget>
#include <QLineEdit>
#include <QObject>


class MainWindow : public QWidget {
    Q_OBJECT
public:
    explicit MainWindow(QWidget* parent = nullptr);
private slots:
    void lineEditingFinished();
    void inputChanged();
private:
    QGraphicsScene* scene;
    QGraphicsView* view;
    QPushButton* button;
    QLineEdit* lineEdit;
};

#endif // MAINWINDOW_H
