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