-- 1) select statement which returns all person's names and person's ages from the city ‘Kazan’
SELECT
	name,
	age
FROM
	person
WHERE
	address = 'Kazan';


-- 2) select statement which returns names , ages for all women from the city ‘Kazan’. Yep, and please sort result by name.
SELECT
	name,
	age
FROM
	person
WHERE
	address = 'Kazan'
	AND gender = 'female'
ORDER BY
	name;


-- 3) select statements which return a list of pizzerias (pizzeria name and rating) 
--  with rating between 3.5 and 5 points (including limit points) and ordered by pizzeria rating.
SELECT
	name,
	rating
FROM
	pizzeria
WHERE
	rating <= 5
	AND rating >= 3.5
ORDER BY
	rating;

SELECT
	name,
	rating
FROM
	pizzeria
WHERE
	rating BETWEEN 3.5 AND 5
ORDER BY
	rating;


-- 4) select statement which returns the person's identifiers (without duplication) 
--  who visited pizzerias in a period from 6th of January 2022 to 9th of January 2022 (including all days)
--  or visited pizzeria with identifier 2. Also include ordering clause by person identifier in descending mode.
SELECT 
	DISTINCT(person_id)
FROM
	person_visits
WHERE
	pizzeria_id = 2
	OR visit_date BETWEEN '06.01.2022' AND '09.01.2022'
ORDER BY
	person_id DESC;


-- 5) select statement which returns one calculated field with name ‘person_information’ in one string like described in the next sample:
-- Anna (age:16,gender:'female',address:'Moscow')
-- Finally, please add the ordering clause by calculated column in ascending mode.
SELECT 
	concat(name,
	' (age:',
	age,
	',gender:',
	'''',
	gender,
	'''',
	',address:',
	'''',
	address,
	'''',
	')') AS person_information
FROM
	person
ORDER BY
	person_information ASC;


-- 6) select statement which returns person's names (based on internal query in SELECT clause)
--  who made orders for the menu with identifiers 13 , 14 and 18 and date of orders should be equal 7th of January 2022.
SELECT
	(
	SELECT
		name
	FROM
		person AS p
	WHERE
		p.id = po.person_id) AS name
FROM
	person_order AS po
WHERE
	(menu_id = 14
		OR menu_id = 13
		OR menu_id = 18)
	AND (order_date = '07.01.2022');


-- 7) a new calculated column (use column's name ‘check_name’) with a check statement:
-- if (person_name == 'Denis') then return true
--     else return false

SELECT
	(
	SELECT
		name
	FROM
		person AS p
	WHERE
		p.id = po.person_id) AS name,
	(
	SELECT
		CASE
			WHEN name = 'Denis' THEN TRUE
			ELSE FALSE
		END
	FROM
		person AS p
	WHERE
		p.id = po.person_id) AS check_name
FROM
	person_order AS po
WHERE
	(menu_id = 14
		OR menu_id = 13
		OR menu_id = 18)
	AND (order_date = '07.01.2022');


-- 8) SQL statement which returns a person's identifiers, person's names and interval of person’s ages:
-- if (age >= 10 and age <= 20) then return 'interval #1'
-- else if (age > 20 and age < 24) then return 'interval #2'
-- else return 'interval #3'
SELECT
	id,
	name,
	(CASE 
		WHEN age BETWEEN 10 AND 20 THEN 'interval #1'
		WHEN age BETWEEN 21 AND 23 THEN 'interval #2'
		ELSE 'interval #3'
	END) AS interval_info
FROM
	person
ORDER BY
	interval_info ASC;


-- 9) SQL statement which returns all columns from the person_order table with rows whose identifier is an even number.
SELECT
	*
FROM
	person_order
WHERE
	id % 2 = 0
ORDER BY
	id;


-- 10) select statement that returns person names and pizzeria names based on the person_visits table
--  with date of visit in a period from 07th of January to 09th of January 2022 (including all days)
SELECT
	(
	SELECT
		name
	FROM
		person p
	WHERE
		p.id = pv.person_id) AS person_name,
	(
	SELECT
		name
	FROM
		pizzeria pz
	WHERE
		pz.id = pv.pizzeria_id) AS pizzeria_name,
	visit_date
FROM
	(
	SELECT
		*
	FROM
		person_visits
	WHERE
		visit_date BETWEEN '07.01.2022' AND '09.01.2022') AS pv
ORDER BY
	person_name ASC,
	pizzeria_name DESC;
