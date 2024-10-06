CREATE TABLE IF NOT EXISTS dns
(
    dns_id         INT AUTO_INCREMENT PRIMARY KEY,
    transaction_id INT,
    flags          INT,
    questions      INT,
    answer_rrs     INT,
    authority_rrs  INT,
    additional_rrs INT,
    queries        BLOB,
    answers        BLOB
);