#include <kandinsky/context.h>

constexpr KDCoordinate min(KDCoordinate x, KDCoordinate y) {
    return (x < y)? x : y;
}

constexpr KDCoordinate max(KDCoordinate x, KDCoordinate y) {
    return (x > y)? x : y;
}

bool insidePolygon(const KDPoint *polygonv, uint32_t polygonc, const KDPoint &point) {
    int counter = 0;
    
    KDPoint p1 = polygonv[0];

    for (uint32_t i = 1; i <= polygonc; i++) {
        KDPoint p2 = polygonv[i % polygonc];

        if (point.y() > min(p1.y(), p2.y())) {
            if (point.y() <= max(p1.y(), p2.y())) {
                if (point.x() <= max(p1.x(), p2.x())) {
                    if (p1.y() != p2.y()) {
                        double xinters = (point.y()-p1.y())*(p2.x()-p1.x())/(p2.y()-p1.y())+p1.x();

                        if (p1.x() == p2.x() || point.x() <= xinters) counter ++;
                    }
                }
            }
        }

        p1 = p2;
    }

    if (counter % 2 == 0)
        return false;
    return true;
}

void KDContext::drawPolygon(const KDPoint *points, uint32_t number, KDColor color) {
    if (points == nullptr || number == 0) return;

    KDCoordinate min_x = points[0].x();
    KDCoordinate max_x = min_x;
    KDCoordinate min_y = points[0].y();
    KDCoordinate max_y = min_y;

    for (uint32_t i = 1; i < number; i++) {
        min_x = min(min_x, points[i].x());
        max_x = max(max_x, points[i].x());
        min_y = min(min_y, points[i].y());
        max_y = max(max_y, points[i].y());
    }

    min_x = max(min_x, 0);
    max_x = min(max_x, m_clippingRect.width());
    
    min_y = max(min_y, 0);
    max_y = min(max_y, m_clippingRect.height());

    for (KDCoordinate x = min_x; x <= max_x; x++) {
        for (KDCoordinate y = min_y; y <= max_y; y++) {
            if (insidePolygon(points, number, KDPoint(x,y))) {
                setPixel(KDPoint(x, y), color);
            }
        }
    }
}