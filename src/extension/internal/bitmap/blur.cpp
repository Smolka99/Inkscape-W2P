// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * Copyright (C) 2007 Authors:
 *   Christopher Brown <audiere@gmail.com>
 *   Ted Gould <ted@gould.cx>
 *
 * Released under GNU GPL v2+, read the file 'COPYING' for more information.
 */

#include "extension/effect.h"
#include "extension/system.h"

#include "blur.h"
#include <Magick++.h>

namespace Inkscape {
namespace Extension {
namespace Internal {
namespace Bitmap {
	
void
Blur::applyEffect(Magick::Image *image) {
	image->blur(_radius, _sigma);
}

void
Blur::refreshParameters(Inkscape::Extension::Effect *module) {	
	_radius = module->get_param_float("radius");
	_sigma = module->get_param_float("sigma");
}

#include "../clear-n_.h"

void
Blur::init()
{
    // clang-format off
    Inkscape::Extension::build_from_mem(
        "<inkscape-extension xmlns=\"" INKSCAPE_EXTENSION_URI "\">\n"
            "<name>" N_("Blur") "</name>\n"
            "<id>org.inkscape.effect.bitmap.blur</id>\n"
            "<param name=\"radius\" gui-text=\"" N_("Radius:") "\" type=\"float\" min=\"0\" max=\"100\">1</param>\n"
            "<param name=\"sigma\"  gui-text=\"" N_("Sigma:") "\"  type=\"float\" min=\"0\" max=\"100\">0.5</param>\n"
            "<effect>\n"
                "<object-type>all</object-type>\n"
                "<effects-menu>\n"
                    "<submenu name=\"" N_("Raster") "\" />\n"
                "</effects-menu>\n"
                "<menu-tip>" N_("Blur selected bitmap(s)") "</menu-tip>\n"
            "</effect>\n"
        "</inkscape-extension>\n", std::make_unique<Blur>());
    // clang-format on
}

}; /* namespace Bitmap */
}; /* namespace Internal */
}; /* namespace Extension */
}; /* namespace Inkscape */
