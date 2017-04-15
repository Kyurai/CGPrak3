#include "myglwidget.h"
#include <iostream>
#include <QKeyEvent>
#include <QWheelEvent>

double MyGLWidget::getCoord_x() const
{
    return coord_x;
}
void MyGLWidget::setCoord_x(double value)
{
    coord_x = value;
}
double MyGLWidget::getCoord_y() const
{
    return coord_y;
}
void MyGLWidget::setCoord_y(double value)
{
    coord_y = value;
}
double MyGLWidget::getZoom() const
{
    return zoom;
}
void MyGLWidget::setZoom(double value)
{
    zoom = value;
}

MyGLWidget::MyGLWidget(){

}

MyGLWidget::MyGLWidget(QWidget*& parent): QOpenGLWidget(parent){
    this->setVisible(true);
    this->setFocusPolicy(Qt::StrongFocus);
    //this->parentWidget()->sizePolicy().setHeightForWidth(true);
}

/*void MyGLWidget::updateGL(){
    std::cout << "test !!!!!!!!!!!!!!!!!!!!";
    QGLWidget::glDraw();
}*/

void MyGLWidget::setAngle(int _angle){
    this->angle = _angle;
}

int MyGLWidget::getAngle(){
    return this->angle;
}

//Slot: set Rotation of quad
void MyGLWidget::receiveRotationZ(int _angle){
    this->setAngle(_angle);
}

void MyGLWidget::wheelEvent(QWheelEvent *event){
    if(event->delta() > 0){
        this->setZoom(this->getZoom()+0.1f);
        emit zoomChanged(this->getZoom()); //emits signal to update slider
    }
    else if(event->delta() < 0){
        this->setZoom(this->getZoom()-0.1f);
        emit zoomChanged(this->getZoom()); //'emit' is just syntactic sugar
    }
}

void MyGLWidget::keyPressEvent(QKeyEvent *event)
{
    //Move upwards (y)
    if (event->key() == Qt::Key_W || event->key() == Qt::Key_Up) {
        this->setCoord_y(this->getCoord_y() + 0.05);
    }
    //Move downwards (y)
    else if(event->key() == Qt::Key_S || event->key() == Qt::Key_Down){
        this->setCoord_y(this->getCoord_y() - 0.05);
    }
    //move to the left (x)
    else if(event->key() == Qt::Key_A || event->key() == Qt::Key_Left){
        this->setCoord_x(this->getCoord_x() - 0.05);
    }
    //move to the right (x)
    else if(event->key() == Qt::Key_D || event->key() == Qt::Key_Right){
        this->setCoord_x(this->getCoord_x() + 0.05);
    }
}

