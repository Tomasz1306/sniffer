CREATE TABLE IF NOT EXISTS icmpv4
(
    icmpv4_id             INT AUTO_INCREMENT PRIMARY KEY,
    icmpv4_type           INT,
    icmpv4_code           INT,
    icmpv4_checksum       INT,
    icmpv4_rest_of_header BLOB
);