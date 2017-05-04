#ifndef PLANET_H
#define PLANET_H
#include <QOpenGLBuffer>
#include <QOpenGLTexture>

class Planet
{
public:
    Planet(QOpenGLBuffer vbo,QOpenGLBuffer ibo, double x, double y, double z, QImage texture, double angle, double rotation);
};

#endif // PLANET_H
