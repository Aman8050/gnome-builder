/* gb-source-snippet-parser.h
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

#ifndef GB_SOURCE_SNIPPET_PARSER_H
#define GB_SOURCE_SNIPPET_PARSER_H

#include <gio/gio.h>

G_BEGIN_DECLS

#define GB_TYPE_SOURCE_SNIPPET_PARSER            (gb_source_snippet_parser_get_type())
#define GB_SOURCE_SNIPPET_PARSER(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), GB_TYPE_SOURCE_SNIPPET_PARSER, GbSourceSnippetParser))
#define GB_SOURCE_SNIPPET_PARSER_CONST(obj)      (G_TYPE_CHECK_INSTANCE_CAST ((obj), GB_TYPE_SOURCE_SNIPPET_PARSER, GbSourceSnippetParser const))
#define GB_SOURCE_SNIPPET_PARSER_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass),  GB_TYPE_SOURCE_SNIPPET_PARSER, GbSourceSnippetParserClass))
#define GB_IS_SOURCE_SNIPPET_PARSER(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GB_TYPE_SOURCE_SNIPPET_PARSER))
#define GB_IS_SOURCE_SNIPPET_PARSER_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass),  GB_TYPE_SOURCE_SNIPPET_PARSER))
#define GB_SOURCE_SNIPPET_PARSER_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj),  GB_TYPE_SOURCE_SNIPPET_PARSER, GbSourceSnippetParserClass))

typedef struct _GbSourceSnippetParser        GbSourceSnippetParser;
typedef struct _GbSourceSnippetParserClass   GbSourceSnippetParserClass;
typedef struct _GbSourceSnippetParserPrivate GbSourceSnippetParserPrivate;

struct _GbSourceSnippetParser
{
   GObject parent;

   /*< private >*/
   GbSourceSnippetParserPrivate *priv;
};

struct _GbSourceSnippetParserClass
{
   GObjectClass parent_class;
};

GType                  gb_source_snippet_parser_get_type       (void) G_GNUC_CONST;
GbSourceSnippetParser *gb_source_snippet_parser_new            (void);
gboolean               gb_source_snippet_parser_load_from_file (GbSourceSnippetParser  *parser,
                                                                GFile                  *file,
                                                                GError                **error);
GList                 *gb_source_snippet_parser_get_snippets   (GbSourceSnippetParser  *parser);

G_END_DECLS

#endif /* GB_SOURCE_SNIPPET_PARSER_H */
