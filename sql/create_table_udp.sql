CREATE TABLE IF NOT EXISTS udp
(
    udp_id   INT AUTO_INCREMENT PRIMARY KEY,
    src_port INT,
    dst_port INT,
    length   INT,
    checksum INT
);