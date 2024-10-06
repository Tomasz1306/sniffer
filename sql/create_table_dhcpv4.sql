CREATE TABLE IF NOT EXISTS dhcpv4
(
    dhcpv4_id         INT AUTO_INCREMENT PRIMARY KEY,
    dhcpv4_op         INT,
    dhcpv4_htype      INT,
    dhcpv4_hlen       INT,
    dhcpv4_hops       INT,
    dhcpv4_xid        INT,
    dhcpv4_secs       INT,
    dhcpv4_flags      INT,
    dhcpv4_client_ip  VARCHAR(15),
    dhcpv4_your_ip    VARCHAR(15),
    dhcpv4_server_ip  VARCHAR(15),
    dhcpv4_gateway_ip VARCHAR(15),
    dhcpv4_client_mac CHAR(17),
    dhcpv4_options    BLOB
);