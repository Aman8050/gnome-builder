/* python-pack-plugin.c
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

#include <libpeas/peas.h>

#include "ide-python-indenter.h"
#include "ide-python-format-provider.h"

void _ide_python_indenter_register_type (GTypeModule *module);
void _ide_python_format_provider_register_type (GTypeModule *module);

void
peas_register_types (PeasObjectModule *module)
{
  _ide_python_indenter_register_type (G_TYPE_MODULE (module));
  _ide_python_format_provider_register_type (G_TYPE_MODULE (module));

  peas_object_module_register_extension_type (module, IDE_TYPE_INDENTER, IDE_TYPE_PYTHON_INDENTER);
  /*
   * TODO: I think we should make a generic interface for format providers
   *       so that we don't take up a "completion provider" for them in the
   *       plugins. That way, the sourceview can own the completion provider
   *       and change the format provider when the language changes.
   */
  peas_object_module_register_extension_type (module, IDE_TYPE_COMPLETION_PROVIDER, IDE_TYPE_PYTHON_FORMAT_PROVIDER);
}
