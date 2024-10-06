CREATE TABLE IF NOT EXISTS arp
(
    arp_id         INT AUTO_INCREMENT PRIMARY KEY,
    arp_hw_type    VARCHAR(50),
    arp_proto_type VARCHAR(50),
    arp_hw_size    INT,
    arp_proto_size INT,
    arp_opcode     VARCHAR(50),
    arp_src_mac    CHAR(17),
    arp_src_ip     VARCHAR(15),
    arp_dst_mac    CHAR(17),
    arp_dst_ip     VARCHAR(15)
);