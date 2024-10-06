CREATE TABLE IF NOT EXISTS ethernet
(
    ethernet_id       INT AUTO_INCREMENT PRIMARY KEY,
    ethernet_src_mac  CHAR(17),
    ethernet_dst_mac  CHAR(17),
    ethernet_eht_type VARCHAR(10)
);