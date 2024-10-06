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