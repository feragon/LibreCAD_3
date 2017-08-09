#pragma once

#include "lcpainter.h"
#include <cad/geometry/geoarea.h>
#include <map>
#include <cad/meta/color.h>

namespace LCViewer {
    class OpenGLPainter : public LcPainter {
        public:
            OpenGLPainter(unsigned int width, unsigned int height);

            void new_path() override;
            void close_path() override;
            void new_sub_path() override;
            void clear(double r, double g, double b) override;
            void clear(double r, double g, double b, double a) override;
            void move_to(double x, double y) override;
            void line_to(double x, double y) override;
            void lineWidthCompensation(double lwc) override;
            void line_width(double lineWidth) override;
            double scale() override;
            void scale(double s) override;
            void rotate(double r) override;
            void arc(double x, double y, double r, double start, double end) override;
            void arcNegative(double x, double y, double r, double start, double end) override;
            void circle(double x, double y, double r) override;
            void ellipse(double cx, double cy, double rx, double ry, double sa, double ea, double ra) override;
            void rectangle(double x1, double y1, double w, double h) override;
            void stroke() override;
            void source_rgb(double r, double g, double b) override;
            void source_rgba(double r, double g, double b, double a) override;
            void translate(double x, double y) override;
            void user_to_device(double* x, double* y) override;
            void device_to_user(double* x, double* y) override;
            void user_to_device_distance(double* dx, double* dy) override;
            void device_to_user_distance(double* dx, double* dy) override;
            void font_size(double size, bool deviceCoords) override;
            void select_font_face(const char* text_val) override;
            void text(const char* text_val) override;
            TextExtends text_extends(const char* text_val) override;
            void quadratic_curve_to(double x1, double y1, double x2, double y2) override;
            void curve_to(double x1, double y1, double x2, double y2, double x3, double y3) override;
            void save() override;
            void restore() override;
            long pattern_create_linear(double x1, double y1, double x2, double y2) override;
            void pattern_add_color_stop_rgba(long pat, double offset, double r, double g, double b, double a) override;
            void set_pattern_source(long pat) override;
            void pattern_destroy(long pat) override;
            void fill() override;
            void point(double x, double y, double size, bool deviceCoords) override;
            void reset_transformations() override;
            unsigned char* data() override;
            void set_dash(const double* dashes, const int num_dashes, double offset, bool scaled) override;
            long image_create(const std::string& file) override;
            void image_destroy(long image) override;
            void image(long image, double uvx, double vy, double vvx, double vvy, double x, double y) override;
            void disable_antialias() override;
            void enable_antialias() override;
            void getTranslate(double* x, double* y) override;

        private:
            struct Pattern {
                lc::geo::Area area;
                std::map<double, lc::Color> colors;
            };

            std::map<long, Pattern> _patterns;
            long _nextID;

            lc::geo::Coordinate _pos;
            std::vector<lc::geo::Coordinate> _path;
            long _selectedPattern;
    };
}