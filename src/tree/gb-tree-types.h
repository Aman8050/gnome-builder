/* gb-tree-types.h
 *
 * Copyright (C) 2015 Christian Hergert <christian@hergert.me>
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

#ifndef GB_TREE_TYPES_H
#define GB_TREE_TYPES_H

#include <gtk/gtk.h>

G_BEGIN_DECLS

#define GB_TYPE_TREE         (gb_tree_get_type())
#define GB_TYPE_TREE_NODE    (gb_tree_node_get_type())
#define GB_TYPE_TREE_BUILDER (gb_tree_builder_get_type())

G_DECLARE_DERIVABLE_TYPE (GbTree,        gb_tree,         GB, TREE,         GtkTreeView)
G_DECLARE_DERIVABLE_TYPE (GbTreeBuilder, gb_tree_builder, GB, TREE_BUILDER, GInitiallyUnowned)
G_DECLARE_FINAL_TYPE     (GbTreeNode,    gb_tree_node,    GB, TREE_NODE,    GInitiallyUnowned)

G_END_DECLS

#endif /* GB_TREE_TYPES_H */
