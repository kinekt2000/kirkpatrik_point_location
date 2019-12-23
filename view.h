#ifndef VIEW_H
#define VIEW_H

#include <QGraphicsView>
#include <QWheelEvent>

#include "graph.h"

class View : public QGraphicsView
{
    Q_OBJECT

public:
    View();
    void drawGraph(const Graph *graph);

private slots:
    void wheelEvent(QWheelEvent *event);

private:
    const Graph *graph = nullptr;
    double scale_factor = 1;
    double vertex_radius = 5;
    double edge_width = 2;
};

#endif // VIEW_H
