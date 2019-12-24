#include "mainwidget.h"
#include "ui_mainwidget.h"

#include <QFileDialog>
#include <QMessageBox>

#include <fstream>

#include "view.h"
#include "graph.h"

MainWidget::MainWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MainWidget)
{
    ui->setupUi(this);
    view = new View;
    ui->painting_area->addWidget(view);
}

MainWidget::~MainWidget()
{
    delete ui;
}


void MainWidget::on_open_button_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("Open file"), QString(), tr("Text files (*.txt)"));
    if(filename.isEmpty()) return;

    std::ifstream file(filename.toStdString());
    if(!file.is_open()){
        QMessageBox::warning(this, tr("Open file"), tr("Don't have permission"));
        return;
    }

    size_t n;
    file >> n;      // Vertices count

    for(size_t i = 0; i < n; i++){
        double x, y;
        file >> x >> y;
        vertices.emplace_back(x, y);
    }

    file >> n;      // Polygons count (named 'p' in task)
    for(size_t i = 0; i < n; i++){
        size_t m;   // Count of vertices in a polygon
        file >> m;

        Polygon polygon;
        for(size_t j = 0; j < m; j++){
            unsigned int k;
            file >> k;
            k--;
            polygon.points.push_back(k);
        }
        polygons.push_back(polygon);
    }

    graph = new Graph(vertices);
    graph->setPolygons(polygons);
    graph->installEdgesFromPolygons();

    std::vector<Vertex*> hull = graph->grahamScan();

    graph->setOuterTriangleVertices();

    graph->triangulateOuterTriangle(hull);
    graph->installEdgesFromPolygons();

    view->drawGraph(graph);

//    Graph *convex_hull = new Graph(hull);

//    convex_hull->setOuterTriangleVertices();
//    convex_hull->triangulateOuterTriangle(hull);

//    view->drawGraph(convex_hull);


}
