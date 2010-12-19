-- property table definition
CREATE TABLE PropTbl (DisplayedName TEXT UNIQUE, -- property displayed name
                    ProtocolName TEXT UNIQUE, -- real protocol command string
                    PropertyType TEXT, -- property type
                    PropertyFormat TEXT, -- property format
                    InitValue TEXT, -- init property value
                    CurrentValue TEXT -- current property value
);

-- property list
INSERT INTO PropTbl VALUES ("BoardName", "BOARD_NAME", "String", "19", "SimCube", "SimCube");
INSERT INTO PropTbl VALUES ("Board H/W Revison", "BOARD_HW_REV", "String", "11", "2973043701", "2973043701");
INSERT INTO PropTbl VALUES ("DHCP Client", "DHCP_CLIENT", "List", "DISABLE;ENABLE", "ENABLE", "ENABLE");
INSERT INTO PropTbl VALUES ("Host Domain", "HOST_DOMAIN", "String", "31", "deltaww.com", "deltaww.com");
INSERT INTO PropTbl VALUES ("MAC Address", "MAC_ADDR", "String", "19", "00:18:23:4D:41:43", "00:18:23:4D:41:43");
INSERT INTO PropTbl VALUES ("Netmask", "NETMASK", "String", "19", "255.255.255.0", "255.255.255.0");
INSERT INTO PropTbl VALUES ("Static IP", "STATIC_IP", "String", "19", "192.168.0.1", "192.168.0.1");
INSERT INTO PropTbl VALUES ("Brightness", "BRIGHTNESS", "Numeric", "0;255", "127", "127");
INSERT INTO PropTbl VALUES ("Contrast", "CONTRAST", "Numeric", "0;255", "127", "127");
INSERT INTO PropTbl VALUES ("Color Temp", "DLP_COLOR_TEMPERATURE", "List", "6500K;9300K;USER1;USER2;USER3", "USER1", "USER1");
INSERT INTO PropTbl VALUES ("CutOff R", "R_CUTOFF", "Numeric", "0;255", "128", "128");
INSERT INTO PropTbl VALUES ("CutOff G", "G_CUTOFF", "Numeric", "0;255", "128", "128");
INSERT INTO PropTbl VALUES ("CutOff B", "B_CUTOFF", "Numeric", "0;255", "128", "128");
INSERT INTO PropTbl VALUES ("Drive R", "R_DRIVE", "Numeric", "0;200", "100", "100");
INSERT INTO PropTbl VALUES ("Drive G", "G_DRIVE", "Numeric", "0;200", "100", "100");
INSERT INTO PropTbl VALUES ("Drive B", "B_DRIVE", "Numeric", "0;200", "100", "100");
INSERT INTO PropTbl VALUES ("Color", "ADV_COLOR", "Numeric", "0;100", "50", "50");
INSERT INTO PropTbl VALUES ("Hue", "ADV_HUE", "Numeric", "0;360", "180", "180");
INSERT INTO PropTbl VALUES ("Black Level", "ADV_BLACK", "List", "0IRE;7.5IRE", "0IRE", "0IRE");
INSERT INTO PropTbl VALUES ("Sharpness", "ADV_SHARPNESS", "Numeric", "0;100", "50", "50");
INSERT INTO PropTbl VALUES ("Detail Enhancement", "ADV_DETAIL_ENHANCEMENT", "Numeric", "0;100", "0", "0");
INSERT INTO PropTbl VALUES ("Input Offset R", "R_INPUT_OFFSET", "Numeric", "0;127", "63", "63");
INSERT INTO PropTbl VALUES ("Input Offset G", "G_INPUT_OFFSET", "Numeric", "0;127", "63", "63");
INSERT INTO PropTbl VALUES ("Input Offset B", "B_INPUT_OFFSET", "Numeric", "0;127", "63", "63");
INSERT INTO PropTbl VALUES ("Input Gain R", "R_INPUT_GAIN", "Numeric", "0;255", "127", "127");
INSERT INTO PropTbl VALUES ("Input Gain G", "G_INPUT_GAIN", "Numeric", "0;255", "127", "127");
INSERT INTO PropTbl VALUES ("Input Gain B", "B_INPUT_GAIN", "Numeric", "0;255", "127", "127");
INSERT INTO PropTbl VALUES ("Video Brightness", "VIDEO_BRIGHTNESS", "Numeric", "0;255", "128", "128");
INSERT INTO PropTbl VALUES ("Video Contrast", "VIDEO_CONTRAST", "Numeric", "0;255", "128", "128");
INSERT INTO PropTbl VALUES ("Video Sharpness", "VIDEO_SHARPNESS", "Numeric", "0;6", "3", "3");
INSERT INTO PropTbl VALUES ("Video Color", "VIDEO_COLOR", "Numeric", "0;255", "128", "128");
INSERT INTO PropTbl VALUES ("Video Tint", "VIDEO_TINT", "Numeric", "0;255", "128", "128");
INSERT INTO PropTbl VALUES ("Video Standard", "VIDEO_STANDARD", "List", "AUTO;NTSC_M;NTSC_44;PAL_BG_DK;PAL_M;PAL_N;SECAM", "AUTO", "AUTO");
INSERT INTO PropTbl VALUES ("Video Black Level", "VIDEO_BLACK", "List", "0IRE;7.5IRE", "0IRE", "0IRE");
INSERT INTO PropTbl VALUES ("Video Overscan", "VIDEO_OVERSCAN", "List", "OFF;ON", "OFF", "OFF");
INSERT INTO PropTbl VALUES ("Aspect Ratio", "ASPECT_RATIO", "List", "4_BY_3;16_BY_9", "4_BY_3", "4_BY_3");
INSERT INTO PropTbl VALUES ("Function Type", "POSITION_TYPE", "List", "LEGACY;ANYPLACE", "ANYPLACE", "ANYPLACE");
INSERT INTO PropTbl VALUES ("Position H", "H_POSITION", "Numeric", "-15;15", "0", "0");
INSERT INTO PropTbl VALUES ("Position V", "V_POSITION", "Numeric", "-5;5", "0", "0");
INSERT INTO PropTbl VALUES ("Top Left H", "TOPLEFT_H_POSITION", "Numeric", "-100;100", "0", "0");
INSERT INTO PropTbl VALUES ("Top Left V", "TOPLEFT_V_POSITION", "Numeric", "-100;100", "0", "0");
INSERT INTO PropTbl VALUES ("Top Right H", "TOPRIGHT_H_POSITION", "Numeric", "-100;100", "0", "0");
INSERT INTO PropTbl VALUES ("Top Right V", "TOPRIGHT_V_POSITION", "Numeric", "-100;100", "0", "0");
INSERT INTO PropTbl VALUES ("Bottom Left H", "BOTTOMLEFT_H_POSITION", "Numeric", "-100;100", "0", "0");
INSERT INTO PropTbl VALUES ("Bottom Left V", "BOTTOMLEFT_V_POSITION", "Numeric", "-100;100", "0", "0");
INSERT INTO PropTbl VALUES ("Bottom Right H", "BOTTOMRIGHT_H_POSITION", "Numeric", "-100;100", "0", "0");
INSERT INTO PropTbl VALUES ("Bottom Right V", "BOTTOMRIGHT_V_POSITION", "Numeric", "-100;100", "0", "0");
INSERT INTO PropTbl VALUES ("Reset Corner Position", "RESET_CORNER_POSITION", "List", "TOPLEFT;TOPRIGHT;BOTTOMLEFT;BOTTOMRIGHT;ALL", "ALL", "ALL");
INSERT INTO PropTbl VALUES ("Capture H", "H_CAPTURE", "Numeric", "-200;200", "0", "0");
INSERT INTO PropTbl VALUES ("CApture V", "V_CAPTURE", "Numeric", "-50;50", "0", "0");
INSERT INTO PropTbl VALUES ("ADC Frequency", "ADC_FREQUENCY", "Numeric", "-100;100", "0", "0");
INSERT INTO PropTbl VALUES ("ADC Phase", "ADC_PHASE", "Numeric", "-16;15", "0", "0");
INSERT INTO PropTbl VALUES ("Main Input", "MAIN_INPUT", "List", "VGA1;VGA2;BNC1;BNC2;YPBPR1;YPBPR2;VIDEO;DVI1;DVI2;DVI3;DVI4", "VGA1", "VGA1");
INSERT INTO PropTbl VALUES ("PIP Mode", "PIP_MODE", "List", "OFF;ON", "OFF", "OFF");
INSERT INTO PropTbl VALUES ("PIP Input", "PIP_INPUT", "List", "VIDEO", "VIDEO", "VIDEO");
INSERT INTO PropTbl VALUES ("PIP H Position", "PIP_H_POSITION", "Numeric", "0;100", "0", "0");
INSERT INTO PropTbl VALUES ("PIP V Position", "PIP_V_POSITION", "Numeric", "0;100", "0", "0");
INSERT INTO PropTbl VALUES ("Load User Mode", "USER_MODE_LOAD", "Numeric", "1;30", "1", "1");
INSERT INTO PropTbl VALUES ("Save User Mode", "USER_MODE_SAVE", "Numeric", "1;30", "1", "1");
INSERT INTO PropTbl VALUES ("Language", "LANGUAGE", "List", "ENGLISH;SIMPLIFIED_CHINESE", "ENGLISH", "ENGLISH");
INSERT INTO PropTbl VALUES ("Image Orientation", "IMAGE_ORIENTATION", "List", "FRONT;REAR;INVERTED_FRONT;INVERTED_REAR", "FRONT", "FRONT");
INSERT INTO PropTbl VALUES ("Baud Rate", "BAUD_RATE", "List", "9600;19200;57600;115200", "115200", "115200");
INSERT INTO PropTbl VALUES ("Gamma", "DLP_GAMMA", "List", "FILM;GRAPHIC;VIDEO;LINEAR;NORMAL;MAX.BRIGHTNESS", "FILM", "FILM");
INSERT INTO PropTbl VALUES ("Menu H Position", "MENU_H_POSITION", "Numeric", "0;50", "50", "50");
INSERT INTO PropTbl VALUES ("Menu V Position", "MENU_V_POSITION", "Numeric", "0;50", "0", "0");
INSERT INTO PropTbl VALUES ("Lamp Switch", "LAMP_SELECTION", "List", "LAMPA;LAMPB;LAMPA+B", "LAMPA+B", "LAMPA+B");
INSERT INTO PropTbl VALUES ("Lamp A Power", "BALLAST_A_POWER", "Numeric", "0;14", "14", "14");
INSERT INTO PropTbl VALUES ("Lamp B Power", "BALLAST_B_POWER", "Numeric", "0;14", "14", "14");
INSERT INTO PropTbl VALUES ("Lamp Hot Swap", "LAMP_HOT_SWAP", "List", "OFF;ON", "OFF", "OFF");
INSERT INTO PropTbl VALUES ("Lamp Error Auto Switch", "LAMP_ERROR_AUTO_SWITCH", "List", "OFF;ON", "ON", "ON");
INSERT INTO PropTbl VALUES ("Lamp Timeout Change", "LAMP_TIMEOUT_CHANGE", "List", "OFF;ON", "ON", "ON");
INSERT INTO PropTbl VALUES ("Lamp Timeout Change Hours", "LAMP_TIMEOUT_CHANGE_HOURS", "Numeric", "100;1000", "100", "100");
INSERT INTO PropTbl VALUES ("Lamp A Hours Threshold", "LAMP_A_HOURS_THRESHOLD", "Numeric", "100;10000", "3500", "3500");
INSERT INTO PropTbl VALUES ("Lamp B Hours Threshold", "LAMP_B_HOURS_THRESHOLD", "Numeric", "100;10000", "3500", "3500");
INSERT INTO PropTbl VALUES ("Lamp A Lit Count Threshold", "LAMP_A_LIT_COUNT_THRESHOLD", "Numeric", "100;5000", "1000", "1000");
INSERT INTO PropTbl VALUES ("Lamp B Lit Count Threshold", "LAMP_B_LIT_COUNT_THRESHOLD", "Numeric", "100;5000", "1000", "1000");
INSERT INTO PropTbl VALUES ("All Number Of Column", "WALL_COLUMN_DIMENSION", "Numeric", "1;8", "1", "1");
INSERT INTO PropTbl VALUES ("All Number Of Row", "WALL_ROW_DIMENSION", "Numeric", "1;8", "1", "1");
INSERT INTO PropTbl VALUES ("Projector Column", "PROJECTOR_COLUMN_LOCATION", "Numeric", "1;8", "1", "1");
INSERT INTO PropTbl VALUES ("Projector Row", "PROJECTOR_ROW_LOCATION", "Numeric", "1;8", "1", "1");
INSERT INTO PropTbl VALUES ("Projector Id", "PROJECTOR_ID", "Numeric", "0:99", "00", "00");
INSERT INTO PropTbl VALUES ("Power Delay", "POWER_DELAY", "Numeric", "0;10", "0", "0");
INSERT INTO PropTbl VALUES ("Frame Lock", "FRAME_LOCK", "List", "OFF;ON", "ON", "ON");
INSERT INTO PropTbl VALUES ("RGB First Auto", "RGB_FIRST_AUTO", "List", "OFF;ON", "OFF", "OFF");
INSERT INTO PropTbl VALUES ("AGC", "VIDEO_AGC", "List", "OFF;ON", "OFF", "OFF");
INSERT INTO PropTbl VALUES ("Test Pattern", "SCALAR_TEST_PATTERN", "Numeric", "0;59", "0", "0");
INSERT INTO PropTbl VALUES ("CCA", "CCA", "List", "OFF;ON", "OFF", "OFF");
INSERT INTO PropTbl VALUES ("R.Primary Red", "RED_PRIMARY_RED", "Numeric", "0;100", "100", "100");
INSERT INTO PropTbl VALUES ("R.Primary Green", "RED_PRIMARY_GREEN", "Numeric", "0;100", "0", "0");
INSERT INTO PropTbl VALUES ("R.Primary Blue", "RED_PRIMARY_BLUE", "Numeric", "0;100", "0", "0");
INSERT INTO PropTbl VALUES ("G.Primary Red", "GREEN_PRIMARY_RED", "Numeric", "0;100", "0", "0");
INSERT INTO PropTbl VALUES ("G.Primary Green", "GREEN_PRIMARY_GREEN", "Numeric", "0;100", "100", "100");
INSERT INTO PropTbl VALUES ("G.Primary Blue", "GREEN_PRIMARY_BLUE", "Numeric", "0;100", "0", "0");
INSERT INTO PropTbl VALUES ("B.Primary Red", "BLUE_PRIMARY_RED", "Numeric", "0;100", "0", "0");
INSERT INTO PropTbl VALUES ("B.Primary Green", "BLUE_PRIMARY_GREEN", "Numeric", "0;100", "0", "0");
INSERT INTO PropTbl VALUES ("B.Primary Blue", "BLUE_PRIMARY_BLUE", "Numeric", "0;100", "100", "100");
INSERT INTO PropTbl VALUES ("Force Timing", "FORCE_TIMING", "List", "DISABLE;640x350;720x400", "DISABLE", "DISABLE");
INSERT INTO PropTbl VALUES ("White Peaking", "WHITE_PEAKING", "Numeric", "0;100", "100", "100");
INSERT INTO PropTbl VALUES ("DVI34 EQ Setting", "DVI34_EQ", "List", "NORMAL;HIGH", "NORMAL", "NORMAL");
INSERT INTO PropTbl VALUES ("Brilliant Color Mode", "BRILLIANT_COLOR", "List", "DISABLE;ENABLE", "ENABLE", "ENABLE");
INSERT INTO PropTbl VALUES ("Edge Blending Alpha", "OVL_EB_ALPHA", "Numeric", "0;255", "0", "0");
INSERT INTO PropTbl VALUES ("Upper Blending", "OVL_EB_TOP_ENABLE", "List", "OFF;ON", "OFF", "OFF");
INSERT INTO PropTbl VALUES ("Upper Width", "OVL_EB_TOP_WIDTH", "Numeric", "0;128", "0", "0");
INSERT INTO PropTbl VALUES ("Lower Blending", "OVL_EB_BOTTOM_ENABLE", "List", "OFF;ON", "OFF", "OFF");
INSERT INTO PropTbl VALUES ("Lower Width", "OVL_EB_BOTTOM_WIDTH", "Numeric", "0;128", "0", "0");
INSERT INTO PropTbl VALUES ("Left Blending", "OVL_EB_LEFT_ENABLE", "List", "OFF;ON", "OFF", "OFF");
INSERT INTO PropTbl VALUES ("Left Width", "OVL_EB_LEFT_WIDTH", "Numeric", "0;128", "0", "0");
INSERT INTO PropTbl VALUES ("Right Blending", "OVL_EB_RIGHT_ENABLE", "List", "OFF;ON", "OFF", "OFF");
INSERT INTO PropTbl VALUES ("Right Width", "OVL_EB_RIGHT_WIDTH", "Numeric", "0;128", "0", "0");
INSERT INTO PropTbl VALUES ("EWarp App", "EWARP_APP", "List", "ANYPLACE;KEYSTONE;ROTATION", "ANYPLACE", "ANYPLACE");
INSERT INTO PropTbl VALUES ("H Keystone", "H_KEYSTONE", "Numeric", "-40;40", "0", "0");
INSERT INTO PropTbl VALUES ("V Keystone", "V_KEYSTONE", "Numeric", "-30;30", "0", "0");
INSERT INTO PropTbl VALUES ("Rotation", "ROTATION", "Numeric", "-180;180", "0", "0");
INSERT INTO PropTbl VALUES ("Pincushion", "PINCUSHION", "Numeric", "-20;20", "0", "0");
INSERT INTO PropTbl VALUES ("Projector Mounting", "PROJECTOR_MOUNTING", "List", "FRONT_TABLETOP;FRONT_CEILING;REAR_TABLETOP;REAR_CEILING", "FRONT_TABLETOP", "FRONT_TABLETOP");
INSERT INTO PropTbl VALUES ("Freeze", "FREEZE", "List", "OFF;ON", "OFF", "OFF");
INSERT INTO PropTbl VALUES ("Index Delay", "INDEX_DELAY", "Numeric", "0;359", "180", "180");
INSERT INTO PropTbl VALUES ("Key", "KEY", "List", "AUTO;BNC;BRIGHTNESS;CCA;CONTRAST;DIGITAL;DOWN;ENTER;FREEZE;LAMP;LEFT;MENU;OSDOFF;PIP;PIXEL;POSITION;POWER;POWERON;POWEROFF;PROJECTOR;RIGHT;STATUS;TEST.P;UP;VGA;VIDEO;YPBPR;0;1;2;3;4;5;6;7;8;9", "MENU", "MENU");


