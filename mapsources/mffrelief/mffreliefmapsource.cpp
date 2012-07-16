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
#include "mffreliefmapsource.h"

#include <QtCore/QPoint>
#include <QtCore/QPointF>
#include <QtCore/QString>

#include <cmath>

namespace {
	inline qreal deg2rad(qreal d)
	{
		return d * M_PI / 180;
	}

	inline qreal rad2deg(qreal r)
	{
		return 180 * r / M_PI;
	}
}

MffReliefMapSource::MffReliefMapSource()
	: MapSourceInterface()
{
}

QString MffReliefMapSource::name()
{
	return QString("mffrelief");
}

QString MffReliefMapSource::license()
{
	return QString("Map data available under GNU Free Documentation license, Version 1.2 or later");
}

QString MffReliefMapSource::urlTemplate()
{
	return QString("http://maps-for-free.com/layer/relief/z#Z#/row#Y#/#Z#_#X#-#Y#.jpg");
}

int MffReliefMapSource::maxZoom()
{
	return 11;
}

int MffReliefMapSource::minZoom()
{
	return 0;
}

int MffReliefMapSource::tileSize()
{
	return 256;
}

QPointF MffReliefMapSource::coordinateFromDisplay(const QPoint &point, int zoom)
{
	int tiles_size = (1 << zoom) * tileSize();

	qreal lat = (((tiles_size / 2) - point.y()) * 2 * M_PI ) / tiles_size;
	lat = rad2deg(asin(tanh(lat)));

	qreal lon = ((point.x() - tiles_size / 2) * 2 * M_PI) / tiles_size;
	lon = rad2deg(lon);

	return QPointF(lon, lat);
}

QPoint MffReliefMapSource::displayFromCooordinate(const QPointF &coordinate, int zoom)
{
	int tiles_size = (1 << zoom) * tileSize();

	qreal lat_r = atanh(sin(deg2rad(coordinate.y())));
	qreal lon_r = deg2rad(coordinate.x());

	int x =  lon_r * tiles_size / (2 * M_PI) + tiles_size / 2;
	int y = -lat_r * tiles_size / (2 * M_PI) + tiles_size  / 2;

	return QPoint(x, y);
}

bool MffReliefMapSource::isValidCoordinate(const QPointF &coordinate)
{
	qreal lat = coordinate.y();
	qreal lon = coordinate.x();

	if ((lat < -85.0511 || lat > 85.0511) || (lon < -180 || lon > 180))
		return false;
	else
		return true;
}

Q_EXPORT_PLUGIN2(mffreliefmapsource, MffReliefMapSource)
