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
#include "mapwidget.h"
#include "private/mapwidget_p.h"

#include <QtCore/QDir>
#include <QtCore/QPluginLoader>

#include <QtGui/QMouseEvent>
#include <QtGui/QWheelEvent>
#include <QtGui/QGraphicsScene>
#include <QtGui/QCursorShape>

#include "mapsourceinterface.h"
#include "tileprovider.h"
#include "abstractmarker.h"

#include <QDebug>

using namespace GeoQt;

Q_GLOBAL_STATIC(QList<MapSourceInterface *>, map_source_interfaces);

uint qHash(const QPoint& p)
{
	return p.x() * 17 ^ p.y();
}

MapWidgetPrivate::MapWidgetPrivate()
	: pressed(false), map_scene(new QGraphicsScene), map_source(NULL),
      tile_provider(new TileProvider), map_center(QPointF(0, 0)), screenCoordinate(QPointF(0,0)), zoom_level(0), shiftPressed(false),
      rOrigin(QPoint(0,0)), shiftActivated(false), rubberband(NULL), tracePressed(false), traceActivated(false), tracePath (new QPainterPath), paintPath (new QPainter)
      //rubberband(new QRubberBand)

    //m ight have to add rubberRect and rubberband
{
}

MapWidgetPrivate::~MapWidgetPrivate()
{
	map_scene->deleteLater();
	tile_provider->deleteLater();
}

void MapWidgetPrivate::requestTiles(const QPolygonF &viewport)
{
	if (!map_source)
		return;

	QRect rect = viewport.boundingRect().toRect();
	int tile_size = map_source->tileSize();
	int x_start = rect.x() / tile_size - 1;
	int y_start = rect.y() / tile_size - 1;
	int x_end = rect.right() / tile_size + 1;
	int y_end = rect.bottom() / tile_size + 1;
	int tiles = 1 << zoom_level;

	map_scene->invalidate(viewport.boundingRect(), QGraphicsScene::ForegroundLayer);

	for (int x = x_start; x < x_end; ++x) {
		if (x < 0 || x >= tiles) continue;
		for (int y = y_start; y < y_end; ++y) {
			if (y < 0 || y >= tiles) continue;
			if (!tile_list.contains(QPoint(x, y)))
				tile_provider->getTile(x, y, zoom_level);
		}
	}
}

void MapWidgetPrivate::resizeScene(const QSize &size)
{
	if (!map_source)
		return;

	int tiles_size = (1 << zoom_level) * map_source->tileSize();

	qreal border_x = size.width() / 2.0;
	qreal border_y = size.height() / 2.0;

	QRect scene_rect(0, 0, tiles_size, tiles_size);
	map_scene->setSceneRect(scene_rect.adjusted(-border_x, -border_y , border_x, border_y));
}

void MapWidgetPrivate::repositionMarkers()
{
	foreach (QGraphicsItem *item, map_scene->items()) {
		AbstractMarker *marker = qgraphicsitem_cast<AbstractMarker *>(item);
		if (marker)
			marker->setPos(map_source->displayFromCooordinate(marker->coordinate(), zoom_level));
	}
}

MapWidget::MapWidget(QWidget *parent)
	: QGraphicsView(parent), d_ptr(new MapWidgetPrivate)
{
	Q_D(MapWidget);

	setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
	setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	setDragMode(QGraphicsView::ScrollHandDrag);
	setScene(d->map_scene);

	connect(d->tile_provider, SIGNAL(tileRetrived(QPixmap,int,int,int)), SLOT(setTile(QPixmap,int,int,int)));

	centerOn(d->map_center);
}

MapWidget::~MapWidget()
{
	delete d_ptr;
}

QPointF MapWidget::mapCenter() const
{
	Q_D(const MapWidget);

	return d->map_center;
}

int MapWidget::zoomLevel() const  //Gets the zoom level
{
	Q_D(const MapWidget);

	return d->zoom_level;
}

void MapWidget::setMapSource(MapSourceInterface *map_source)
{
	if (!map_source)
		return;

	Q_D(MapWidget);

	d->map_source = map_source;
	d->tile_provider->setUrlTemplate(d->map_source->urlTemplate());

	if (d->zoom_level > map_source->maxZoom()) {
		setZoomLevel(map_source->maxZoom());
	}

	if (d->zoom_level < map_source->minZoom()) {
		setZoomLevel(map_source->minZoom());
	}

	d->tile_list.clear();
	d->requestTiles(mapToScene(viewport()->rect()));
}

