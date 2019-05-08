#include "svgexporter.h"

#include <cassert>
#include <cmath>
#include <cstdint>
#include <regex>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

#include "../rasterizer/rasterizer.h"
#include "../bitmap/rgba.h"
#include "../shape/shape.h"
#include "../shape/circle.h"
#include "../shape/ellipse.h"
#include "../shape/line.h"
#include "../shape/polyline.h"
#include "../shape/quadraticbezier.h"
#include "../shape/rectangle.h"
#include "../shape/rotatedellipse.h"
#include "../shape/rotatedrectangle.h"
#include "../shape/triangle.h"
#include "../shaperesult.h"
#include "../commonutil.h"

namespace
{

std::string getSvgShapeData(const geometrize::Circle& s)
{
    std::stringstream strm;
    strm << "<circle cx=\"" << s.m_x << "\" cy=\"" << s.m_y << "\" r=\"" << s.m_r << "\" " << geometrize::exporter::SVG_STYLE_HOOK << " />";
    return strm.str();
}

std::string getSvgShapeData(const geometrize::Ellipse& s)
{
    std::stringstream strm;
    strm << "<ellipse cx=\"" << s.m_x << "\" cy=\"" << s.m_y << "\" rx=\"" << s.m_rx << "\" ry=\"" << s.m_ry << "\" " << geometrize::exporter::SVG_STYLE_HOOK << " />";
    return strm.str();
}

std::string getSvgShapeData(const geometrize::Line& s)
{
    std::stringstream strm;
    strm << "<line x1=\"" << s.m_x1 << "\" y1=\"" << s.m_y1 << "\" x2=\"" << s.m_x2 << "\" y2=\"" << s.m_y2 << "\" " << geometrize::exporter::SVG_STYLE_HOOK << " />";
    return strm.str();
}

std::string getSvgShapeData(const geometrize::Polyline& s)
{
    std::stringstream strm;
    strm << "<polyline points=\"";
    for(std::size_t i = 0; i < s.m_points.size(); i++) {
        strm << s.m_points[i].first << "," << s.m_points[i].second;
        if(i != s.m_points.size() - 1) {
            strm << " ";
        }
    }
    strm << "\" ";

    strm << geometrize::exporter::SVG_STYLE_HOOK << " ";
    strm << "/>";

    return strm.str();
}

std::string getSvgShapeData(const geometrize::QuadraticBezier& s)
{
    std::stringstream strm;
    strm << "<path d=\"M" << s.m_x1 << " " << s.m_y1 << " Q " << s.m_cx << " " << s.m_cy << " " << s.m_x2 << " " << s.m_y2 << "\" " << geometrize::exporter::SVG_STYLE_HOOK << " />";
    return strm.str();
}

std::string getSvgShapeData(const geometrize::Rectangle& s)
{
    std::stringstream strm;
    strm << "<rect x=\"" << (std::fmin)(s.m_x1, s.m_x2) << "\" y=\"" << (std::fmin)(s.m_y1, s.m_y2) << "\" width=\"" << (std::fmax)(s.m_x1, s.m_x2) - (std::fmin)(s.m_x1, s.m_x2) << "\" height=\"" << (std::fmax)(s.m_y1, s.m_y2) - (std::fmin)(s.m_y1, s.m_y2) << "\" " << geometrize::exporter::SVG_STYLE_HOOK << " />";
    return strm.str();
}

std::string getSvgShapeData(const geometrize::RotatedEllipse& s, const geometrize::exporter::RotatedEllipseSVGExportMode mode)
{
    std::stringstream strm;

    switch(mode) {
    case geometrize::exporter::RotatedEllipseSVGExportMode::ELLIPSE_ITEM:
        {
            strm << "<g transform=\"translate(" << s.m_x << " " << s.m_y << ") rotate(" << s.m_angle << ") scale(" << s.m_rx << " " << s.m_ry << ")\">"
              << "<ellipse cx=\"" << 0 << "\" cy=\"" << 0 << "\" rx=\"" << 1 << "\" ry=\"" << 1 << "\" " << geometrize::exporter::SVG_STYLE_HOOK << " />"
              << "</g>";
        }
        break;
    case geometrize::exporter::RotatedEllipseSVGExportMode::POLYGON:
        {
            const std::size_t pointCount = 20;
            std::vector<std::pair<float, float>> points{geometrize::getPointsOnRotatedEllipse(s, pointCount)};

            strm << "<polygon points=\"";
            for(std::size_t i = 0; i < points.size(); i++) {
                strm << points[i].first << "," << points[i].second;
                if(i != points.size() - 1) {
                    strm << " ";
                }
            }
            strm << "\" " << geometrize::exporter::SVG_STYLE_HOOK << "/>";
        }
        break;
    }

    return strm.str();
}

std::string getSvgShapeData(const geometrize::RotatedRectangle& s)
{
    const std::vector<std::pair<float, float>> points{geometrize::getCornerPoints(s)};
    std::stringstream strm;
    strm << "<polygon points=\"";
    for(std::size_t i = 0; i < points.size(); i++) {
        strm << points[i].first << "," << points[i].second;
        if(i != points.size() - 1) {
            strm << " ";
        }
    }
    strm << "\" " << geometrize::exporter::SVG_STYLE_HOOK << "/>";
    return strm.str();
}

std::string getSvgShapeData(const geometrize::Triangle& s)
{
    std::stringstream strm;
    strm << "<polygon points=\"" << s.m_x1 << "," << s.m_y1 << " " << s.m_x2 << "," << s.m_y2 << " " << s.m_x3 << "," << s.m_y3 << "\" " << geometrize::exporter::SVG_STYLE_HOOK << " " << "/>";
    return strm.str();
}

std::string getSvgShapeData(const geometrize::Shape& s, const geometrize::exporter::SVGExportOptions& options)
{
    switch(s.getType()) {
    case geometrize::ShapeTypes::RECTANGLE:
        return getSvgShapeData(static_cast<const geometrize::Rectangle&>(s));
    case geometrize::ShapeTypes::ROTATED_RECTANGLE:
        return getSvgShapeData(static_cast<const geometrize::RotatedRectangle&>(s));
    case geometrize::ShapeTypes::TRIANGLE:
        return getSvgShapeData(static_cast<const geometrize::Triangle&>(s));
    case geometrize::ShapeTypes::ELLIPSE:
        return getSvgShapeData(static_cast<const geometrize::Ellipse&>(s));
    case geometrize::ShapeTypes::ROTATED_ELLIPSE:
        return getSvgShapeData(static_cast<const geometrize::RotatedEllipse&>(s), options.rotatedEllipseExportMode);
    case geometrize::ShapeTypes::CIRCLE:
        return getSvgShapeData(static_cast<const geometrize::Circle&>(s));
    case geometrize::ShapeTypes::LINE:
        return getSvgShapeData(static_cast<const geometrize::Line&>(s));
    case geometrize::ShapeTypes::QUADRATIC_BEZIER:
        return getSvgShapeData(static_cast<const geometrize::QuadraticBezier&>(s));
    case geometrize::ShapeTypes::POLYLINE:
        return getSvgShapeData(static_cast<const geometrize::Polyline&>(s));
    default:
        assert(0 && "Bad shape type");
        return "";
    }
}

std::string getSVGRgbColorAttrib(const geometrize::rgba color)
{
    std::ostringstream stream;
    stream << "rgb("
           << static_cast<std::int32_t>(color.r) << ","
           << static_cast<std::int32_t>(color.g) << ","
           << static_cast<std::int32_t>(color.b) << ")";
    return stream.str();
}

std::string getSVGStrokeAttrib(const geometrize::rgba color)
{
    std::stringstream stream;
    stream << "stroke=\"" << getSVGRgbColorAttrib(color) << "\"";
    return stream.str();
}

std::string getSVGFillAttrib(const geometrize::rgba color)
{
    std::stringstream stream;
    stream << "fill=\"" << getSVGRgbColorAttrib(color) << "\"";
    return stream.str();
}

std::string getSVGFillOpacityAttrib(const geometrize::rgba color)
{
    std::stringstream stream;
    stream << "fill-opacity=\"" << static_cast<float>(color.a) / 255.0f << "\"";
    return stream.str();
}

std::string getSVGStrokeOpacityAttrib(const geometrize::rgba color)
{
    std::stringstream stream;
    stream << "stroke-opacity=\"" << static_cast<float>(color.a) / 255.0f << "\"";
    return stream.str();
}

std::string getSingleShapeSVGData(const geometrize::rgba& color, const geometrize::Shape& shape, const geometrize::exporter::SVGExportOptions& options)
{
    std::stringstream stream;

    std::string shapeData{getSvgShapeData(shape, options)};
    const geometrize::ShapeTypes shapeType{shape.getType()};

    std::string styles{""};

    styles.append("id=\"" + std::to_string(options.itemId) + "\" ");

    if(shapeType == geometrize::ShapeTypes::LINE
            || shapeType == geometrize::ShapeTypes::POLYLINE
            || shapeType == geometrize::ShapeTypes::QUADRATIC_BEZIER) {
        styles.append(getSVGStrokeAttrib(color));
        styles.append(" stroke-width=\"1\" fill=\"none\" ");
        styles.append(getSVGStrokeOpacityAttrib(color));
    } else {
        styles.append(getSVGFillAttrib(color));
        styles.append(" ");
        styles.append(getSVGFillOpacityAttrib(color));
    }

    shapeData = std::regex_replace(shapeData, std::regex(geometrize::exporter::SVG_STYLE_HOOK), styles);

    stream << shapeData << "\n";

    return stream.str();
}

}

