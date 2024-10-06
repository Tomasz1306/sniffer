CREATE TABLE IF NOT EXISTS ipv4
(
    ipv4_id              INT AUTO_INCREMENT PRIMARY KEY,
    ipv4_version         INT,
    ipv4_ihl             INT,
    ipv4_tos             INT,
    ipv4_total_length    INT,
    ipv4_identification  INT,
    ipv4_flags           INT,
    ipv4_fragment_offset INT,
    ipv4_ttl             INT,
    ipv4_protocol        INT,
    ipv4_header_checksum INT,
    ipv4_src_ip          VARCHAR(15),
    ipv4_dst_ip          VARCHAR(15)
);