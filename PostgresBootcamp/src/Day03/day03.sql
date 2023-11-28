-- 1) SQL statement which returns a list of pizza names, pizza prices, pizzerias names 
-- and dates of visit for Kate and for prices in range from 800 to 1000 rubles.
WITH kate_visits AS (
SELECT
	pizzeria_id,
	visit_date
FROM
	person p
JOIN person_visits pv ON
	p.id = pv.person_id
WHERE
	p.name = 'Kate'),

medium_pizza AS (
SELECT
	pizzeria_id,
	pizza_name,
	price,
	name
FROM
	menu m
JOIN pizzeria pz ON
	pz.id = m.pizzeria_id
WHERE
	m.price BETWEEN 800 AND 1000)
	
	
SELECT
	pizza_name,
	price,
	name AS pizzeria_name,
	visit_date
FROM
	kate_visits kv
JOIN medium_pizza mp ON
	mp.pizzeria_id = kv.pizzeria_id
ORDER BY
	pizza_name,
	price,
	pizzeria_name;


-- 2) Please find all menu identifiers which are not ordered by anyone. The result should be sorted by identifiers.
SELECT
	id AS menu_id
FROM
	menu
WHERE
	id NOT IN (
	SELECT
		menu_id
	FROM
		person_order)
ORDER BY
	menu_id;


-- 3) SQL statement from Exercise #2 and show pizza names from pizzeria which are not ordered by anyone, including corresponding prices also.
SELECT
	pizza_name,
	price,
	name AS pizzeria_name
FROM
	menu m
JOIN pizzeria pz ON
	pz.id = m.pizzeria_id
WHERE
	m.id NOT IN (
	SELECT
		menu_id
	FROM
		person_order)
ORDER BY
	pizza_name,
	price;


-- 4) Please find pizzerias that have been visited more often by women or by men. 
SELECT
	pz.name AS pizzeria_name
FROM
	person p
JOIN person_visits pv ON
	pv.person_id = p.id
JOIN pizzeria pz ON
	pz.id = pv.pizzeria_id
GROUP BY
	pz.name
HAVING
	sum(CASE
		WHEN gender = 'female' THEN 1
		ELSE -1
	END) != 0
ORDER BY
	pizzeria_name;


-- 5) Please find a union of pizzerias that have orders either from women or  from men. 
WITH orderd_by_men AS (
SELECT
	pz.name AS pizzeria_name
FROM
	person_order po
JOIN person p ON
	p.id = po.person_id
JOIN menu m ON
	po.menu_id = m.id
JOIN pizzeria pz ON
	pz.id = m.pizzeria_id
WHERE
	gender = 'male'),
	
ordered_by_women AS (
SELECT
	pz.name AS pizzeria_name
FROM
	person_order po
JOIN person p ON
	p.id = po.person_id
JOIN menu m ON
	po.menu_id = m.id
JOIN pizzeria pz ON
	pz.id = m.pizzeria_id
WHERE
	gender = 'female')
	
SELECT
	pizzeria_name
FROM
	ordered_by_women
EXCEPT
SELECT
	pizzeria_name
FROM
	orderd_by_men
ORDER BY
	pizzeria_name;


-- 6) SQL statement which returns a list of pizzerias which Andrey visited but did not make any orders.
--  Please order by the pizzeria name.
WITH andrey_visited AS (
SELECT
	pz.name AS pizzeria_name
FROM
	person_visits pv
JOIN person p ON
	p.id = pv.person_id
JOIN pizzeria pz ON
	pz.id = pv.pizzeria_id
WHERE
	p.name = 'Andrey'),
	
andrey_ordered AS (
SELECT
	pz.name AS pizzeria_name
FROM
	person_order po
JOIN person p ON
	p.id = po.person_id
JOIN menu m ON
	po.menu_id = m.id
JOIN pizzeria pz ON
	pz.id = m.pizzeria_id
WHERE
	p.name = 'Andrey')
	
SELECT
	pizzeria_name
FROM
	andrey_visited
EXCEPT
SELECT
	pizzeria_name
FROM
	andrey_ordered
ORDER BY
	pizzeria_name;


