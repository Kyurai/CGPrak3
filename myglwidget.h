#ifndef MYGLWIDGET_H
#define MYGLWIDGET_H

#include <QOpenGLWidget>
#include <iostream>
#include <QKeyEvent>
#include <QWheelEvent>
#include <QOpenGLBuffer>

class MyGLWidget : public QOpenGLWidget
{
    Q_OBJECT
private:
    int angle = 0;
    double coord_x = 0.0;
    double coord_y = 0.0;
    double zoom = 1.0f;

    QOpenGLBuffer vbo; //Vertex Buffer Object
    QOpenGLBuffer ibo; //Indices Buffer Object

    // Eckpunkte
    // (1 Rechteck mit 4 Eckpunkten mit je 4 Koordinaten und 4 Farbkanälen)
    GLfloat vertices[4*(4+4)];
    // Verwendete Elemente (1 Rechteck, das durch 2 Dreiecke dargestellt wird)
    GLubyte indices[6]; // für große Meshes lieber GLuint
    // Handle für VBO (Eckpunkte + Farben)
    GLuint vboHandle;
    // Handle für BO der Indizes für die zu zeichnenden Elemente
    GLuint indicesHandle;


public:
   MyGLWidget();
   MyGLWidget(QWidget*& parent);

   void initializeGL();
   void resizeGL(int width, int height);
   void paintGL();
   void keyPressEvent(QKeyEvent *event);
   void wheelEvent(QWheelEvent *event);
   void createGeo();
   void fillBuffers();

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
