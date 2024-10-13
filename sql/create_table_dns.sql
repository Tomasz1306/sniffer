CREATE TABLE IF NOT EXISTS dns
(
    dns_id         INT AUTO_INCREMENT PRIMARY KEY,
    dns_transaction_id INT,
    dns_flags          VARCHAR(100),
    dns_questions      INT,
    dns_answer_rrs     INT,
    dns_authority_rrs  INT,
    dns_additional_rrs INT,
    dns_queries        BLOB,
    dns_answers        BLOB
);