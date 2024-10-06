CREATE TABLE IF NOT EXISTS ftp
(
    ftp_id           INT AUTO_INCREMENT PRIMARY KEY,
    command          VARCHAR(255),
    argument         VARCHAR(255),
    response_code    INT,
    response_message VARCHAR(255)
);