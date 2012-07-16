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
#ifndef ABSTRACTMARKER_H
#define ABSTRACTMARKER_H

#include <QGraphicsItem>

namespace GeoQt {
class AbstractMarkerPrivate;
class AbstractMarker : public QGraphicsItem
{
public:
	enum { Type = QGraphicsItem::UserType + 1 };

	explicit AbstractMarker(const QString &name);
	virtual ~AbstractMarker();

	int type() const;

	QString name() const;

	QPointF coordinate() const;
	void setCoordinate(const QPointF &coordinate);

protected:
	AbstractMarker(AbstractMarkerPrivate &dd, const QString &name);
	AbstractMarkerPrivate * const d_ptr;

private:
	Q_DISABLE_COPY(AbstractMarker);
	Q_DECLARE_PRIVATE(AbstractMarker);
};
}
#endif // ABSTRACTMARKER_H
