/* gb-source-snippet-chunk.h
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

#ifndef GB_SOURCE_SNIPPET_CHUNK_H
#define GB_SOURCE_SNIPPET_CHUNK_H

#include <glib-object.h>

G_BEGIN_DECLS

#define GB_TYPE_SOURCE_SNIPPET_CHUNK            (gb_source_snippet_chunk_get_type())
#define GB_SOURCE_SNIPPET_CHUNK(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), GB_TYPE_SOURCE_SNIPPET_CHUNK, GbSourceSnippetChunk))
#define GB_SOURCE_SNIPPET_CHUNK_CONST(obj)      (G_TYPE_CHECK_INSTANCE_CAST ((obj), GB_TYPE_SOURCE_SNIPPET_CHUNK, GbSourceSnippetChunk const))
#define GB_SOURCE_SNIPPET_CHUNK_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass),  GB_TYPE_SOURCE_SNIPPET_CHUNK, GbSourceSnippetChunkClass))
#define GB_IS_SOURCE_SNIPPET_CHUNK(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GB_TYPE_SOURCE_SNIPPET_CHUNK))
#define GB_IS_SOURCE_SNIPPET_CHUNK_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass),  GB_TYPE_SOURCE_SNIPPET_CHUNK))
#define GB_SOURCE_SNIPPET_CHUNK_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj),  GB_TYPE_SOURCE_SNIPPET_CHUNK, GbSourceSnippetChunkClass))

typedef struct _GbSourceSnippetChunk        GbSourceSnippetChunk;
typedef struct _GbSourceSnippetChunkClass   GbSourceSnippetChunkClass;
typedef struct _GbSourceSnippetChunkPrivate GbSourceSnippetChunkPrivate;

struct _GbSourceSnippetChunk
{
   GObject parent;

   /*< private >*/
   GbSourceSnippetChunkPrivate *priv;
};

struct _GbSourceSnippetChunkClass
{
   GObjectClass parent_class;
};

const gchar          *gb_source_snippet_chunk_get_text (GbSourceSnippetChunk *chunk);
GType                 gb_source_snippet_chunk_get_type (void) G_GNUC_CONST;
GbSourceSnippetChunk *gb_source_snippet_chunk_new      (void);
void                  gb_source_snippet_chunk_set_text (GbSourceSnippetChunk *chunk,
                                                        const gchar          *text);

G_END_DECLS

#endif /* GB_SOURCE_SNIPPET_CHUNK_H */
