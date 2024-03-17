#!/usr/bin/env python3
# coding=utf-8
#
# Copyright (C) 2005 Pim Snel, pim@lingewoud.com
# Copyright (C) 2008 Aaron Spike, aaron@ekips.org
# Copyright (C) 2011 Nicolas Dufour, nicoduf@yahoo.fr
#
#    * Fix for a bug related to special characters in the path (LP #456248).
#    * Fix for Windows support (LP #391307 ).
#    * Font list and image directory features.
#
# this is  the first Python script  ever created
# its based on embedimage.py
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
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
#
# TODOs
# - fix bug: not saving existing .zip after a Collect for Output is run
#     this bug occurs because after running an effect extension the inkscape:output_extension is reset to svg.inkscape
#     the file name is still xxx.zip. after saving again the file xxx.zip is written with a plain .svg which
#     looks like a corrupt zip
# - maybe add better extension
# - consider switching to lzma in order to allow cross platform compression with no encoding problem...
#
"""
An extension which collects all images to the documents directory and
creates a zip archive containing all images and the document
"""

import os
import tempfile
import zipfile

from typing import List, Tuple
from urllib.parse import urlparse
from urllib.request import url2pathname

import inkex
from inkex import TextElement, Tspan, FlowRoot, FlowPara, FlowSpan
from inkex.localization import inkex_gettext as _


class CompressedMedia(inkex.OutputExtension):
    """Output a compressed file"""

    def __init__(self):
        super().__init__()
        self.path_dict = {}

    def add_arguments(self, pars):
        pars.add_argument("--image_dir", help="Image directory", default="images")
        pars.add_argument("--font_list", type=inkex.Boolean, help="Add font list")

    def process_path(self, path: str) -> Tuple[str, bool]:
        """Processes an absolute path and returns
        (unique filename, exists)."""

        if path in self.path_dict:
            return os.path.split(path)[1], True

        index = 0
        pth, ext = os.path.splitext(path)
        _, filename = os.path.split(pth)
        trypath = filename + ext
        while True:
            if trypath in list(self.path_dict.values()):
                index += 1
                trypath = f"{filename}{index}{ext}"
            else:
                self.path_dict[path] = trypath
                return trypath, False

    def collect_images(self, docname, z: zipfile.ZipFile):
        """
        Collects all images in the document
        and copy them to the temporary directory.
        """
        imgdir = self.options.image_dir

        for node in self.svg.xpath("//svg:image"):
            xlink = node.get("xlink:href")
            if xlink[:4] != "data":
                url = urlparse(xlink)
                href = url2pathname(url.path)

                image_path = self.absolute_href(href or "")

                # Backup directory where we can find the image
                if not os.path.isfile(image_path):
                    image_path = node.get("sodipodi:absref", image_path)

                if not os.path.isfile(image_path):
                    inkex.errormsg(
                        _('File not found "{}". Unable to embed image.').format(
                            image_path
                        )
                    )
                    continue
                else:
                    zippath, exists = self.process_path(image_path)
                    if not exists:
                        z.write(image_path, os.path.join(imgdir, zippath))

                    node.set("xlink:href", f"{imgdir}/{zippath}")

    def collect_svg(self, docstripped, z: zipfile.ZipFile):
        """
        Copy SVG document to the temporary directory
        and add it to the temporary compressed file
        """
        dst_file = os.path.join(self.tmp_dir, docstripped)
        with open(dst_file, "wb") as stream:
            self.document.write(stream)
        z.write(dst_file, docstripped + ".svg")

    def is_text(self, node):
        """
        Returns true if the tag in question is an element that
        can hold text.
        """
        return isinstance(node, (TextElement, Tspan, FlowRoot, FlowPara, FlowSpan))

    def get_fonts(self, node):
        """
        Given a node, returns a list containing all the fonts that
        the node is using.
        """
        fonts = []
        s = ""
        if "style" in node.attrib:
            s = dict(inkex.Style.parse_str(node.attrib["style"]))
        if not s:
            return fonts

        if "font-family" in s:
            if "font-weight" in s:
                fonts.append(s["font-family"] + " " + s["font-weight"])
            else:
                fonts.append(s["font-family"])
        elif "-inkscape-font-specification" in s:
            fonts.append(s["-inkscape-font-specification"])
        return fonts

    def list_fonts(self, z: zipfile.ZipFile):
        """
        Walks through nodes, building a list of all fonts found, then
        reports to the user with that list.
        Based on Craig Marshall's replace_font.py
        """
        nodes: List[inkex.BaseElement] = []
        items = self.svg.iterdescendants()
        nodes.extend(filter(self.is_text, items))
        fonts_found = []
        for node in nodes:
            for f in self.get_fonts(node):
                if not f in fonts_found:
                    fonts_found.append(f)
        findings = sorted(fonts_found)
        # Write list to the temporary compressed file
        filename = "fontlist.txt"
        dst_file = os.path.join(self.tmp_dir, filename)
        with open(dst_file, "w") as stream:
            if len(findings) == 0:
                stream.write(_("Didn't find any fonts in this document/selection."))
            else:
                if len(findings) == 1:
                    stream.write(_("Found the following font only: %s") % findings[0])
                else:
                    stream.write(
                        _("Found the following fonts:\n%s") % "\n".join(findings)
                    )
        z.write(dst_file, filename)

    def save(self, stream):
        docname = self.svg.get("sodipodi:docname")

        if docname is None:
            docname = self.options.input_file

        # TODO: replace whatever extension
        docstripped = os.path.basename(docname.replace(".zip", ""))
        docstripped = docstripped.replace(".svg", "")
        docstripped = docstripped.replace(".svgz", "")

        # Create os temp dir
        self.tmp_dir = tempfile.mkdtemp()

        # Create destination zip in same directory as the document
        with zipfile.ZipFile(stream, "w") as z:
            self.collect_images(docname, z)
            self.collect_svg(docstripped, z)
            if self.options.font_list:
                self.list_fonts(z)


if __name__ == "__main__":
    CompressedMedia().run()
