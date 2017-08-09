#include <GL/gl.h>
#include <GL/glu.h>
#include "openglpainter.h"
#include <cairo.h>

LCViewer::OpenGLPainter::OpenGLPainter(unsigned int width, unsigned int height) :
    _nextID(0),
    _selectedPattern(-1) {

    glClearColor(0,0,0,1);
    glDepthFunc(GL_NEVER);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    if(height == 0) {
        height = 1;
    }

    auto ratio = (double) width / (double) height;

    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-ratio, ratio, -1, 1, -1, 1);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void LCViewer::OpenGLPainter::new_path() {

}

void LCViewer::OpenGLPainter::close_path() {

}

void LCViewer::OpenGLPainter::new_sub_path() {

}

void LCViewer::OpenGLPainter::clear(double r, double g, double b) {
    glClearColor(r, g, b, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void LCViewer::OpenGLPainter::clear(double r, double g, double b, double a) {
    glClearColor(r, g, b, a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void LCViewer::OpenGLPainter::move_to(double x, double y) {
    _pos = lc::geo::Coordinate(x, y);
}

void LCViewer::OpenGLPainter::line_to(double x, double y) {
    glBegin(GL_LINES);
        glVertex2d(_pos.x(), _pos.y());
        glVertex2d(x, y);
    glEnd();

    _pos = lc::geo::Coordinate(x, y);
}

void LCViewer::OpenGLPainter::lineWidthCompensation(double lwc) {

}

void LCViewer::OpenGLPainter::line_width(double lineWidth) {
    glLineWidth(lineWidth);
}

double LCViewer::OpenGLPainter::scale() {
    GLdouble modelview[16];
    glGetDoublev(GL_MODELVIEW_MATRIX, modelview);

    return modelview[0];
}

void LCViewer::OpenGLPainter::scale(double s) {
    glScaled(s, s, s);
}

void LCViewer::OpenGLPainter::rotate(double r) {
    glRotated(r * 180.0f / M_PI, 0, 0, 1);
}

void LCViewer::OpenGLPainter::arc(double x, double y, double r, double start, double end) {
    auto offset = M_PI / 50;
    auto difference = std::abs(lc::Math::getAngleDifference(start, end, false));

    glBegin(GL_LINE_STRIP);
    for(double angle = 0; angle <= difference; angle += offset) {
        glVertex2d(
                x + (r * cos(start - angle)),
                y + (r * sin(start - angle))
        );
    }
    glEnd();
}

void LCViewer::OpenGLPainter::arcNegative(double x, double y, double r, double start, double end) {
    auto offset = M_PI / 50;
    auto difference = std::abs(lc::Math::getAngleDifference(start, end, true));

    glBegin(GL_LINE_STRIP);
    for(double angle = 0; angle <= difference; angle += offset) {
        glVertex2d(
                x + (r * cos(start + angle)),
                y + (r * sin(start + angle))
        );
    }
    glEnd();
}

void LCViewer::OpenGLPainter::circle(double x, double y, double r) {
    int nbLines = 100;

    auto twicePi = 2 * M_PI;

    glBegin(GL_LINE_LOOP);
        for(auto i = 0; i < nbLines; i++) {
            glVertex2d(
                    x + (r * cos(i *  twicePi / nbLines)),
                    y + (r * sin(i * twicePi / nbLines))
            );
        }
    glEnd();
}

void LCViewer::OpenGLPainter::ellipse(double cx, double cy, double rx, double ry, double sa, double ea, double ra) {
    if(rx == 0) {
        rx = 0.1;
    }
    if(ry == 0) {
        ry = 0.1;
    }

    save();
    translate(cx, - cy);
    rotate(ra);
    glScaled(rx, ry, 1);
    arc(0, 0, 1, sa, ea);
    restore();
}

void LCViewer::OpenGLPainter::rectangle(double x1, double y1, double w, double h) {
    _path.clear();
    _path.emplace_back(x1 + w, y1);
    _path.emplace_back(x1 + w, y1 + h);
    _path.emplace_back(x1, y1 + h);
    _path.emplace_back(x1, y1);

    glBegin(GL_LINE_LOOP);
        glVertex2d(_path[0].x(), _path[0].y());
        glVertex2d(_path[1].x(), _path[1].y());
        glVertex2d(_path[2].x(), _path[2].y());
        glVertex2d(_path[3].x(), _path[3].y());
    glEnd();
}

void LCViewer::OpenGLPainter::stroke() {

}

void LCViewer::OpenGLPainter::source_rgb(double r, double g, double b) {
    glColor3f(r, g, b);
    _selectedPattern = -1;
}

void LCViewer::OpenGLPainter::source_rgba(double r, double g, double b, double a) {
    glColor4f(r, g, b, a);
    _selectedPattern = -1;

}

void LCViewer::OpenGLPainter::translate(double x, double y) {
    glTranslated(x, - y, 0);
}

void LCViewer::OpenGLPainter::user_to_device(double* x, double* y) {
    GLdouble z = 0;

    GLint viewport[4];
    GLdouble modelview[16];
    GLdouble projection[16];

    glGetIntegerv(GL_VIEWPORT, viewport);
    glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
    glGetDoublev(GL_PROJECTION_MATRIX, projection);

    (*y) = viewport[3] - *y;

    gluProject(*x, *y, z, modelview, projection, viewport, x, y, &z);
}

void LCViewer::OpenGLPainter::device_to_user(double* x, double* y) {
    GLint viewport[4];
    GLdouble modelview[16];
    GLdouble projection[16];

    GLdouble z = 0;

    glGetIntegerv(GL_VIEWPORT, viewport);
    glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
    glGetDoublev(GL_PROJECTION_MATRIX, projection);

    (*y) = viewport[3] - *y;

    gluUnProject(*x, *y, z, modelview, projection, viewport, x, y, &z);
}

void LCViewer::OpenGLPainter::user_to_device_distance(double* dx, double* dy) {
    double originX = 0;
    double originY = 0;

    user_to_device(&originX, &originY);
    user_to_device(dx, dy);
    (*dx) = (*dx) - originX;
    (*dy) = (*dy) - originY;
}

void LCViewer::OpenGLPainter::device_to_user_distance(double* dx, double* dy) {
    double originX = 0;
    double originY = 0;

    device_to_user(&originX, &originY);
    device_to_user(dx, dy);
    (*dx) = (*dx) - originX;
    (*dy) = (*dy) - originY;
}

void LCViewer::OpenGLPainter::font_size(double size, bool deviceCoords) {

}

void LCViewer::OpenGLPainter::select_font_face(const char* text_val) {

}

void LCViewer::OpenGLPainter::text(const char* text_val) {

}

LCViewer::TextExtends LCViewer::OpenGLPainter::text_extends(const char* text_val) {
    return LCViewer::TextExtends();
}

void LCViewer::OpenGLPainter::quadratic_curve_to(double x1, double y1, double x2, double y2) {
    double x0 = _pos.x();
    double y0 = _pos.y();
    double x3 = x2;
    double y3 = y2;

    x2 = x1 + (x2 - x1) / 3.;
    y2 = y1 + (y2 - y1) / 3.;
    x1 = x0 + 2. * (x1 - x0) / 3.;
    y1 = y0 + 2. * (y1 - y0) / 3.;
    curve_to(x1, y1, x2, y2, x3, y3);
}

void LCViewer::OpenGLPainter::curve_to(double x1, double y1, double x2, double y2, double x3, double y3) {
    double x0 = _pos.x();
    double y0 = _pos.y();

    glBegin(GL_LINE_STRIP);
        glVertex2d(x0, y0);
        for(double t = 0.0; t <= 1.0; t += 0.01) {
            double x = x0 * pow(1 - t, 3) + 3 * x1 * t * pow(1 - t, 2) + 3 * x2 * pow(t, 2) * (1 - t) + x3 * pow(t, 3);
            double y = y0 * pow(1 - t, 3) + 3 * y1 * t * pow(1 - t, 2) + 3 * y2 * pow(t, 2) * (1 - t) + y3 * pow(t, 3);
            glVertex2d(x, y);
        }
        glVertex2d(x3, y3);
    glEnd();

    _pos = lc::geo::Coordinate(x3, y3);
}

void LCViewer::OpenGLPainter::save() {
    glPushMatrix();
}

void LCViewer::OpenGLPainter::restore() {
    glPopMatrix();
}

long LCViewer::OpenGLPainter::pattern_create_linear(double x1, double y1, double x2, double y2) {
    auto id = _nextID;
    _nextID++;

    Pattern newPattern;
    newPattern.area = lc::geo::Area(lc::geo::Coordinate(x1, y1), lc::geo::Coordinate(x2, y2));

    _patterns[id] = newPattern;

    return id;
}

void LCViewer::OpenGLPainter::pattern_add_color_stop_rgba(long pat, double offset, double r, double g, double b, double a) {
    _patterns.at(pat).colors[offset] = lc::Color(r, g, b, a);
}

void LCViewer::OpenGLPainter::set_pattern_source(long pat) {
    if(_patterns.find(pat) != _patterns.end()) {
        _selectedPattern = pat;
    }
}

void LCViewer::OpenGLPainter::pattern_destroy(long pat) {
    if(_selectedPattern == pat) {
        pat = -1;
    }

    _patterns.erase(pat);
}

void LCViewer::OpenGLPainter::fill() {
    if(_path.size() == 4 && _selectedPattern != -1) {
        //That's the only gradient we need atm
        auto pattern = _patterns.at(_selectedPattern);

        auto startColor = pattern.colors.begin()->second;
        auto endColor = pattern.colors.end()->second;
        auto middleColor = lc::Color(
                (startColor.red() + endColor.red()) / 2,
                (startColor.green() + endColor.green()) / 2,
                (startColor.blue() + endColor.blue()) / 2,
                (startColor.alpha() + endColor.alpha()) / 2
        );

        glBegin(GL_QUADS);
            glColor4d(startColor.red(), startColor.green(), startColor.blue(), startColor.alpha());
            glVertex2d(_path[0].x(), _path[0].y());

            glColor4d(startColor.red(), middleColor.green(), middleColor.blue(), middleColor.alpha());
            glVertex2d(_path[1].x(), _path[1].y());

            glColor4d(startColor.red(), endColor.green(), endColor.blue(), endColor.alpha());
            glVertex2d(_path[2].x(), _path[2].y());

            glColor4d(startColor.red(), middleColor.green(), middleColor.blue(), middleColor.alpha());
            glVertex2d(_path[3].x(), _path[3].y());
        glEnd();
    }
    else {
        glBegin(GL_POLYGON);
            for(auto coordinate : _path) {
                glVertex2d(coordinate.x(), coordinate.y());
            }
        glEnd();
    }
}

void LCViewer::OpenGLPainter::point(double x, double y, double size, bool deviceCoords) {

}

void LCViewer::OpenGLPainter::reset_transformations() {
    glLoadIdentity();
}

unsigned char* LCViewer::OpenGLPainter::data() {
    return nullptr;
}

void LCViewer::OpenGLPainter::set_dash(const double* dashes, const int num_dashes, double offset, bool scaled) {

}

long LCViewer::OpenGLPainter::image_create(const std::string& file) {
    return 0;
}

void LCViewer::OpenGLPainter::image_destroy(long image) {

}

void LCViewer::OpenGLPainter::image(long image, double uvx, double vy, double vvx, double vvy, double x, double y) {

}

void LCViewer::OpenGLPainter::disable_antialias() {
    glDisable(GL_MULTISAMPLE);
}

void LCViewer::OpenGLPainter::enable_antialias() {
    glEnable(GL_MULTISAMPLE);
}

void LCViewer::OpenGLPainter::getTranslate(double* x, double* y) {
    GLdouble modelview[16];
    glGetDoublev(GL_MODELVIEW_MATRIX, modelview);

    (*x) = modelview[3];
    (*y) = - modelview[7];
}