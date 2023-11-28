-- 1) Please create a simple BTree index for every foreign key in our database.
CREATE INDEX idx_menu_pizzeria_id ON menu USING btree
(
	pizzeria_id
);

CREATE INDEX idx_person_order_person_id ON person_order USING btree
(
	person_id
);

CREATE INDEX idx_person_order_menu_id ON person_order USING btree
(
	menu_id
);

CREATE INDEX idx_person_visits_person_id ON person_visits USING btree
(
	person_id
);

CREATE INDEX idx_person_visits_pizzeria_id ON person_visits USING btree
(
	pizzeria_id
);


-- 2) SQL statement that returns pizzas’ and corresponding pizzeria names. 
SET enable_seqscan = OFF;

EXPLAIN ANALYZE
SELECT
	m.pizza_name,
	p.name AS pizzeria_name
FROM
	menu m
JOIN pizzeria p 
ON
	m.pizzeria_id = p.id;


-- 3) Please create a functional B-Tree index with name idx_person_name for the column name of the person table.
--  Index should contain person names in upper case.
CREATE INDEX idx_person_name ON person USING btree
(
	upper(name) 
);

SET enable_seqscan = OFF;

EXPLAIN ANALYZE
SELECT
	*
FROM
	person
WHERE
	upper(name) = 'ANNA';


-- 4) Please create a better multicolumn B-Tree index with the name idx_person_order_multi for the SQL statement below.
CREATE INDEX idx_person_order_multi ON person_order USING btree
(
	person_id,
	menu_id,
	order_date
);

SET enable_seqscan = OFF;

EXPLAIN ANALYZE
SELECT
	person_id,
	menu_id,
	order_date
FROM
	person_order
WHERE
	person_id = 8
	AND menu_id = 19;


-- 5) Please create a unique BTree index with the name idx_menu_unique on the menu table for  pizzeria_id and pizza_name columns.
CREATE UNIQUE INDEX idx_menu_unique ON menu USING btree
(
	pizzeria_id,
	pizza_name
);

SET enable_seqscan = OFF;

EXPLAIN ANALYZE
SELECT
	pizzeria_id,
	pizza_name
FROM 
	menu;

-- we can't insert a row of combination such params cause it's already exist
INSERT INTO menu (id, pizzeria_id , pizza_name , price)
VALUES
	((SELECT max(id) + 1 FROM menu), 2, 'sicilian pizza', 1200);


-- 6) Please create a partial unique BTree index with the name idx_person_order_order_date on the person_order 
-- table for person_id and menu_id attributes with partial uniqueness for order_date column for date ‘2022-01-01’.
CREATE UNIQUE INDEX idx_person_order_order_date ON person_order USING btree
(
	person_id,
	menu_id
) WHERE order_date = '2022-01-01';

SET enable_seqscan = OFF;

EXPLAIN ANALYZE
SELECT
	person_id,
	menu_id
FROM 
	person_order
WHERE order_date = '2022-01-01';


-- we can't insert a row of combination such params only with order_date = '2022-01-01' cause it's already exist
INSERT INTO person_order (id, person_id , menu_id , order_date)
VALUES
	((SELECT max(id) + 1 FROM person_order), 6, 13, '2022-01-01');

-- but this will work	
INSERT INTO person_order (id, person_id , menu_id , order_date)
VALUES
	((SELECT max(id) + 1 FROM person_order), 6, 13, '2022-01-02');

DELETE FROM person_order WHERE id = (SELECT max(id) FROM person_order);


-- 7) Create a new BTree index with name idx_1 which should improve the “Execution Time” metric of this SQL.
--  Please provide proof (EXPLAIN ANALYZE) that SQL was improved.
CREATE INDEX idx_1 ON pizzeria USING btree
(
	rating DESC
);

SET enable_seqscan = Off;

EXPLAIN ANALYZE
SELECT
    m.pizza_name AS pizza_name,
    max(rating) OVER (PARTITION BY rating ORDER BY rating ROWS BETWEEN UNBOUNDED PRECEDING AND UNBOUNDED FOLLOWING) AS k
FROM  menu m
INNER JOIN pizzeria pz ON m.pizzeria_id = pz.id
ORDER BY 1,2;

DROP INDEX IF EXISTS idx_1;

EXPLAIN ANALYZE
SELECT
    m.pizza_name AS pizza_name,
    max(rating) OVER (PARTITION BY rating ORDER BY rating ROWS BETWEEN UNBOUNDED PRECEDING AND UNBOUNDED FOLLOWING) AS k
FROM  menu m
INNER JOIN pizzeria pz ON m.pizzeria_id = pz.id
ORDER BY 1,2;
