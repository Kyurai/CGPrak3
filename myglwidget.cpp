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
double MyGLWidget::getCoord_z() const{
    return coord_z;
}
void MyGLWidget::setCoord_z(double value){
    coord_z = value;
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
    double max_speed = 2.0;
    double scaleUp = 1.05;

    //double direction_X = 0.5;
    //double direction_Y = 1.0;
    //double direction_Z = 0.0;

    //Move forward (z)
    if (event->key() == Qt::Key_W || event->key() == Qt::Key_Up) {
        //this->setCoord_x(this->getCoord_x() + direction_X);
        //this->setCoord_y(this->getCoord_y() + direction_Y);
        //this->setCoord_z(this->getCoord_z() + direction_Z);


        if(this->lastMov == "w" ){
            if(this->speed < max_speed){
                this->speed = this->speed * scaleUp;
            }
        }
        else{
            this->speed = 0.05;
        }
        this->setCoord_z(this->getCoord_z() + speed);
        this->lastMov = "w";
    }
    //Move backward (y)
    else if(event->key() == Qt::Key_S || event->key() == Qt::Key_Down){
        if(this->lastMov == "s"){
            if(this->speed < max_speed){
                this->speed = this->speed * scaleUp;
            }
        }
        else{
            this->speed = 0.05;
        }
        this->setCoord_z(this->getCoord_z() - speed);
        this->lastMov = "s";
    }
    //move to the left (x)
    else if(event->key() == Qt::Key_A || event->key() == Qt::Key_Left){
        if(this->lastMov == "a"){
            if(this->speed < max_speed){
                this->speed = this->speed * scaleUp;
            }
        }
        else{
            this->speed = 0.05;
        }
        this->setCoord_x(this->getCoord_x() + speed);
        this->lastMov = "a";
    }
    //move to the right (x)
    else if(event->key() == Qt::Key_D || event->key() == Qt::Key_Right){
        if(this->lastMov == "d"){
            if(this->speed < max_speed){
                this->speed = this->speed * scaleUp;
            }
        }
        else{
            this->speed = 0.05;
        }
        this->setCoord_x(this->getCoord_x() - speed);
        this->lastMov = "d";
    }
}

void MyGLWidget::mouseMoveEvent(QMouseEvent *event){
    if ((event->x() - this->mouseX) > 0){		// mouse moved to the right
         this->rotY += 1.0f;
    }
     else if ((event->x() - this->mouseX) < 0){	// mouse moved to the left
         this->rotY -= 1.0f;
    }
    this->mouseX = event->x();

    if((event->y() - this->mouseY) > 0){ //mouse moved down
        this->rotX += 1.0f;
    }
    else if((event->y() - this->mouseY) < 0){ //mouse moved up
        this->rotX -= 1.0f;
    }
    this->mouseY = event->y();
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

      QPoint p = this->mapFromGlobal(QCursor::pos());
      this->mouseX = p.x();
      this->mouseY = p.y();
}

void MyGLWidget::loadPlanets(){
    glEnable(GL_TEXTURE_2D);
    int scaling = 50;
    double distance = 0.1;

    //create Planets
    //Planet(                       x,y,z,                texture,     angle,  sR,     rA,    scale,          hasMoonIndex)
    Planet *sun =       new Planet( 0,0,0,                "sun",        0.0,  0.001,  0.0,    1.0,              false);
    Planet *mercury =   new Planet(41.0  *distance,0,0,   "mercury",    0.5,  0.009,  0.04,  0.0035*scaling,   false);
    Planet *venus =     new Planet(77.0  *distance,0,0,   "venus",      0.0,  0.005,  0.02,  0.0086*scaling,   false);
    Planet *earth =     new Planet(107.0 *distance,0,0,   "earth",     23.0,  0.008,  0.08,  0.0091*scaling,   true);
    earth->moonIndex.resize(1);
    earth->moonIndex.at(0) = 0;
    Planet *mars =      new Planet(163.0 *distance,0,0,   "mars",       0.5,  0.006,  0.06,  0.0049*scaling,   true);
    mars->moonIndex.resize(2);
    mars->moonIndex.at(0) = 1;
    mars->moonIndex.at(1) = 2;
    Planet *jupiter =   new Planet(556.0 *distance,0,0,   "jupiter",    0.0,  0.005,  0.03,  0.0102*scaling,   false);
    Planet *saturn =    new Planet(1019.0*distance,0,0,   "saturn",     0.0,  0.002,  0.01,  0.0086*scaling,   false);
    Planet *uranus =    new Planet(2051.0*distance,0,0,   "uranus",     0.0,  0.004,  0.08,  0.0037*scaling,   false);
    Planet *neptune =   new Planet(3213.0*distance,0,0,   "neptune",    0.0,  0.003,  0.04,  0.0035*scaling,   false);

    //Create Skybox(Stars)
    Planet *skybox =    new Planet(0,0,0,                  "sky",       0.0,  0.001,  0.0,    20000.0,          false);

    //create Moons
    Planet *moon =      new Planet(27.5 *distance,0,0,     "moon",      0.0,  0.002,  0.5,   0.005*scaling,     false);
    Planet *phobos =    new Planet(6.7*distance,0,0,      "phobos",     0.0,  0.002,  0.3,   0.0018*scaling,    false);
    Planet *deimos =    new Planet(16.8*distance,0,0,     "deimos",     0.0,  0.003,  0.2,   0.0012*scaling,    false);


    //Add to Planet Vector
    this->planets.resize(10);
    this->planets[0] = sun;
    this->planets[1] = mercury;
    this->planets[2] = venus;
    this->planets[3] = earth;
    this->planets[4] = mars;
    this->planets[5] = jupiter;
    this->planets[6] = saturn;
    this->planets[7] = uranus;
    this->planets[8] = neptune;
    this->planets[9] = skybox;

    //Add to Moon Vector
    this->moons.resize(3);
    this->moons[0] = moon;
    this->moons[1] = phobos;
    this->moons[2] = deimos;
}

