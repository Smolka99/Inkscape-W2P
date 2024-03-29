// SPDX-License-Identifier: GPL-2.0-or-later
/**
 *  \file src/snapped-point.cpp
 *  SnappedPoint class.
 *
 *  Authors:
 *    Mathieu Dimanche <mdimanche@free.fr>
 *    Diederik van Lierop <mail@diedenrezi.nl>
 *
 *  Released under GNU GPL v2+, read the file 'COPYING' for more information.
 */

#include <utility>
#include "snapped-point.h"
#include "preferences.h"

// overloaded constructor
Inkscape::SnappedPoint::SnappedPoint(Geom::Point const &p, SnapSourceType const &source, long source_num, SnapTargetType const &target, Geom::Coord const &d, Geom::Coord const &t, bool const &a, bool const &constrained_snap, bool const &fully_constrained, Geom::OptRect target_bbox) :
    _point(p),
    //_alignment_target(Geom::Point(0,0)),
    //_alignment_target2(Geom::Point(0,0)),
    _equal_distance(Geom::infinity()),
    _tangent(Geom::Point(0,0)),
    _source(source),
    _source_num(source_num),
    _target(target),
    _at_intersection (false),
    _constrained_snap (constrained_snap),
    _fully_constrained (fully_constrained),
    _distance(d),
    _tolerance(std::max(t,1.0)),// tolerance should never be smaller than 1 px, as it is used for normalization in isOtherSnapBetter. We don't want a division by zero.
    _always_snap(a),
    _second_distance (Geom::infinity()),
    _second_tolerance (1),
    _second_always_snap (false),
    _target_bbox(std::move(target_bbox)),
    _pointer_distance (Geom::infinity())
{
}

Inkscape::SnappedPoint::SnappedPoint(Geom::Point const &p, Geom::Point const &ap, SnapSourceType const &source, long source_num, SnapTargetType const &target, Geom::Coord const &d, Geom::Coord const &t, bool const &a, bool const &constrained_snap, bool const &fully_constrained, Geom::OptRect target_bbox) :
    _point(p),
    _alignment_target(ap),
    //_alignment_target2(Geom::Point(0,0)),
    _alignment_target_type(target),
    _equal_distance(Geom::infinity()),
    _tangent(Geom::Point(0,0)),
    _source(source),
    _source_num(source_num),
    _target(target),
    _at_intersection (false),
    _constrained_snap (constrained_snap),
    _fully_constrained (fully_constrained),
    _distance(d),
    _tolerance(std::max(t,1.0)),// tolerance should never be smaller than 1 px, as it is used for normalization in isOtherSnapBetter. We don't want a division by zero.
    _always_snap(a),
    _second_distance (Geom::infinity()),
    _second_tolerance (1),
    _second_always_snap (false),
    _target_bbox(std::move(target_bbox)),
    _pointer_distance (Geom::infinity())
{
}

Inkscape::SnappedPoint::SnappedPoint(Geom::Point const &p, Geom::Point const &ap, Geom::Point const &ap2, SnapSourceType const &source, long source_num, SnapTargetType const &target, Geom::Coord const &d, Geom::Coord const &t, bool const &a, bool const &constrained_snap, bool const &fully_constrained, Geom::OptRect target_bbox) :
    _point(p),
    _alignment_target(ap),
    _alignment_target2(ap2),
    _alignment_target_type(target),
    _equal_distance(Geom::infinity()),
    _tangent(Geom::Point(0,0)),
    _source(source),
    _source_num(source_num),
    _target(target),
    _at_intersection (false),
    _constrained_snap (constrained_snap),
    _fully_constrained (fully_constrained),
    _distance(d),
    _tolerance(std::max(t,1.0)),// tolerance should never be smaller than 1 px, as it is used for normalization in isOtherSnapBetter. We don't want a division by zero.
    _always_snap(a),
    _second_distance (Geom::infinity()),
    _second_tolerance (1),
    _second_always_snap (false),
    _target_bbox(std::move(target_bbox)),
    _pointer_distance (Geom::infinity())
{
}

