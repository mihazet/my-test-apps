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
    myID_VIEW_MAC_ADDR_DEFINE_PANE,
    myID_VIEW_MAC_ADDR_USAGE_PANE,
    myID_VIEW_MAC_ADDR_UPDATE_PANE,
    myID_VIEW_DOWNLOAD_PANE,
    myID_VIEW_LOG_PANE,
    myID_VIEW_OPTION_PANE,
    myID_VIEW_PANE_END,
    myID_VIEW_RESET_LAYOUT,
    myID_HELP_DOC,

    /* download */
    myID_TARGET_CHECK_ALL,
    myID_TARGET_UNCHECK_ALL,
    myID_DOWNLOAD_TARGET_LIST_SELECT_NONE,
    myID_DOWNLOAD_SEARCH_BTN,
    myID_SEARCH_METHOD1_RB,
    myID_SEARCH_METHOD2_RB,
    myID_DOWNLOAD_TARGET_LIST,
    myID_DOWNLOAD_SPECIFIC_RB,
    myID_DOWNLOAD_GLOBAL_RB,
    myID_DOWNLOAD_GLOBAL_FILE,
    myID_DOWNLOAD_SELECTED_BTN,
    myID_MODIFY_MAC_BTN,

    /* log */

    /* threads */
    myID_SEARCH_THREAD,
    myID_UPDATE_THREAD,
};

#endif /* _WIDGET_ID_H_ */
