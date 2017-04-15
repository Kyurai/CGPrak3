#ifndef MYGLWIDGET_H
#define MYGLWIDGET_H

//#include <QWidget>
#include <QOpenGLWidget>

class MyGLWidget : public QOpenGLWidget
{
    Q_OBJECT
private:
    int angle = 0;
    double coord_x = 0.0;
    double coord_y = 0.0;
    double zoom = 1.0f;

public:
   MyGLWidget();
   MyGLWidget(QWidget*& parent);

   void initializeGL();
   void resizeGL(int width, int height);
   void paintGL();
   //void updateGL();
   void keyPressEvent(QKeyEvent *event);
   void wheelEvent(QWheelEvent *event);

   //Getter & Setter for Attributes
   void setAngle(int _angle);
   int getAngle();
   double getCoord_x() const;
   void setCoord_x(double value);
   double getCoord_y() const;
   void setCoord_y(double value);
   double getZoom() const;
   void setZoom(double value);

public slots:
   void receiveRotationZ(int _angle);

signals:
   double zoomChanged(double zoom);
};

#endif // MYGLWIDGET_H
