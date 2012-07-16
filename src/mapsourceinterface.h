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
#ifndef MAPPROVIDER_H
#define MAPPROVIDER_H

#include <QtPlugin>

class QString;
class QPointF;
class QPoint;

namespace GeoQt {

class MapSourceInterface
{
public:
	virtual QString name() = 0;
	virtual QString license() = 0;

	virtual QString urlTemplate() = 0;

	virtual int maxZoom() = 0;
	virtual int minZoom() = 0;

	virtual int tileSize() = 0;

	virtual QPointF coordinateFromDisplay(const QPoint &point, int zoom) = 0;
	virtual QPoint displayFromCooordinate(const QPointF &coordinate, int zoom) = 0;

	virtual bool isValidCoordinate(const QPointF &coordinate) = 0;
};
}

Q_DECLARE_INTERFACE(GeoQt::MapSourceInterface, "com.develer.MapWidget.MapSourceInterface/1.0")

#endif // MAPPROVIDER_H
