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
#include "abstractmarker.h"
#include "private/abstractmarker_p.h"

using namespace GeoQt;

AbstractMarkerPrivate::~AbstractMarkerPrivate()
{
}

AbstractMarker::AbstractMarker(const QString &name)
	: QGraphicsItem(), d_ptr(new AbstractMarkerPrivate)
{
	Q_D(AbstractMarker);

	d->name = name;
}

AbstractMarker::~AbstractMarker()
{
	delete d_ptr;
}

int AbstractMarker::type() const
{
	return static_cast<int>(Type);
}

QString AbstractMarker::name() const
{
	Q_D(const AbstractMarker);

	return d->name;
}

QPointF AbstractMarker::coordinate() const
{
	Q_D(const AbstractMarker);

	return d->coordinate;
}

void AbstractMarker::setCoordinate(const QPointF &coordinate)
{
	Q_D(AbstractMarker);

	d->coordinate = coordinate;
}

AbstractMarker::AbstractMarker(AbstractMarkerPrivate &dd, const QString &name)
	: QGraphicsItem(), d_ptr(&dd)
{
	Q_D(AbstractMarker);

	d->name = name;
}
