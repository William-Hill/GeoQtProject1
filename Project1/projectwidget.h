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
    //void keyZoomEvent (QKeyEvent *event);


protected:
    void keyZoomEvent(QKeyEvent *event);

private:
    Ui::ProjectWidget *ui;
    QString mapsname;
    QStringList mapsAvailable;
    int key;

};

#endif // PROJECTWIDGET_H
