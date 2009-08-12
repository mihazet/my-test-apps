-- property table definition
CREATE TABLE property (DisplayedName TEXT, -- property displayed name
                    ProtocolName TEXT, -- real protocol command string
                    PropertyType TEXT, -- property type
                    PropertyFormat TEXT, -- property format
                    PropertyValue TEXT -- current property value
);

-- property list
INSERT INTO property VALUES ("Brightness", "BRIGHTNESS", "Numeric", "0;255", "127");
INSERT INTO property VALUES ("Contrast", "CONTRAST", "Numeric", "0;255", "127");
INSERT INTO property VALUES ("Color Temp", "DLP_COLOR_TEMPERATURE", "List", "6500K;9300K;USER1;USER2;USER3", "USER1");
INSERT INTO property VALUES ("CutOff R", "R_CUTOFF", "Numeric", "0;255", "127");
INSERT INTO property VALUES ("CutOff G", "G_CUTOFF", "Numeric", "0;255", "127");
INSERT INTO property VALUES ("CutOff B", "B_CUTOFF", "Numeric", "0;255", "127");
INSERT INTO property VALUES ("Drive R", "R_DRIVE", "Numeric", "0;255", "127");
INSERT INTO property VALUES ("Drive G", "G_DRIVE", "Numeric", "0;255", "127");
INSERT INTO property VALUES ("Drive B", "B_DRIVE", "Numeric", "0;255", "127");
INSERT INTO property VALUES ("Color", "ADV_COLOR", "Numeric", "0;100", "50");
INSERT INTO property VALUES ("Hue", "ADV_HUE", "Numeric", "0;360", "180");
INSERT INTO property VALUES ("Black Level", "ADV_BLACK", "List", "0IRE;7.5IRE", "0IRE");
INSERT INTO property VALUES ("Sharpness", "ADV_SHARPNESS", "Numeric", "0;100", "50");
INSERT INTO property VALUES ("Detail Enhancement", "ADV_DETAIL_ENHANCEMENT", "Numeric", "0;100", "0");
INSERT INTO property VALUES ("Input Offset R", "R_INPUT_OFFSET", "Numeric", "0;127", "63");
INSERT INTO property VALUES ("Input Offset G", "G_INPUT_OFFSET", "Numeric", "0;127", "63");
INSERT INTO property VALUES ("Input Offset B", "B_INPUT_OFFSET", "Numeric", "0;127", "63");
INSERT INTO property VALUES ("Input Gain R", "R_INPUT_GAIN", "Numeric", "0;255", "127");
INSERT INTO property VALUES ("Input Gain G", "G_INPUT_GAIN", "Numeric", "0;255", "127");
INSERT INTO property VALUES ("Input Gain B", "B_INPUT_GAIN", "Numeric", "0;255", "127");
INSERT INTO property VALUES ("Video Brightness", "VIDEO_BRIGHTNESS", "Numeric", "0;255", "128");
INSERT INTO property VALUES ("Video Contrast", "VIDEO_CONTRAST", "Numeric", "0;255", "128");
INSERT INTO property VALUES ("Video Sharpness", "VIDEO_SHARPNESS", "Numeric", "0;6", "3");
INSERT INTO property VALUES ("Video Color", "VIDEO_COLOR", "Numeric", "0;255", "128");
INSERT INTO property VALUES ("Video Tint", "VIDEO_TINT", "Numeric", "0;255", "128");
INSERT INTO property VALUES ("Video Standard", "VIDEO_STANDARD", "List", "AUTO;NTSC_M;NTSC_44;PAL_BG_DK;PAL_M;PAL_N;SECAM", "AUTO");
INSERT INTO property VALUES ("Video Black Level", "VIDEO_BLACK", "List", "0IRE;7.5IRE", "0IRE");
INSERT INTO property VALUES ("Video Overscan", "VIDEO_OVERSCAN", "List", "OFF;ON", "OFF");
INSERT INTO property VALUES ("Aspect Ratio", "ASPECT RATIO", "List", "4_BY_3;16_BY_9", "4_BY_3");
INSERT INTO property VALUES ("Function Type", "POSITION_TYPE", "List", "LEGACY;ANYPLACE", "ANYPLACE");
INSERT INTO property VALUES ("Position H", "H_POSITION", "Numeric", "-15;15", "0");
INSERT INTO property VALUES ("Position V", "V_POSITION", "Numeric", "-5;5", "0");
INSERT INTO property VALUES ("Top Left H", "TOPLEFT_H_POSITION", "Numeric", "-100;100", "0");
INSERT INTO property VALUES ("Top Left V", "TOPLEFT_V_POSITION", "Numeric", "-100;100", "0");
INSERT INTO property VALUES ("Top Right H", "TOPRIGHT_H_POSITION", "Numeric", "-100;100", "0");
INSERT INTO property VALUES ("Top Right V", "TOPRIGHT_V_POSITION", "Numeric", "-100;100", "0");
INSERT INTO property VALUES ("Bottom Left H", "BOTTOMLEFT_H_POSITION", "Numeric", "-100;100", "0");
INSERT INTO property VALUES ("Bottom Left V", "BOTTOMLEFT_V_POSITION", "Numeric", "-100;100", "0");
INSERT INTO property VALUES ("Bottom Right H", "BOTTOMRIGHT_H_POSITION", "Numeric", "-100;100", "0");
INSERT INTO property VALUES ("Bottom Right V", "BOTTOMRIGHT_V_POSITION", "Numeric", "-100;100", "0");
INSERT INTO property VALUES ("Capture H", "H_CAPTURE", "Numeric", "-200;200", "0");
INSERT INTO property VALUES ("CApture V", "V_CAPTURE", "Numeric", "-50;50", "0");
INSERT INTO property VALUES ("Frequency", "ADC_FREQUENCY", "Numeric", "-100;100", "0");
INSERT INTO property VALUES ("Phase", "ADC_PHASE", "Numeric", "-16;15", "0");
INSERT INTO property VALUES ("Main Input", "MAIN_INPUT", "List", "VGA1;VGA2;BNC1;BNC2;YPBPR1;YPBPR2;VIDEO;DVI1;DVI2;DVI3;DVI4", "VGA1");
INSERT INTO property VALUES ("PIP Mode", "PIP_MODE", "List", "OFF;ON", "OFF");
INSERT INTO property VALUES ("PIP Input", "PIP_INPUT", "List", "VIDEO", "VIDEO");
INSERT INTO property VALUES ("PIP H Position", "PIP_H_POSITION", "Numeric", "0;100", "0");
INSERT INTO property VALUES ("PIP V Position", "PIP_V_POSITION", "Numeric", "0;100", "0");
INSERT INTO property VALUES ("Load User Mode", "USER_MODE_LOAD", "Numeric", "1;30", "1");
INSERT INTO property VALUES ("Save User Mode", "USER_MODE_SAVE", "Numeric", "1;30", "1");
INSERT INTO property VALUES ("Language", "LANGUAGE", "List", "ENGLISH;SIMPLIFIED_CHINESE", "ENGLISH");
INSERT INTO property VALUES ("Image Orientation", "IMAGE_ORIENTATION", "List", "FRONT;REAR;INVERTED_FRONT;INVERTED_REAR", "FRONT");
INSERT INTO property VALUES ("Baud Rate", "BAUD_RATE", "List", "9600;19200;57600;115200", "115200");
INSERT INTO property VALUES ("Gamma", "DLP_GAMMA", "List", "FILM;GRAPHIC;VIDEO;LINEAR;NORMAL;MAX.BRIGHTNESS", "FILM");
INSERT INTO property VALUES ("Menu H Position", "MENU_H_POSITION", "Numeric", "0;50", "0");
INSERT INTO property VALUES ("Menu V Position", "MENU_V_POSITION", "Numeric", "0;50", "0");
INSERT INTO property VALUES ("Reset All Setting", "RESET_ALL", "List", "OFF;ON", "OFF");

-- configuration table definition
CREATE TABLE configuration (ConfigName TEXT, ConfigValue TEXT);

-- configuration list
INSERT INTO configuration VALUES ("BoardName", "SimCube");
INSERT INTO configuration VALUES ("ListenPort", "40000");