-- trap table definition
CREATE TABLE TrapTbl (ProtocolName TEXT UNIQUE, CurrentValue TEXT);

-- trap list
INSERT INTO TrapTbl VALUES ("LEDSTATUS", "8738");
INSERT INTO TrapTbl VALUES ("LAMP_A_STATUS", "IN_USE");
INSERT INTO TrapTbl VALUES ("LAMP_A_HOURS", "2345");
INSERT INTO TrapTbl VALUES ("LAMP_A_LIT_COUNT", "50");
INSERT INTO TrapTbl VALUES ("LAMP_A_TEMP_COND", "OK");
INSERT INTO TrapTbl VALUES ("LAMP_B_STATUS", "IN_USE");
INSERT INTO TrapTbl VALUES ("LAMP_B_HOURS", "3456");
INSERT INTO TrapTbl VALUES ("LAMP_B_LIT_COUNT", "50");
INSERT INTO TrapTbl VALUES ("LAMP_B_TEMP_COND", "OK");


-- configuration table definition
CREATE TABLE CfgTbl (ConfigName TEXT UNIQUE, ConfigValue TEXT);

-- configuration list
INSERT INTO CfgTbl VALUES ("Language", "System");
INSERT INTO CfgTbl VALUES ("ListenPort", "40000");
INSERT INTO CfgTbl VALUES ("FrameX", "-1");
INSERT INTO CfgTbl VALUES ("FrameY", "-1");
INSERT INTO CfgTbl VALUES ("FrameW", "-1");
INSERT INTO CfgTbl VALUES ("FrameH", "-1");
INSERT INTO CfgTbl VALUES ("Perspective", "");
INSERT INTO CfgTbl VALUES ("DefaultPerspective", "");

