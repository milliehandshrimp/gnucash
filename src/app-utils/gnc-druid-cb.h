

#ifndef GNC_DRUID_CB_H
#define GNC_DRUID_CB_H

#ifdef __cplusplus
//extern "C" {
#endif

#include <glib.h>
#include <glib-object.h>

#define G_TYPE_GNC_DRUID_CB	gnc_druid_cb_get_type()
#define GNC_DRUID_CB(obj)	G_TYPE_CHECK_INSTANCE_CAST((obj), G_TYPE_GNC_DRUID_CB, GNCDruidCB)
#define IS_GNC_DRUID_CB(obj)	G_TYPE_CHECK_INSTANCE_TYPE((obj), G_TYPE_GNC_DRUID_CB)

typedef struct _GNCDruidCB GNCDruidCB;
typedef struct _GNCDruidCBClass GNCDruidCBClass;

#include "gnc-druid.h"

struct _GNCDruidCB
{
  GObject obj;

  GNCDruid* druid_ctx;
  gpointer be_ctx;
};

struct _GNCDruidCBClass
{
  GObjectClass parent_class;
};

GType	gnc_druid_cb_get_type(void);
GNCDruidCB* gnc_druid_cb_new(void);

/* methods */

#ifdef __cplusplus
//}
#endif

#endif /* GNC_DRUID_CB_H */
