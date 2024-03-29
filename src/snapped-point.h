// SPDX-License-Identifier: GPL-2.0-or-later
#ifndef SEEN_SNAPPEDPOINT_H
#define SEEN_SNAPPEDPOINT_H

/**
 *    \file src/snapped-point.h
 *    \brief SnappedPoint class.
 *
 *    Authors:
 *      Mathieu Dimanche <mdimanche@free.fr>
 *      Diederik van Lierop <mail@diedenrezi.nl>
 *
 *    Released under GNU GPL v2+, read the file 'COPYING' for more information.
 */

#include <2geom/geom.h>
#include <list>
#include <optional>
#include <vector>
#include <glib.h>

#include "snap-candidate.h"

namespace Inkscape
{

/// Class describing the result of an attempt to snap.
class SnappedPoint
{

public:
    SnappedPoint();
    SnappedPoint(Geom::Point const &p);
    SnappedPoint(Geom::Point const &p, SnapSourceType const &source, long source_num, SnapTargetType const &target, Geom::Coord const &d, Geom::Coord const &t, bool const &a, bool const &at_intersection, bool const &constrained_snap, bool const &fully_constrained, Geom::Coord const &d2, Geom::Coord const &t2, bool const &a2);
    SnappedPoint(Geom::Point const &p, SnapSourceType const &source, long source_num, SnapTargetType const &target, Geom::Coord const &d, Geom::Coord const &t, bool const &a, bool const &constrained_snap, bool const &fully_constrained, Geom::OptRect target_bbox = Geom::OptRect());
    SnappedPoint(Geom::Point const &p, Geom::Point const &ap, SnapSourceType const &source, long source_num, SnapTargetType const &target, Geom::Coord const &d, Geom::Coord const &t, bool const &a, bool const &constrained_snap, bool const &fully_constrained, Geom::OptRect target_bbox); 
    SnappedPoint(Geom::Point const &p, Geom::Point const &ap, Geom::Point const &ap2, SnapSourceType const &source, long source_num, SnapTargetType const &target, Geom::Coord const &d, Geom::Coord const &t, bool const &a, bool const &constrained_snap, bool const &fully_constrained, Geom::OptRect target_bbox); 
    SnappedPoint(Geom::Point const &p, std::vector<Geom::Rect> const &bboxes, Geom::Rect const &source_bbox, Geom::Coord equal_dist, SnapSourceType const &source, long source_num, SnapTargetType const &target, Geom::Coord const &d, Geom::Coord const &t, bool const &a, bool const &constrained_snap, bool const &fully_constrained); 
    SnappedPoint(Geom::Point const &p, std::vector<Geom::Rect> const &bboxes,std::vector<Geom::Rect> const &bboxes2, Geom::Rect const &source_bbox, Geom::Coord equal_dist, Geom::Coord equal_dist2, SnapSourceType const &source, long source_num, SnapTargetType const &target, Geom::Coord const &d, Geom::Coord const &t, bool const &a, bool const &constrained_snap, bool const &fully_constrained); 
    SnappedPoint(SnapCandidatePoint const &p, SnapTargetType const &target, Geom::Coord const &d, Geom::Coord const &t, bool const &a, bool const &constrained_snap, bool const &fully_constrained);
    ~SnappedPoint();

    Geom::Coord getSnapDistance() const {return _distance;}
    void setSnapDistance(Geom::Coord const d) {_distance = d;}
    Geom::Coord getTolerance() const {return _tolerance;}
    bool getAlwaysSnap() const {return _always_snap;}
    Geom::Coord getSecondSnapDistance() const {return _second_distance;}
    void setSecondSnapDistance(Geom::Coord const d) {_second_distance = d;}
    Geom::Coord getSecondTolerance() const {return _second_tolerance;}
    bool getSecondAlwaysSnap() const {return _second_always_snap;}
    Geom::Coord getPointerDistance() const {return _pointer_distance;}
    void setPointerDistance(Geom::Coord const d) {_pointer_distance = d;}

    std::vector<Geom::Rect> const &getBBoxes() const {return _distribution_bboxes;}
    std::vector<Geom::Rect> const &getBBoxes2() const {return _distribution_bboxes2;}

    /* This is the preferred method to find out which point we have snapped
     * to, because it only returns a point if snapping has actually occurred
     * (by overwriting p)
     */
    void getPointIfSnapped(Geom::Point &p) const;

    /* This method however always returns a point, even if no snapping
     * has occurred; A check should be implemented in the calling code
     * to check for snapping. Use this method only when really needed, e.g.
     * when the calling code is trying to snap multiple points and must
     * determine itself which point is most appropriate, or when doing a
     * constrainedSnap that also enforces projection onto the constraint (in
     * which case you need the new point anyway, even if we didn't snap)
     */
    Geom::Point getPoint() const {return _point;}
    void setPoint(Geom::Point const &p) {_point = p;}
    void setAlignmentTarget(std::optional<Geom::Point> const &p) {
        if (p.has_value())
            _alignment_target = p;
    }
    void setAlignmentTarget2(std::optional<Geom::Point> const &p) {
        if (p.has_value())
            _alignment_target2 = p;
    }
    Geom::Point getTangent() const {return _tangent;}
    std::optional<Geom::Point> getAlignmentTarget() const
    {
        if (_alignment_target.has_value())
            return *_alignment_target;
        else
            return {};
    }

