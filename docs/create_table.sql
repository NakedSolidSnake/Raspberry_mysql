CREATE DATABASE led_info;

USE led_info;

CREATE TABLE led_table(
    id          int AUTO_INCREMENT,
    timestamp   NUMERIC NOT NULL,
    state       VARCHAR(10) NOT NULL, 
    PRIMARY KEY (id)
);