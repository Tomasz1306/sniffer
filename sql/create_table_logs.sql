CREATE TABLE IF NOT EXISTS logs (
    log_id INT AUTO_INCREMENT PRIMARY KEY,
    log_date DATETIME,
    log_description VARCHAR(400),
    log_status VARCHAR(100)
    session_id INT,
    FOREIGN KEY (session_id) REFERENCES Sessions(session_id)
);