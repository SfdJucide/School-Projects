-- Please define 2 SQL-functions (please be aware, not pl/pgsql-functions) with names
-- + fnc_persons_female (should return female persons)
-- + fnc_persons_male (should return male persons)
CREATE OR REPLACE FUNCTION fnc_persons_male()
  RETURNS SETOF person
  LANGUAGE SQL
  AS
$$
	SELECT * FROM person WHERE gender = 'male';
$$;

CREATE OR REPLACE FUNCTION fnc_persons_female()
  RETURNS SETOF person
  LANGUAGE SQL
  AS
$$
	SELECT * FROM person WHERE gender = 'female';
$$;


SELECT *
FROM fnc_persons_male();

SELECT *
FROM fnc_persons_female();


