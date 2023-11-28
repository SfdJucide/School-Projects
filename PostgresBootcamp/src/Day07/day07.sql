-- 1) SQL statement that returns person identifiers and corresponding number of visits in any pizzerias
--  and sorting by count of visits in descending mode and sorting in person_id in ascending mode.
SELECT
	person_id,
	COUNT(id) AS count_of_visits
FROM
	person_visits
GROUP BY
	person_id
ORDER BY
	count_of_visits DESC,
	person_id ASC;


-- 2) SQL statement from Exercise #1 and return a person name. 
-- Additional clause is  we need to see only top-4 persons with maximal visits in any pizzerias and sorted by a person name.
SELECT 
	p.name,
	COUNT(pv.id) AS count_of_visits
FROM
	person_visits pv
JOIN person p ON
	p.id = pv.person_id
GROUP BY
	p.name
ORDER BY
	count_of_visits DESC,
	p.name ASC
LIMIT 4;


-- 3) SQL statement to see 3 favorite restaurants by visits and by orders in one list.
(
SELECT 
	pz.name,
	count(*) AS count,
	'order' AS action_type
FROM
	person_order po
JOIN menu m ON
	po.menu_id = m.id
JOIN pizzeria pz ON
	pz.id = m.pizzeria_id
GROUP BY 
	pz.name
ORDER BY
	count DESC
LIMIT 3)

UNION

(
SELECT 
	pz.name,
	count(*) AS count,
	'visit' AS action_type
FROM
	person_visits pv
JOIN pizzeria pz ON
	pz.id = pv.pizzeria_id
GROUP BY 
	pz.name
ORDER BY
	count DESC
LIMIT 3)

ORDER BY 
	action_type ASC,
	count DESC;


-- 4) SQL statement to see restaurants are grouping by visits and by orders and joined with each other by using restaurant name.
SELECT 
	name,
	sum(total_count) AS total_count
FROM
	((
	SELECT
		pz.name,
		count(*) AS total_count
	FROM
		person_order po
	JOIN menu m ON
		po.menu_id = m.id
	JOIN pizzeria pz ON
		pz.id = m.pizzeria_id
	GROUP BY
		pz.name)
		
	UNION
	
	(
	SELECT 
		pz.name,
		count(*) AS total_count
	FROM
		person_visits pv
	JOIN pizzeria pz ON
		pz.id = pv.pizzeria_id
	GROUP BY 
		pz.name))
GROUP BY
	name
ORDER BY
	total_count DESC,
	name ASC;


-- 5) SQL statement that returns the person name and corresponding number of visits in any pizzerias if the person has visited more than 3 times (> 3).
SELECT
	p.name,
	count(pv.pizzeria_id) AS count_of_visits
FROM
	person_visits pv
JOIN person p ON
	p.id = pv.person_id
GROUP BY 
	p.name
HAVING
	count(pv.pizzeria_id) > 3;


-- 6) SQL query that returns a list of unique person names who made orders in any pizzerias. The result should be sorted by person name.
SELECT 
	DISTINCT(p.name)
FROM
	person_order po
JOIN person p ON
	p.id = po.person_id
ORDER BY 
	p.name;


-- 7) SQL statement that returns the amount of orders, average of price, maximum and minimum prices for sold pizza by corresponding pizzeria restaurant.
--  The result should be sorted by pizzeria name.
SELECT
	pz.name,
	count(*) AS count_of_orders,
	round(avg(m.price), 2) AS average_price,
	max(m.price) AS max_price,
	min(m.price) AS min_price
FROM
	pizzeria pz
JOIN menu m ON
	m.pizzeria_id = pz.id
JOIN person_order po ON
	m.id = po.menu_id
GROUP BY
	pz.name
ORDER BY
	pz.name;


-- 8) SQL statement that returns a common average rating (the output attribute name is global_rating) for all restaurants.
--  Round your average rating to 4 floating numbers.
SELECT
	round(avg(rating), 4) AS global_rating
FROM
	pizzeria;


-- 9) SQL statement that returns address, pizzeria name and amount of persons’ orders.
--  The result should be sorted by address and then by restaurant name.
SELECT
	person.address,
	pizzeria.name,
	COUNT(person.address) AS count_of_orders
FROM
	person_order
JOIN person ON
	person_order.person_id = person.id
JOIN menu ON
	person_order.menu_id = menu.id
JOIN pizzeria ON
	menu.pizzeria_id = pizzeria.id
GROUP BY
	person.address,
	pizzeria.name
ORDER BY
	address,
	pizzeria.name;


-- 10) SQL statement that returns aggregated information by person’s address , the result of “Maximal Age - (Minimal Age  / Maximal Age)”
--  that is presented as a formula column, next one is average age per address and the result of comparison between formula and average columns
SELECT
	address,
	round(max(age) - (min(age) * 1.0 / max(age)), 2) AS formula,
	round(avg(age), 2) AS average,
	CASE
		WHEN (max(age) - (min(age) * 1.0 / max(age)) > avg(age)) THEN 'true'
		ELSE 'false'
	END AS comparison
FROM
	person
GROUP BY
	address
ORDER BY
	address;