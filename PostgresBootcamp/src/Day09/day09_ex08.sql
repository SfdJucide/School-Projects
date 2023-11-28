-- Please write a SQL or pl/pgsql function fnc_fibonacci (it’s up to you) that has an input parameter pstop with type integer (by default is 10)
--  and the function output is a table with all Fibonacci numbers less than pstop.
CREATE OR REPLACE FUNCTION fnc_fibonacci(IN pstop integer DEFAULT 10)
  RETURNS SETOF integer
  LANGUAGE SQL
  AS
$$
	WITH RECURSIVE Fibonacci (PrevN, N) AS (
		SELECT
			1, 1
		UNION ALL
		SELECT
			N,
			PrevN + N
		FROM
			Fibonacci
		WHERE
			N < pstop
	)

	SELECT
		PrevN AS Fibo
	FROM
		Fibonacci;
$$;

select * from fnc_fibonacci(100);
select * from fnc_fibonacci();

