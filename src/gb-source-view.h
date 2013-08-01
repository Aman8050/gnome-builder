/* gb-source-view.h
 *
 * Copyright (C) 2013 Christian Hergert <christian@hergert.me>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef GB_SOURCE_VIEW_H
#define GB_SOURCE_VIEW_H

#include <gtksourceview/gtksourceview.h>

G_BEGIN_DECLS

#define GB_TYPE_SOURCE_VIEW            (gb_source_view_get_type())
#define GB_SOURCE_VIEW(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), GB_TYPE_SOURCE_VIEW, GbSourceView))
#define GB_SOURCE_VIEW_CONST(obj)      (G_TYPE_CHECK_INSTANCE_CAST ((obj), GB_TYPE_SOURCE_VIEW, GbSourceView const))
#define GB_SOURCE_VIEW_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass),  GB_TYPE_SOURCE_VIEW, GbSourceViewClass))
#define GB_IS_SOURCE_VIEW(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GB_TYPE_SOURCE_VIEW))
#define GB_IS_SOURCE_VIEW_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass),  GB_TYPE_SOURCE_VIEW))
#define GB_SOURCE_VIEW_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj),  GB_TYPE_SOURCE_VIEW, GbSourceViewClass))

typedef struct _GbSourceView        GbSourceView;
typedef struct _GbSourceViewClass   GbSourceViewClass;
typedef struct _GbSourceViewPrivate GbSourceViewPrivate;

struct _GbSourceView
{
   GtkSourceView parent;

   /*< private >*/
   GbSourceViewPrivate *priv;
};

struct _GbSourceViewClass
{
   GtkSourceViewClass parent_class;
};

GType      gb_source_view_get_type (void) G_GNUC_CONST;
GtkWidget *gb_source_view_new      (void);

G_END_DECLS

#endif /* GB_SOURCE_VIEW_H */
