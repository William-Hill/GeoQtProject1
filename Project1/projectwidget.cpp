#include "projectwidget.h"
#include "ui_projectwidget.h"

#include <MapSourceInterface>
#include <MapWidget>
#include <iostream>
#include <QTextStream>

using namespace GeoQt;
//GeoQt::MapWidget *map_widget;
//map_widget = new GeoQt::MapWidget(ProjectWidget);



ProjectWidget::ProjectWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ProjectWidget)
{
    ui->setupUi(this);
    mapsname = "mapnik";
    mapsAvailable = MapWidget::avaiableMapSources();

    for (int i = 0; i < mapsAvailable.size(); i++)   //function for debugging
    {
        std::cout << mapsAvailable.at(i).toAscii().data() <<endl;
    }

    selectMapSource(mapsname);
    //changeMapSource(mapnik);
    //MapSourceInterface *map_source = MapWidget::mapSourceFactory(mapnik);
   // ui->map_widget->setMapSource(mapnik);
}

void ProjectWidget::selectMapSource(const QString &name)
{
    MapSourceInterface *map_source = MapWidget::mapSourceFactory(name);
    ui->map_widget->setMapSource(map_source);  //setMapSouce accepts a MapSourceInterface
    ui->map_widget->setZoomLevel(4);
    ui->map_widget->centerOn(QPointF(64.300914, 56.2877190));
}

ProjectWidget::~ProjectWidget()
{
    delete ui;
}
