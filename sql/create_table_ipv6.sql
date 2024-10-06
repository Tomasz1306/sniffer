CREATE TABLE IF NOT EXISTS ipv6
(
    ipv6_id             INT AUTO_INCREMENT PRIMARY KEY,
    ipv6_version        INT,
    ipv6_traffic_class  INT,
    ipv6_flow_label     INT,
    ipv6_payload_length INT,
    ipv6_next_header    INT,
    ipv6_hop_limit      INT,
    ipv6_src_ip         VARCHAR(39),
    ipv6_dst_ip         VARCHAR(39)
);