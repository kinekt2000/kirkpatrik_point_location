#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include "point.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWidget; }
QT_END_NAMESPACE

class View;
class Graph;

class MainWidget : public QWidget
{
    Q_OBJECT

public:
    MainWidget(QWidget *parent = nullptr);
    ~MainWidget();

private slots:
    void on_open_button_clicked();

private:
    Graph *graph;
    View *view;
    std::vector<Point> vertices;
    std::vector<std::vector<unsigned int>> polygons;
    Ui::MainWidget *ui;
};
#endif // MAINWIDGET_H
