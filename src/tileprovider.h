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
#ifndef TILEPROVIDER_H
#define TILEPROVIDER_H

#include <QtCore/QObject>
#include <QtGui/QPixmap>

class QNetworkReply;

namespace GeoQt {
class TileProviderPrivate;
class TileProvider : public QObject
{
	Q_OBJECT

public:
    explicit TileProvider(QObject *parent = 0);
	~TileProvider();

	void setUrlTemplate(const QString &url_template);
	QString urlTemplate() const;

	void getTile(int x, int y, int z);

signals:
	void tileRetrived(const QPixmap &pixmap, int x, int y, int z);
	void error(const QString &error);

private slots:
	void getData(QNetworkReply *reply);

private:
	Q_DISABLE_COPY(TileProvider);
	Q_DECLARE_PRIVATE(TileProvider);
	TileProviderPrivate * const d_ptr;
};
}
#endif // TILEPROVIDER_H
