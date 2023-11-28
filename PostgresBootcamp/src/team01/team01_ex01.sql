-- Please write a SQL statement that returns all Users , all Balance transactions 
--  with currency name and calculated value of currency in USD for the nearest day.
insert into currency values (100, 'EUR', 0.85, '2022-01-01 13:29');
insert into currency values (100, 'EUR', 0.79, '2022-01-08 13:29');


SELECT DISTINCT
    COALESCE(u.name, 'not defined') name,
    COALESCE(u.lastname, 'not defined') lastname,
    c.name currency_name,
    COALESCE(
        (
            SELECT MAX(rate_to_usd)
            FROM currency cr
            WHERE cr.updated <= b.updated
                  AND
                  cr.id = c.id
            GROUP BY cr.updated
            ORDER BY cr.updated DESC
            LIMIT 1
        ),
        (
            SELECT MAX(rate_to_usd)
            FROM currency cr
            WHERE cr.updated > b.updated
                  AND
                  cr.id = c.id
            GROUP BY cr.updated
            ORDER BY cr.updated
            LIMIT 1
        )
    ) * b.money currency_in_usd

FROM "user" u

FULL JOIN balance b
ON u.id = b.user_id

INNER JOIN currency c
ON c.id = b.currency_id

ORDER BY
    name DESC,
    lastname,
    currency_name;
