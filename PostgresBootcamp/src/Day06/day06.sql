-- 1) Every person wants to see a personal discount and every business wants to be closer for clients.
-- Please think about personal discounts for people from one side and pizzeria restaurants from other
CREATE TABLE person_discounts
(
	id bigint PRIMARY KEY,
	person_id bigint,
	pizzeria_id bigint,
	discount NUMERIC,
	CONSTRAINT fk_person_discounts_person_id FOREIGN KEY (person_id) REFERENCES person(id),
	CONSTRAINT fk_person_discounts_pizzeria_id FOREIGN KEY (pizzeria_id) REFERENCES pizzeria(id)
);


-- 2) DML statement that makes inserts new records into person_discounts table based on the next rules.
-- + take aggregated state by person_id and pizzeria_id columns
-- + calculate personal discount value by the next pseudo code:
-- + if “amount of orders” = 1 then “discount” = 10.5  else if “amount of orders” = 2 then  “discount” = 22 else  “discount” = 30
-- + to generate a primary key for the person_discounts table please use  SQL construction below (this construction is from the WINDOW FUNCTION  SQL area).
INSERT INTO person_discounts
SELECT
	ROW_NUMBER () OVER () AS id,
	person_id,
	m.pizzeria_id,
	CASE
		WHEN count(1) = 1 THEN 10.5
		WHEN count(1) = 2 THEN 22
		ELSE 30
	END AS discount
FROM
	person_order po
JOIN 
	menu m 
ON
	m.id = po.menu_id
GROUP BY
	person_id,
	pizzeria_id;


-- 3) SQL statement that returns orders with actual price and price with applied discount for each person
--  in the corresponding pizzeria restaurant and sort by person name, and pizza name.
SELECT
	p.name AS name,
	m.pizza_name AS pizza_name,
	m.price AS price,
	(100 - pd.discount) * m.price / 100 AS discount_price,
	pz.name AS pizzeria_name
FROM
	person p
JOIN person_order po ON
	p.id = po.person_id
JOIN menu m ON
	m.id = po.menu_id
JOIN pizzeria pz ON
	pz.id = m.pizzeria_id
JOIN person_discounts pd ON
	pd.person_id = p.id 
	AND pd.pizzeria_id = pz.id
ORDER BY
	name,
	pizza_name;


-- 4) Please create a multicolumn unique index that prevents duplicates of pair values person and pizzeria identifiers.
CREATE UNIQUE INDEX idx_person_discounts_unique ON person_discounts USING btree
(
	person_id,
	pizzeria_id
);

SET enable_seqscan = OFF;

EXPLAIN ANALYZE
SELECT
	person_id,
	pizzeria_id
FROM
	person_discounts;


-- 5) adding the following constraint rules for existing columns of the person_discounts table.
ALTER TABLE person_discounts
ADD CONSTRAINT ch_nn_person_id CHECK (person_id IS NOT NULL);

ALTER TABLE person_discounts
ADD CONSTRAINT ch_nn_pizzeria_id CHECK (pizzeria_id IS NOT NULL);

ALTER TABLE person_discounts
ADD CONSTRAINT ch_nn_discount CHECK (discount IS NOT NULL);

ALTER TABLE person_discounts
ALTER COLUMN discount SET DEFAULT 0;

ALTER TABLE person_discounts
ADD CONSTRAINT ch_range_discount CHECK (discount BETWEEN 0 AND 100);


-- 6) Please add English or Russian comments (it's up to you) that explain what is a business goal of a table and all included attributes.
COMMENT ON TABLE person_discounts IS 'Inforamtion about discount percentages for pizzeria''s customers';

COMMENT ON COLUMN person_discounts.id IS 'Unique id of each entry';
COMMENT ON COLUMN person_discounts.person_id IS 'identifier of person';
COMMENT ON COLUMN person_discounts.pizzeria_id IS 'identifier of pizzeria';
COMMENT ON COLUMN person_discounts.discount IS 'Value calculated based on number of orders';


-- 7) Let’s create a Database Sequence with the name seq_person_discounts
--  and set a default value for id attribute of person_discounts table to take a value from seq_person_discounts each time automatically.
CREATE SEQUENCE seq_person_discounts START WITH 1;

ALTER TABLE person_discounts 
ALTER COLUMN id SET DEFAULT nextval('seq_person_discounts');

SELECT setval('seq_person_discounts', (SELECT COUNT(*) + 1 FROM person_discounts));