namespace geometrize
{

namespace exporter
{

std::string getSingleShapeSVGData(const geometrize::rgba& color, const geometrize::Shape& shape, SVGExportOptions options)
{
    return ::getSingleShapeSVGData(color, shape, options);
}

std::string exportSingleShapeSVG(const geometrize::rgba& color, const geometrize::Shape& shape, const std::uint32_t width, const std::uint32_t height, SVGExportOptions options)
{
    std::stringstream stream;

    stream << "<?xml version=\"1.0\" standalone=\"no\"?>" << "\n";
    stream << "<svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.2\" baseProfile=\"tiny\" " <<
              "width=\"" << width << "\" " << "height=\"" << height << "\" " <<
              "viewBox=\"" << 0 << " " << 0 << " " << width << " " << height << "\">" << "\n";

    stream << ::getSingleShapeSVGData(color, shape, options);

    stream << "</svg>";

    return stream.str();
}

std::string exportSVG(const std::vector<geometrize::ShapeResult>& data, const std::uint32_t width, const std::uint32_t height, SVGExportOptions options)
{
    std::stringstream stream;

    stream << "<?xml version=\"1.0\" standalone=\"no\"?>" << "\n";
    stream << "<svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.2\" baseProfile=\"tiny\" " <<
              "width=\"" << width << "\" " << "height=\"" << height << "\" " <<
              "viewBox=\"" << 0 << " " << 0 << " " << width << " " << height << "\">" << "\n";

    for(std::size_t i = 0; i < data.size(); i++) {
        options.itemId = i;

        const geometrize::ShapeResult& s(data[i]);

        stream << ::getSingleShapeSVGData(s.color, *(s.shape), options);
    }

    stream << "</svg>";

    return stream.str();
}

}

}
