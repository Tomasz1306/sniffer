CREATE TABLE IF NOT EXISTS ftp_response
(
    ftp_response_id INT AUTO_INCREMENT PRIMARY KEY,
    ftp_response_code INT,
    ftp_response_message TEXT,
    ftp_headers     BLOB,
    ftp_payload     BLOB
);