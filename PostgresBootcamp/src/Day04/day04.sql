-- 1) Please create 2 Database Views (with similar attributes like the original table) based on simple filtering of gender of persons.
CREATE OR REPLACE VIEW v_persons_female AS
	SELECT
		*
	FROM
		person
	WHERE 
		gender = 'female';
	

CREATE OR REPLACE VIEW v_persons_male AS
	SELECT
		*
	FROM
		person
	WHERE 
		gender = 'male';


-- 2) Please use 2 Database Views from Exercise #00 and write SQL to get female and male person names in one list.
--  Please set the order by person name.
SELECT
	name
FROM
	v_persons_female
UNION
SELECT
	name
FROM
	v_persons_male
ORDER BY 
	name;


-- 3) Please create a Database View (with name v_generated_dates) which should be “store” generated dates from 1st to 31th of January 2022 in DATE type.
CREATE OR REPLACE VIEW v_generated_dates AS
	SELECT
		gs::date AS generated_date
	FROM
		generate_series(
		'2022-01-01',
		'2022-01-31',
		INTERVAL '1 day') AS gs
	ORDER BY
		generated_date;


-- 4) Please write a SQL statement which returns missing days for persons’ visits in January of 2022.
--  Use v_generated_dates view for that task and sort the result by missing_date column. 
SELECT
	generated_date AS missing_date
FROM 
	v_generated_dates
EXCEPT 
SELECT
	visit_date AS missing_date
FROM
	person_visits
WHERE
	EXTRACT(months FROM visit_date) = 01
	AND EXTRACT(years FROM visit_date) = 2022
ORDER BY
	missing_date;


-- 5) Please write a SQL statement which satisfies a formula (R - S)∪(S - R) .
-- Where R is the person_visits table with filter by 2nd of January 2022, S is also person_visits table but with a different filter by 6th of January 2022.
--  Please make your calculations with sets under the person_id column and this column will be alone in a result.  
CREATE OR REPLACE VIEW v_symmetric_union AS
	WITH R AS (
		SELECT
			person_id
		FROM
			person_visits
		WHERE 
			visit_date = '2022-01-02'),
		
	S AS (
		SELECT
			person_id
		FROM
			person_visits
		WHERE
			visit_date = '2022-01-06')
	
	(
	SELECT
		person_id
	FROM
		R
	EXCEPT
	SELECT
		person_id
	FROM
		S)
	UNION
	(
	SELECT
		person_id
	FROM
		S
	EXCEPT
	SELECT
		person_id
	FROM
		R)
	ORDER BY
		person_id;


-- 6) Please create a Database View v_price_with_discount that returns a person's orders with person names, pizza names,
--  real price and calculated column discount_price (with applied 10% discount and satisfies formula price - price*0.1).
CREATE OR REPLACE VIEW v_price_with_discount AS
	SELECT
		p.name AS name,
		m.pizza_name AS pizza_name,
		m.price AS price,
		round(m.price - m.price * 0.1) AS discount_price
	FROM
			person p
	JOIN person_order po ON
		p.id = po.person_id
	JOIN menu m ON
		m.id = po.menu_id
	ORDER BY
			name,
			pizza_name;


-- 7) Please create a Materialized View mv_dmitriy_visits_and_eats (with data included) based on SQL statement that finds the name of pizzeria
--  Dmitriy visited on January 8, 2022 and could eat pizzas for less than 800 rubles
CREATE MATERIALIZED VIEW mv_dmitriy_visits_and_eats AS
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


-- 8) Refresh data in our Materialized View mv_dmitriy_visits_and_eats from exercise #7.
-- Before this action, please generate one more Dmitriy visit that satisfies the SQL clause of Materialized View except pizzeria that we can see in a result from exercise #7.
-- After adding a new visit please refresh a state of data for mv_dmitriy_visits_and_eats.
INSERT INTO person_visits (id, person_id, pizzeria_id, visit_date)
VALUES ((SELECT max(id) + 1 FROM person_visits),
		(SELECT id FROM person WHERE name = 'Dmitriy'),
		(SELECT pizzeria_id FROM menu WHERE price < 800 AND pizzeria_id != (SELECT id FROM pizzeria WHERE name = 'Dominos') ORDER BY random() LIMIT 1),
		'2022-01-08');

REFRESH MATERIALIZED VIEW mv_dmitriy_visits_and_eats;


-- 9) DROP views!
DROP MATERIALIZED VIEW mv_dmitriy_visits_and_eats;

DROP VIEW IF EXISTS v_generated_dates,
					v_persons_female,
					v_persons_male,
					v_price_with_discount,
					v_symmetric_union;