#ifndef PROJECTWIDGET_H
#define PROJECTWIDGET_H

#include <QWidget>



namespace Ui {
class ProjectWidget;
}

class ProjectWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit ProjectWidget(QWidget *parent = 0);
    ~ProjectWidget();
    void selectMapSource(const QString &name);



protected:
    void keyPressEvent(QKeyEvent *event);
    //void mouseMoveEvent(QMouseEvent *event);

signals:
    void zoomInOnKeyPress(int i);

private slots:
    void updateCoordinates(const QPointF &coordinate);

private:
    Ui::ProjectWidget *ui;
    QString mapsname;
    QStringList mapsAvailable;
    int key;
    int newZoomLevel;
    QPointF screenCoordinate;
   //QString coordinateText;


};

#endif // PROJECTWIDGET_H
