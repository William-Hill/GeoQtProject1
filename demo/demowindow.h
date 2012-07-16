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
#ifndef DEMOWINDOW_H
#define DEMOWINDOW_H

#include <QWidget>

namespace Ui {
    class DemoWindow;
}

class DemoWindow : public QWidget {
    Q_OBJECT
public:
    DemoWindow(QWidget *parent = 0);
    ~DemoWindow();

private slots:
	void changeMapSource(const QString &name);
	void updateCoordinateLabel(const QPointF &coordinate);

private:
    Ui::DemoWindow *ui;
};

#endif // DEMOWINDOW_H