void MyGLWidget::paintGL(){
    // Clear buffer to set color and alpha
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glCullFace(GL_BACK);

    std::stack<QMatrix4x4> matrixStack;
    QMatrix4x4 perspectiveMatrix, modelMatrix;
    modelMatrix.setToIdentity();
    perspectiveMatrix.setToIdentity();
    perspectiveMatrix.perspective(100.0, 1, 0.01, 100000.0);
    perspectiveMatrix.translate(this->coord_x, this->coord_y, this->coord_z);
    perspectiveMatrix.rotate(this->rotX,1,0,0);
    perspectiveMatrix.rotate(this->rotY,0,1,0);
    perspectiveMatrix.rotate(this->rotZ,0,0,1);
    perspectiveMatrix.scale(this->zoom,this->zoom,this->zoom);
    //Bug: Camera is rotating around sun !

    //Sun
    Planet *sun = this->planets.at(0);
    //modelMatrix.rotate(sun->_selfRotation,0,1,0);
    render(sun,perspectiveMatrix,modelMatrix);
    matrixStack.push(modelMatrix);
    sun->_selfRotation += sun->_selfRotationOffset;

        //Render Planets
        for(unsigned int i = 1; i < this->planets.size(); i++){
            if(i == 9){
                glCullFace(GL_FRONT);
            }
            modelMatrix = matrixStack.top();
            Planet *pl = this->planets.at(i);
            modelMatrix.rotate(pl->_rotationAround,0,1,0);  //Rotation around sun
            modelMatrix.translate(pl->_x,pl->_y, pl->_z);   //Translation from sun
            modelMatrix.rotate(pl->_angle,0,0,1);           //Angle of axis

            matrixStack.push(modelMatrix);

            modelMatrix.rotate(pl->_selfRotation,0,1,0);    //Selfrotation
            modelMatrix.scale(pl->_scale);                  //Scale
            render(pl,perspectiveMatrix,modelMatrix);
            //matrixStack.push(modelMatrix);

            //Render Moons
            //Bug: Moon not rotating and also not orbiting !
            if(pl->_hasMoon != false){
                for(unsigned int j = 0; j < pl->moonIndex.size(); j++){
                    modelMatrix = matrixStack.top();
                    Planet *moon = this->moons.at(pl->moonIndex.at(j));
                    modelMatrix.rotate(moon->_rotationAround,0,1,0);    //Rotation around planet
                    modelMatrix.translate(moon->_x,moon->_y,moon->_z);  //Verschiebung im Raum
                    modelMatrix.rotate(moon->_angle,0.0,0.0,1.0);       //Neigung
                    modelMatrix.rotate(moon->_selfRotation,0,1,0);      //Eigenrotation
                    modelMatrix.scale(moon->_scale);
                    render(moon,perspectiveMatrix,modelMatrix);
                    //matrixStack.push(modelMatrix);
                    //matrixStack.pop();
                    moon->_rotationAround += moon->_rotationAroundOffset;
                    moon->_selfRotation += pl->_selfRotationOffset;
                }
            }
            matrixStack.pop();

            pl->_rotationAround += pl->_rotationAroundOffset;
            pl->_selfRotation += pl->_selfRotationOffset;         
        }

       // this->update();
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
