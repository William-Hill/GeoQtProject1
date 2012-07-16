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
#ifndef OSMARENDERMAPPROVIDER_H
#define OSMARENDERMAPPROVIDER_H

#include <QtCore/QObject>
#include <MapSourceInterface>

using namespace GeoQt;

class OSMARenderMapSource : public QObject, public MapSourceInterface
{
	Q_OBJECT
	Q_INTERFACES(GeoQt::MapSourceInterface)

public:
	OSMARenderMapSource();

	virtual QString name();
	virtual QString license();

	virtual QString urlTemplate();

	virtual int maxZoom();
	virtual int minZoom();

	virtual int tileSize();

	virtual QPointF coordinateFromDisplay(const QPoint &point, int zoom);
	virtual QPoint displayFromCooordinate(const QPointF &coordinate, int zoom);

	virtual bool isValidCoordinate(const QPointF &coordinate);
};

#endif // OSMARENDERMAPPROVIDER_H
