CREATE TABLE IF NOT EXISTS ftp
(
    ftp_id          INT AUTO_INCREMENT PRIMARY KEY,
    ftp_command     VARCHAR(100),
    ftp_response_code INT,
    ftp_response_message TEXT,
    ftp_headers     BLOB,
    ftp_payload     BLOB
);