CREATE TABLE IF NOT EXISTS http
(
    http_id       INT AUTO_INCREMENT PRIMARY KEY,
    url           TEXT,
    http_version  VARCHAR(10),
    status_code   INT,
    reason_phrase TEXT,
    headers       BLOB,
    body          BLOB
);