Inkscape::SnappedPoint::SnappedPoint(Geom::Point const &p, std::vector<Geom::Rect> const &bboxes, Geom::Rect const &source_bbox, Geom::Coord equal_dist, SnapSourceType const &source, long source_num, SnapTargetType const &target, Geom::Coord const &d, Geom::Coord const &t, bool const &a, bool const &constrained_snap, bool const &fully_constrained) :
    _point(p),
    _equal_distance(equal_dist),
    _distribution_bboxes(std::move(bboxes)), 
    _source_bbox(source_bbox),
    _tangent(Geom::Point(0,0)),
    _source(source),
    _source_num(source_num),
    _target(target),
    _at_intersection (false),
    _constrained_snap (constrained_snap),
    _fully_constrained (fully_constrained),
    _distance(d),
    _tolerance(std::max(t,1.0)),// tolerance should never be smaller than 1 px, as it is used for normalization in isOtherSnapBetter. We don't want a division by zero.
    _always_snap(a),
    _second_distance (Geom::infinity()),
    _second_tolerance (1),
    _second_always_snap (false),
    //_target_bbox(std::move(target_bbox)),
    _pointer_distance (Geom::infinity())
{
}

Inkscape::SnappedPoint::SnappedPoint(Geom::Point const &p, std::vector<Geom::Rect> const &bboxes, std::vector<Geom::Rect> const &bboxes2, Geom::Rect const &source_bbox, Geom::Coord equal_dist, Geom::Coord equal_dist2, SnapSourceType const &source, long source_num, SnapTargetType const &target, Geom::Coord const &d, Geom::Coord const &t, bool const &a, bool const &constrained_snap, bool const &fully_constrained) :
    _point(p),
    _equal_distance(equal_dist),
    _equal_distance2(equal_dist2),
    _distribution_bboxes(std::move(bboxes)), 
    _distribution_bboxes2(std::move(bboxes2)), 
    _source_bbox(source_bbox),
    _tangent(Geom::Point(0,0)),
    _source(source),
    _source_num(source_num),
    _target(target),
    _at_intersection (false),
    _constrained_snap (constrained_snap),
    _fully_constrained (fully_constrained),
    _distance(d),
    _tolerance(std::max(t,1.0)),// tolerance should never be smaller than 1 px, as it is used for normalization in isOtherSnapBetter. We don't want a division by zero.
    _always_snap(a),
    _second_distance (Geom::infinity()),
    _second_tolerance (1),
    _second_always_snap (false),
    //_target_bbox(std::move(target_bbox)),
    _pointer_distance (Geom::infinity())
{
}

Inkscape::SnappedPoint::SnappedPoint(Inkscape::SnapCandidatePoint const &p, SnapTargetType const &target, Geom::Coord const &d, Geom::Coord const &t, bool const &a, bool const &constrained_snap, bool const &fully_constrained) :
    _point (p.getPoint()),
    _equal_distance(Geom::infinity()),
    _tangent (Geom::Point(0,0)),
    _source (p.getSourceType()),
    _source_num (p.getSourceNum()),
    _target(target),
    _at_intersection (false),
    _constrained_snap (constrained_snap),
    _fully_constrained (fully_constrained),
    _distance(d),
    _tolerance(std::max(t,1.0)),
    _always_snap(a),
    _second_distance (Geom::infinity()),
    _second_tolerance (1),
    _second_always_snap (false),
    _target_bbox (p.getTargetBBox()),
    _pointer_distance (Geom::infinity())
{
}

Inkscape::SnappedPoint::SnappedPoint(Geom::Point const &p, SnapSourceType const &source, long source_num, SnapTargetType const &target, Geom::Coord const &d, Geom::Coord const &t, bool const &a, bool const &at_intersection, bool const &constrained_snap, bool const &fully_constrained, Geom::Coord const &d2, Geom::Coord const &t2, bool const &a2) :
    _point(p),
    _equal_distance(Geom::infinity()),
    _tangent (Geom::Point(0,0)),
    _source(source),
    _source_num(source_num),
    _target(target),
    _at_intersection(at_intersection),
    _constrained_snap(constrained_snap),
    _fully_constrained(fully_constrained),
    _distance(d),
    _tolerance(std::max(t,1.0)),
    _always_snap(a),
    _second_distance(d2),
    _second_tolerance(std::max(t2,1.0)),
    _second_always_snap(a2),
    // tolerance should never be smaller than 1 px, as it is used for normalization in
    // isOtherSnapBetter. We don't want a division by zero.
    _target_bbox (Geom::OptRect()),
    _pointer_distance (Geom::infinity())
{
}

