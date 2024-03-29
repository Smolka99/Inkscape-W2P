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

#include "contrast.h"
#include <Magick++.h>

namespace Inkscape {
namespace Extension {
namespace Internal {
namespace Bitmap {
	
void
Contrast::applyEffect(Magick::Image *image) {
	// the contrast method's argument seems to be binary, so we perform it multiple times
	// to get the desired level of effect
	for (unsigned int i = 0; i < _sharpen; i ++)
    	image->contrast(1);
}

void
Contrast::refreshParameters(Inkscape::Extension::Effect *module) {	
	_sharpen = module->get_param_int("sharpen");
}

#include "../clear-n_.h"

void
Contrast::init()
{
    // clang-format off
    Inkscape::Extension::build_from_mem(
        "<inkscape-extension xmlns=\"" INKSCAPE_EXTENSION_URI "\">\n"
            "<name>" N_("Contrast") "</name>\n"
            "<id>org.inkscape.effect.bitmap.contrast</id>\n"
            "<param name=\"sharpen\" gui-text=\"" N_("Adjust:") "\" type=\"int\" min=\"0\" max=\"10\">0</param>\n"
            "<effect>\n"
                "<object-type>all</object-type>\n"
                "<effects-menu>\n"
                    "<submenu name=\"" N_("Raster") "\" />\n"
                "</effects-menu>\n"
                "<menu-tip>" N_("Increase or decrease contrast in bitmap(s)") "</menu-tip>\n"
            "</effect>\n"
        "</inkscape-extension>\n", std::make_unique<Contrast>());
    // clang-format on
}

}; /* namespace Bitmap */
}; /* namespace Internal */
}; /* namespace Extension */
}; /* namespace Inkscape */
