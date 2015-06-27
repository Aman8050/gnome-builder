/* ide-language.c
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

#include <glib/gi18n.h>

#include "ide-context.h"
#include "ide-ctags-service.h"
#include "ide-diagnostician.h"
#include "ide-gca-diagnostic-provider.h"
#include "ide-indenter.h"
#include "ide-internal.h"
#include "ide-language.h"
#include "ide-refactory.h"

typedef struct
{
  const gchar *id;
} IdeLanguagePrivate;

G_DEFINE_TYPE_WITH_PRIVATE (IdeLanguage, ide_language, IDE_TYPE_OBJECT)

enum {
  PROP_0,
  PROP_DIAGNOSTICIAN,
  PROP_INDENTER,
  PROP_ID,
  PROP_NAME,
  PROP_REFACTORY,
  LAST_PROP
};

static GParamSpec *gParamSpecs [LAST_PROP];
static IdeDiagnostician *gDiagnostician;

/**
 * ide_language_get_source_language:
 *
 * Retrieves the source language to use for the file.
 *
 * Returns: (transfer none) (nullable): A #GtkSourceLanguage or %NULL.
 */
GtkSourceLanguage *
ide_language_get_source_language (IdeLanguage *self)
{
  IdeLanguagePrivate *priv = ide_language_get_instance_private (self);
  GtkSourceLanguageManager *languages;
  GtkSourceLanguage *language = NULL;

  if (IDE_LANGUAGE_GET_CLASS (self)->get_source_language)
    return IDE_LANGUAGE_GET_CLASS (self)->get_source_language (self);

  if (priv->id)
    {
      languages = gtk_source_language_manager_get_default ();
      language = gtk_source_language_manager_get_language (languages, priv->id);
    }

  return language;
}

static GList *
ide_language_real_get_completion_providers (IdeLanguage *self)
{
  IdeLanguagePrivate *priv = ide_language_get_instance_private (self);
  GList *providers = NULL;

  g_assert (IDE_IS_LANGUAGE (self));

  if (ide_str_equal0 (priv->id, "c") ||
      ide_str_equal0 (priv->id, "cpp") ||
      ide_str_equal0 (priv->id, "chdr") ||
      ide_str_equal0 (priv->id, "python") ||
      ide_str_equal0 (priv->id, "js") ||
      ide_str_equal0 (priv->id, "css") ||
      ide_str_equal0 (priv->id, "html"))
    {
      IdeCtagsService *service;
      GtkSourceCompletionProvider *provider;
      IdeContext *context;

      context = ide_object_get_context (IDE_OBJECT (self));
      service = ide_context_get_service_typed (context, IDE_TYPE_CTAGS_SERVICE);
      provider = ide_ctags_service_get_provider (service);
      providers = g_list_prepend (providers, g_object_ref (provider));
    }

  return providers;
}

/**
 * ide_language_get_completion_providers:
 * @self: An #IdeLanguage.
 *
 * Gets the completion providers that can be used to autocomplete for this language.
 *
 * Returns: (transfer full) (element-type GtkSourceCompletionProvider*): A
 *   #GList of #GtkSourceCompletionProvider.
 */
GList *
ide_language_get_completion_providers (IdeLanguage *self)
{
  g_return_val_if_fail (IDE_IS_LANGUAGE (self), NULL);

  if (IDE_LANGUAGE_GET_CLASS (self)->get_completion_providers)
    return IDE_LANGUAGE_GET_CLASS (self)->get_completion_providers (self);

  return NULL;
}

/**
 * ide_language_get_diagnostician:
 *
 * Returns the #IdeDiagnostician for the #IdeLanguage.
 *
 * The diagnostician is responsible for querying the proper language tools to
 * diagnose issues with a particular #IdeFile.
 *
 * See ide_diagnostician_diagnose_async() for more information.
 *
 * If the #IdeLanguage does not have an #IdeDiagnostician, then %NULL is
 * returned.
 *
 * Returns: (transfer none) (nullable): An #IdeDiagnostician or %NULL.
 */
IdeDiagnostician *
ide_language_get_diagnostician (IdeLanguage *self)
{
  g_return_val_if_fail (IDE_IS_LANGUAGE (self), NULL);

  if (IDE_LANGUAGE_GET_CLASS (self)->get_diagnostician)
    return IDE_LANGUAGE_GET_CLASS (self)->get_diagnostician (self);

  return NULL;
}

static IdeDiagnostician *
ide_language_real_get_diagnostician (IdeLanguage *self)
{
  if (!gDiagnostician)
    {
      IdeDiagnosticProvider *provider;
      IdeContext *context;

      context = ide_object_get_context (IDE_OBJECT (self));
      gDiagnostician = g_object_new (IDE_TYPE_DIAGNOSTICIAN,
                                     "context", context,
                                     NULL);
      provider = g_object_new (IDE_TYPE_GCA_DIAGNOSTIC_PROVIDER,
                               "context", context,
                               NULL);
      _ide_diagnostician_add_provider (gDiagnostician, provider);
    }

  return gDiagnostician;
}

/**
 * ide_language_get_indenter:
 *
 * Fetches the #IdeIndenter for @language.
 *
 * If @language does not provide an #IdeIndenter, then %NULL is returned.
 *
 * Returns: (transfer none) (nullable): An #IdeIndenter or %NULL.
 */