Inkscape::SnappedPoint::SnappedPoint():
    _point (Geom::Point(0,0)),
    _equal_distance(Geom::infinity()),
    _tangent (Geom::Point(0,0)),
    _source (SNAPSOURCE_UNDEFINED),
    _source_num (-1),
    _target (SNAPTARGET_UNDEFINED),
    _at_intersection (false),
    _constrained_snap (false),
    _fully_constrained (false),
    _distance (Geom::infinity()),
    _tolerance (1),
    _always_snap (false),
    _second_distance (Geom::infinity()),
    _second_tolerance (1),
    _second_always_snap (false),
    _target_bbox (Geom::OptRect()),
    _pointer_distance (Geom::infinity())
{
}

Inkscape::SnappedPoint::SnappedPoint(Geom::Point const &p):
    _point (p),
    _equal_distance(Geom::infinity()),
    _tangent (Geom::Point(0,0)),
    _source (SNAPSOURCE_UNDEFINED),
    _source_num (-1),
    _target (SNAPTARGET_UNDEFINED),
    _at_intersection (false),
    _constrained_snap (false),
    _fully_constrained (false),
    _distance (Geom::infinity()),
    _tolerance (1),
    _always_snap (false),
    _second_distance (Geom::infinity()),
    _second_tolerance (1),
    _second_always_snap (false),
    _target_bbox (Geom::OptRect()),
    _pointer_distance (Geom::infinity())
{
}

Inkscape::SnappedPoint::~SnappedPoint()
= default;

void Inkscape::SnappedPoint::getPointIfSnapped(Geom::Point &p) const
{
    // When we have snapped
    if (getSnapped()) {
        // then return the snapped point by overwriting p
        p = _point;
    } //otherwise p will be left untouched; this way the caller doesn't have to check whether we've snapped
}

// search for the closest snapped point
// This function give preference to the snapped points that are not in SNAPTARGET_ALIGNMENT_CATEGORY
// ie. for example, a longer Corner to Corner snap will be given prefrence over 
// a Corner to alignment snap with lesser snapDistance.
//
// Incase there is an alignment snap along with a distribution snap possible, this
// function returns a new SnappedPoint that is are mix of the both
bool getClosestSP(std::list<Inkscape::SnappedPoint> const &list, Inkscape::SnappedPoint &result)
{
    bool success = false;
    bool aligned_success = false;

    Inkscape::SnappedPoint aligned;

    for (std::list<Inkscape::SnappedPoint>::const_iterator i = list.begin(); i != list.end(); ++i) {
        bool alignment = (*i).getTarget() & Inkscape::SNAPTARGET_ALIGNMENT_CATEGORY;
        if (i == list.begin()) {
            result = *i;
            success = !alignment;
            aligned = *i;
            aligned_success = alignment;
        } else if (alignment) {
            if (!aligned_success || (*i).getSnapDistance() <= aligned.getSnapDistance()) {
                if ((*i).getSnapDistance() == aligned.getSnapDistance()) {
                    if ((*i).getDistanceToAlignTarget() < aligned.getDistanceToAlignTarget()) {
                        aligned = *i;
                        aligned_success = true; 
                    }
                } else {
                        aligned = *i;
                        aligned_success = true; 
                }
           }
        } else if (!success || (*i).getSnapDistance() < result.getSnapDistance()){
            result = *i;
            success = true;
        }
        
    }

    if (!success && aligned_success)
        result = aligned;

    // the following code merges an alignment snap and a distribution snap
    if (success && aligned_success) {
        bool align_intersection = aligned.getTarget() == Inkscape::SNAPTARGET_ALIGNMENT_INTERSECTION;
        auto type = result.getTarget();

        if (type & Inkscape::SNAPTARGET_DISTRIBUTION_CATEGORY) {
            switch (type) {
                case Inkscape::SNAPTARGET_DISTRIBUTION_X:
                case Inkscape::SNAPTARGET_DISTRIBUTION_RIGHT:
                case Inkscape::SNAPTARGET_DISTRIBUTION_LEFT:
                    if (aligned.getPoint().y() == aligned.getAlignmentTarget()->y()) {
                        result.setPoint({result.getPoint().x(), aligned.getPoint().y()});
                        result.setAlignmentTargetType(Inkscape::SNAPTARGET_ALIGNMENT_BBOX_CORNER);
                        result.setAlignmentTarget(aligned.getAlignmentTarget());

                        if (align_intersection && abs(result.getPoint().x() - aligned.getAlignmentTarget2()->x()) <1e-4) {
                            result.setPoint(aligned.getPoint());
                            result.setAlignmentTarget2(aligned.getAlignmentTarget2());
                            result.setAlignmentTargetType(aligned.getAlignmentTargetType());
                        }
                    }
                    break;

                case Inkscape::SNAPTARGET_DISTRIBUTION_Y:
                case Inkscape::SNAPTARGET_DISTRIBUTION_UP:
                case Inkscape::SNAPTARGET_DISTRIBUTION_DOWN:
                    if (!align_intersection) {
                        if (aligned.getPoint().x() == aligned.getAlignmentTarget()->x()) {
                            result.setPoint({aligned.getPoint().x(), result.getPoint().y()});
                            result.setAlignmentTargetType(aligned.getAlignmentTargetType());
                            result.setAlignmentTarget(aligned.getAlignmentTarget());
                        }
                    } else if (aligned.getPoint().x() == aligned.getAlignmentTarget2()->x()) {
                            result.setPoint({aligned.getPoint().x(), result.getPoint().y()});
                            result.setAlignmentTargetType(Inkscape::SNAPTARGET_ALIGNMENT_BBOX_CORNER);
                            result.setAlignmentTarget(aligned.getAlignmentTarget2());

                            if (abs(result.getPoint().y() - aligned.getAlignmentTarget()->y()) <1e-4) {
                                result.setPoint(aligned.getPoint());
                                result.setAlignmentTarget2(aligned.getAlignmentTarget());
                                result.setAlignmentTargetType(aligned.getAlignmentTargetType());
                            }
                    }
                    break;

                case Inkscape::SNAPTARGET_DISTRIBUTION_XY:
                    if (Geom::L2(result.getPoint() - aligned.getPoint()) < 1e-4) {
                        result.setPoint(aligned.getPoint());
                        result.setAlignmentTargetType(aligned.getAlignmentTargetType());
                        result.setAlignmentTarget(aligned.getAlignmentTarget());
                        result.setAlignmentTarget2(aligned.getAlignmentTarget2());
                    }
                    break;

                default:
                    g_warning("getClosestSP(): unknown distribution snap target %i", result.getTarget());
                    break;
            }

            return true;
        }
    }

    return success ? success : aligned_success;
}

