//////////////////////////////////////////////////////////////////////////////
// THIS FILE IS GENERATED AUTOMATICALLY, DO NOT MODIFY IT BY HAND.
//////////////////////////////////////////////////////////////////////////////

#ifndef _VERSION_H_
#define _VERSION_H_

#define VER_MAJOR               0
#define VER_MINOR               1
#define VER_RELEASE             0
#define VER_BUILD               $WCREV$$WCMODS?*:$
#define STRINGIFY_HELPER(x)     #x
#define STRINGIFY(x)            STRINGIFY_HELPER(x)
#define VER_MAJOR_STRING        STRINGIFY(VER_MAJOR)
#define VER_MINOR_STRING        STRINGIFY(VER_MINOR)
#define VER_RELEASE_STRING      STRINGIFY(VER_RELEASE)
#define VER_BUILD_STRING        STRINGIFY(VER_BUILD)
#define BUILDDATE_STRING        "$WCNOW$"
#define SRC_REPOSITORY          "$WCURL$"

#endif /* _VERSION_H_ */
