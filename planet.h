#ifndef PLANET_H
#define PLANET_H
#include <QOpenGLBuffer>
#include <QOpenGLTexture>
#include <QMatrix4x4>
#include <string>
#include <QOpenGLShaderProgram>

class Planet
{
public:
    double _x;
    double _y;
    double _z;
    double _angle;
    double _selfRotation = 0.0;
    double _selfRotationOffset;
    double _rotationAround = 0.0;
    double _rotationAroundOffset;
    double _scale;
    bool _hasMoon = 0;
    std::vector<int> moonIndex;
    QOpenGLTexture* _texture;
    QOpenGLTexture* distortionTex;
    QOpenGLShaderProgram *shaderProgram;
    std::string planetName;


    Planet();
    Planet(double x, double y, double z,QOpenGLShaderProgram *shader, std::string textureName, double angle, double selfRotation,double rotationAround, double scale, bool hasMoon = false);
};

#endif // PLANET_H
