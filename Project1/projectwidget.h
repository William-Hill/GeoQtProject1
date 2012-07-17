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


    
private:
    Ui::ProjectWidget *ui;
    QString mapsname;
    QStringList mapsAvailable;

};

#endif // PROJECTWIDGET_H