-- 7) Please find the same pizza names who have the same price, but from different pizzerias.
--  Make sure that the result is ordered by pizza name.
SELECT
	m1.pizza_name,
	pz1.name AS pizzeria_name_1,
	pz2.name AS pizzeria_name_2,
	m1.price
FROM
	menu m1
JOIN menu m2 ON
	m1.pizza_name = m2.pizza_name
JOIN pizzeria pz1 ON
	pz1.id = m1.pizzeria_id
JOIN pizzeria pz2 ON
	pz2.id = m2.pizzeria_id
WHERE
	m1.price = m2.price
	AND m1.pizzeria_id != m2.pizzeria_id
	AND m1.id > m2.id
ORDER BY
	pizza_name;


-- 8) Please register a new pizza with name “greek pizza” (use id = 19) with price 800 rubles in “Dominos” restaurant (pizzeria_id = 2).
INSERT INTO menu (id, pizzeria_id, pizza_name, price)
VALUES (19, 2, 'greek pizza', '800');


-- 9) Please register a new pizza with name “sicilian pizza” (whose id should be calculated by formula is “maximum id value + 1”) 
-- with a price of 900 rubles in “Dominos” restaurant (please use internal query to get identifier of pizzeria).
INSERT INTO menu (id, pizzeria_id, pizza_name, price)
VALUES ((SELECT max(id) + 1 FROM menu), (SELECT id FROM pizzeria WHERE name = 'Dominos'), 'sicilian pizza', '900');


-- 10) Please register new visits into Dominos restaurant from Denis and Irina on 24th of February 2022.
INSERT INTO person_visits  (id, person_id, pizzeria_id, visit_date)
VALUES 
	((SELECT max(id) + 1 FROM person_visits), (SELECT id FROM person WHERE name = 'Irina'), (SELECT id FROM pizzeria WHERE name = 'Dominos'), '2022-02-24'),
	((SELECT max(id) + 2 FROM person_visits), (SELECT id FROM person WHERE name = 'Denis'), (SELECT id FROM pizzeria WHERE name = 'Dominos'), '2022-02-24');


-- 11) Please register new orders from Denis and Irina on 24th of February 2022 for the new menu with “sicilian pizza”.
INSERT INTO person_order (id, person_id, menu_id, order_date)
VALUES 
	((SELECT max(id) + 1 FROM person_order), (SELECT id FROM person WHERE name = 'Irina'), (SELECT id FROM menu WHERE pizza_name = 'sicilian pizza'), '2022-02-24'),
	((SELECT max(id) + 2 FROM person_order), (SELECT id FROM person WHERE name = 'Denis'), (SELECT id FROM menu WHERE pizza_name = 'sicilian pizza'), '2022-02-24');


-- 12) Please change the price for “greek pizza” on -10% from the current value.
UPDATE menu
SET price = (SELECT price * 0.9 FROM menu WHERE pizza_name = 'greek pizza')
WHERE pizza_name = 'greek pizza';


-- 13) Please register new orders from all persons for “greek pizza” on 25th of February 2022.
WITH date_generator AS (
SELECT
	gs::date AS g_date
FROM
	generate_series(
	'2022-02-25',
	'2022-02-25',
	INTERVAL '1 day') AS gs),

id_generator AS (
SELECT
	gs
FROM
	generate_series((
	SELECT max(id) + 1 FROM person_order),
	(SELECT max(id) + 1 + (SELECT count(id) FROM person) FROMperson_order)) AS gs)
	
	
INSERT INTO person_order (id, person_id, menu_id, order_date)
SELECT
	ig.gs,
	p.id,
	m.id,
	dg.g_date
FROM
	person p
CROSS JOIN id_generator ig
CROSS JOIN date_generator dg
CROSS JOIN menu m
WHERE
	m.pizza_name = 'greek pizza'
	AND ig.gs - p.id = (
	SELECT
		max(id)
	FROM
		person_order);


-- 14) Please write 2 SQL (DML) statements that delete all new orders from exercise #12 based on order date. Then delete “greek pizza” from the menu.
DELETE FROM person_order
WHERE order_date = '2022-02-25';

DELETE FROM menu
WHERE pizza_name  = 'greek pizza';