CREATE TABLE IF NOT EXISTS http_response
(
    http_response_id INT AUTO_INCREMENT PRIMARY KEY,
    http_status_code INT,
    http_reason_phrase TEXT,
    http_version     VARCHAR(10),
    http_headers     BLOB,
    http_body        BLOB
);