bool Inkscape::SnappedPoint::isOtherSnapBetter(Inkscape::SnappedPoint const &other_one, bool weighted) const
{

    if (getSnapped() && !other_one.getSnapped()) {
        return false;
    }

    if (!getSnapped() && other_one.getSnapped()) {
        return true;
    }

    double dist_other = other_one.getSnapDistance();
    double dist_this = getSnapDistance();

    // The distance to the pointer should only be taken into account when finding the best snapped source node (when
    // there's more than one). It is not useful when trying to find the best snapped target point.
    // (both the snap distance and the pointer distance are measured in document pixels, not in screen pixels)
    if (weighted) {
        Geom::Coord const dist_pointer_other = other_one.getPointerDistance();
        Geom::Coord const dist_pointer_this = getPointerDistance();
        // Weight factor: controls which node should be preferred for snapping, which is either
        // the node with the closest snap (w = 0), or the node closest to the mousepointer (w = 1)
        Inkscape::Preferences *prefs = Inkscape::Preferences::get();
        double w = prefs->getDoubleLimited("/options/snapweight/value", 0.5, 0, 1);
        if (prefs->getBool("/options/snapclosestonly/value", false)) {
            w = 1;
        }
        if (w > 0) {
            if (!(w == 1 && dist_pointer_this == dist_pointer_other)) {
                // When accounting for the distance to the mouse pointer, then at least one of the snapped points should
                // have that distance set. If not, then this is a bug. Either "weighted" must be set to false, or the
                // mouse pointer distance must be set.
                g_assert(dist_pointer_this != Geom::infinity() || dist_pointer_other != Geom::infinity());
                // The snap distance will always be smaller than the tolerance set for the snapper. The pointer distance can
                // however be very large. To compare these in a fair way, we will have to normalize these metrics first
                // The closest pointer distance will be normalized to 1.0; the other one will be > 1.0
                // The snap distance will be normalized to 1.0 if it's equal to the snapper tolerance
                double const norm_p = std::min(dist_pointer_this, dist_pointer_other) + 1;
                // make sure norm_p is never too close to zero (e.g. when snapping the bbox-corner that was grabbed), by incr. with 1
                double const norm_t_other = std::min(50.0, other_one.getTolerance());
                double const norm_t_this = std::min(50.0, getTolerance());
                dist_other = w * dist_pointer_other / norm_p + (1-w) * dist_other / norm_t_other;
                dist_this = w * dist_pointer_this / norm_p + (1-w) * dist_this / norm_t_this;
            }
        }
    }

    // When snapping to a constraint line only, which is not really a snap but merely a projection
    // to the constraint line, then give this snap a very low priority. Basically, any other snap will do
    if (other_one.getTarget() == SNAPTARGET_CONSTRAINT) {
        dist_other += 1e6;
    }
    if (getTarget() == SNAPTARGET_CONSTRAINT) {
        dist_this += 1e6;
    }

    // If it's closer
    bool c1 = dist_other < dist_this;
    // or, if it's for a snapper with "always snap" turned on, and the previous wasn't
    bool c2 = other_one.getAlwaysSnap() && !getAlwaysSnap();
    // But in no case fall back from a snapper with "always snap" on to one with "always snap" off
    bool c2n = !other_one.getAlwaysSnap() && getAlwaysSnap();
    // or, if we have a fully constrained snappoint (e.g. to a node or an intersection), while the previous one was only partly constrained (e.g. to a line)
    bool c3 = (other_one.getFullyConstrained() && !other_one.getConstrainedSnap()) && !getFullyConstrained(); // Do not consider constrained snaps here, because these will always be fully constrained anyway
    // But in no case fall back; (has less priority than c3n, so it is allowed to fall back when c3 is true, see below)
    bool c3n = !other_one.getFullyConstrained() && (getFullyConstrained() && !getConstrainedSnap());

    // When both are fully constrained AND coincident, then prefer nodes over intersections
    bool d = other_one.getFullyConstrained() && getFullyConstrained() && (Geom::L2(other_one.getPoint() - getPoint()) < 1e-9);
    bool c4 = d && !other_one.getAtIntersection() && getAtIntersection();
    // But don't fall back...
    bool c4n = d && other_one.getAtIntersection() && !getAtIntersection();

    // or, if it's just as close then consider the second distance ...
    bool c5a = (dist_other == dist_this);
    bool c5b = (other_one.getSecondSnapDistance() < getSecondSnapDistance()) && (getSecondSnapDistance() < Geom::infinity());
    // ... or prefer free snaps over constrained snaps
    bool c5c = !other_one.getConstrainedSnap() && getConstrainedSnap();

    bool other_is_better = (c1 || c2 || c3 || c4 || (c5a && (c5b || c5c))) && !c2n && (!c3n || c2) && !c4n;

    /*
    std::cout << other_one.getPoint() << " (Other one, dist = " << dist_other << ") vs. " << getPoint() << " (this one, dist = " << dist_this << ") ---> ";
    std::cout << "c1 = " << c1 << " | c2 = " << c2 << " | c2n = " << c2n << " | c3 = " << c3 << " | c3n = " << c3n << " | c4 = " << c4 << " | c4n = " << c4n << " | c5a = " << c5a << " | c5b = " << c5b << " | c5c = " << c5c << std::endl;
    std::cout << "Other one provides a better snap: " << other_is_better << std::endl;
    */

    return other_is_better;
}

