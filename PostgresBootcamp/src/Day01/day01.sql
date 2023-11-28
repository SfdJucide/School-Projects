-- 1) SQL statement which returns menu’s identifier and pizza names from menu table and person’s identifier
--  and person name from person table in one global list ordered by object_id and then by object_name columns.
SELECT 
	id AS object_id,
	pizza_name AS object_name
FROM
	menu
UNION
SELECT 
	id,
	name
FROM
	person
ORDER BY
	object_id,
	object_name;


-- 2) SQL statement which returns pizza names from menu table and person name from person table in one global list ordered by object_name columns.
(
SELECT 
	name AS object_name
FROM
	person
ORDER BY
	object_name)
UNION ALL
(
SELECT 
	pizza_name AS object_name
FROM
	menu
ORDER BY
	object_name);


-- 3) SQL statement which returns unique pizza names from the menu table and orders by pizza_name column in descending mode.
(
SELECT 
	pizza_name
FROM
	menu)
UNION 
(
SELECT 
	pizza_name
FROM
	menu)
ORDER BY
	pizza_name DESC;


-- 4) SQL statement which returns common rows for attributes order_date, person_id from person_order table from one side and visit_date,
--  person_id from person_visits table from the other side. In other words, let’s find identifiers of persons, who visited and ordered some pizza on the same day.
SELECT 
	order_date AS action_date,
	person_id
FROM
	person_order
INTERSECT  
SELECT 
	visit_date AS action_date,
	person_id
FROM
	person_visits
ORDER BY
	action_date ASC,
	person_id DESC;


-- 5) SQL statement which returns a difference (minus) of person_id column values with saving duplicates between
--  person_order table and person_visits table for order_date and visit_date are for 7th of January of 2022
SELECT 
	order_date AS action_date,
	person_id
FROM
	person_order
WHERE
	order_date = '2022-01-07'
EXCEPT ALL
SELECT 
	visit_date AS action_date,
	person_id
FROM
	person_visits
WHERE
	visit_date = '2022-01-07';


-- 6) SQL statement which returns all possible combinations between person and pizzeria tables
--  and please set ordering by person identifier and then by pizzeria identifier columns.
SELECT
	*
FROM
	person p
CROSS JOIN pizzeria p2
ORDER BY 
	p.id ASC,
	p2.id ASC;


-- 7) SQL statement to return person names instead of person identifiers
--  and change ordering by action_date in ascending mode and then by person_name in descending mode.
SELECT 
	action_date,
	name AS person_name
FROM
	(
	SELECT
		order_date AS action_date,
		person_id
	FROM
		person_order
INTERSECT
	SELECT
		visit_date AS action_date,
		person_id
	FROM
		person_visits)
JOIN person p ON
	person_id = p.id
ORDER BY
	action_date ASC,
	person_name DESC;


-- 8) SQL statement which returns the date of order from the person_order table
--  and corresponding person name which made an order from the person table.
SELECT 
	order_date,
	concat(p.name,
	' (age:',
	p.age,
	')') AS person_information
FROM
	person_order po
JOIN person p ON
	po.person_id = p.id
ORDER BY 
	order_date ASC,
	person_information ASC;


-- 9) SQL statement from exercise #8 by using NATURAL JOIN construction.
SELECT 
	order_date,
	concat(p.name,
	' (age:',
	p.age,
	')') AS person_information
FROM
	(
	SELECT
		order_date,
		person_id AS id
	FROM
		person_order)
NATURAL JOIN person p
ORDER BY 
	order_date ASC,
	person_information ASC;


-- 10) 2 SQL statements which return a list of pizzerias names which have not been visited by persons by using IN for 1st one and EXISTS for the 2nd one.
SELECT
	name
FROM
	pizzeria
WHERE
	id NOT IN (
	SELECT
		DISTINCT(pizzeria_id)
	FROM
		person_visits);

SELECT
	name
FROM
	pizzeria pz
WHERE
	NOT EXISTS (
	SELECT
		DISTINCT(pizzeria_id)
	FROM
		person_visits
	WHERE
		pizzeria_id = pz.id);


-- 11) SQL statement which returns a list of the person names which made an order for pizza in the corresponding pizzeria.
SELECT 
	p.name AS person_name,
	m.pizza_name,
	pz.name AS pizzeria_name
FROM
	person_order po
JOIN person p ON
	po.person_id = p.id
JOIN menu m ON
	po.menu_id = m.id
JOIN pizzeria pz ON
	pz.id = m.pizzeria_id
ORDER BY
	person_name ASC,
	pizza_name ASC,
	pizzeria_name ASC;