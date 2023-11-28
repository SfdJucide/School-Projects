-- Write a common SQL-function (please be aware, not pl/pgsql-function) with the name fnc_persons.
--  This function should have an IN parameter pgender with default value = ‘female’.
DROP FUNCTION fnc_persons_male;
DROP FUNCTION fnc_persons_female;


CREATE OR REPLACE FUNCTION fnc_persons(IN pgender varchar DEFAULT 'female')
  RETURNS SETOF person
  LANGUAGE SQL
  AS
$$
	SELECT * FROM person WHERE gender = pgender;
$$;


select *
from fnc_persons(pgender := 'male');

select *
from fnc_persons();
