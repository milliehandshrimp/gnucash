

#include "config.h"
#include "gnc-druid.h"

static void gnc_druid_class_init	(GNCDruidClass *class);
static void gnc_druid_finalize		(GObject *obj);
static void invalid_setpage(GNCDruid* druid, GNCDruidPage* page);

static GObjectClass *parent_class;
static GNCDruidNew new_druid_fcn = NULL;

GType
gnc_druid_get_type (void)
{
  static GType type = 0;

  if (type == 0) {
    GTypeInfo type_info = {
      sizeof (GNCDruidClass),
      NULL,
      NULL,
      (GClassInitFunc)gnc_druid_class_init,
      NULL,
      NULL,
      sizeof (GNCDruid),
      0,
      NULL,
    };
		
    type = g_type_register_static (G_TYPE_OBJECT, "GNCDruid", &type_info, 0);
  }
  
  return type;
}

static void
gnc_druid_class_init (GNCDruidClass *klass)
{
  GObjectClass *object_class;
	
  object_class = G_OBJECT_CLASS (klass);
  parent_class = g_type_class_peek_parent (klass);

  object_class->finalize = gnc_druid_finalize;

  /* override methods */
  klass->set_page = invalid_setpage;
}

static void
gnc_druid_finalize (GObject *obj)
{
  GNCDruid *druid = (GNCDruid *)obj;

  /* Cancel the backend context */
  if (druid->cancel)
    (druid->cancel)(druid->be_ctx);

  /* Destroy the provider list (the providers themselves are destroyed elsewhere) */
  g_list_free(druid->providers);

  G_OBJECT_CLASS (parent_class)->finalize(obj);
}

static void invalid_setpage(GNCDruid* druid, GNCDruidPage* page)
{
  g_warning("druid with invalid set-page");
  return;
}

void
gnc_druid_register_ui(const gchar* ui_type, GNCDruidNew new_druid)
{
  g_return_if_fail(ui_type);
  g_return_if_fail(new_druid);
  g_return_if_fail(!new_druid_fcn);

  new_druid_fcn = new_druid;
}

/* methods */

void
gnc_druid_set_page(GNCDruid* druid, GNCDruidPage* page)
{
  g_return_if_fail(druid);
  g_return_if_fail(IS_GNC_DRUID(druid));

  ((GNC_DRUID_GET_CLASS(druid))->set_page)(druid, page);
}

GNCDruidProvider*
gnc_druid_next_provider(GNCDruid* druid)
{
  GList *node;

  g_return_val_if_fail(druid, NULL);
  g_return_val_if_fail(IS_GNC_DRUID(druid), NULL);

  if (!druid->provider) {
    node = druid->providers;
  } else {
    node = g_list_find(druid->providers, druid->provider);
    node = node->next;
  }

  druid->provider = (node ? node->data : NULL);
  return druid->provider;
}

GNCDruidProvider*
gnc_druid_prev_provider(GNCDruid* druid)
{
  GList *node;

  g_return_val_if_fail(druid, NULL);
  g_return_val_if_fail(IS_GNC_DRUID(druid), NULL);

  if (!druid->provider) {
    node = g_list_last(druid->providers);
  } else {
    node = g_list_find(druid->providers, druid->provider);
    node = node->prev;
  }

  druid->provider = (node ? node->data : NULL);
  return druid->provider;
}

/* Other functions */

/**
 * gnc_druid_new -- create a druid based on the list of providers.  Hold
 *                  onto the backend context and the function to call if
 *                  the druid is cancelled.
 *
 * This will assume the "registered ui", or internally perform some
 * magic to figure out which "UI" to use..
 *
 * The provider list (and all the providerdesc objects) are owned by
 * the druid and will be freed by the druid.
 */
GNCDruid* gnc_druid_new(const gchar* title, GList *providers, gpointer backend_ctx,
			gboolean (*finish)(gpointer be_ctx),
			void (*cancel)(gpointer be_ctx))
{
  GNCDruid *druid;

  g_return_val_if_fail(title, NULL);
  g_return_val_if_fail(providers, NULL);
  g_return_val_if_fail(new_druid_fcn, NULL);

  /* Build the druid */
  druid = new_druid_fcn(title, providers);

  /* Fill in local data */
  if (druid) {
    druid->be_ctx = backend_ctx;
    druid->finish = finish;
    druid->cancel = cancel;
  }

  /* Free the list (the provider descriptions are in the providers) */
  g_list_free(providers);

  /* And return the new druid. */
  return druid;
}
