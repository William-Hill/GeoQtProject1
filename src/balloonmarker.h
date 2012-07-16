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
#ifndef BALLOONMARKER_H
#define BALLOONMARKER_H

#include <AbstractMarker>

namespace GeoQt {
class BalloonMarkerPrivate;
class BalloonMarker : public GeoQt::AbstractMarker
{
public:
	BalloonMarker(const QString &name);
	BalloonMarker(const QString &name, const QString &html);
	BalloonMarker(const QString &name, const QPixmap &pixmap);

	virtual ~BalloonMarker();

	void setContentItem(QGraphicsItem *item);
	QGraphicsItem *contentItem() const;

	QRectF boundingRect() const;
	QPainterPath shape() const;

	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

	QVariant itemChange(GraphicsItemChange change, const QVariant &value);

protected:
	BalloonMarker(BalloonMarkerPrivate &dd, const QString &name);

private:
	Q_DISABLE_COPY(BalloonMarker);
	Q_DECLARE_PRIVATE(BalloonMarker);
};
}
#endif // BALLOONMARKER_H
