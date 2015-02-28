/* ide-device-manager.h
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

#ifndef IDE_DEVICE_MANAGER_H
#define IDE_DEVICE_MANAGER_H

#include "ide-object.h"
#include "ide-device-manager.h"

G_BEGIN_DECLS

#define IDE_TYPE_DEVICE_MANAGER (ide_device_manager_get_type())

G_DECLARE_FINAL_TYPE (IdeDeviceManager, ide_device_manager, IDE, DEVICE_MANAGER, IdeObject)

struct _IdeDeviceManager
{
  IdeObject parent_instance;
};

void       ide_device_manager_add_provider    (IdeDeviceManager  *self,
                                               IdeDeviceProvider *provider);
GPtrArray *ide_device_manager_get_devices     (IdeDeviceManager  *self);
gboolean   ide_device_manager_get_settled     (IdeDeviceManager  *self);
void       ide_device_manager_remove_provider (IdeDeviceManager  *self,
                                               IdeDeviceProvider *provider);
IdeDevice *ide_device_manager_get_device      (IdeDeviceManager  *self,
                                               const gchar       *device_id);

G_END_DECLS

#endif /* IDE_DEVICE_MANAGER_H */
