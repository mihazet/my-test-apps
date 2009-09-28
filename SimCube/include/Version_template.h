//////////////////////////////////////////////////////////////////////////////
// THIS FILE IS GENERATED AUTOMATICALLY, DO NOT MODIFY IT BY HAND.
//////////////////////////////////////////////////////////////////////////////

#ifndef _SIMCUBE_VERSION_H_
#define _SIMCUBE_VERSION_H_

#define SCVER_MAJOR             1
#define SCVER_MINOR             0
#define SCVER_RELEASE           0
#define SCVER_BUILD             $WCREV$$WCMODS?*:$
#define STRINGIFY_HELPER(x)     #x
#define STRINGIFY(x)            STRINGIFY_HELPER(x)
#define SCVER_MAJOR_STRING      STRINGIFY(SCVER_MAJOR)
#define SCVER_MINOR_STRING      STRINGIFY(SCVER_MINOR)
#define SCVER_RELEASE_STRING    STRINGIFY(SCVER_RELEASE)
#define SCVER_BUILD_STRING      STRINGIFY(SCVER_BUILD)
#define SC_BUILDDATE_STRING     "$WCNOW$";

#endif /* _VERSION_H_ */
