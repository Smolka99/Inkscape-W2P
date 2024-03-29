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

#include "threshold.h"
#include <Magick++.h>

namespace Inkscape {
namespace Extension {
namespace Internal {
namespace Bitmap {
	
void
Threshold::applyEffect(Magick::Image* image) {
	image->threshold(_threshold);
}

void
Threshold::refreshParameters(Inkscape::Extension::Effect* module) {
	_threshold = module->get_param_float("threshold");
}

#include "../clear-n_.h"

void
Threshold::init()
{
    // clang-format off
    Inkscape::Extension::build_from_mem(
        "<inkscape-extension xmlns=\"" INKSCAPE_EXTENSION_URI "\">\n"
            // TRANSLATORS: see http://docs.gimp.org/en/gimp-tool-threshold.html
            "<name>" N_("Threshold") "</name>\n"
            "<id>org.inkscape.effect.bitmap.threshold</id>\n"
            "<param name=\"threshold\" gui-text=\"" N_("Threshold:") "\" type=\"float\" min=\"-100.0\" max=\"100.0\"></param>\n"
            "<effect>\n"
                "<object-type>all</object-type>\n"
                "<effects-menu>\n"
                    "<submenu name=\"" N_("Raster") "\" />\n"
                "</effects-menu>\n"
                "<menu-tip>" N_("Threshold selected bitmap(s)") "</menu-tip>\n"
            "</effect>\n"
        "</inkscape-extension>\n", std::make_unique<Threshold>());
    // clang-format on
}

}; /* namespace Bitmap */
}; /* namespace Internal */
}; /* namespace Extension */
}; /* namespace Inkscape */
