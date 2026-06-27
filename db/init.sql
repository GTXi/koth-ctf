CREATE DATABASE IF NOT EXISTS koth;
USE koth;

CREATE TABLE IF NOT EXISTS koth_standings (
    user           VARCHAR(50) PRIMARY KEY,
    points         INT DEFAULT 0,
    datetime_stamp DATETIME DEFAULT CURRENT_TIMESTAMP
                   ON UPDATE CURRENT_TIMESTAMP
);

INSERT INTO koth_standings (user, points) VALUES ('Node-Afif',    0);
INSERT INTO koth_standings (user, points) VALUES ('Node-Syahmi',  0);
INSERT INTO koth_standings (user, points) VALUES ('Node-Irfan',   0);
INSERT INTO koth_standings (user, points) VALUES ('Node-Hairiel', 0);
INSERT INTO koth_standings (user, points) VALUES ('Node-Faiz',    0);
