// SPDX-License-Identifier: GPL-2.0-or-later

#include "canvas-notice.h"

#include <utility>
#include <glibmm/main.h>

#include "ui/builder-utils.h"

namespace Inkscape {
namespace UI {
namespace Widget {


CanvasNotice::CanvasNotice(BaseObjectType *cobject, Glib::RefPtr<Gtk::Builder> builder)
    : Gtk::Revealer(cobject)
    , _builder(std::move(builder))
    , _icon(get_widget<Gtk::Image>(_builder, "notice-icon"))
    , _label(get_widget<Gtk::Label>(_builder, "notice-label"))
{
    auto &close = get_widget<Gtk::Button>(_builder, "notice-close");
    close.signal_clicked().connect([this]() {
        hide();
    });
}

void CanvasNotice::show(Glib::ustring const &msg, unsigned timeout)
{
    _label.set_text(msg);
    set_reveal_child(true);
    if (timeout != 0) {
        _timeout = Glib::signal_timeout().connect([this]() {
            hide();
            return false;
        }, timeout);
    }
}

void CanvasNotice::hide()
{
    set_reveal_child(false);
}

CanvasNotice *CanvasNotice::create()
{
    auto builder = create_builder("canvas-notice.glade");
    auto widget = &get_derived_widget<CanvasNotice>(builder, "canvas-notice");
    return widget;
}

}}} // namespaces
