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

    /* property */
    myID_PROPERTY_GRID,
    myID_BTN_RESET,

    /* trap */
    myID_LED_POWER,
    myID_LED_FAN,
    myID_LED_LAMPA,
    myID_LED_LAMPB,
    myID_RB_LED_POWER,
    myID_RB_LED_FAN,
    myID_RB_LED_LAMPA,
    myID_RB_LED_LAMPB,
    myID_CHOICE_LED_PRESET,
    myID_BTN_LEDSTATUS_SEND,
    myID_BTN_LAMPASTATE_SEND,
    myID_BTN_LAMPBSTATE_SEND,
    myID_BTN_LAMPAHOURS_SEND,
    myID_BTN_LAMPBHOURS_SEND,
    myID_BTN_LAMPALITCNT_SEND,
    myID_BTN_LAMPBLITCNT_SEND,
    myID_BTN_LAMPATEMPCOND_SEND,
    myID_BTN_LAMPBTEMPCOND_SEND,
    myID_CHOICE_LAMPA_STATUS,
    myID_CHOICE_LAMPB_STATUS,
    myID_CHOICE_LAMPA_TEMPCOND,
    myID_CHOICE_LAMPB_TEMPCOND,
    myID_SPIN_LAMPA_HOURS,
    myID_SPIN_LAMPB_HOURS,
    myID_SPIN_LAMPA_LITCNT,
    myID_SPIN_LAMPB_LITCNT,

    /* peer */

    /* config */
    myID_LANG_DEFAULT,
    myID_LANG_SELECT,
    myID_OPT_AUTOSAVE_HISTORY,
    myID_OPT_USING_ROCKEY4ND,
    myID_OPT_MAX_CONNECTION,
    myID_OPT_HOURS_INTERVAL,
    myID_OPT_SKIP_VMWARE_ADAPTER,

    /* log */
};

#endif /* _WIDGET_ID_H_ */