MapSourceInterface *MapWidget::mapSource() const
{
	Q_D(const MapWidget);

	return d->map_source;
}

void MapWidget::addMarker(AbstractMarker *marker)       //adds a marker to the map scene
{
	Q_D(MapWidget);

    if (!d->map_source || !marker)  //If not a valid map source or not a valid marker, exit
		return;

    d->map_scene->addItem(marker);      //Adds the marker to the QGraphicsScene variable, map_scene
    if (d->map_source->isValidCoordinate(marker->coordinate()))     //If the coordinate of the marker is a valid Coordinate (valid coordinate is a boolean value that is determined by each map source)
        marker->setPos(d->map_source->displayFromCooordinate(marker->coordinate(), d->zoom_level));  //Sets the position of the marker based on the marker's coordinates
    marker->setFlag(QGraphicsItem::ItemIsSelectable, true);     //Sets the marker as a selectable item
    marker->setCursor(Qt::PointingHandCursor);                  //Sets the cursor to become a pointing hand when the marker is moused over
}

QStringList MapWidget::avaiableMapSources()  //Creates a list with a list of available map sources
{
    if (map_source_interfaces()->isEmpty()) {   // if the QList map_source_interfaces is empty
        QDir map_sources_path = QDir("mapsources"); // Use QDir to gain access to the mapsources directory
        foreach (const QString &file_name, map_sources_path.entryList(QDir::Files)) {  // gets the name of each file in the directory
			QPluginLoader loader(map_sources_path.absoluteFilePath(file_name));
			MapSourceInterface *plugin = qobject_cast<MapSourceInterface *>(loader.instance());
			if (plugin)
				map_source_interfaces()->append(plugin);
		}
	}
	QStringList names;
	foreach (MapSourceInterface *interface, *map_source_interfaces())
		names.append(interface->name());
	return names;
}

MapSourceInterface *MapWidget::mapSourceFactory(const QString &name)
{
    if (map_source_interfaces()->isEmpty())  // Checks the QList map_source_interfaces to see if empty; basically if there are map sources available
		return 0;

    foreach(MapSourceInterface *map_source, *map_source_interfaces()) { //the foreach keyword iterates through the entire list in order
        if (map_source->name() == name)     // if the name of a map source matches the name of the QString parameter; then return the map source
			return map_source;
	}
	return 0;
}

void MapWidget::centerOn(const QPointF &coordinate)
{
    Q_D(MapWidget); //Allows you to access the variables in the MapWidget_p.h file; gives you access to the d pointer

    if (!d->map_source)  //If not a valid MapSourceInterface, then exit.  The three map sources inherit from MapSourceInterface, this will return true if d.map_source is one of the three map sources
		return;

    if (d->map_source->isValidCoordinate(coordinate)) {  //Read as d.(insert map name).isValidCoordinate; the d pointer allows you to take one of the map sources and call its individual isValidCoordinate function
        d->map_center = coordinate;   //sets the map_center variable to the QPointF paramater that is passed to the function
		QGraphicsView::centerOn(d->map_source->displayFromCooordinate(coordinate, d->zoom_level));
		d->requestTiles(mapToScene(viewport()->rect()));

        emit mapCenterChanged(d->map_center);  //emits the mapCenterChanged signal
	}
}

void MapWidget::centerOn(const AbstractMarker *marker)
{
	centerOn(marker->coordinate());
}

void MapWidget::setZoomLevel(int zoom_level)
{
	Q_D(MapWidget);

	if (!d->map_source)
		return;

    if (zoom_level <= d->map_source->maxZoom() && zoom_level >= d->map_source->minZoom()) { //Checks to see if zoom is already set to min or max
		d->zoom_level = zoom_level;
		d->resizeScene(viewport()->size());
		d->tile_list.clear();
		d->repositionMarkers();
		centerOn(d->map_center);

		emit zoomLevelChanged(d->zoom_level);
	}
}

