#ifndef MYGLWIDGET_H
#define MYGLWIDGET_H

#include <QOpenGLWidget>
#include <iostream>
#include <QKeyEvent>
#include <QWheelEvent>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <stack>
#include <QOpenGLTexture>
#include "modelloader.h"
#include <string>
#include <vector>
#include "planet.h"

class MyGLWidget : public QOpenGLWidget
{
    Q_OBJECT
private:
    //Perspective
    double angle = 0;
    double coord_x = 0.0;
    double coord_y = 0.0;
    double coord_z = -10.0; //original : -10
    double zoom = 1.0f;
    double rotX = 45.0;
    double rotY = 0.0;
    double rotZ = 0.0;

    //Mouse
    double mouseX;
    double mouseY;

    //Key Movement
    std::string lastMov = "";
    double speed = 0.05;

    //Array for storing Planet Objects
    std::vector<Planet*> planets;
    std::vector<Planet*> moons;

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

    GLfloat* vboData;
    GLuint* indexData; // GLuint statt GLubyte, da viele Modelle groß sind
    unsigned int vboLength;
    unsigned int iboLength;

    //QOpenGLShaderProgram shaderProgram;
    QOpenGLShaderProgram *default130, *normal, *sunShader , *phongShader;
    std::stack<QMatrix4x4> matrixStack;

    QOpenGLTexture*  qTex;
    bool hasTexCoord;

    float time = 0.0f;

public:
   MyGLWidget();
   MyGLWidget(QWidget*& parent);

   void initializeGL();
   void resizeGL(int width, int height);
   void paintGL();
   void keyPressEvent(QKeyEvent *event);
   void wheelEvent(QWheelEvent *event);
   void mouseMoveEvent(QMouseEvent *event);
   void loadPlanets();
   void fillBuffers();
   void createShaders();
   void render(Planet *planet, QMatrix4x4 perspective, QMatrix4x4 model, QMatrix4x4 view);

   //Getter & Setter for Attributes
   void setAngle(int _angle);
   int getAngle();
   double getCoord_x() const;
   void setCoord_x(double value);
   double getCoord_z() const;
   void setCoord_z(double value);
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
