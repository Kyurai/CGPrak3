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

void MyGLWidget::resizeGL(int width, int height){
    height = (height == 0) ? 1 : height;
    // Set viewport to cover the whole window
    glViewport(0, 0, width, height);
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

void MyGLWidget::initializeGL(){
      this->fillBuffers();
      this->loadPlanets();

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

void MyGLWidget::loadPlanets(){
    glEnable(GL_TEXTURE_2D);
    //create Planets
    //Planet(                       x,y,z,  texture,    angle,  sR,rA,      scale,hasMoonIndex)
    Planet *sun =       new Planet( 0,0,0,  "sun",        0.0,  0.01,0.0,   1.0,1);
    Planet *earth =     new Planet( 4,0,0,  "earth",      0.0,  0.01,0.01,  0.3,1);
    Planet *moon =      new Planet( 2,0,0,  "moon",       0.0,  0.02,0.01,  0.2,0);
    Planet *saturn =    new Planet(10,0,0,  "saturn",     0.0,  0.02,0.001, 0.6,0);
    Planet *jupiter =   new Planet( 8,0,0,  "jupiter",    0.0,  0.02,0.005, 0.9,0);
    Planet *neptune =   new Planet(14,0,0,  "neptune",    0.0,  0.05,0.005, 0.4,0);
    Planet *mars =      new Planet( 6,0,0,  "mars",       0.5,  0.05,0.05,  0.001,0);
    Planet *mercury =   new Planet(1.5,0,0, "mercury",    0.5,  0.09,0.05,  0.2,0);
    Planet *venus =     new Planet(2.5,0,0, "venus",      0.0,  0.05,0.03,  0.4,0);
    Planet *uranus =    new Planet(12,0,0,  "uranus",     0.0,  0.05,0.001,  0.58,0);

    //Add to Planet Vector
    this->planets.resize(9);
    this->planets[0] = sun;
    this->planets[1] = earth;
    this->planets[2] = saturn;
    this->planets[3] = jupiter;
    this->planets[4] = neptune;
    this->planets[5] = mars;
    this->planets[6] = mercury;
    this->planets[7] = venus;
    this->planets[8] = uranus;

    this->moons.resize(1);
    this->moons[0] = moon;
}

void MyGLWidget::paintGL(){
    // Clear buffer to set color and alpha
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    std::stack<QMatrix4x4> matrixStack;
    QMatrix4x4 perspectiveMatrix, modelMatrix;
    modelMatrix.setToIdentity();
    perspectiveMatrix.setToIdentity();
    perspectiveMatrix.perspective(100.0, 1.0, 0.1, 1000.0);
    perspectiveMatrix.translate(this->coord_x, this->coord_y, this->coord_z);
    perspectiveMatrix.scale(this->zoom,this->zoom,this->zoom);
    //perspectiveMatrix.rotate(0/*rotx*/,1.0,0.0,0.0);
    //perspectiveMatrix.rotate(0/*roty*/,0.0,1.0,0.0);
    //perspectiveMatrix.rotate(0/*rotz*/,0.0,1.0,0.0);

    //Sun
    Planet *sun = this->planets.at(0);
    //modelMatrix.rotate(sun->_selfRotation,0,1,0);
    render(this->planets.at(0),perspectiveMatrix,modelMatrix);
    matrixStack.push(modelMatrix);

        for(int i = 1; i < this->planets.size(); i++){
            modelMatrix = matrixStack.top();
            Planet *pl = this->planets.at(i);
            modelMatrix.rotate(pl->_rotationAround,0,1,0);
            modelMatrix.translate(pl->_x,pl->_y, pl->_z);
            modelMatrix.rotate(pl->_angle,0,0,1);
            modelMatrix.rotate(pl->_selfRotation,0,1,0);
            modelMatrix.scale(pl->_scale);
            render(pl,perspectiveMatrix,modelMatrix);
            matrixStack.push(modelMatrix);
            matrixStack.pop();

            pl->_rotationAround += pl->_rotationAroundOffset;
            pl->_selfRotation += pl->_selfRotationOffset;

            if(pl->_hasMoon != 0){
                Planet *moon = this->moons.at(pl->_hasMoon-1);
                modelMatrix.rotate(moon->_rotationAround,0,1,0); //Rotation around earth
                modelMatrix.translate(moon->_x,moon->_y,moon->_z); //Verschiebung im Raum
                modelMatrix.rotate(moon->_angle,0.0,0.0,1.0); //Neigung
                modelMatrix.rotate(moon->_selfRotation,0,1,0); //Eigenrotation
                modelMatrix.scale(moon->_scale);
                render(moon,perspectiveMatrix,modelMatrix);
            }
        }

        /*
        //Saturn
        modelMatrix = matrixStack.top();
        Planet *saturn = this->planets.at(3);
        //modelMatrix.rotate(saturn->_rotationAround,0,1,0);
        modelMatrix.translate(saturn->_x,saturn->_y, saturn->_z);
        modelMatrix.rotate(saturn->_angle,0,0,1);
        modelMatrix.rotate(saturn->_selfRotation,0,1,0);
        modelMatrix.scale(saturn->_scale);
        render(saturn,perspectiveMatrix,modelMatrix);
        matrixStack.push(modelMatrix);
        matrixStack.pop();

        //Jupiter
        modelMatrix = matrixStack.top();
        Planet *jupiter = this->planets.at(4);
        //modelMatrix.rotate(jupiter->_rotationAround,0,1,0);
        modelMatrix.translate(jupiter->_x,jupiter->_y, jupiter->_z);
        modelMatrix.rotate(jupiter->_angle,0,0,1);
        modelMatrix.rotate(jupiter->_selfRotation,0,1,0);
        modelMatrix.scale(jupiter->_scale);
        render(jupiter,perspectiveMatrix,modelMatrix);
        matrixStack.push(modelMatrix);
        matrixStack.pop();

        //Neptune
        modelMatrix = matrixStack.top();
        Planet *neptune = this->planets.at(5);
        //modelMatrix.rotate(neptune->_rotationAround,0,1,0);
        modelMatrix.translate(neptune->_x,neptune->_y, neptune->_z);
        modelMatrix.rotate(neptune->_angle,0,0,1);
        modelMatrix.rotate(neptune->_selfRotation,0,1,0);
        modelMatrix.scale(neptune->_scale);
        render(neptune,perspectiveMatrix,modelMatrix);
        matrixStack.push(modelMatrix);
        matrixStack.pop();

        //Mars
        modelMatrix = matrixStack.top();
        Planet *mars = this->planets.at(6);
        //modelMatrix.rotate(mars->_rotationAround,0,1,0);
        modelMatrix.translate(mars->_x,mars->_y, mars->_z);
        modelMatrix.rotate(mars->_angle,0,0,1);
        modelMatrix.rotate(mars->_selfRotation,0,1,0);
        modelMatrix.scale(mars->_scale);
        render(mars,perspectiveMatrix,modelMatrix);
        matrixStack.push(modelMatrix);
        matrixStack.pop();

        //Mercury
        modelMatrix = matrixStack.top();
        Planet *mercury = this->planets.at(7);
        //modelMatrix.rotate(mercury->_rotationAround,0,1,0);
        modelMatrix.translate(mercury->_x,mercury->_y, mercury->_z);
        modelMatrix.rotate(mercury->_angle,0,0,1);
        modelMatrix.rotate(mercury->_selfRotation,0,1,0);
        modelMatrix.scale(mercury->_scale);
        render(mercury,perspectiveMatrix,modelMatrix);
        matrixStack.push(modelMatrix);
        matrixStack.pop();

        //Earth
        modelMatrix = matrixStack.top();
        Planet *earth = this->planets.at(1);
        //modelMatrix.rotate(earth->_rotationAround,0,1,0); //Rotation around sun
        modelMatrix.translate(earth->_x,earth->_y,earth->_z); //Verschiebung im Raum
        modelMatrix.rotate(-23.44f,0.0,0.0,1.0); //Neigung
        matrixStack.push(modelMatrix);
            modelMatrix.rotate(earth->_selfRotation,0,1,0); //Eigenrotation
            modelMatrix.scale(earth->_scale); //Skalierung
            render(earth,perspectiveMatrix,modelMatrix);

            //Moon
            Planet *moon = this->planets.at(2);
            modelMatrix.rotate(moon->_rotationAround,0,1,0); //Rotation around earth
            modelMatrix.translate(moon->_x,moon->_y,moon->_z); //Verschiebung im Raum
            modelMatrix.rotate(6.68,0.0,0.0,1.0); //Neigung
            modelMatrix.rotate(moon->_selfRotation,0,1,0); //Eigenrotation
            modelMatrix.scale(moon->_scale);
            render(moon,perspectiveMatrix,modelMatrix); */

        this->update();
}

void MyGLWidget::render(Planet *planet, QMatrix4x4 perspective, QMatrix4x4 model){
    this->shaderProgram.bind();
    this->vbo.bind();
    this->ibo.bind();

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

    planet->_texture->bind();

    // Lokalisiere bzw. definiere die Schnittstelle für die Transformationsmatrix
    int unifMatrix = 0;
    unifMatrix = this->shaderProgram.uniformLocation("matrix"); // #version 130
    this->shaderProgram.setUniformValue(unifMatrix, model);
    // Fülle die Attribute-Buffer mit den korrekten Daten

    int unifMatrix1 = 1;
    unifMatrix1 = this->shaderProgram.uniformLocation("perspectiveMatrix");
    shaderProgram.setUniformValue(unifMatrix1,perspective);


    int offset = 0;
    int stride = 8 * sizeof(GLfloat);
    this->shaderProgram.setAttributeBuffer(attrVertices, GL_FLOAT, offset, 4, stride);
    offset += 4 * sizeof(GLfloat);
    shaderProgram.setAttributeBuffer(attrTexCoords, GL_FLOAT, offset, 4, stride);

    glDrawElements(GL_TRIANGLES, this->iboLength, GL_UNSIGNED_INT, 0);

    this->shaderProgram.disableAttributeArray(attrTexCoords);
    this->shaderProgram.disableAttributeArray(attrVertices);

    this->vbo.release();
    this->ibo.release();

    // Löse das Shader-Programm
    this->shaderProgram.release();
}
