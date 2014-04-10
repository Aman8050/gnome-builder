/* gb-tab.h
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

#ifndef GB_TAB_H
#define GB_TAB_H

#include <gtk/gtk.h>

G_BEGIN_DECLS

#define GB_TYPE_TAB            (gb_tab_get_type())
#define GB_TAB(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), GB_TYPE_TAB, GbTab))
#define GB_TAB_CONST(obj)      (G_TYPE_CHECK_INSTANCE_CAST ((obj), GB_TYPE_TAB, GbTab const))
#define GB_TAB_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass),  GB_TYPE_TAB, GbTabClass))
#define GB_IS_TAB(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GB_TYPE_TAB))
#define GB_IS_TAB_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass),  GB_TYPE_TAB))
#define GB_TAB_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj),  GB_TYPE_TAB, GbTabClass))

typedef struct _GbTab        GbTab;
typedef struct _GbTabClass   GbTabClass;
typedef struct _GbTabPrivate GbTabPrivate;

struct _GbTab
{
   GtkBin parent;

   /*< private >*/
   GbTabPrivate *priv;
};

struct _GbTabClass
{
   GtkBinClass parent_class;
};

GType        gb_tab_get_type     (void) G_GNUC_CONST;
gboolean     gb_tab_get_can_save (GbTab       *tab);
void         gb_tab_set_can_save (GbTab       *tab,
                                  gboolean     can_save);
const gchar *gb_tab_get_title    (GbTab       *tab);
void         gb_tab_set_title    (GbTab       *tab,
                                  const gchar *title);

G_END_DECLS

#endif /* GB_TAB_H */
