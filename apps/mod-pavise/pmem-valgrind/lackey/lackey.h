#ifndef __LACKEY_H
#define __LACKEY_H

#include "valgrind.h"

typedef
   enum {
      VG_USERREQ__DUMP_STATS = VG_USERREQ_TOOL_BASE('L','K'),
      VG_USERREQ__START_INSTRUMENTATION,
      VG_USERREQ__STOP_INSTRUMENTATION,
      VG_USERREQ__OPERATION_START,
      VG_USERREQ__OPERATION_END
   } Vg_CallgrindClientRequest;

// Starts full instrumentation if not already switched on.
#define LACKEY_START_INSTRUMENTATION                              \
  VALGRIND_DO_CLIENT_REQUEST_STMT(VG_USERREQ__START_INSTRUMENTATION, \
                                  0, 0, 0, 0, 0)

// Stops full instrumentation if not already switched off.
#define LACKEY_STOP_INSTRUMENTATION                              \
  VALGRIND_DO_CLIENT_REQUEST_STMT(VG_USERREQ__STOP_INSTRUMENTATION, \
                                  0, 0, 0, 0, 0)

// Indicates start of persistent operation.
#define LACKEY_OPERATION_START\
  VALGRIND_DO_CLIENT_REQUEST_STMT(VG_USERREQ__OPERATION_START, \
                                  0, 0, 0, 0, 0)

// Indicates end of persistent operation.
#define LACKEY_OPERATION_END\
  VALGRIND_DO_CLIENT_REQUEST_STMT(VG_USERREQ__OPERATION_END, \
                                  0, 0, 0, 0, 0)

#endif /* __LACKEY_H */