IdeIndenter *
ide_language_get_indenter (IdeLanguage *self)
{
  g_return_val_if_fail (IDE_IS_LANGUAGE (self), NULL);

  if (IDE_LANGUAGE_GET_CLASS (self)->get_indenter)
    return IDE_LANGUAGE_GET_CLASS (self)->get_indenter (self);

  return NULL;
}

/**
 * ide_language_get_refactory:
 *
 * Fetches the refactory for @language.
 *
 * If @language does not provide an #IdeRefactory, then %NULL is returned.
 *
 * Returns: (transfer none) (nullable): An #IdeRefactory or %NULL.
 */
IdeRefactory *
ide_language_get_refactory (IdeLanguage *self)
{
  g_return_val_if_fail (IDE_IS_LANGUAGE (self), NULL);

  if (IDE_LANGUAGE_GET_CLASS (self)->get_refactory)
    return IDE_LANGUAGE_GET_CLASS (self)->get_refactory (self);

  return NULL;
}

/**
 * ide_language_get_id:
 *
 * Fetches the unique identifier for the language.
 *
 * Returns: A string such as "c" or "python".
 */
const gchar *
ide_language_get_id (IdeLanguage *self)
{
  IdeLanguagePrivate *priv = ide_language_get_instance_private (self);

  g_return_val_if_fail (IDE_IS_LANGUAGE (self), NULL);

  return priv->id;
}

static void
ide_language_set_id (IdeLanguage *self,
                     const gchar *id)
{
  IdeLanguagePrivate *priv = ide_language_get_instance_private (self);

  g_return_if_fail (IDE_IS_LANGUAGE (self));
  g_return_if_fail (!priv->id);

  priv->id = g_intern_string (id);
}

/**
 * ide_language_get_name:
 *
 * Fetches the display name for the language.
 *
 * Returns: A string containing the display name.
 */
const gchar *
ide_language_get_name (IdeLanguage *self)
{
  g_return_val_if_fail (IDE_IS_LANGUAGE (self), NULL);

  if (IDE_LANGUAGE_GET_CLASS (self)->get_name)
    return IDE_LANGUAGE_GET_CLASS (self)->get_name (self);

  return ide_language_get_id (self);
}

static void
ide_language_get_property (GObject    *object,
                           guint       prop_id,
                           GValue     *value,
                           GParamSpec *pspec)
{
  IdeLanguage *self = IDE_LANGUAGE (object);

  switch (prop_id)
    {
    case PROP_DIAGNOSTICIAN:
      g_value_set_object (value, ide_language_get_diagnostician (self));
      break;

    case PROP_ID:
      g_value_set_string (value, ide_language_get_id (self));
      break;

    case PROP_INDENTER:
      g_value_set_object (value, ide_language_get_indenter (self));
      break;

    case PROP_NAME:
      g_value_set_string (value, ide_language_get_name (self));
      break;

    case PROP_REFACTORY:
      g_value_set_object (value, ide_language_get_refactory (self));
      break;

    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
    }
}

static void
ide_language_set_property (GObject      *object,
                           guint         prop_id,
                           const GValue *value,
                           GParamSpec   *pspec)
{
  IdeLanguage *self = IDE_LANGUAGE (object);

  switch (prop_id)
    {
    case PROP_ID:
      ide_language_set_id (self, g_value_get_string (value));
      break;

    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
    }
}

static void
ide_language_class_init (IdeLanguageClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);

  object_class->get_property = ide_language_get_property;
  object_class->set_property = ide_language_set_property;

  klass->get_completion_providers = ide_language_real_get_completion_providers;
  klass->get_diagnostician = ide_language_real_get_diagnostician;

  gParamSpecs [PROP_DIAGNOSTICIAN] =
    g_param_spec_object ("diagnostician",
                         _("Diagnostician"),
                         _("The diagnostician for the language."),
                         IDE_TYPE_DIAGNOSTICIAN,
                         (G_PARAM_READABLE | G_PARAM_STATIC_STRINGS));

  gParamSpecs [PROP_ID] =
    g_param_spec_string ("id",
                         _("ID"),
                         _("The language identifier such as \"c\"."),
                         NULL,
                         (G_PARAM_READWRITE |
                          G_PARAM_CONSTRUCT_ONLY |
                          G_PARAM_STATIC_STRINGS));

  gParamSpecs [PROP_INDENTER] =
    g_param_spec_object ("indenter",
                         _("Indenter"),
                         _("The semantic indenter for the language."),
                         IDE_TYPE_INDENTER,
                         (G_PARAM_READABLE | G_PARAM_STATIC_STRINGS));

  gParamSpecs [PROP_NAME] =
    g_param_spec_string ("name",
                         _("Name"),
                         _("The name of the language."),
                         NULL,
                         (G_PARAM_READABLE | G_PARAM_STATIC_STRINGS));

  gParamSpecs [PROP_REFACTORY] =
    g_param_spec_object ("refactory",
                         _("Refactory"),
                         _("The refactory engine for the language."),
                         IDE_TYPE_REFACTORY,
                         (G_PARAM_READABLE | G_PARAM_STATIC_STRINGS));

  g_object_class_install_properties (object_class, LAST_PROP, gParamSpecs);
}

static void
ide_language_init (IdeLanguage *self)
{
}