// Returns true if the snapped point is on a path (or on a line); returns false for e.g. bounding box edges, page border, and text baseline
bool Inkscape::SnappedPoint::getOnPath() const {
    std::vector<Inkscape::SnapTargetType> snaptargets_on_path{
        Inkscape::SNAPTARGET_LINE_MIDPOINT,
        Inkscape::SNAPTARGET_PATH,
        Inkscape::SNAPTARGET_PATH_PERPENDICULAR,
        Inkscape::SNAPTARGET_PATH_TANGENTIAL,
        Inkscape::SNAPTARGET_PATH_INTERSECTION,
        Inkscape::SNAPTARGET_PATH_GUIDE_INTERSECTION,
        Inkscape::SNAPTARGET_PATH_CLIP,
        Inkscape::SNAPTARGET_PATH_MASK,
        Inkscape::SNAPTARGET_ELLIPSE_QUADRANT_POINT
    };

    return std::find(snaptargets_on_path.begin(), snaptargets_on_path.end(), _target) != snaptargets_on_path.end();
}


/*
  Local Variables:
  mode:c++
  c-file-style:"stroustrup"
  c-file-offsets:((innamespace . 0)(inline-open . 0)(case-label . +))
  indent-tabs-mode:nil
  fill-column:99
  End:
*/
// vim: filetype=cpp:expandtab:shiftwidth=4:tabstop=8:softtabstop=4 :
