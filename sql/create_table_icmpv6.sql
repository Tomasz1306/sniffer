CREATE TABLE IF NOT EXISTS icmpv6
(
    icmpv6_id             INT AUTO_INCREMENT PRIMARY KEY,
    icmpv6_type           INT,
    icmpv6_code           INT,
    icmpv6_checksum       INT,
    icmpv6_rest_of_header BLOB
);
