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
#include "tileprovider.h"
#include "private/tileprovider_p.h"

#include <QtGui/QDesktopServices>
#include <QtGui/QPixmap>

#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkDiskCache>

using namespace GeoQt;

QUrl TileProviderPrivate::replaceTemplateArgs(int x, int y, int z)
{
	QString url = url_template;
	url.replace("#X#", QString::number(x));
	url.replace("#Y#", QString::number(y));
	url.replace("#Z#", QString::number(z));

	return QUrl(url);
}

TileProvider::TileProvider(QObject *parent) :
	QObject(parent), d_ptr(new TileProviderPrivate)
{
	Q_D(TileProvider);

	d->manager = new QNetworkAccessManager(this);

	QNetworkDiskCache *diskCache = new QNetworkDiskCache(this);
	diskCache->setCacheDirectory(QDesktopServices::storageLocation(QDesktopServices::CacheLocation));
	d->manager->setCache(diskCache);

	connect(d->manager, SIGNAL(finished(QNetworkReply*)), SLOT(getData(QNetworkReply*)));
}

TileProvider::~TileProvider()
{
	delete d_ptr;
}

void TileProvider::setUrlTemplate(const QString &url_template)
{
	Q_D(TileProvider);

	if (!url_template.isEmpty())
		d->url_template = url_template;
}

QString TileProvider::urlTemplate() const
{
	Q_D(const TileProvider);

	return d->url_template;
}

void TileProvider::getTile(int x, int y, int z)
{
	Q_D(TileProvider);

	QUrl request_url = d->replaceTemplateArgs(x, y, z);
	if (d->request_queue.contains(request_url))
		return;
	else
		d->request_queue.append(request_url);

	QNetworkRequest request;
	request.setUrl(request_url);
	request.setAttribute(QNetworkRequest::User, z);
	request.setAttribute(static_cast<QNetworkRequest::Attribute>(QNetworkRequest::User + 1), x);
	request.setAttribute(static_cast<QNetworkRequest::Attribute>(QNetworkRequest::User + 2), y);

	d->manager->get(request);
}

void TileProvider::getData(QNetworkReply *reply)
{
	Q_ASSERT_X(reply != 0, "Tile Provider::getData()", "Invalid reply");

	Q_D(TileProvider);

	d->request_queue.removeOne(reply->request().url());

	if (reply->error()) {
		emit error(reply->errorString());
		reply->deleteLater();
		return;
	}

	QPixmap pixmap;
	pixmap.loadFromData(reply->readAll());
	if (pixmap.isNull()) {
		emit error(tr("Error loading tile."));
		reply->deleteLater();
		return;
	}

	int z = reply->request().attribute(QNetworkRequest::User).toInt();
	int x = reply->request().attribute(static_cast<QNetworkRequest::Attribute>(QNetworkRequest::User + 1)).toInt();
	int y = reply->request().attribute(static_cast<QNetworkRequest::Attribute>(QNetworkRequest::User + 2)).toInt();

	reply->deleteLater();

	emit tileRetrived(pixmap, x, y, z);
}
