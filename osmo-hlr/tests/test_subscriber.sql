
-- 2G only subscriber
INSERT INTO subscriber (id, imsi, msisdn) VALUES (1, '901990000000001', '1');
INSERT INTO auc_2g (subscriber_id, algo_id_2g, ki) VALUES (1, 1, '000102030405060708090a0b0c0d0e0f');

-- 3G only subscriber
INSERT INTO subscriber (id, imsi) VALUES (2, '901990000000002');
INSERT INTO auc_3g (subscriber_id, algo_id_3g, k, opc, sqn) VALUES (2, 5, '000102030405060708090a0b0c0d0e0f', '101112131415161718191a1b1c1d1e1f', 4223);

-- 2G + 3G subscriber
INSERT INTO subscriber (id, imsi, msisdn) VALUES (3, '901990000000003', '103');
INSERT INTO auc_2g (subscriber_id, algo_id_2g, ki) VALUES (3, 1, '000102030405060708090a0b0c0d0e0f');
INSERT INTO auc_3g (subscriber_id, algo_id_3g, k, opc, sqn) VALUES (3, 5, '000102030405060708090a0b0c0d0e0f', '101112131415161718191a1b1c1d1e1f', 2342);

-- A subscriber id > 7 and > 15 to check against octal and hex notations
INSERT INTO subscriber (id, imsi, msisdn) VALUES (123, '123123', '123');
INSERT INTO auc_2g (subscriber_id, algo_id_2g, ki) VALUES (123, 3, 'BeefedCafeFaceAcedAddedDecadeFee');
