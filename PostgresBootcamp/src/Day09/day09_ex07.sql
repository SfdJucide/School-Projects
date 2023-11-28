-- Please write a SQL or pl/pgsql function func_minimum (it’s up to you) that has an input parameter is an array of numbers and the function should return a minimum value.
CREATE OR REPLACE FUNCTION func_minimum(VARIADIC arr numeric[])
  RETURNS numeric
  LANGUAGE SQL
  AS
$$
	SELECT min($1[i]) FROM generate_subscripts($1, 1) g(i);
$$;


SELECT func_minimum(VARIADIC arr => ARRAY[10.0, -1.0, 5.0, 4.4]);