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
    else if(event->delta() < 0 && this->zoom > 0){
        this->setZoom(this->getZoom()-0.1f);
        emit zoomChanged(this->getZoom()); //'emit' is just syntactic sugar
    }
}

void MyGLWidget::keyPressEvent(QKeyEvent *event)
{
    //Move upwards (y)
    if (event->key() == Qt::Key_W || event->key() == Qt::Key_Up) {
        if(this->lastMov == "w"){
           this->speed = this->speed * 1.1;
        }
        else{
            this->speed = 0.05;
        }
        this->setCoord_y(this->getCoord_y() + speed);
        this->lastMov = "w";
    }
    //Move downwards (y)
    else if(event->key() == Qt::Key_S || event->key() == Qt::Key_Down){
        if(this->lastMov == "s"){
            this->speed = this->speed * 1.1;
        }
        else{
            this->speed = 0.05;
        }
        this->setCoord_y(this->getCoord_y() - speed);
        this->lastMov = "s";
    }
    //move to the left (x)
    else if(event->key() == Qt::Key_A || event->key() == Qt::Key_Left){
        if(this->lastMov == "a"){
            this->speed = this->speed * 1.1;
        }
        else{
            this->speed = 0.05;
        }
        this->setCoord_x(this->getCoord_x() - speed);
        this->lastMov = "a";
    }
    //move to the right (x)
    else if(event->key() == Qt::Key_D || event->key() == Qt::Key_Right){
        if(this->lastMov == "d"){
            this->speed = this->speed * 1.1;
        }
        else{
            this->speed = 0.05;
        }
        this->setCoord_x(this->getCoord_x() + speed);
        this->lastMov = "d";
    }
}

void MyGLWidget::fillBuffers(){
    ModelLoader model;
    bool res = model.loadObjectFromFile("/home/dustin/Documents/CG_Prakt/Praktikum3/models/sphere_high.obj");
    // Wenn erfolgreich, generiere VBO und Index-Array
    if (res) {
      // Frage zu erwartende Array-Längen ab
      vboLength = model.lengthOfVBO(0,false,true);
      iboLength = model.lengthOfIndexArray();
      // Generiere VBO und Index-Array
      vboData = new GLfloat[vboLength];
      indexData = new GLuint[iboLength];
      model.genVBO(vboData,0,false,true);
      model.genIndexArray(indexData);
    }
    else {
        std::cout << "model nicht geladen..." << std::endl;
    }
    vbo.create();
    vbo.bind();
    vbo.setUsagePattern(QOpenGLBuffer::StaticDraw);
    vbo.allocate(vboData, sizeof(GLfloat) * vboLength);
    vbo.release();

    ibo.create();
    ibo.bind();
    ibo.setUsagePattern(QOpenGLBuffer::StaticDraw);
    ibo.allocate(indexData, sizeof(GLuint) * iboLength);
    ibo.release();
}

void MyGLWidget::createGeo(){

}

void MyGLWidget::initializeGL(){
     glEnable(GL_TEXTURE_2D);
     qTex = new QOpenGLTexture(QImage(":/textures/moon.jpg").mirrored());
     qTex->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
     qTex->setMagnificationFilter(QOpenGLTexture::Linear);
     // Anm.: Wenn qTex->textureId() == 0 ist, dann ist etwas schief gegangen

      this->fillBuffers();

      // Lade die Shader-Sourcen aus externen Dateien (ggf. anpassen)
      this->shaderProgram.addShaderFromSourceFile(QOpenGLShader::Vertex,":/shader/default330.vert");
      this->shaderProgram.addShaderFromSourceFile(QOpenGLShader::Fragment,":/shader/default330.frag");
      // Kompiliere und linke die Shader-Programme
      this->shaderProgram.link();


      glEnable(GL_DEPTH_TEST);
      glCullFace(GL_BACK);
      glEnable(GL_CULL_FACE);//Disabled by default
      glDepthFunc(GL_LEQUAL);
      glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

      glClearDepth(1.0f);
      glClearColor(.0f, .0f, .0f, .0f);//Set bgColor black

}

void MyGLWidget::resizeGL(int width, int height){
    height = (height == 0) ? 1 : height;

    // Set viewport to cover the whole window
    glViewport(0, 0, width, height);

}

void MyGLWidget::paintGL(){
    // Clear buffer to set color and alpha
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Apply model view transformations
    std :: stack<QMatrix4x4> matrixStack;
    QMatrix4x4 matrix;
    matrix.setToIdentity();
    matrix.perspective(60.0,1.0, 0.1, 1000.0) ;
    matrix.translate(this->coord_x,this->coord_y, 0.0f - 10) ;
    matrix.rotate(this->angle,0,1,0) ;
    matrix.scale(this->zoom,this->zoom,this->zoom);

    this->shaderProgram.bind();
    this->vbo.bind();
    this->ibo.bind();
    this->qTex->bind();

    // Lokalisiere bzw. definiere die Schnittstelle für die Eckpunkte
    int attrVertices = 0;
    attrVertices = this->shaderProgram.attributeLocation("vert"); // #version 130
    // Übergebe die Texturkoordinaten an den Shader als weiteres Attribut
    int attrTexCoords = 2;
    attrTexCoords = shaderProgram.attributeLocation("texCoord"); // #version 130


    // Aktiviere die Verwendung der Attribute-Arrays
    this->shaderProgram.enableAttributeArray(attrVertices);
    this->shaderProgram.enableAttributeArray(attrTexCoords);

    // Übergebe die Textur an die Uniform-Variable
    // Die 0 steht dabei für die verwendete Unit (0=Standard)
    shaderProgram.setUniformValue("texture", 0);

    // Lokalisiere bzw. definiere die Schnittstelle für die Transformationsmatrix
    // Die Matrix kann direkt übergeben werden, da setUniformValue für diesen Typ
    // überladen ist
    int unifMatrix = 0;
    unifMatrix = this->shaderProgram.uniformLocation("matrix"); // #version 130
    this->shaderProgram.setUniformValue(unifMatrix, matrix);
    // Fülle die Attribute-Buffer mit den korrekten Daten
    int offset = 0;
    int stride = 8 * sizeof(GLfloat);
    this->shaderProgram.setAttributeBuffer(attrVertices, GL_FLOAT, offset, 4, stride);
    offset += 4 * sizeof(GLfloat);
    shaderProgram.setAttributeBuffer(attrTexCoords, GL_FLOAT, offset, 4, stride);


    // Zeichne die 6 Elemente (Indizes) als Dreiecke
    // Die anderen Parameter verhalten sich wie oben
    glDrawElements(GL_TRIANGLES, this->iboLength, GL_UNSIGNED_INT, 0);


    this->shaderProgram.disableAttributeArray(attrTexCoords);
    this->shaderProgram.disableAttributeArray(attrVertices);

    this->vbo.release();
    this->ibo.release();
    this->qTex->release();

    // Löse das Shader-Programm
    this->shaderProgram.release();

    // Increment this->counter
    this->angle += 0.01;
    this->update();
}
