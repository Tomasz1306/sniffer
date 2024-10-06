CREATE TABLE IF NOT EXISTS Sessions
(
    session_id   INT      NOT NULL PRIMARY KEY,
    session_date DATETIME NOT NULL,
    interface_id INT,
    FOREIGN KEY (interface_id) REFERENCES Interfaces (interface_id)

);