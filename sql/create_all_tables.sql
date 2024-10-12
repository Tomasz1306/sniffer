CREATE TABLE IF NOT EXISTS ethernet
(
    ethernet_id       INT AUTO_INCREMENT PRIMARY KEY,
    ethernet_src_mac  CHAR(17),
    ethernet_dst_mac  CHAR(17),
    ethernet_eht_type VARCHAR(10)
);

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

CREATE TABLE IF NOT EXISTS icmpv4
(
    icmpv4_id             INT AUTO_INCREMENT PRIMARY KEY,
    icmpv4_type           INT,
    icmpv4_code           INT,
    icmpv4_checksum       INT,
    icmpv4_rest_of_header BLOB
);

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

CREATE TABLE IF NOT EXISTS tcp
(
    tcp_id                    INT AUTO_INCREMENT PRIMARY KEY,
    tcp_src_port              INT,
    tcp_dst_port              INT,
    tcp_sequence_number       INT,
    tcp_acknowledgment_number INT,
    tcp_data_offset           INT,
    tcp_reserved              INT,
    tcp_SYN                   BOOLEAN,
    tcp_ACK                   BOOLEAN,
    tcp_FIN                   BOOLEAN,
    tcp_RST                   BOOLEAN,
    tcp_URG                   BOOLEAN,
    tcp_PSH                   BOOLEAN,
    tcp_window                INT,
    tcp_checksum              INT,
    tcp_urgent_pointer        INT,
    tcp_options               BLOB
);

CREATE TABLE IF NOT EXISTS udp
(
    udp_id   INT AUTO_INCREMENT PRIMARY KEY,
    udp_src_port INT,
    udp_dst_port INT,
    udp_length   INT,
    udp_checksum INT
);

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

CREATE TABLE IF NOT EXISTS dns
(
    dns_id         INT AUTO_INCREMENT PRIMARY KEY,
    dns_transaction_id INT,
    dns_flags          INT,
    dns_questions      INT,
    dns_answer_rrs     INT,
    dns_authority_rrs  INT,
    dns_additional_rrs INT,
    dns_queries        BLOB,
    dns_answers        BLOB
);

CREATE TABLE IF NOT EXISTS ftp
(
    ftp_id           INT AUTO_INCREMENT PRIMARY KEY,
    ftp_command          VARCHAR(255),
    ftp_argument         VARCHAR(255),
    ftp_response_code    INT,
    ftp_response_message VARCHAR(255)
);

CREATE TABLE IF NOT EXISTS http
(
    http_id       INT AUTO_INCREMENT PRIMARY KEY,
    http_url           TEXT,
    http_version  VARCHAR(10),
    http_status_code   INT,
    http_reason_phrase TEXT,
    http_headers       BLOB,
    http_body          BLOB
);

CREATE TABLE IT NOT EXISTS smtp
(
    smtp_id INT AUTO_INCREMENT PRIMARY KEY,
    smtp_payload TEXT
);

CREATE TABLE IF NOT EXISTS ssh
(
    ssh_id       INT AUTO_INCREMENT PRIMARY KEY,
    ssh_payload      BLOB
);

CREATE TABLE IF NOT EXISTS telnet
(
    telent_id INT AUTO_INCREMENT PRIMARY KEY,
    telnet_data BLOB
);

CREATE TABLE IF NOT EXISTS Packets
(
    packet_id           INT PRIMARY KEY,
    packet_capture_date DATETIME NOT NULL,
    ethernet_id         INT,
    ipv4_id             INT,
    ipv6_id             INT,
    tcp_id              INT,
    udp_id              INT,
    http_id             INT,
    ftp_id              INT,
    smtp_id             INT,
    ssh_id              INT,
    telent_id           INT,
    icmpv4_id           INT,
    icmpv6_id           INT,
    dhcpv4_id           INT,
    dhcpv6_id           INT,
    dns_id              INT,
    arp_id              INT,
    FOREIGN KEY (ethernet_id) REFERENCES ethernet (ethernet_id),
    FOREIGN KEY (ipv4_id) REFERENCES ipv4 (ipv4_id),
    FOREIGN KEY (ipv6_id) REFERENCES ipv6 (ipv6_id),
    FOREIGN KEY (tcp_id) REFERENCES tcp (tcp_id),
    FOREIGN KEY (udp_id) REFERENCES udp (udp_id),
    FOREIGN KEY (http_id) REFERENCES http (http_id),
    FOREIGN KEY (ftp_id) REFERENCES ftp (ftp_id),
    FOREIGN KEY (smtp_id) REFERENCES smtp (smtp_id),
    FOREIGN KEY (ssh_id) REFERENCES ssh (ssh_id),
    FOREIGN KEY (telent_id) REFERENCES telnet (telent_id),
    FOREIGN KEY (icmpv4_id) REFERENCES icmpv4 (icmpv4_id),
    FOREIGN KEY (icmpv6_id) REFERENCES icmpv6 (icmpv6_id),
    FOREIGN KEY (dhcpv4_id) REFERENCES dhcpv4 (dhcpv4_id),
    FOREIGN KEY (dhcpv6_id) REFERENCES dhcpv6 (dhcpv6_id),
    FOREIGN KEY (dns_id) REFERENCES dns (dns_id),
    FOREIGN KEY (arp_id) REFERENCES arp (arp_id)
);

CREATE TABLE IF NOT EXISTS Interfaces
(
    interface_id              INT          NOT NULL PRIMARY KEY,
    interface_name            VARCHAR(100) NOT NULL,
    interface_ipv4            VARCHAR(15)  NOT NULL,
    interface_ipv6            VARCHAR(100) NOT NULL,
    interface_mac_address     VARCHAR(17)  NOT NULL,
    interface_default_gateway VARCHAR(15)  NOT NULL,
    interface_dns_server      VARCHAR(15)  NOT NULL,
    interface_mtu             INT          NOT NULL,
    packet_id                 INT,
    FOREIGN KEY (packet_id) REFERENCES Packets (packet_id)

);

CREATE TABLE IF NOT EXISTS Sessions
(
    session_id   INT      NOT NULL PRIMARY KEY,
    session_date DATETIME NOT NULL,
    interface_id INT,
    FOREIGN KEY (interface_id) REFERENCES Interfaces (interface_id)

);






