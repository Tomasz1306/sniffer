CREATE TABLE IF NOT EXISTS ftp_request
(
    ftp_request_id  INT AUTO_INCREMENT PRIMARY KEY,
    ftp_command     VARCHAR(100),
    ftp_arguments   TEXT,
    ftp_headers     BLOB,
    ftp_payload     BLOB
);