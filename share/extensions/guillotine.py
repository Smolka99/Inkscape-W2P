#!/usr/bin/env python3
# coding=utf-8
#
# Copyright (C) 2010 Craig Marshall, craig9 [at] gmail.com
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
#
"""
This script slices an inkscape drawing along the guides, similarly to
the GIMP plugin called "guillotine". It can optionally export to the
same directory as the SVG file with the same name, but with a number
suffix. e.g.

/home/foo/drawing.svg

will export to:

/home/foo/drawing0.png
/home/foo/drawing1.png
/home/foo/drawing2.png
/home/foo/drawing3.png

etc.

"""

import os
import locale

import inkex
from inkex.command import inkscape
from inkex.localization import inkex_gettext as _


class Guillotine(inkex.EffectExtension):
    """Exports slices made using guides"""

    def add_arguments(self, pars):
        pars.add_argument("--directory", type=str, dest="directory")
        pars.add_argument("--image", type=str, dest="image")
        pars.add_argument("--ignore", type=inkex.Boolean, dest="ignore")

    def get_all_horizontal_guides(self):
        """
        Returns all horizontal guides as a list of floats stored as
        strings. Each value is the position from 0 in pixels.
        """
        for guide in self.svg.namedview.get_guides():
            if guide.is_horizontal:
                yield guide.raw_position.y

    def get_all_vertical_guides(self):
        """
        Returns all vertical guides as a list of floats stored as
        strings. Each value is the position from 0 in pixels.
        """
        for guide in self.svg.namedview.get_guides():
            if guide.is_vertical:
                yield guide.raw_position.x

    def get_horizontal_slice_positions(self):
        """
        Make a sorted list of all horizontal guide positions,
        including 0 and the document height, but not including
        those outside of the canvas
        """
        horizontals = [0.0]
        height = float(self.svg.viewbox_height)
        for y in self.get_all_horizontal_guides():
            if 0.0 < y <= height:
                horizontals.append(height - y)
        horizontals.append(height)
        return sorted(horizontals)

    def get_vertical_slice_positions(self):
        """
        Make a sorted list of all vertical guide positions,
        including 0 and the document width, but not including
        those outside of the canvas.
        """
        verticals = [0.0]
        width = float(self.svg.viewbox_width)
        for x in self.get_all_vertical_guides():
            if 0.0 < x <= width:
                verticals.append(x)
        verticals.append(width)
        return sorted(verticals)

    def get_slices(self):
        """
        Returns a list of all "slices" as denoted by the guides
        on the page. Each slice is really just a 4 element list of
        floats (stored as strings), consisting of the X and Y start
        position and the X and Y end position.
        """
        hs = self.get_horizontal_slice_positions()
        vs = self.get_vertical_slice_positions()
        # The --export-width argument is in viewport units
        hs = [self.svg.unit_to_viewport(i) for i in hs]
        vs = [self.svg.unit_to_viewport(j) for j in vs]
        slices = []
        for i in range(len(hs) - 1):
            for j in range(len(vs) - 1):
                slices.append([vs[j], hs[i], vs[j + 1], hs[i + 1]])
        return slices

    def get_filename_parts(self):
        """
        Attempts to get directory and image as passed in by the inkscape
        dialog. If the boolean ignore flag is set, then it will ignore
        these settings and try to use the settings from the export
        filename.
        """

        if not self.options.ignore:
            if self.options.image == "" or self.options.image is None:
                raise inkex.AbortExtension(_("Please enter an image name"))
            return self.options.directory, self.options.image
        else:
            """
            First get the export-filename from the document, if the
            document has been exported before (TODO: Will not work if it
            hasn't been exported yet), then uses this to return a tuple
            consisting of the directory to export to, and the filename
            without extension.
            """
            try:
                export_file = self.svg.get("inkscape:export-filename")
            except KeyError:
                raise inkex.AbortExtension(
                    _(
                        "To use the export hints option, you "
                        "need to have previously exported the document. "
                        "Otherwise no export hints exist!"
                    )
                )
            dirname, filename = os.path.split(export_file)
            filename = filename.rsplit(".", 1)[0]  # Without extension
            return dirname, filename

    def get_localised_string(self, name):
        return locale.format_string("%.f", float(name), 0)

    def export_slice(self, sli, filename):
        """
        Runs inkscape's command line interface and exports the image
        slice from the 4 coordinates in s, and saves as the filename
        given.
        """
        coords = ":".join([self.get_localised_string(dim) for dim in sli])
        inkex.errormsg(coords)
        inkscape(self.options.input_file, export_area=coords, export_filename=filename)

    def export_slices(self, slices):
        """
        Takes the slices list and passes each one with a calculated
        filename/directory into export_slice.
        """
        dirname, filename = self.get_filename_parts()
        # Remove some crusty extensions from name template
        if filename.endswith(".svg") or filename.endswith(".png"):
            filename = filename.rsplit(".", 1)[0]
        if "{" not in filename:
            filename += "_{}"

        dirname = os.path.abspath(
            os.path.expanduser(os.path.expandvars(dirname or "./"))
        )
        if not os.path.isdir(dirname):
            os.makedirs(dirname)

        output_files = []
        for i, slico in enumerate(slices):
            fname = os.path.join(dirname, filename.format(i) + ".png")
            output_files.append(fname)
            self.export_slice(slico, fname)

        self.debug(
            _("The sliced bitmaps have been saved as:\n\n") + "\n".join(output_files)
        )

    def effect(self):
        self.export_slices(self.get_slices())


if __name__ == "__main__":
    Guillotine().run()
