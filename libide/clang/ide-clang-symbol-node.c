/* ide-clang-symbol-node.c
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

#include <clang-c/Index.h>

#include "ide-clang-symbol-node.h"
#include "ide-symbol.h"

struct _IdeClangSymbolNode
{
  IdeSymbolNode parent_instance;

  CXCursor  cursor;
  GArray   *children;
};

G_DEFINE_TYPE (IdeClangSymbolNode, ide_clang_symbol_node, IDE_TYPE_SYMBOL_NODE)

static enum CXChildVisitResult
find_child_type (CXCursor     cursor,
                 CXCursor     parent,
                 CXClientData user_data)
{
  enum CXCursorKind *child_kind = user_data;
  enum CXCursorKind kind = clang_getCursorKind (cursor);

  switch ((int)kind)
    {
    case CXCursor_StructDecl:
    case CXCursor_UnionDecl:
    case CXCursor_EnumDecl:
      *child_kind = kind;
      return CXChildVisit_Break;

    case CXCursor_TypeRef:
      cursor = clang_getCursorReferenced (cursor);
      *child_kind = clang_getCursorKind (cursor);
      return CXChildVisit_Break;

    default:
      break;
    }

  return CXChildVisit_Continue;
}

static IdeSymbolKind
get_symbol_kind (CXCursor        cursor,
                 IdeSymbolFlags *flags)
{
  enum CXAvailabilityKind availability;
  enum CXCursorKind cxkind;
  IdeSymbolFlags local_flags = 0;
  IdeSymbolKind kind = 0;

  availability = clang_getCursorAvailability (cursor);
  if (availability == CXAvailability_Deprecated)
    local_flags |= IDE_SYMBOL_FLAGS_IS_DEPRECATED;

  cxkind = clang_getCursorKind (cursor);

  if (cxkind == CXCursor_TypedefDecl)
    {
      enum CXCursorKind child_kind = 0;

      clang_visitChildren (cursor, find_child_type, &child_kind);
      cxkind = child_kind;
    }

  switch ((int)cxkind)
    {
    case CXCursor_StructDecl:
      kind = IDE_SYMBOL_STRUCT;
      break;

    case CXCursor_UnionDecl:
      kind = IDE_SYMBOL_UNION;
      break;

    case CXCursor_ClassDecl:
      kind = IDE_SYMBOL_CLASS;
      break;

    case CXCursor_FunctionDecl:
      kind = IDE_SYMBOL_FUNCTION;
      break;

    case CXCursor_EnumDecl:
      kind = IDE_SYMBOL_ENUM;
      break;

    case CXCursor_EnumConstantDecl:
      kind = IDE_SYMBOL_ENUM_VALUE;
      break;

    case CXCursor_FieldDecl:
      kind = IDE_SYMBOL_FIELD;
      break;

    default:
      break;
    }

  *flags = local_flags;

  return kind;
}

IdeClangSymbolNode *
_ide_clang_symbol_node_new (CXCursor cursor)
{
  IdeClangSymbolNode *self;
  IdeSymbolFlags flags = 0;
  IdeSymbolKind kind;
  CXString cxname;

  /*
   * TODO: Create IdeSourceLocation for cursor.
   */

  kind = get_symbol_kind (cursor, &flags);
  cxname = clang_getCursorSpelling (cursor);

  self = g_object_new (IDE_TYPE_CLANG_SYMBOL_NODE,
                       "kind", kind,
                       "flags", flags,
                       "name", clang_getCString (cxname),
                       NULL);

  self->cursor = cursor;

  clang_disposeString (cxname);

  return self;
}

CXCursor
_ide_clang_symbol_node_get_cursor (IdeClangSymbolNode *self)
{
  g_return_val_if_fail (IDE_IS_CLANG_SYMBOL_NODE (self), clang_getNullCursor ());;

  return self->cursor;
}

static void
ide_clang_symbol_node_class_init (IdeClangSymbolNodeClass *klass)
{
}

static void
ide_clang_symbol_node_init (IdeClangSymbolNode *self)
{
}

GArray *
_ide_clang_symbol_node_get_children (IdeClangSymbolNode *self)
{
  g_return_val_if_fail (IDE_IS_CLANG_SYMBOL_NODE (self), NULL);

  return self->children;
}

void
_ide_clang_symbol_node_set_children (IdeClangSymbolNode *self,
                                     GArray             *children)
{
  g_return_if_fail (IDE_IS_CLANG_SYMBOL_NODE (self));
  g_return_if_fail (self->children == NULL);
  g_return_if_fail (children != NULL);

  self->children = g_array_ref (children);
}
