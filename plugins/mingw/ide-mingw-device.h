/* ide-mingw-device.h
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

#ifndef IDE_MINGW_DEVICE_H
#define IDE_MINGW_DEVICE_H

#include "ide-device.h"

G_BEGIN_DECLS

#define IDE_TYPE_MINGW_DEVICE (ide_mingw_device_get_type())

G_DECLARE_FINAL_TYPE (IdeMingwDevice, ide_mingw_device, IDE, MINGW_DEVICE, IdeDevice)

IdeDevice *ide_mingw_device_new (IdeContext  *context,
                                 const gchar *display_name,
                                 const gchar *id,
                                 const gchar *system_type);

G_END_DECLS

#endif /* IDE_MINGW_DEVICE_H */
