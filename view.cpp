#include "view.h"

#include <QGraphicsScene>
#include <QGraphicsEllipseItem>
#include <QGraphicsLineItem>

#include <QDebug>

View::View()
{
    QGraphicsScene *scene = new QGraphicsScene;
    setAlignment(Qt::AlignCenter);
    scene->setSceneRect(geometry().x()-geometry().width()/2., geometry().y()-geometry().height()/2., geometry().width(), geometry().height());
    setRenderHint(QPainter::Antialiasing);
    setScene(scene);

    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    setDragMode(ScrollHandDrag);
}



void View::drawGraph(const Graph *graph){
    if(!graph) return;
    this->graph = graph;

    //// CLEAR SCENE
    for(auto item: scene()->items()){
        delete item;
    }

    QPen pen;
    pen.setWidth(0);
    for(unsigned int i = 0; i < graph->verticesCount(); i++){
        pen.setWidthF(edge_width);
        for(auto adjacent: (*graph)[i].getAdjacent()){
            if(i < unsigned(adjacent->index())){
                QGraphicsLineItem *edge = new QGraphicsLineItem((*graph)[i].x(), (*graph)[i].y(), adjacent->x(), adjacent->y());
                edge->setPen(pen);
                scene()->addItem(edge);
            }
        }

        QGraphicsEllipseItem *vertex = new QGraphicsEllipseItem((*graph)[i].x()-vertex_radius, (*graph)[i].y()-vertex_radius, 2*vertex_radius, 2*vertex_radius);
        pen.setWidth(0);
        vertex->setPen(pen);
        vertex->setBrush(Qt::red);

        scene()->addItem(vertex);
    }
    viewport()->update();
}


void View::wheelEvent(QWheelEvent *event){
    const double scale_factor = 1.08;
    if(event->delta() > 0)
    {
        edge_width /= scale_factor;
        vertex_radius /= scale_factor;
        scale(scale_factor, scale_factor);
    }
    else
    {
        edge_width *= scale_factor;
        vertex_radius *= scale_factor;
        scale(1./scale_factor, 1./scale_factor);
    }
    drawGraph(graph);
}
