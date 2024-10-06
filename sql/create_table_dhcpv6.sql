CREATE TABLE IF NOT EXISTS dhcpv6
(
    dhcpv6_id      INT AUTO_INCREMENT PRIMARY KEY,
    msg_type       INT,
    transaction_id INT,
    options        BLOB
);