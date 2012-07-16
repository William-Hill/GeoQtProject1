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
#include <QtTest/QtTest>

#include <QtCore/QTimer>
#include <QtCore/QEventLoop>

#include "tileprovider.h"

using namespace GeoQt;

class TestTileProvider: public QObject
{
	Q_OBJECT
 private slots:
	void tileRetriveSuccess();
	void tileRetriveFailed();
};

// Test the emit of tileRetrived() signal for a valid tile.
void TestTileProvider::tileRetriveSuccess()
{
	TileProvider *tile_provider = new TileProvider(this);
    tile_provider->setUrlTemplate("http://a.tile.openstreetmap.org/7/25/50.png");
	QSignalSpy *spy_retrived = new QSignalSpy(tile_provider, SIGNAL(tileRetrived(QPixmap,int,int,int)));
	QSignalSpy *spy_error = new QSignalSpy(tile_provider, SIGNAL(error(QString)));
	QEventLoop event_loop;
	QTimer timer;

	timer.setSingleShot(true);
	connect(&timer, SIGNAL(timeout()), &event_loop, SLOT(quit()));
	connect(tile_provider, SIGNAL(tileRetrived(QPixmap,int,int,int)), &event_loop, SLOT(quit()));
	connect(tile_provider, SIGNAL(error(QString)), &event_loop, SLOT(quit()));

	tile_provider->getTile(0, 0, 0);

	timer.start(5000);
	event_loop.exec();

	if (timer.isActive())
		timer.stop();

	QCOMPARE(spy_error->count(), 0);
	QCOMPARE(spy_retrived->count(), 1);
}

// Test the emit of error() signal for an invalid tile.
void TestTileProvider::tileRetriveFailed()
{
	TileProvider *tile_provider = new TileProvider(this);
	tile_provider->setUrlTemplate("http://tile.openstreetmap.org/#Z#/#X#/#Y#.png");
	QSignalSpy *spy_retrived = new QSignalSpy(tile_provider, SIGNAL(tileRetrived(QPixmap,int,int,int)));
	QSignalSpy *spy_error = new QSignalSpy(tile_provider, SIGNAL(error(QString)));
	QEventLoop event_loop;
	QTimer timer;

	timer.setSingleShot(true);
	connect(&timer, SIGNAL(timeout()), &event_loop, SLOT(quit()));
	connect(tile_provider, SIGNAL(tileRetrived(QPixmap,int,int,int)), &event_loop, SLOT(quit()));
	connect(tile_provider, SIGNAL(error(QString)), &event_loop, SLOT(quit()));

	tile_provider->getTile(-1, -1, -1);

	timer.start(5000);
	event_loop.exec();

	if (timer.isActive())
		timer.stop();

	QCOMPARE(spy_error->count(), 1);
	QCOMPARE(spy_retrived->count(), 0);
}

QTEST_MAIN(TestTileProvider)
#include "testtileprovider.moc"
