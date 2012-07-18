#include "projectwidget.h"
#include "ui_projectwidget.h"

#include <MapSourceInterface>
#include <MapWidget>
#include <iostream>
#include <QTextStream>
#include <QKeyEvent>

using namespace GeoQt;
//GeoQt::MapWidget *map_widget;
//map_widget = new GeoQt::MapWidget(ProjectWidget);



ProjectWidget::ProjectWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ProjectWidget)
{
    ui->setupUi(this);
    setFocusPolicy(Qt::StrongFocus);
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
    MapSourceInterface *map_source = MapWidget::mapSourceFactory(name);  //
    ui->map_widget->setMapSource(map_source);  //setMapSouce accepts a MapSourceInterface
    ui->map_widget->setZoomLevel(4);
    ui->map_widget->centerOn(QPointF(64.300914, 56.2877190));
}

void ProjectWidget::keyZoomEvent(QKeyEvent *event)
{
    //Q_D(MapWidget);


    key = event->key();

    QWidget::keyPressEvent(event);

    if (key == Qt::Key_Plus)
    {

        //maybe set an int to 1 and pass it to set the parameter of setZoomLevel
        //if (zoom_level <= d->map_source )
        int zoomPlus = 1;
        ui->map_widget->setZoomLevel(zoomPlus);
    }

    //should be able to just increment or decrement zoomLevel by 1 and emit mapCenterChanged
}

ProjectWidget::~ProjectWidget()
{
    delete ui;
}
