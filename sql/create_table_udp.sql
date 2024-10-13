CREATE TABLE IF NOT EXISTS udp
(
    udp_id   INT AUTO_INCREMENT PRIMARY KEY,
    udp_src_port INT,
    udp_dst_port INT,
    udp_length   INT,
    udp_checksum INT
);