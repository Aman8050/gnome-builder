/* ide-source-view-capture.c
 *
 * Copyright (C) 2015 Christian Hergert <christian@hergert.me>
 *
 * This file is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation; either version 3 of the
 * License, or (at your option) any later version.
 *
 * This file is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <glib/gi18n.h>

#include "ide-internal.h"
#include "ide-source-view-capture.h"

typedef struct
{
  guint     type  : 1;
  guint     count : 31;
  gunichar  modifier;
  GdkEvent *event;
} CaptureFrame;

enum {
  FRAME_EVENT,
  FRAME_MODIFIER,
};

struct _IdeSourceViewCapture
{
  GObject        parent_instance;

  struct {
    gchar                 *mode_name;
    IdeSourceViewModeType  mode_type;
    guint                  count;
    gunichar               modifier;
  } starting_state;

  IdeSourceView *view;
  GArray        *frames;
};

G_DEFINE_TYPE (IdeSourceViewCapture, ide_source_view_capture, G_TYPE_OBJECT)

enum {
  PROP_0,
  PROP_VIEW,
  LAST_PROP
};

static GParamSpec *gParamSpecs [LAST_PROP];

IdeSourceViewCapture *
ide_source_view_capture_new (IdeSourceView         *view,
                             const gchar           *mode_name,
                             IdeSourceViewModeType  mode_type,
                             guint                  count,
                             gunichar               modifier)
{
  IdeSourceViewCapture *self;

  self = g_object_new (IDE_TYPE_SOURCE_VIEW_CAPTURE,
                       "view", view,
                       NULL);

  self->starting_state.mode_name = g_strdup (mode_name);
  self->starting_state.mode_type = mode_type;
  self->starting_state.count = count;
  self->starting_state.modifier = modifier;

  return self;
}

IdeSourceView *
ide_source_view_capture_get_view (IdeSourceViewCapture *self)
{
  g_return_val_if_fail (IDE_IS_SOURCE_VIEW_CAPTURE (self), NULL);

  return self->view;
}

static void
ide_source_view_capture_set_view (IdeSourceViewCapture *self,
                                  IdeSourceView        *view)
{
  g_return_if_fail (IDE_IS_SOURCE_VIEW_CAPTURE (self));

  g_set_object (&self->view, view);
}

void
ide_source_view_capture_replay (IdeSourceViewCapture *self)
{
  gsize i;

  g_return_if_fail (IDE_IS_SOURCE_VIEW_CAPTURE (self));

  g_signal_emit_by_name (self->view,
                         "set-mode",
                         self->starting_state.mode_name,
                         self->starting_state.mode_type);
  _ide_source_view_set_count (self->view, self->starting_state.count);
  _ide_source_view_set_modifier (self->view, self->starting_state.modifier);

  for (i = 0; i < self->frames->len; i++)
    {
      CaptureFrame *frame;

      frame = &g_array_index (self->frames, CaptureFrame, i);

      switch (frame->type)
        {
        case FRAME_EVENT:
          _ide_source_view_set_count (self->view, frame->count);
          _ide_source_view_set_modifier (self->view, frame->modifier);
          gtk_widget_event (GTK_WIDGET (self->view), frame->event);
          break;

        case FRAME_MODIFIER:
          _ide_source_view_set_modifier (self->view, frame->modifier);
          break;

        default:
          g_assert_not_reached ();
          break;
        }
   }
}

void
ide_source_view_capture_record_modifier (IdeSourceViewCapture *self,
                                         gunichar              modifier)
{
  CaptureFrame frame = { 0 };

  g_assert (IDE_IS_SOURCE_VIEW_CAPTURE (self));

  frame.type = FRAME_MODIFIER;
  frame.count = 0;
  frame.modifier = modifier;
  frame.event = NULL;

  g_array_append_val (self->frames, frame);
}

void
ide_source_view_capture_record_event (IdeSourceViewCapture *self,
                                      const GdkEvent       *event,
                                      guint                 count,
                                      gunichar              modifier)
{
  CaptureFrame frame = { 0 };

  g_assert (IDE_IS_SOURCE_VIEW_CAPTURE (self));
  g_assert (event);

  frame.type = FRAME_EVENT;
  frame.count = (count & 0x7FFFFFFF);
  frame.modifier = modifier;
  frame.event = gdk_event_copy ((GdkEvent*)event);

  g_array_append_val (self->frames, frame);
}

static void
clear_frame (CaptureFrame *frame)
{
  g_clear_pointer (&frame->event, gdk_event_free);
}

static void
ide_source_view_capture_finalize (GObject *object)
{
  IdeSourceViewCapture *self = (IdeSourceViewCapture *)object;

  g_clear_object (&self->view);
  g_clear_pointer (&self->frames, g_array_unref);
  g_clear_pointer (&self->starting_state.mode_name, g_free);

  G_OBJECT_CLASS (ide_source_view_capture_parent_class)->finalize (object);
}

static void
ide_source_view_capture_get_property (GObject    *object,
                                      guint       prop_id,
                                      GValue     *value,
                                      GParamSpec *pspec)
{
  IdeSourceViewCapture *self = IDE_SOURCE_VIEW_CAPTURE (object);

  switch (prop_id)
    {
    case PROP_VIEW:
      g_value_set_object (value, ide_source_view_capture_get_view (self));
      break;

    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
    }
}

static void
ide_source_view_capture_set_property (GObject      *object,
                                      guint         prop_id,
                                      const GValue *value,
                                      GParamSpec   *pspec)
{
  IdeSourceViewCapture *self = IDE_SOURCE_VIEW_CAPTURE (object);

  switch (prop_id)
    {
    case PROP_VIEW:
      ide_source_view_capture_set_view (self, g_value_get_object (value));
      break;

    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
    }
}

static void
ide_source_view_capture_class_init (IdeSourceViewCaptureClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);

  object_class->finalize = ide_source_view_capture_finalize;
  object_class->get_property = ide_source_view_capture_get_property;
  object_class->set_property = ide_source_view_capture_set_property;

 gParamSpecs [PROP_VIEW] =
    g_param_spec_object ("view",
                         _("View"),
                         _("The source view."),
                         IDE_TYPE_SOURCE_VIEW,
                         (G_PARAM_READWRITE |
                          G_PARAM_CONSTRUCT_ONLY |
                          G_PARAM_STATIC_STRINGS));
  g_object_class_install_property (object_class, PROP_VIEW, gParamSpecs [PROP_VIEW]);
}

static void
ide_source_view_capture_init (IdeSourceViewCapture *self)
{
  self->frames = g_array_new (FALSE, FALSE, sizeof(CaptureFrame));
  g_array_set_clear_func (self->frames, (GDestroyNotify)clear_frame);
}