void MyGLWidget::initializeGL(){
    glEnable(GL_DEPTH_TEST);
    glCullFace(GL_BACK); // don't draw back faces
    glEnable(GL_CULL_FACE); // don't draw back faces
    glDepthFunc(GL_LEQUAL);
    glShadeModel(GL_FLAT);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

    glClearDepth(1.0f);
    //--change clear color to alter background color
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

void MyGLWidget::resizeGL(int width, int height){
    // Compute aspect ratio
    height = (height == 0) ? 1 : height;
    //GLfloat aspect = (GLfloat)width / (GLfloat)height;

    // Set viewport to cover the whole window
    glViewport(0, 0, width, height);

    // Set projection matrix to a perspective projection
    glMatrixMode(GL_PROJECTION); //Projection
    glLoadIdentity();

    glFrustum(-0.05,0.05,-0.05,0.05,0.1,100.0);
    //glDraw();
}

void MyGLWidget::paintGL(){
    // Clear buffer to set color and alpha
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Apply model view transformations
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    //--Change values here to move the cameras
    glTranslatef(this->getCoord_x(), this->getCoord_y(), -7.0f);

    //--1.5 rotate Function
    glRotatef(45 + this->angle*1,1.3f,1.6f,1.0f); //1 = angle, 2 = rotate x, 3 = rotate y, 4 =  rotate z
    //glRotatef(45*this->angle*1,0.0f,0.0f,1.0f); //rotation for quad plane

    //--1.6 rotate y
    //glRotatef(180,0.0f,1.0f,0.0f);
    //--Backside not visible with culling turned on !
    //--Culling -> if object is not visible, dont draw it

    // Set color for drawing
    glColor4f(1.0f, 0.0f, 0.0f, 1.0f);

    //Used for zooming in and out
    glScalef(this->getZoom(),this->getZoom(),this->getZoom());

    /*
    // Draw quad
    //--first Triangle
    glBegin(GL_TRIANGLES);
        //bottom right
        glColor3f(1.0f,0.0f,0.0f); //1= red, 2= green, 3= blue
        glVertex3f( 1.0f, -1.0f,  0.0f);
        //top right
        glColor3f(1.0f,1.0f,1.0f);
        glVertex3f( 1.0f,  1.0f,  0.0f);
        //top left
        glColor3f(0.0f,0.0f,1.0f);
        glVertex3f(-1.0f,  1.0f,  0.0f);
    glEnd();

    //--second Triangle
    glBegin(GL_TRIANGLES);
        //top left
        glColor3f(0.0f,0.0f,1.0f);
        glVertex3f(-1.0, 1.0f, 0.0f);
        //bottom left
        glColor3f(0.0f,1.0f,0.0f);
        glVertex3f(-1.0f, -1.0f, 0.0f);
        //bottom right
        glColor3f(1.0f,0.0f,0.0f);
        glVertex3f(1.0f, -1.0f, 0.0f);
    glEnd();
    */

    //draw Cube
    //front -- red
    glBegin(GL_QUADS);
        glColor3f(1.0f,0.0f,0.0f);
        glVertex3f( 1.0f, -1.0f,  1.0f);
        glVertex3f( 1.0f,  1.0f,  1.0f);
        glVertex3f(-1.0f,  1.0f,  1.0f);
        glVertex3f(-1.0f,  -1.0f,  1.0f);
    glEnd();
    //back -- green
    glBegin(GL_QUADS);
        glColor3f(0.0f,1.0f,0.0f);
        glVertex3f( 1.0f,  1.0f,  -1.0f);
        glVertex3f( 1.0f, -1.0f,  -1.0f);
        glVertex3f(-1.0f,  -1.0f,  -1.0f);
        glVertex3f(-1.0f,  1.0f,  -1.0f);
    glEnd();
    //left -- blue
    glBegin(GL_QUADS);
        glColor3f(0.0f,0.0f,1.0f);
        glVertex3f(-1.0f,  1.0f,  1.0f);
        glVertex3f(-1.0f,  1.0f, -1.0f);
        glVertex3f(-1.0f, -1.0f, -1.0f);
        glVertex3f(-1.0f, -1.0f,  1.0f);
    glEnd();
    //right -- lightblue
    glBegin(GL_QUADS);
        glColor3f(0.0f,1.0f,1.0f);
        glVertex3f( 1.0f, -1.0f,  -1.0f);
        glVertex3f( 1.0f,  1.0f,  -1.0f);
        glVertex3f(1.0f,  1.0f,  1.0f);
        glVertex3f(1.0f,  -1.0f,  1.0f);
    glEnd();
    //top -- pink
    glBegin(GL_QUADS);
        glColor3f(1.0f,0.0f,1.0f);
        glVertex3f( 1.0f, 1.0f,-1.0f);
        glVertex3f(-1.0f, 1.0f,-1.0f);
        glVertex3f(-1.0f, 1.0f, 1.0f);
        glVertex3f( 1.0f, 1.0f, 1.0f);             
    glEnd();
    //bottom -- gelb
    glBegin(GL_QUADS);
        glColor3f(1.0f,1.0f,0.0f);
        glVertex3f( 1.0f, -1.0f,  1.0f);
        glVertex3f(-1.0f, -1.0f,  1.0f);
        glVertex3f(-1.0f, -1.0f, -1.0f);
        glVertex3f( 1.0f, -1.0f, -1.0f);
    glEnd();

    //needed for gradient
    glShadeModel(GL_SMOOTH);

    // Execute all issued GL commands
    //glFlush(); // replace with glutSwapBuffers() for double buffered mode
    // Increment angle for rotation
    this->angle++;
    this->update(); //to update the widget constantly

}
