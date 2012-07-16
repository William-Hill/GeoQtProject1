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
#include "balloonmarker.h"
#include "private/balloonmarker_p.h"

#include <QtGui/QPainter>
#include <QtGui/QPainterPath>
#include <QtGui/QPolygonF>
#include <QtGui/QStyleOptionGraphicsItem>
#include <QtGui/QPalette>

#include <QtGui/QGraphicsTextItem>
#include <QtGui/QGraphicsPixmapItem>

using namespace GeoQt;

BalloonMarkerPrivate::BalloonMarkerPrivate()
	: AbstractMarkerPrivate(), content_item(0)
{
}

void BalloonMarkerPrivate::createMarkerPath()   //This creates the marker path
{
    marker_path = QPainterPath();   //Can be used to create shapes

    marker_path.addEllipse(-10, -5, 20, 10);    //Creates an ellipse within the bounding rectangle defined by its top-left corner at (x, y), width and height, and adds it to the painter path as a closed subpath
}

void BalloonMarkerPrivate::createBalloonPath()
{
    if (!content_item)  // if not a valid QGraphicsItem, exit
		return;

    balloon_path = QPainterPath(); // Constructs variable ballon_path as a new QPainterPath

	balloon_path.setFillRule(Qt::WindingFill);

	qreal content_y = content_item->pos().y();
	qreal content_width = content_item->boundingRect().width();
	qreal content_height = content_item->boundingRect().height();

	QVector<QPointF> points;
	points.append(QPointF(0, 0));
	points.append(QPointF(5, content_y + content_height));
	points.append(QPointF(20, content_y + content_height));

	QPolygonF pol(points);
	balloon_path.addPolygon(pol);

	balloon_path.addRoundedRect(-5, content_y - 5, content_width + 10, content_height + 10, 15, 15);

	balloon_path = balloon_path.simplified();
}

BalloonMarker::BalloonMarker(const QString &name)
	: AbstractMarker(*new BalloonMarkerPrivate, name)
{
}

BalloonMarker::BalloonMarker(const QString &name, const QString &html)
	: AbstractMarker(*new BalloonMarkerPrivate, name)
{
	QGraphicsTextItem *item = new QGraphicsTextItem;
	item->setHtml(html);
	setContentItem(item);
}

BalloonMarker::BalloonMarker(const QString &name, const QPixmap &pixmap)
	: AbstractMarker(*new BalloonMarkerPrivate, name)
{
	QGraphicsPixmapItem *item = new QGraphicsPixmapItem;
	item->setPixmap(pixmap);
	setContentItem(item);
}

BalloonMarker::~BalloonMarker()
{
}

void BalloonMarker::setContentItem(QGraphicsItem *item)
{
	Q_D(BalloonMarker);

	if (item) {
		delete d->content_item;
		d->content_item = item;
		item->setParentItem(this);
		d->content_item->setPos(QPointF(0, -d->content_item->boundingRect().height() - 25));
		d->content_item->setVisible(isSelected());
		d->createMarkerPath();
		d->createBalloonPath();
		update();
	}
}

QGraphicsItem *BalloonMarker::contentItem() const
{
	Q_D(const BalloonMarker);

	return d->content_item;
}

QRectF BalloonMarker::boundingRect() const
{
	Q_D(const BalloonMarker);

	QPainterPath full_path;
	full_path.addPath(d->marker_path);

	if (isSelected())
		full_path.addPath(d->balloon_path);

	return full_path.boundingRect();
}

QPainterPath BalloonMarker::shape() const
{
	Q_D(const BalloonMarker);

	QPainterPath full_path;
	full_path.addPath(d->marker_path);

	if (isSelected())
		full_path.addPath(d->balloon_path);

	return full_path;
}

void BalloonMarker::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	Q_UNUSED(widget);

	Q_D(BalloonMarker);

	painter->setRenderHint(QPainter::Antialiasing, true);

	if (isSelected()) {
		painter->setPen(option->palette.color(QPalette::Highlight));
		painter->setBrush(option->palette.color(QPalette::Highlight));
	}
	else {
		painter->setPen(Qt::gray);
		painter->setBrush(Qt::gray);
	}
	painter->setOpacity(0.5);

	painter->drawPath(d->marker_path);

	if (isSelected()) {
		painter->setBrush(Qt::white);
		painter->setPen(Qt::black);
		painter->setOpacity(1);
		painter->drawPath(d->balloon_path);
	}
}

QVariant BalloonMarker::itemChange(GraphicsItemChange change, const QVariant &value)
{
	Q_D(BalloonMarker);

	switch (change) {
	case QGraphicsItem::ItemSelectedHasChanged:
		prepareGeometryChange();
		d->content_item->setVisible(value.toBool());
		return QVariant();
		break;
	default:
		return QGraphicsItem::itemChange(change, value);
	}
}

BalloonMarker::BalloonMarker(BalloonMarkerPrivate &dd, const QString &name)
	: AbstractMarker(dd, name)
{
}
