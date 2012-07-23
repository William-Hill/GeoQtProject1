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
    //void mousePressEvent(QMouseEvent *event);
    //void mouseReleaseEvent(QMouseEvent *event);


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
   //QString coordinateText;


};

#endif // PROJECTWIDGET_H
