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
#ifndef MAPWIDGET_P_H
#define MAPWIDGET_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the GeoQt API.
// This header file may change from version to version without notice,
// or even be removed.
//
// We mean it.
//

#include <QtCore/QHash>

class QPointF;
class QPoint;
class QPolygonF;
class QSize;
class QPixmap;
class QGraphicsScene;

namespace GeoQt {

class MapSourceInterface;
class TileProvider;
struct Tile;

class MapWidgetPrivate
{
public:
	MapWidgetPrivate();
	~MapWidgetPrivate();

	void requestTiles(const QPolygonF &viewport);
	void resizeScene(const QSize &size);
	void repositionMarkers();

	bool pressed;
	QGraphicsScene *map_scene;
	MapSourceInterface *map_source;
	TileProvider *tile_provider;
	QPointF map_center;
	int zoom_level;
	QHash<QPoint, QPixmap> tile_list;
};
}
#endif // MAPWIDGET_P_H
