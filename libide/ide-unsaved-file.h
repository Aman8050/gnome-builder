/* ide-unsaved-file.h
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

#ifndef IDE_UNSAVED_FILE_H
#define IDE_UNSAVED_FILE_H

#include <gio/gio.h>

#include "ide-types.h"

G_BEGIN_DECLS

GType           ide_unsaved_file_get_type     (void);
IdeUnsavedFile *ide_unsaved_file_ref          (IdeUnsavedFile *self);
void            ide_unsaved_file_unref        (IdeUnsavedFile *self);
GBytes         *ide_unsaved_file_get_content  (IdeUnsavedFile *self);
GFile          *ide_unsaved_file_get_file     (IdeUnsavedFile *self);
gint64          ide_unsaved_file_get_sequence (IdeUnsavedFile *file);

G_END_DECLS

#endif /* IDE_UNSAVED_FILE_H */
