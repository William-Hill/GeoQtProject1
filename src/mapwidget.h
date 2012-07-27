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
#ifndef MAPWIDGET_H
#define MAPWIDGET_H

#include <QtGui/QGraphicsView>

namespace GeoQt {
class MapSourceInterface;
class AbstractMarker;
struct Tile;

class MapWidgetPrivate;
class MapWidget : public QGraphicsView
{
	Q_OBJECT

    Q_PROPERTY(QPointF mapCenter READ mapCenter WRITE centerOn NOTIFY mapCenterChanged)     //Property says to read from mapCenter, write to centerOn, and notify mapCenterChanged
	Q_PROPERTY(int zoomLevel READ zoomLevel WRITE setZoomLevel NOTIFY zoomLevelChanged)


public:
	explicit MapWidget(QWidget *parent = 0);
	~MapWidget();

	QPointF mapCenter() const;
	int zoomLevel() const;

	void setMapSource(MapSourceInterface *map_provider);
	MapSourceInterface *mapSource() const;

	void addMarker(AbstractMarker *marker);

	static QStringList avaiableMapSources();
	static MapSourceInterface *mapSourceFactory(const QString &name);

public slots:
	void centerOn(const QPointF &coordinate);
	void centerOn(const AbstractMarker *marker);

	void setZoomLevel(int zoom_level);

signals:
	void mapCenterChanged(QPointF map_center);
	void zoomLevelChanged(int zoom);
    void coordinateChange(QPointF screenCoordinate);

protected:
	void drawBackground(QPainter *painter, const QRectF &exposed);
	void drawForeground(QPainter *painter, const QRectF &exposed);

	void resizeEvent(QResizeEvent *event);
	void mousePressEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);
	void wheelEvent(QWheelEvent *event);
	void keyPressEvent(QKeyEvent *event);

private slots:
	void setTile(const QPixmap &pixmap, int x, int y, int z);

private:
	Q_DISABLE_COPY(MapWidget);
	Q_DECLARE_PRIVATE(MapWidget);

	MapWidgetPrivate * const d_ptr;
};
}
#endif // MAPWIDGET_H
