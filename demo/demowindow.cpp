/* Copyright (C) 2010 Lorenzo Masini <rugginoso@develer.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser Public License for more details.
 *
 * You should have received a copy of the GNU Lesser Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  US
*/
#include "demowindow.h"
#include "ui_demowindow.h"

#include <QtGui/QPixmap>

#include <MapSourceInterface>
#include <BalloonMarker>

using namespace GeoQt;

DemoWindow::DemoWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DemoWindow)
{
    ui->setupUi(this);
    ui->map_source_combo->addItems(MapWidget::avaiableMapSources());  //Creates a combo box with the available map sources

    connect(ui->map_source_combo, SIGNAL(currentIndexChanged(QString)), SLOT(changeMapSource(QString))); //connects the index signal from the current index of the combo box with the slot to change the map source to the corresponding map
    changeMapSource(ui->map_source_combo->itemText(0));   //Sets map source index to an initial value of 0, which corresponds to mapnik (itemText gets the text of the item that corresponds to index that is given as a parameter)

    connect(ui->map_widget, SIGNAL(mapCenterChanged(QPointF)), SLOT(updateCoordinateLabel(QPointF)));  //connects the mapCenterChanged signal with the updateCoordinate label (says that when the center of the map is changed, trigger an event to update coordinates)

    BalloonMarker *develer = new BalloonMarker("Develer", "<b><i>Develer</i></b><br />I'm working here");  //Creates a ballon marker that displays on click; first String parameter is an identifier, second is the actual label that is displayed
    develer->setCoordinate(QPointF(11.148151, 43.85157));  //Hard codes the location of the develer BallonMarker

    BalloonMarker *home = new BalloonMarker("Home", QPixmap(":/images/home.png")); //Creates a ballon marker that displays on click; displays the QPixMap image that is passed into it
    home->setCoordinate(QPointF(10.941305, 43.719730));  // Hard codes the location of the home BallonMarker

    ui->map_widget->addMarker(develer);  //adds the develer Balloon Marker to the map view
    ui->map_widget->addMarker(home);      // adds the home Balloon Marker to the map view

    ui->map_widget->centerOn(develer);      //sets the map's initial location to the location of the develer Balloon Marker
    ui->map_widget->setZoomLevel(17);       // sets the initial zoom level to 17.
}

DemoWindow::~DemoWindow()  //Destructor
{
    delete ui;
}

void DemoWindow::changeMapSource(const QString &name)
{
    MapSourceInterface *map_source = MapWidget::mapSourceFactory(name);    //Sets the map source to parameter, name.
    ui->zoom_spin->setMaximum(map_source->maxZoom());  // Sets up a QSpinBox that allows user to set the maximum zoom level
    ui->zoom_spin->setMinimum(map_source->minZoom());  // Sets up a QSpinBox that allows user to set the minimum zoom level
    ui->map_widget->setMapSource(map_source);           //sets the user interface's map widget's map source to the parameter, name, passed to the function.
}

void DemoWindow::updateCoordinateLabel(const QPointF &coordinate)
{
	QString coordinate_text("Coordinates: %1, %2");

	ui->coordinate_label->setText(coordinate_text
								  .arg(coordinate.y(), 12, 'f', 8)
								  .arg(coordinate.x(), 13, 'f', 8));
}
