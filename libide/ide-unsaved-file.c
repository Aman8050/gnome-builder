/* ide-unsaved-file.c
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

#define G_LOG_DOMAIN "ide-unsaved-file"

#include "ide-debug.h"
#include "ide-unsaved-file.h"

G_DEFINE_BOXED_TYPE (IdeUnsavedFile, ide_unsaved_file,
                     ide_unsaved_file_ref, ide_unsaved_file_unref)

struct _IdeUnsavedFile
{
  volatile gint  ref_count;
  GBytes        *content;
  GFile         *file;
  gchar         *temp_path;
  gint64         sequence;
};

IdeUnsavedFile *
_ide_unsaved_file_new (GFile       *file,
                       GBytes      *content,
                       const gchar *temp_path,
                       gint64       sequence)
{
  IdeUnsavedFile *ret;

  g_return_val_if_fail (G_IS_FILE (file), NULL);
  g_return_val_if_fail (content, NULL);

  ret = g_slice_new0 (IdeUnsavedFile);
  ret->ref_count = 1;
  ret->file = g_object_ref (file);
  ret->content = g_bytes_ref (content);
  ret->sequence = sequence;
  ret->temp_path = g_strdup (temp_path);

  return ret;
}

const gchar *
ide_unsaved_file_get_temp_path (IdeUnsavedFile *self)
{
  g_return_val_if_fail (self, NULL);

  return self->temp_path;
}

gboolean
ide_unsaved_file_persist (IdeUnsavedFile  *self,
                          GCancellable    *cancellable,
                          GError         **error)
{
  gboolean ret;

  IDE_ENTRY;

  g_return_val_if_fail (self, FALSE);
  g_return_val_if_fail (!cancellable || G_IS_CANCELLABLE (cancellable), FALSE);

  IDE_TRACE_MSG ("Saving draft to \"%s\"", self->temp_path);

  /*
   * TODO: Support cancellable.
   */

  ret = g_file_set_contents (self->temp_path,
                             g_bytes_get_data (self->content, NULL),
                             g_bytes_get_size (self->content),
                             error);

  IDE_RETURN (ret);
}

gint64
ide_unsaved_file_get_sequence (IdeUnsavedFile *self)
{
  g_return_val_if_fail (self, -1);

  return self->sequence;
}

IdeUnsavedFile *
ide_unsaved_file_ref (IdeUnsavedFile *self)
{
  g_return_val_if_fail (self, NULL);
  g_return_val_if_fail (self->ref_count > 0, NULL);

  g_atomic_int_inc (&self->ref_count);

  return self;
}

void
ide_unsaved_file_unref (IdeUnsavedFile *self)
{
  g_return_if_fail (self);
  g_return_if_fail (self->ref_count > 0);

  if (g_atomic_int_dec_and_test (&self->ref_count))
    {
      g_clear_pointer (&self->temp_path, g_free);
      g_clear_pointer (&self->content, g_bytes_unref);
      g_clear_object (&self->file);
      g_slice_free (IdeUnsavedFile, self);
    }
}

GBytes *
ide_unsaved_file_get_content (IdeUnsavedFile *self)
{
  g_return_val_if_fail (self, NULL);

  return self->content;
}

/**
 * ide_unsaved_file_get_file:
 *
 * Retrieves the underlying file represented by @self.
 *
 * Returns: (transfer none): A #GFile.
 */
GFile *
ide_unsaved_file_get_file (IdeUnsavedFile *self)
{
  g_return_val_if_fail (self, NULL);

  return self->file;
}
