-- Функция, определяющая предложения,  ориентированные на рост частоты визитов
-- Параметры функции:
--  -первая и последняя даты периода
--  -добавляемое число транзакций
--  -максимальный индекс оттока
--  -максимальная доля транзакций со скидкой (в процентах)
--  -допустимая доля маржи (в процентах)

CREATE OR REPLACE FUNCTION fnc_personal_offers(
    IN f_date date,
    IN l_date date,
    IN n_transactions int,
    IN max_ci numeric,
    IN max_sdt numeric,
    IN allowable_ms numeric
)
RETURNS TABLE (
    "Customer_ID" int,
    "Start_Date" date,
    "End_Date" date,
    "Required_Transactions_Count" int,
    "Group_Name" varchar,
    "Offer_Discount_Depth" int
)
LANGUAGE PLPGSQL
AS
$$
BEGIN
    RETURN QUERY
    SELECT
		DISTINCT AD.Customer_ID AS "Customer_ID",
		f_date AS "Start_Date",
		l_date AS "End_Date",
		FIRST_VALUE(AD.Required_Transactions_Count) OVER (PARTITION BY AD.Customer_ID ORDER BY AD.Customer_ID, AD.GAI DESC) AS "Required_Transactions_Count",
		FIRST_VALUE(AD.Group_Name) OVER (PARTITION BY AD.Customer_ID ORDER BY AD.Customer_ID, AD.GAI DESC) AS "Group_Name",
		(FIRST_VALUE(AD.GMD) OVER (PARTITION BY AD.Customer_ID ORDER BY AD.Customer_ID, AD.GAI DESC) * 100)::int AS "Offer_Discount_Depth"
	FROM (
		SELECT
			DISTINCT c."Customer_ID" AS Customer_ID,
			((l_date - f_date)::int / c."Customer_Frequency")::int + n_transactions AS Required_Transactions_Count,
			gs.group_name AS Group_Name,
			allowable_ms / 100.0 * avg(g."Group_Margin") OVER (PARTITION BY g."Customer_ID", g."Group_ID") AS MAD,
			g."Group_Affinity_Index" AS GAI,
			g."Group_Minimum_Discount" * 1.05 AS GMD
		FROM
			customers c
		JOIN groups g ON
			g."Customer_ID" = c."Customer_ID"
		JOIN "Groups_SKU" gs ON
			g."Group_ID" = gs.group_id
		WHERE
			g."Group_Churn_Rate" <= max_ci
			AND g."Group_Discount_Share" <= max_sdt
		ORDER BY
			c."Customer_ID") AS AD
	WHERE
		AD.GMD < AD.MAD;
END;
$$;

SELECT * FROM fnc_personal_offers('2018-08-18'::date, '2019-08-18'::date, 1, 4, 0.6, 35.0);
SELECT * FROM fnc_personal_offers('2022-08-18'::date, '2022-08-18'::date, 1, 3, 70, 30);
SELECT * FROM fnc_personal_offers('2022-08-18'::date, '2022-08-18'::date, 2, 4, 50, 20);
SELECT * FROM fnc_personal_offers('2022-08-18'::date, '2022-08-18'::date, 7, 3, 70, 60);
SELECT * FROM fnc_personal_offers('2022-08-18'::date, '2022-08-18'::date, 9, 11, 10, 20);