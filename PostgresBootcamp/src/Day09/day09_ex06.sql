-- Please create a pl/pgsql function  fnc_person_visits_and_eats_on_date based on SQL statement 
-- that finds the names of pizzerias which person (IN pperson parameter with default value is ‘Dmitriy’)
--  visited and in which he could buy pizza for less than the given sum in rubles (IN pprice parameter with default value is 500)
--  on the specific date (IN pdate parameter with default value is 8th of January 2022).
CREATE OR REPLACE FUNCTION fnc_person_visits_and_eats_on_date(
	IN pperson varchar DEFAULT 'Dmitriy', 
	IN pprice NUMERIC DEFAULT 500, 
	IN pdate date DEFAULT '2022-01-08')
  RETURNS SETOF varchar 
  LANGUAGE PLPGSQL
  AS
$$
BEGIN
	RETURN QUERY
	SELECT
		DISTINCT(pz.name)
	FROM
			person_visits pv
	JOIN person p ON
		p.id = pv.person_id
	JOIN pizzeria pz ON
		pz.id = pv.pizzeria_id
	JOIN menu m ON
		pz.id = m.pizzeria_id
	WHERE
		p.name = pperson
		AND m.price < pprice
		AND pv.visit_date = pdate;
END;
$$;

select *
from fnc_person_visits_and_eats_on_date(pprice := 800);

select *
from fnc_person_visits_and_eats_on_date(pperson := 'Anna',pprice := 1300,pdate := '2022-01-01');
