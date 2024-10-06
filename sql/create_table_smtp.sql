CREATE TABLE IF NOT EXISTS smtp
(
    smtp_id          INT AUTO_INCREMENT PRIMARY KEY,
    command          VARCHAR(255),
    argument         TEXT,
    response_code    INT,
    response_message TEXT
);