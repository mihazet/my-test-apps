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
INSERT INTO PropTbl VALUES ("Brightness", "BRIGHTNESS", "Numeric", "0;255", "127", "127");
INSERT INTO PropTbl VALUES ("Contrast", "CONTRAST", "Numeric", "0;255", "127", "127");
INSERT INTO PropTbl VALUES ("Color Temp", "DLP_COLOR_TEMPERATURE", "List", "6500K;9300K;USER1;USER2;USER3", "USER1", "USER1");
INSERT INTO PropTbl VALUES ("CutOff R", "R_CUTOFF", "Numeric", "0;255", "127", "127");
INSERT INTO PropTbl VALUES ("CutOff G", "G_CUTOFF", "Numeric", "0;255", "127", "127");
INSERT INTO PropTbl VALUES ("CutOff B", "B_CUTOFF", "Numeric", "0;255", "127", "127");
INSERT INTO PropTbl VALUES ("Drive R", "R_DRIVE", "Numeric", "0;255", "127", "127");
INSERT INTO PropTbl VALUES ("Drive G", "G_DRIVE", "Numeric", "0;255", "127", "127");
INSERT INTO PropTbl VALUES ("Drive B", "B_DRIVE", "Numeric", "0;255", "127", "127");
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
INSERT INTO PropTbl VALUES ("Aspect Ratio", "ASPECT RATIO", "List", "4_BY_3;16_BY_9", "4_BY_3", "4_BY_3");
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
INSERT INTO PropTbl VALUES ("Capture H", "H_CAPTURE", "Numeric", "-200;200", "0", "0");
INSERT INTO PropTbl VALUES ("CApture V", "V_CAPTURE", "Numeric", "-50;50", "0", "0");
INSERT INTO PropTbl VALUES ("Frequency", "ADC_FREQUENCY", "Numeric", "-100;100", "0", "0");
INSERT INTO PropTbl VALUES ("Phase", "ADC_PHASE", "Numeric", "-16;15", "0", "0");
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
INSERT INTO PropTbl VALUES ("Menu H Position", "MENU_H_POSITION", "Numeric", "0;50", "0", "0");
INSERT INTO PropTbl VALUES ("Menu V Position", "MENU_V_POSITION", "Numeric", "0;50", "0", "0");
INSERT INTO PropTbl VALUES ("Lamp Switch", "LAMP_SELECTION", "List", "LAMPA;LAMPB;LAMPA+B", "LAMPA+B", "LAMPA+B");
INSERT INTO PropTbl VALUES ("Lamp A Power", "BALLAST_A_POWER", "Numeric", "0;14", "14", "14");
INSERT INTO PropTbl VALUES ("Lamp B Power", "BALLAST_B_POWER", "Numeric", "0;14", "14", "14");
INSERT INTO PropTbl VALUES ("Lamp Hot Swap", "LAMP_HOT_SWAP", "List", "OFF;ON", "OFF", "OFF");
INSERT INTO PropTbl VALUES ("Lamp Error Auto Switch", "LAMP_ERROR_AUT_SWITCH", "List", "OFF;ON", "ON", "ON");
INSERT INTO PropTbl VALUES ("Lamp Timeout Change", "LAMP_TIMEOUT_CHANGE", "List", "OFF;ON", "ON", "ON");
INSERT INTO PropTbl VALUES ("Lamp Timeout Change Hours", "LAMP_TIMEOUT_CHANGE_HOURS", "Numeric", "100;1000", "100", "100");

-- configuration table definition
CREATE TABLE CfgTbl (ConfigName TEXT UNIQUE, ConfigValue TEXT);

-- configuration list
INSERT INTO CfgTbl VALUES ("Language", "System");
INSERT INTO CfgTbl VALUES ("Mode", "Normal");
INSERT INTO CfgTbl VALUES ("ListenPort", "40000");
INSERT INTO CfgTbl VALUES ("FrameX", "-1");
INSERT INTO CfgTbl VALUES ("FrameY", "-1");
INSERT INTO CfgTbl VALUES ("FrameW", "-1");
INSERT INTO CfgTbl VALUES ("FrameH", "-1");
