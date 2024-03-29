// SPDX-License-Identifier: GPL-2.0-or-later
#ifndef INKSCAPE_LIVEPATHEFFECT_PARAMETER_VECTOR_H
#define INKSCAPE_LIVEPATHEFFECT_PARAMETER_VECTOR_H

/*
 * Inkscape::LivePathEffectParameters
 *
 * Copyright (C) Johan Engelen 2008 <j.b.c.engelen@utwente.nl>
 *
 * Released under GNU GPL v2+, read the file 'COPYING' for more information.
 */

#include <glib.h>
#include <2geom/point.h>

#include "live_effects/parameter/parameter.h"
#include "display/control/canvas-item-enums.h"

namespace Inkscape {

namespace LivePathEffect {


class VectorParam : public Parameter {
public:
    VectorParam( const Glib::ustring& label,
                const Glib::ustring& tip,
                const Glib::ustring& key,
                Inkscape::UI::Widget::Registry* wr,
                Effect* effect,
                Geom::Point default_vector = Geom::Point(1,0) );
    ~VectorParam() override;

    Gtk::Widget * param_newWidget() override;
    inline const gchar *handleTip() const { return param_tooltip.c_str(); }

    bool param_readSVGValue(const gchar * strvalue) override;
    Glib::ustring param_getSVGValue() const override;
    Glib::ustring param_getDefaultSVGValue() const override;

    Geom::Point getVector() const { return vector; };
    Geom::Point getOrigin() const { return origin; };
    void setValues(Geom::Point const &new_origin, Geom::Point const &new_vector) { setVector(new_vector); setOrigin(new_origin); };
    void setVector(Geom::Point const &new_vector) { vector = new_vector; };
    void setOrigin(Geom::Point const &new_origin) { origin = new_origin; };
    void param_set_default() override;

    void set_and_write_new_values(Geom::Point const &new_origin, Geom::Point const &new_vector);

    void param_transform_multiply(Geom::Affine const &postmul, bool set) override;

    void set_vector_oncanvas_looks(CanvasItemCtrlShape shape, uint32_t color);
    void set_origin_oncanvas_looks(CanvasItemCtrlShape shape, uint32_t color);
    void set_oncanvas_color(guint32 color);
    void param_update_default(Geom::Point default_point);
    void param_update_default(const gchar * default_point) override;
    bool providesKnotHolderEntities() const override { return true; }
    void addKnotHolderEntities(KnotHolder *knotholder, SPItem *item) override;
    ParamType paramType() const override { return ParamType::VECTOR; };
private:
    VectorParam(const VectorParam&) = delete;
    VectorParam& operator=(const VectorParam&) = delete;

    Geom::Point defvalue;

    Geom::Point origin;
    Geom::Point vector;

    /// The looks of the vector and origin knots oncanvas
    Inkscape::CanvasItemCtrlShape vec_knot_shape = Inkscape::CANVAS_ITEM_CTRL_SHAPE_DIAMOND;
    uint32_t                      vec_knot_color = 0xffffb500;
    Inkscape::CanvasItemCtrlShape ori_knot_shape = Inkscape::CANVAS_ITEM_CTRL_SHAPE_CIRCLE;
    uint32_t                      ori_knot_color = 0xffffb500;

    friend class VectorParamKnotHolderEntity_Origin;
    friend class VectorParamKnotHolderEntity_Vector;
};


} //namespace LivePathEffect

} //namespace Inkscape

#endif
