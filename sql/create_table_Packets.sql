CREATE TABLE IF NOT EXISTS Packets
(
    packet_id           INT AUTO_INCREMENT PRIMARY KEY,
    packet_capture_date DATETIME NOT NULL,
    ethernet_id         INT,
    ipv4_id             INT,
    ipv6_id             INT,
    tcp_id              INT,
    udp_id              INT,
    http_request_id     INT,
    http_response_id    INT,
    ftp_request_id      INT,
    ftp_response_id     INT,
    smtp_id             INT,
    ssh_id              INT,
    telent_id           INT,
    icmpv4_id           INT,
    dhcpv4_id           INT,
    dns_id              INT,
    arp_id              INT,
    interface_id        INT,
    FOREIGN KEY (ethernet_id) REFERENCES ethernet (ethernet_id),
    FOREIGN KEY (ipv4_id) REFERENCES ipv4 (ipv4_id),
    FOREIGN KEY (ipv6_id) REFERENCES ipv6 (ipv6_id),
    FOREIGN KEY (tcp_id) REFERENCES tcp (tcp_id),
    FOREIGN KEY (udp_id) REFERENCES udp (udp_id),
    FOREIGN KEY (http_request_id) REFERENCES http_request (http_request_id),
    FOREIGN KEY (http_response_id) REFERENCES http_response (http_response_id),
    FOREIGN KEY (ftp_request_id) REFERENCES ftp_request (ftp_request_id),
    FOREIGN KEY (ftp_response_id) REFERENCES ftp_response (ftp_response_id),
    FOREIGN KEY (smtp_id) REFERENCES smtp (smtp_id),
    FOREIGN KEY (ssh_id) REFERENCES ssh (ssh_id),
    FOREIGN KEY (telent_id) REFERENCES telnet (telent_id),
    FOREIGN KEY (icmpv4_id) REFERENCES icmpv4 (icmpv4_id),
    FOREIGN KEY (dhcpv4_id) REFERENCES dhcpv4 (dhcpv4_id),
    FOREIGN KEY (dns_id) REFERENCES dns (dns_id),
    FOREIGN KEY (arp_id) REFERENCES arp (arp_id),
    FOREIGN KEY (interface_id) REFERENCES Interfaces (interface_id)
);