#include "projectwidget.h"
#include "ui_projectwidget.h"

#include <MapSourceInterface>
#include <MapWidget>

using namespace GeoQt;
//GeoQt::MapWidget *map_widget;
//map_widget = new GeoQt::MapWidget(ProjectWidget);


ProjectWidget::ProjectWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ProjectWidget)
{
    ui->setupUi(this);
    mapsname = "mapnik";

    selectMapSource(mapsname);
    //changeMapSource(mapnik);
    //MapSourceInterface *map_source = MapWidget::mapSourceFactory(mapnik);
   // ui->map_widget->setMapSource(mapnik);
}

void ProjectWidget::selectMapSource(const QString &name)
{
    MapSourceInterface *map_source = MapWidget::mapSourceFactory(name);
    ui->map_widget->setMapSource(map_source);
}

ProjectWidget::~ProjectWidget()
{
    delete ui;
}