void MapWidget::drawBackground(QPainter *painter, const QRectF &exposed)
{
	Q_D(MapWidget);

	painter->fillRect(exposed, Qt::gray);

	if (!d->map_source)
		return;

	int tile_size = d->map_source->tileSize();

	foreach (const QPoint &point, d->tile_list.keys()) {
		QRectF rect(point.x() * tile_size, point.y() * tile_size, tile_size, tile_size);
		if (exposed.intersects(rect))
			painter->drawPixmap(rect.topLeft(), d->tile_list.value(point));
	}
}

void MapWidget::drawForeground(QPainter *painter, const QRectF &exposed)
{
	Q_D(MapWidget);

	QPointF center = exposed.center();
	qreal x = center.x();
	qreal y = center.y();

	painter->drawLine(x - 10, y, x + 10, y);
	painter->drawLine(x, y - 10, x, y + 10);

	if (!d->map_source)
		return;

	painter->drawText(exposed, Qt::AlignBottom | Qt::AlignRight | Qt::TextWordWrap,
					  d->map_source->license());
}

void MapWidget::resizeEvent(QResizeEvent *event)
{
	Q_D(MapWidget);

	QGraphicsView::resizeEvent(event);

	d->resizeScene(event->size());

	centerOn(d->map_center);

	d->requestTiles(mapToScene(viewport()->rect()));
}

void MapWidget::mousePressEvent(QMouseEvent *event)
{
    Q_D(MapWidget);

    if (d->shiftPressed == true && event->button() == Qt::LeftButton && d->tracePressed == false)    // if the shift modifier has been activated
    {
        d->shiftActivated = true;       // Flag for activating the shift mod key
        d->rOrigin = event->pos();      // get the origin of the mouse click
        if (!d->rubberband)
            d->rubberband = new QRubberBand (QRubberBand::Rectangle,this);      // create new rubberband object
        d->rubberband->setGeometry(QRect(d->rOrigin, QSize()));          // set the geometry of the rubberband rectangle; the top left corner will be the origin
        d->rubberband->show();

        //return;
    }

    if (d->tracePressed == true && event->button() == Qt::LeftButton && d->shiftPressed == false)  // activate tracing if mouse is clicked after tracing was enabled
    {
        if (!d->tracePath)
        {
            d->tracePath = new QPainterPath();
        }

        if (!d->paintPath)
        {
            d->paintPath = new QPainter();
        }
        d->traceActivated = true;
        d->tracePath->moveTo(event->posF());       //Start tracing path at the point of the mouse click
    }

    if (event->button() == Qt::LeftButton && d->shiftPressed == false && d->tracePressed == false) // shift modifier must be deactivated for drag function to work
    {
        d->pressed = true;
    }
    QGraphicsView::mousePressEvent(event);
}

void MapWidget::mouseMoveEvent(QMouseEvent *event)
{
    Q_D(MapWidget);     // Access MapWidget's private variables;

    if (!d->map_source)     // If not a valid map souce, exit
		return;

    if (d->shiftPressed == true && d->shiftActivated == true)    // if the shift key has been pressed and the shift mod key has been activated by a mouse click
    {
        this->setDragMode(NoDrag);  // Keeps the map from dragging when mod key zoom is used
        if (!d->rubberband)     // if rubberband doesn't exist, create a new rubberband
        {
            d->rubberband = new QRubberBand (QRubberBand::Rectangle,this);
        }

           d->rubberband->setGeometry(QRect(d->rOrigin,event->pos()).normalized());    // returns a normalized (non-negative) rectangle; sets the top left corner to the origin and gets the rest of the coordinates from the QPoint returned by event->pos()
           d->rubberRect = QRect(d->rOrigin, event->pos()).normalized();               // sets a QRect variable, rubberRect, to the normalized rectangle.  Will get the center of this rectangle

    }

    if (d->tracePressed == true && d->traceActivated == true)
    {
        this->setDragMode(NoDrag);      // set dragging to no drag
        if (!d->tracePath)      // if no Painterpath has been created, call constructor
        {
            d->tracePath = new QPainterPath();
        }
        if (!d->paintPath)      // if no Painter has been created, call constructor
        {
            d->paintPath = new QPainter();
        }
            //QPainterPath newPath = d->tracePath;
            d->tracePath->lineTo(event->posF());        // draw a line according to mouse move event
            d->paintPath->drawPath(*(d->tracePath));       // draw the path
    }

  else  if (d->pressed == true) //If the left button has been clicked, respond to the drag and request tiles to redraw the map
    {
		QRect rect = viewport()->rect();
		d->map_center = d->map_source->coordinateFromDisplay(mapToScene(rect.center()).toPoint(), d->zoom_level);
        d->screenCoordinate = d->map_center;
        emit coordinateChange(d->screenCoordinate);     // emits the coordinate of the center of the map on a drag event
		d->requestTiles(mapToScene(rect));


        emit mapCenterChanged(d->map_center);  // emit signal for mapCenterChanged
	}

    else  //if the mouse hasn't been clicked
    {
        QPoint sCoor = mapToScene(event->pos()).toPoint();  //Map the position of the mouse cursor to the screen in order to get the correct coordinates.

        d->screenCoordinate = d->map_source->coordinateFromDisplay(sCoor, d->zoom_level);  //Gets the coordinates of the map source using the position of the mouse cursor and the zoom level
    emit coordinateChange(d->screenCoordinate);     //emits the coordinateChange signal
    }




    QGraphicsView::mouseMoveEvent(event);   //event handler
}

