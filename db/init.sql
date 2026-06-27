CREATE DATABASE IF NOT EXISTS koth;
USE koth;

CREATE TABLE IF NOT EXISTS koth_standings (
    user           VARCHAR(50) PRIMARY KEY,
    points         INT DEFAULT 0,
    datetime_stamp DATETIME DEFAULT CURRENT_TIMESTAMP
                   ON UPDATE CURRENT_TIMESTAMP
);

INSERT INTO koth_standings (user, points) VALUES ('Afif',    0);
INSERT INTO koth_standings (user, points) VALUES ('Syahmi',  0);
INSERT INTO koth_standings (user, points) VALUES ('Irfan',   0);
INSERT INTO koth_standings (user, points) VALUES ('Hairiel', 0);
INSERT INTO koth_standings (user, points) VALUES ('Faiz',    0);
