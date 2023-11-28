-- 1) SQL statement which returns a list of pizzerias names with corresponding rating value which have not been visited by persons.
SELECT
	pz.name,
	pz.rating
FROM
	person_visits pv
RIGHT JOIN pizzeria pz ON
	pv.pizzeria_id = pz.id
WHERE
	pv.pizzeria_id IS NULL;


-- 2) SQL statement which returns the missing days from 1st to 10th of January 2022 (including all days) for visits
--   of persons with identifiers 1 or 2 (it means days missed by both). Please order by visiting days in ascending mode.
SELECT
	gs::date AS missing_date
FROM
	generate_series('2022-01-01',
	'2022-01-10',
	INTERVAL '1 day') AS gs
LEFT JOIN (
	SELECT
		*
	FROM
		person_visits
	WHERE
		person_id = 1
		OR person_id = 2) AS pv ON
	gs::date = pv.visit_date
WHERE
	pv.visit_date IS NULL
ORDER BY
	missing_date;


-- 3) SQL statement that returns a whole list of person names visited (or not visited)
--  pizzerias during the period from 1st to 3rd of January 2022 from one side 
--  and the whole list of pizzeria names which have been visited (or not visited) from the other side.
SELECT 
	CASE
		WHEN p.name IS NULL THEN '-'
		ELSE p.name
	END AS person_name,
	pv.visit_date,
	CASE
		WHEN pz.name IS NULL THEN '-'
		ELSE pz.name
	END AS pizzeria_name
FROM
	(
	SELECT
		*
	FROM
		person_visits
	WHERE
		visit_date BETWEEN '2022-01-01' AND '2022-01-03') AS pv
FULL JOIN person p ON
	p.id = pv.person_id
FULL JOIN pizzeria pz ON
	pz.id = pv.pizzeria_id
ORDER BY 
	person_name,
	visit_date,
	pizzeria_name;


-- 4) SQL from #2 by using the CTE (Common Table Expression) pattern.
WITH date_generator AS (
SELECT
	gs::date AS g_date
FROM
	generate_series('2022-01-01',
	'2022-01-10',
	INTERVAL '1 day') AS gs)

SELECT
	g_date AS missing_date
FROM
	date_generator
LEFT JOIN (
	SELECT
		*
	FROM
		person_visits
	WHERE
		person_id = 1
		OR person_id = 2) AS pv ON
	g_date = pv.visit_date
WHERE
	pv.visit_date IS NULL
ORDER BY
	missing_date;


-- 5) Find full information about all possible pizzeria names and prices to get mushroom or pepperoni pizzas.
--  Please sort the result by pizza name and pizzeria name then.
SELECT 
	m.pizza_name,
	pz.name AS pizzeria_name,
	m.price
FROM
	menu m
JOIN pizzeria pz ON
	pz.id = m.pizzeria_id
WHERE
	pizza_name IN ('mushroom pizza', 'pepperoni pizza')
ORDER BY
	pizza_name,
	pizzeria_name;


-- 6) Find names of all female persons older than 25 and order the result by name.
SELECT 
	name
FROM
	person
WHERE
	gender = 'female'
	AND age > 25
ORDER BY
	name;


-- 7) Please find all pizza names (and corresponding pizzeria names using menu table) that Denis or Anna ordered. Sort a result by both columns.
WITH Denis_and_Anna_orders AS (
SELECT
	menu_id
FROM
	person_order po
JOIN person p ON
	p.id = po.person_id
WHERE
	p.name IN ('Denis', 'Anna'))
	
SELECT 
	pizza_name,
	pz.name AS pizzeria_name
FROM
	Denis_and_Anna_orders dna
JOIN menu m ON
	dna.menu_id = m.id
JOIN pizzeria pz ON
	m.pizzeria_id = pz.id
ORDER BY
	pizza_name,
	pizzeria_name;


-- 8) Please find the name of pizzeria Dmitriy visited on January 8, 2022 and could eat pizza for less than 800 rubles.
SELECT 
	pz.name AS pizzeria_name,
	m.pizza_name,
	m.price
FROM
	person_visits pv
JOIN person p ON
	p.id = pv.person_id
JOIN pizzeria pz ON
	pz.id = pv.pizzeria_id
JOIN menu m ON
	pz.id = m.pizzeria_id
WHERE
	p.name = 'Dmitriy'
	AND pv.visit_date = '2022-01-08'
	AND m.price < 800;


-- 9) Please find the names of all males from Moscow or Samara cities who orders either pepperoni or mushroom pizzas (or both).
--  Please order the result by person name in descending mode.
WITH males AS
	(
SELECT
	id,
	name
FROM
	person
WHERE
	gender = 'male'
	AND address IN ('Moscow', 'Samara')),

	mush_and_pep AS (
SELECT
	id,
	pizza_name
FROM
	menu
WHERE
	pizza_name IN ('mushroom pizza', 'pepperoni pizza'))
	
SELECT 
	ml.name
FROM
	person_order po
JOIN mush_and_pep m ON
	m.id = po.menu_id
JOIN males ml ON
	ml.id = po.person_id
ORDER BY
	name DESC;


-- 10) Please find the names of all females who ordered both pepperoni and cheese pizzas (at any time and in any pizzerias).
--  Make sure that the result is ordered by person name.
WITH females AS
	(
SELECT
	id,
	name
FROM
	person
WHERE
	gender = 'female'),

	chee_and_pep AS (
SELECT
	id,
	pizza_name
FROM
	menu
WHERE
	pizza_name IN ('cheese pizza', 'pepperoni pizza'))
	
SELECT 
	fem.name
FROM
	person_order po
JOIN chee_and_pep cp ON
	cp.id = po.menu_id
JOIN females fem ON
	fem.id = po.person_id
GROUP BY
	fem.name
HAVING
	count(cp.pizza_name) = 2
ORDER BY
	name ASC;


-- 11) Please find the names of persons who live on the same address.
--  Make sure that the result is ordered by 1st person, 2nd person's name and common address.
	p1.name AS person_name1,
	p2.name AS person_name2,
	p1.address AS common_address
FROM
	person p1
JOIN person p2 ON
	p1.address = p2.address
WHERE
	p1.id > p2.id
ORDER BY
	person_name1,
	person_name2;