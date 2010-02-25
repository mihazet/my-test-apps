#ifndef _WIDGET_ID_H_
#define _WIDGET_ID_H_

#include <wx/defs.h>

enum
{
    myID_DUMMY = wxID_HIGHEST + 1,

    /* app */
    myID_CXTMENU_ADAPTER_FIRST,
    myID_CXTMENU_ADAPTER_UDP = myID_CXTMENU_ADAPTER_FIRST,
    myID_CXTMENU_ADAPTER_TCP,
    myID_CXTMENU_ADAPTER_LAST,
    myID_CXTMENU_ADAPTER_DUMMY = myID_CXTMENU_ADAPTER_FIRST + (myID_CXTMENU_ADAPTER_LAST - myID_CXTMENU_ADAPTER_FIRST) * 10,

    /* frame */
    myID_VIEW_PANE_START,
    myID_VIEW_PROPERTY_PANE = myID_VIEW_PANE_START,
    myID_VIEW_TRAP_PANE,
    myID_VIEW_PEER_PANE,
    myID_VIEW_CONFIG_PANE,
    myID_VIEW_LOG_PANE,
    myID_VIEW_PANE_END = myID_VIEW_LOG_PANE,
    myID_VIEW_RESET_LAYOUT,
    myID_HELP_DOC,

    /* targets */
    myID_TARGET_SEARCH_BTN,

    /* log */

    /* threads */
    myID_SEARCH_THREAD,
};

#endif /* _WIDGET_ID_H_ */
