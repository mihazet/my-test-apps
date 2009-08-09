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

-- configuration table definition
CREATE TABLE configuration (ConfigName TEXT, ConfigValue TEXT);

-- configuration list
INSERT INTO configuration VALUES ("BoardName", "SimCube");
INSERT INTO configuration VALUES ("ListenPort", "40000");