    std::optional<Geom::Point> getAlignmentTarget2() const
    {
        if (_alignment_target2.has_value())
            return *_alignment_target2;
        else
            return  {};
    }

    Geom::Coord getDistanceToAlignTarget() const
    {
        return _alignment_target.has_value() ? Geom::L2(_point - *_alignment_target) : Geom::infinity();
    }

    bool getAtIntersection() const {return _at_intersection;}
    bool getFullyConstrained() const {return _fully_constrained;}
    bool getConstrainedSnap() const {return _constrained_snap;}
    bool getSnapped() const {return _distance < Geom::infinity();}
    void setTarget(SnapTargetType const target) {_target = target;}
    void setAlignmentTargetType(SnapTargetType const target) {_alignment_target_type = target;}
    SnapTargetType getTarget() const {return _target;}
    SnapTargetType getAlignmentTargetType() const
    {
        return _alignment_target_type.has_value() ? *_alignment_target_type : SNAPTARGET_UNDEFINED;
    }
    bool getOnPath() const;

    void setTargetBBox(Geom::OptRect const target) {_target_bbox = target;}
    Geom::OptRect const getTargetBBox() const {return _target_bbox;}
    Geom::OptRect const getSourceBBox() const {return _source_bbox;}
    Geom::Coord getDistributionDistance() const {return _equal_distance;}
    Geom::Coord getDistributionDistance2() const {return _equal_distance2;}
    void setSource(SnapSourceType const source) {_source = source;}
    SnapSourceType getSource() const {return _source;}
    long getSourceNum() const {return _source_num;}

    bool isOtherSnapBetter(SnappedPoint const &other_one, bool weighted) const;

    /*void dump() const {
        std::cout << "_point              = " << _point << std::endl;
        std::cout << "_source             = " << _source << std::endl;
        std::cout << "_source_num         = " << _source_num << std::endl;
        std::cout << "_target             = " << _target << std::endl;
        std::cout << "_at_intersection    = " << _at_intersection << std::endl;
        std::cout << "_fully_constrained  = " << _fully_constrained << std::endl;
        std::cout << "_distance           = " << _distance << std::endl;
        std::cout << "_tolerance          = " << _tolerance << std::endl;
        std::cout << "_always_snap        = " << _always_snap << std::endl;
        std::cout << "_second_distance    = " << _second_distance << std::endl;
        std::cout << "_second_tolerance   = " << _second_tolerance << std::endl;
        std::cout << "_second_always_snap = " << _second_always_snap << std::endl;
        std::cout << "_transformation     = " << _transformation << std::endl;
        std::cout << "_pointer_distance   = " << _pointer_distance << std::endl;
    }*/

protected:
    Geom::Point _point; // Location of the snapped point
    Geom::Point _tangent; // Tangent of the curve we snapped to, at the snapped point
    std::optional<Geom::Point> _alignment_target; // Target point for alignment snapping
    std::optional<Geom::Point> _alignment_target2; // Target point when alignment guides intersect
    std::vector<Geom::Rect> _distribution_bboxes; // A list of bounding boxes in case of distribution snapping
    std::vector<Geom::Rect> _distribution_bboxes2; // Target point when there is a bidirectional distribution snap 
    SnapSourceType _source; // Describes what snapped
    long _source_num; // Sequence number of the source point that snapped, if that point is part of a set of points. (starting at zero if we might have a set of points; -1 if we only have a single point)
    SnapTargetType _target; // Describes to what we've snapped to
    std::optional<SnapTargetType> _alignment_target_type; // TODO: this is only used in case of both alignment and distribution snap
    bool _at_intersection; // If true, the snapped point is at an intersection
    bool _constrained_snap; // If true, then the snapped point was found when looking for a constrained snap
    bool _fully_constrained; // When snapping for example to a node, then the snap will be "fully constrained".
                            // When snapping to a line however, the snap is only partly constrained (i.e. only in one dimension)

    /* Distance from original point to snapped point. If the snapped point is at
       an intersection of e.g. two lines, then this is the distance to the closest
       line */
    Geom::Coord _distance;
    /* The snapping tolerance in screen pixels (depends on zoom)*/
    Geom::Coord _tolerance;
    /* If true then "Always snap" is on */
    bool _always_snap;

    /* If the snapped point is at an intersection of e.g. two lines, then this is
       the distance to the farthest line */
    Geom::Coord _second_distance;
    /* The snapping tolerance in screen pixels (depends on zoom)*/
    Geom::Coord _second_tolerance;
    /* The equal distance between objects in screen pixels (depends on zoom) in case of distribution snapping*/
    Geom::Coord _equal_distance;
    /* The equal distance between objects in screen pixels (depends on zoom) in case of bidirectional distribution snapping*/
    Geom::Coord _equal_distance2;
    /* If true then "Always snap" is on */
    bool _second_always_snap;
    /* The bounding box we've snapped to (when applicable); will be used by the snapindicator */
    Geom::OptRect _target_bbox;
    /* The bounding box of the object we've snapped */
    Geom::OptRect _source_bbox;
    /* Distance from the un-transformed point to the mouse pointer, measured at the point in time when dragging started */
    Geom::Coord _pointer_distance;
};

}// end of namespace Inkscape

bool getClosestSP(std::list<Inkscape::SnappedPoint> const &list, Inkscape::SnappedPoint &result);

#endif /* !SEEN_SNAPPEDPOINT_H */

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
