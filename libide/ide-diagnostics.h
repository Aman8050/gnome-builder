/* ide-diagnostics.h
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

#ifndef IDE_DIAGNOSTICS_H
#define IDE_DIAGNOSTICS_H

#include "ide-types.h"

G_BEGIN_DECLS

#define IDE_TYPE_DIAGNOSTICS (ide_diagnostics_get_type())

GType           ide_diagnostics_get_type (void);
IdeDiagnostics *ide_diagnostics_ref      (IdeDiagnostics *self);
void            ide_diagnostics_unref    (IdeDiagnostics *self);
gsize           ide_diagnostics_get_size (IdeDiagnostics *self);
IdeDiagnostic  *ide_diagnostics_index    (IdeDiagnostics *self,
                                          gsize           index);
void            ide_diagnostics_merge    (IdeDiagnostics *self,
                                          IdeDiagnostics *other);

G_DEFINE_AUTOPTR_CLEANUP_FUNC (IdeDiagnostics, ide_diagnostics_unref)

G_END_DECLS

#endif /* IDE_DIAGNOSTICS_H */
