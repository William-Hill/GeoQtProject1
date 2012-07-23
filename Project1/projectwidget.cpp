#include "projectwidget.h"
#include "ui_projectwidget.h"

#include <MapSourceInterface>
#include <MapWidget>
#include <iostream>
#include <QTextStream>
#include <QKeyEvent>
#include <QtGui/QGraphicsScene>

using namespace GeoQt;
//GeoQt::MapWidget *map_widget;
//map_widget = new GeoQt::MapWidget(ProjectWidget);
MapSourceInterface *map_source;



ProjectWidget::ProjectWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ProjectWidget)
{
    ui->setupUi(this);
    setFocusPolicy(Qt::StrongFocus);  //Widget to capture focus on both click and tab from the keyboard.
    setMouseTracking(true); //Enables mouse tracking without first needing to click
    isClicked = false;

    mapsname = "mapnik";  // Hard codes in the map to be used
    mapsAvailable = MapWidget::avaiableMapSources();  //Gets a QStringList of the available map sources stored in the mapsources project folder

    for (int i = 0; i < mapsAvailable.size(); i++)   //function for debugging
    {
        std::cout << mapsAvailable.at(i).toAscii().data() <<endl;
    }

    selectMapSource(mapsname);  //Set the map to be displayed
    connect(ui->map_widget, SIGNAL(coordinateChange(QPointF)), SLOT(updateCoordinates(QPointF)));  //when signal from coordinateChange is received, activate slot updateCoordinates; connect to map_widget
    connect(ui->map_widget,SIGNAL(focusChange(QPointF)),SLOT (updateCoordinates(QPointF)));  //when signal from focusChange is emitted, then activate updateCoordinates slot
    connect(ui->pushButton, SIGNAL(clicked()),this,SLOT(focusOn()));            //when the focus button is clicked, emit a signal to activate the focusOn slot
    //changeMapSource(mapnik);
    //MapSourceInterface *map_source = MapWidget::mapSourceFactory(mapnik);
   // ui->map_widget->setMapSource(mapnik);
}

void ProjectWidget::selectMapSource(const QString &name)
{
    map_source = MapWidget::mapSourceFactory(name);  //
    ui->map_widget->setMapSource(map_source);  //setMapSouce accepts a MapSourceInterface
    ui->map_widget->setZoomLevel(4);
    QPointF usaSouth(32.3235, -90.234);  //Not picking up correct coordinates
    ui->map_widget->centerOn(usaSouth);
    updateCoordinates(usaSouth);
}

void ProjectWidget::keyPressEvent(QKeyEvent *event)
{
    //Q_D(MapWidget);


    key = event->key();     //Gets the index of the key entered from the keyboard

    QWidget::keyPressEvent(event);          //Event handler

    if (key == Qt::Key_Plus  || key == Qt::Key_Equal)  //if the key entered is a plus sign key
    {

        //maybe set an int to 1 and pass it to set the parameter of setZoomLevel
        //if (zoom_level <= d->map_source )
        int zoomPlus = 1;
        newZoomLevel = ui->map_widget->zoomLevel();     //gets the current zoom level of the widget
        ui->map_widget->setZoomLevel(newZoomLevel+1);       //increments the zoom level by 1
    }

    if (key ==Qt::Key_Minus || key == Qt::Key_Less)    //if the key entered is a minus sign key
    {
        newZoomLevel = ui->map_widget->zoomLevel();         //gets the current zoom level of the widget
        ui->map_widget->setZoomLevel(newZoomLevel-1);       //decrements the zoom level by 1
    }


    if (key == Qt::Key_Shift)
    {
        shiftMod = true;
        //emit a signal
    }

    //should be able to just increment or decrement zoomLevel by 1 and emit mapCenterChanged
}

void ProjectWidget::keyReleaseEvent(QKeyEvent *event)
{
    key = event->key();

    if (key == Qt::Key_Shift)
        shiftMod = false;
}

//void ProjectWidget::mousePressEvent(QMouseEvent *event)
//{
//    if (event->button() == Qt::LeftButton)
//        isClicked = true;
//    QWidget::mousePressEvent(event); //event handler

//}

//void ProjectWidget::mouseReleaseEvent(QMouseEvent *event)
//{
//    if (event->button() == Qt::LeftButton)
//        isClicked = false;
//    if(ui->pushButton->clicked();)
//}


void ProjectWidget::focusOn()
{
    double setLong = ui->longEdit->text().toDouble();       //Get long coordinate from ui and convert to double
    double setLat = ui->latEdit->text().toDouble();         //get lat coordinate from ui and convert to double
    int setZLevel = ui->zoomEdit->text().toInt();   //Get the entered zoom level from the ui
    QPointF focusCenter(setLong, setLat);           //Create a QPointF from the user entered doubles
    ui->map_widget->setZoomLevel(setZLevel);        //Set zoom level to user input
    ui->map_widget->centerOn(focusCenter);            //Center on user given coordinate

    emit focusChange(focusCenter);      //emit the signal

}

void ProjectWidget::updateCoordinates(const QPointF &coordinate)
{
   QString coordinateText("Coordinates: %1, %2");
    ui->label_2->setText(coordinateText
                         .arg(coordinate.y(), 12, 'f', 8)
                         .arg(coordinate.x(), 13, 'f', 8));
}

ProjectWidget::~ProjectWidget()
{
    delete ui;
}
