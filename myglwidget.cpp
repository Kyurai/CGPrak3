#include "myglwidget.h"

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
void MyGLWidget::setAngle(int _angle){
    this->angle = _angle;
}
int MyGLWidget::getAngle(){
    return this->angle;
}

MyGLWidget::MyGLWidget(){
    std::cout << "Nothing to do here !";
}

MyGLWidget::MyGLWidget(QWidget*& parent):
    QOpenGLWidget(parent),
    vbo(QOpenGLBuffer::VertexBuffer),
    ibo(QOpenGLBuffer::IndexBuffer)
{
    this->setVisible(true);
    this->setFocusPolicy(Qt::StrongFocus);
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

void MyGLWidget::fillBuffers(){
    ModelLoader model;
    bool res = model.loadObjectFromFile("/home/dustin/Documents/CG_Prakt/Praktikum3/models/bunny.obj");
    // Wenn erfolgreich, generiere VBO und Index-Array
    if (res) {
    // Frage zu erwartende Array-Längen ab
    vboLength = model.lengthOfSimpleVBO();
    iboLength = model.lengthOfIndexArray();
    // Generiere VBO und Index-Array
    vboData = new GLfloat[vboLength];
    indexData = new GLuint[iboLength];
    model.genSimpleVBO(vboData);
    model.genIndexArray(indexData);
    }
    else {
    // Modell konnte nicht geladen werden
    }

    // Erzeuge VBO, die Parameter verteilen sich hier auf mehrere Methoden
    vbo.create();
    vbo.bind();
    vbo.setUsagePattern(QOpenGLBuffer::StaticDraw);
    //vbo.allocate(this->vertices, sizeof(GLfloat) * 4 * 8);
    vbo.allocate(vboData, sizeof(GLfloat) * vboLength);
    vbo.release();

    // Erzeuge Index-Buffer
    ibo.create();
    ibo.bind();
    ibo.setUsagePattern(QOpenGLBuffer::StaticDraw);
    //ibo.allocate(this->indices, sizeof(GLubyte) * 6);
    ibo.allocate(indexData, sizeof(GLuint) * iboLength);
    ibo.release();
}

void MyGLWidget::createGeo(){
    // 1. Vertex, Position
    vertices[0] = 1.0f;
    vertices[1] = -1.0f;
    vertices[2] = 0.0f;
    vertices[3] = 1.0f; // 4. Komponente ist immer 1
    // 1. Vertex, Farbe
    vertices[4] = 1.0f;
    vertices[5] = 0.0f;
    vertices[6] = 0.0f;
    vertices[7] = 1.0f; // 4. Komponente ist immer 1

    // 2. Vertex, Position
    vertices[8] = -1.0f;
    vertices[9] = -1.0f;
    vertices[10] = 0.0f;
    vertices[11] = 1.0f; // 4. Komponente ist immer 1
    // 2. Vertex, Farbe
    vertices[12] = 0.0f;
    vertices[13] = 1.0f;
    vertices[14] = 0.0f;
    vertices[15] = 1.0f; // 4. Komponente ist immer 1

    // 3. Vertex, Position
    vertices[16] = -1.0f;
    vertices[17] = 1.0f;
    vertices[18] = 0.0f;
    vertices[19] = 1.0f; // 4. Komponente ist immer 1
    // 3. Vertex, Farbe
    vertices[20] = 0.0f;
    vertices[21] = 0.0f;
    vertices[22] = 1.0f;
    vertices[23] = 1.0f; // 4. Komponente ist immer 1

    // 4. Vertex, Position
    vertices[24] = 1.0f;
    vertices[25] = 1.0f;
    vertices[26] = 0.0f;
    vertices[27] = 1.0f; // 4. Komponente ist immer 1
    // 4. Vertex, Farbe
    vertices[28] = 1.0f;
    vertices[29] = 1.0f;
    vertices[30] = 0.0f;
    vertices[31] = 1.0f; // 4. Komponente ist immer 1

    // Initialisiere Elemente
    //im Uhrzeigersinn aufgespannt
    indices[0] = 0; //1.Dreieck
    indices[1] = 1;
    indices[2] = 2;
    indices[3] = 2; //2.Dreieck
    indices[4] = 3;
    indices[5] = 0;

    //vbo = vertices;
    //ibo = indices;
}

void MyGLWidget::initializeGL(){
    createGeo();
    fillBuffers();

    // Lade die Shader-Sourcen aus externen Dateien (ggf. anpassen)
    shaderProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/default330.vert");
    shaderProgram.addShaderFromSourceFile(QOpenGLShader::Fragment,":/default330.frag");
    // Kompiliere und linke die Shader-Programme
    shaderProgram.link();

    glEnable(GL_DEPTH_TEST);
    glCullFace(GL_FRONT);
    glEnable(GL_CULL_FACE);
    glDepthFunc(GL_LEQUAL);    
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

    glClearDepth(1.0f);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

void MyGLWidget::resizeGL(int width, int height){
    height = (height == 0) ? 1 : height;

    //QMatrix4x4 matrix;
    //matrix.setToIdentity();
    //matrix.viewport(0,0,width,height);
    //matrix.frustum(-0.05,0.05,-0.05,0.05,0.1,100.0);
    //this->matrixStack.push(matrix);

    // Set viewport to cover the whole window
    glViewport(0, 0, width, height);

    /*
    // Set projection matrix to a perspective projection
    glMatrixMode(GL_PROJECTION); //deprecated
    glLoadIdentity(); //deprecated

    glFrustum(-0.05,0.05,-0.05,0.05,0.1,100.0); //deprecated*/
}

void MyGLWidget::paintGL(){

    // Clear buffer to set color and alpha
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //Matrix Transformation
    /*QMatrix4x4 matrix;
    matrix.setToIdentity();
    matrix.rotate(45+ this->angle*1, 1.0f,1.6f,1.0f);
    matrix.translate(this->getCoord_x(),this->getCoord_y(), -7.0f);
    this->matrixStack.push(matrix);*/

    QMatrix4x4 matrix;
     matrix.setToIdentity();
     matrix.perspective(60.0, 4.0/3.0, 0.1, 1000.0);
     matrix.translate(this->coord_x, this->coord_y, this->coord_z);
     matrix.rotate(45.0f,0.0,0.0,1.0);
     matrix.translate(0.0f, 0.0f, -7.0f);
     matrix.rotate(this->angle,0.0,0.0,1.0);
     matrix.scale(this->getZoom(),this->getZoom(),this->getZoom());
     //matrixStack.push(matrix); // glPushMatrix
     //matrix.translate(0.0, 0.0, 1.0) ;
     //matrix = matrixStack.top(); // glPopMatrix
     //matrixStack.pop();

    /*
    // Apply model view transformations
    glMatrixMode(GL_MODELVIEW); //deprecated
    glLoadIdentity(); //deprecated
    //--Change values here to move the cameras
    glTranslatef(this->getCoord_x(), this->getCoord_y(), -7.0f); //deprecated

    //--1.5 rotate Function
    //deprecated
    glRotatef(45 + this->angle*1,1.3f,1.6f,1.0f); //1 = angle, 2 = rotate x, 3 = rotate y, 4 =  rotate z
    */

    //Used for zooming in and out
    //glScalef(this->getZoom(),this->getZoom(),this->getZoom());

    this->shaderProgram.bind();
    this->vbo.bind();
    this->ibo.bind();

    //Lokalisiere bzw definiere Schnittstelle für Eckpunkte
    int attrVertices = 0;

    //Lokalisiere bzw definiere Schnittstelle für Farben
    //int attrColors = 1;

    //Aktiviere die Verwendung der Attribute-Arrays
    shaderProgram.enableAttributeArray(attrVertices);
    //shaderProgram.enableAttributeArray(attrColors);

    //Lokalisiere bzw. definiere Schnittstelle für die Trans-Matrix
    //die Matrix kann direkt übergeben werden, da setUniformValue für diesen Typ überladen ist
    int unifMatrix = 0;
    unifMatrix = shaderProgram.uniformLocation("transMatrix"); //#version 130
    shaderProgram.setUniformValue(unifMatrix,matrix);
    //this->matrixStack.pop();

    int persMatrix = 0;
    persMatrix = shaderProgram.uniformLocation("persMatrix"); //#version 130
    shaderProgram.setUniformValue(persMatrix,matrix);
    //this->matrixStack.pop();

    //Fülle die Attribute Buffer mit den korrekten Daten
    int offset = 0;
    int stride = 4 * sizeof(GLfloat);
    shaderProgram.setAttributeBuffer(attrVertices,GL_FLOAT,offset,4,stride);
    offset += 4 * sizeof(GLfloat);
    //shaderProgram.setAttributeBuffer(attrColors,GL_FLOAT,offset,4,stride);


    // Zeichne die 6 Elemente (Indizes) als Dreiecke
    // Die anderen Parameter verhalten sich wie oben
    //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0);
    glDrawElements(GL_TRIANGLES, iboLength, GL_UNSIGNED_INT, 0);

    //Deaktiviere die Verwendung derr Attribute-Arrays
    shaderProgram.disableAttributeArray(attrVertices);
    //shaderProgram.disableAttributeArray(attrColors);
    vbo.release();
    ibo.release();

    //Löse das Shader Programm
    shaderProgram.release();

    // Increment angle for rotation
    this->angle += 0.05;
    this->update(); //to update the widget constantly
}
