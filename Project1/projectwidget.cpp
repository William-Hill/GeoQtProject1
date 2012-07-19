#include "projectwidget.h"
#include "ui_projectwidget.h"

#include <MapSourceInterface>
#include <MapWidget>
#include <iostream>
#include <QTextStream>
#include <QKeyEvent>
#include <QtGui/QGraphicsScene>

using namespace GeoQt;
//GeoQt::MapWidget *map_widget;
//map_widget = new GeoQt::MapWidget(ProjectWidget);
MapSourceInterface *map_source;



ProjectWidget::ProjectWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ProjectWidget)
{
    ui->setupUi(this);
    setFocusPolicy(Qt::StrongFocus);  //Widget to capture focus on both click and tab from the keyboard.
    setMouseTracking(true); //Enables mouse tracking without first needing to click
    connect(ui->map_widget, SIGNAL(coordinateChange(QPointF)), SLOT(updateCoordinates(QPointF)));
    mapsname = "mapnik";  // Hard codes in the map to be used
    mapsAvailable = MapWidget::avaiableMapSources();  //Gets a QStringList of the available map sources stored in the mapsources project folder

    for (int i = 0; i < mapsAvailable.size(); i++)   //function for debugging
    {
        std::cout << mapsAvailable.at(i).toAscii().data() <<endl;
    }

    selectMapSource(mapsname);  //Set the map to be displayed
    //changeMapSource(mapnik);
    //MapSourceInterface *map_source = MapWidget::mapSourceFactory(mapnik);
   // ui->map_widget->setMapSource(mapnik);
}

void ProjectWidget::selectMapSource(const QString &name)
{
    map_source = MapWidget::mapSourceFactory(name);  //
    ui->map_widget->setMapSource(map_source);  //setMapSouce accepts a MapSourceInterface
    ui->map_widget->setZoomLevel(4);
    ui->map_widget->centerOn(QPointF(64.300914, 56.2877190));
    updateCoordinates(QPointF(64.300914, 56.2877190));
}

void ProjectWidget::keyPressEvent(QKeyEvent *event)
{
    //Q_D(MapWidget);


    key = event->key();     //Gets the index of the key entered from the keyboard

    QWidget::keyPressEvent(event);          //Event handler

    if (key == Qt::Key_Plus  || key == Qt::Key_Equal)  //if the key entered is a plus sign key
    {

        //maybe set an int to 1 and pass it to set the parameter of setZoomLevel
        //if (zoom_level <= d->map_source )
        int zoomPlus = 1;
        newZoomLevel = ui->map_widget->zoomLevel();     //gets the current zoom level of the widget
        ui->map_widget->setZoomLevel(newZoomLevel+1);       //increments the zoom level by 1
    }

    if (key ==Qt::Key_Minus || key == Qt::Key_Less)    //if the key entered is a minus sign key
    {
        newZoomLevel = ui->map_widget->zoomLevel();         //gets the current zoom level of the widget
        ui->map_widget->setZoomLevel(newZoomLevel-1);       //decrements the zoom level by 1
    }

    //should be able to just increment or decrement zoomLevel by 1 and emit mapCenterChanged
}

//void ProjectWidget::mouseMoveEvent(QMouseEvent *event)
//{


//    screenCoordinate = map_source->coordinateFromDisplay(event->pos(), ui->map_widget->zoomLevel());  //gets the coordinates of current map source and stores them in screenCoordinates
//   updateCoordinates(screenCoordinate);  // calls updateCoordinates
//   //QGraphicsView::mouseMoveEvent(event);

//}

void ProjectWidget::updateCoordinates(const QPointF &coordinate)
{
   QString coordinateText("Coordinates: %1, %2");
    ui->label_2->setText(coordinateText
                         .arg(coordinate.y(), 12, 'f', 8)
                         .arg(coordinate.x(), 13, 'f', 8));
}

ProjectWidget::~ProjectWidget()
{
    delete ui;
}
