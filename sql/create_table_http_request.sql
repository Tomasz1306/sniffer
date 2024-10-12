CREATE TABLE IF NOT EXISTS http_request
(
    http_request_id  INT AUTO_INCREMENT PRIMARY KEY,
    http_url         TEXT,
    http_version     VARCHAR(10),
    http_method      VARCHAR(10),
    http_headers     BLOB,
    http_body        BLOB
);