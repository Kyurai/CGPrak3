#include "planet.h"

Planet::Planet(){

}

Planet::Planet(double x, double y, double z, std::string textureName, double angle, double selfRotation, double rotationAround, double scale, bool hasMoon)
{
    this->_x = x;
    this->_y = y;
    this->_z = z;

    QString _textureName = QString::fromStdString(textureName);
    this->_texture = new QOpenGLTexture(QImage(":/textures/"+_textureName+".jpg").mirrored());
    this->_texture->setMagnificationFilter(QOpenGLTexture::LinearMipMapLinear);
    this->_texture->setMagnificationFilter(QOpenGLTexture::Linear);

    this->_angle = angle;
    this->_selfRotationOffset = selfRotation;
    this->_rotationAroundOffset = rotationAround;
    this->_scale = scale;
    this->_hasMoon = hasMoon;
}
