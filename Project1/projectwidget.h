#ifndef PROJECTWIDGET_H
#define PROJECTWIDGET_H

#include <QWidget>
#include <QRubberBand>



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
    void modKeyZoom();




protected:
    void keyPressEvent(QKeyEvent *event);
    //void mousePressEvent(QMouseEvent *event);
    //void mouseMoveEvent(QMouseEvent *event);
    //void mouseReleaseEvent(QMouseEvent *event);
    //void keyReleaseEvent(QKeyEvent *event);
    //Define a paint event
    //void paintEvent(QPaintEvent *event);


signals:
    void zoomInOnKeyPress(int i);
    void focusChange(QPointF focusCenter);

private slots:
    void updateCoordinates(const QPointF &coordinate);
    void focusOn();

private:
    Ui::ProjectWidget *ui;
    QString mapsname;
    QStringList mapsAvailable;
    int key;
    int newZoomLevel;
    QPointF screenCoordinate;
    bool isClicked;
    bool shiftMod;
    QPoint origin;
    QRubberBand *rubberband;
    QRect rubberRect;
    QString zNumber;
   //QString coordinateText;


};

#endif // PROJECTWIDGET_H
