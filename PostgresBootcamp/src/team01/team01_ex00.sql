-- Please write a SQL statement that returns the total volume (sum of all money) of transactions
-- from user balance aggregated by user and balance type. Please be aware, all data should be processed including data with anomalies.
WITH last_updates AS (
	SELECT
		c.id,
		c.name,
		c.rate_to_usd,
		c.updated
	FROM 
		currency c
	JOIN (SELECT id, max(updated) AS updated FROM currency GROUP BY id) lu 
	ON lu.id = c.id 
	AND lu.updated = c.updated
)

SELECT
	COALESCE(u.name, 'not defined') AS name,
	COALESCE(u.lastname, 'not defined') AS lastname,
	b.type AS TYPE,
	sum(b.money) AS volume,
	COALESCE(max(lu.name), 'not defined') AS currency_name,
	COALESCE(max(lu.rate_to_usd), 1) AS last_rate_to_usd,
	COALESCE(max(lu.rate_to_usd) * sum(b.money), sum(b.money)) AS total_volume_in_usd
FROM
	"user" u
FULL JOIN balance b ON
	u.id = b.user_id
FULL JOIN last_updates lu ON
	lu.id = b.currency_id
GROUP BY
	u.id,
	b.type
ORDER BY
	name DESC,
	lastname ASC,
	TYPE ASC;