void MapWidget::mouseReleaseEvent(QMouseEvent *event)
{
	Q_D(MapWidget);

    if (d->shiftPressed == true)
    {
        //int newZoom = d->zoom_level+2;
        //d->zoom_level = newZoom;
        setZoomLevel(d->zoom_level+1);  // Got proper zooming

        d->map_center = d->map_source->coordinateFromDisplay(mapToScene(d->rubberRect.center()).toPoint(), d->zoom_level);
        //d->requestTiles(mapToScene(d->rubberRect));
        centerOn(d->map_center);        // Got proper centering
        d->rubberband->hide();
        //d->rubberband->setVisible(false);
        d->rubberband = NULL;
        //delete d->rubberband;
    }

    if (d->tracePressed == true)
    {
        d->tracePath->closeSubpath();       //end the drawing
    }

    if (event->button() == Qt::LeftButton && d->pressed == true)
        d->pressed = false;
	QGraphicsView::mouseReleaseEvent(event);
}

void MapWidget::wheelEvent(QWheelEvent *event)
{
    Q_D(MapWidget);
	
	int num_degrees = event->delta() / 8;
	int num_steps = num_degrees / 15;

	setZoomLevel(d->zoom_level + num_steps);

	centerOn(d->map_center);

	event->accept();
}

void MapWidget::keyPressEvent(QKeyEvent *event)
{
	Q_D(MapWidget);

	int key = event->key();

	if (key == Qt::Key_PageUp || key == Qt::Key_PageDown)
		return;

    QGraphicsView::keyPressEvent(event);

    if (key >= Qt::Key_Left && key <= Qt::Key_Down) // checks to see if key entered is a directional key; between Key_Left (0x01000012) and Key_Down (0x01000015)
    {
		QRect rect = viewport()->rect();
		d->map_center = d->map_source->coordinateFromDisplay(mapToScene(rect.center()).toPoint(), d->zoom_level);
		d->requestTiles(mapToScene(rect));

		emit mapCenterChanged(d->map_center);
	}

    if (key == Qt::Key_Shift)
    {
        d->shiftPressed = true;

    }

    if (key == Qt::Key_Control)
    {
        d->tracePressed = true;
    }

    if (key == Qt::Key_Escape)
    {
        d->shiftPressed = false;
        this->setDragMode(ScrollHandDrag);          // Sets drag mode back to scrolling
        d->tracePressed = false;
    }
}




void MapWidget::setTile(const QPixmap &pixmap, int x, int y, int z)
{
	Q_D(MapWidget);

	if (!d->map_source)
		return;

	if (z != d->zoom_level)
		return;

	int tiles = 1 << d->zoom_level;
	int tile_size = d->map_source->tileSize();

	if (x >= 0 && x < tiles && y >= 0 && y < tiles) {
		QPoint point(x, y);
		if (!d->tile_list.contains(point)) {
			d->tile_list.insert(point, pixmap);
			d->map_scene->invalidate(QRectF(x * tile_size, y * tile_size, tile_size, tile_size), QGraphicsScene::BackgroundLayer);
		}
	}
}
