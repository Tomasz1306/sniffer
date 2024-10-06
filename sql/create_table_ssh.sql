CREATE TABLE IF NOT EXISTS ssh
(
    ssh_id       INT AUTO_INCREMENT PRIMARY KEY,
    message_type INT,
    payload      BLOB
);