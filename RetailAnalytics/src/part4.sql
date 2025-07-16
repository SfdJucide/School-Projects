-- Функция, определяющая предложения, ориентированные на рост среднего чека
-- Параметры функции:
--  -метод расчета среднего чека (1 - за период, 2 - за количество)
--  -первая и последняя даты периода (для 1 метода)
--  -количество транзакций (для 2 метода)
--  -коэффициент увеличения среднего чека
--  -максимальный индекс оттока
--  -максимальная доля транзакций со скидкой (в процентах)
--  -допустимая доля маржи (в процентах)

CREATE OR REPLACE FUNCTION average_periods(
    IN start_date date,
    IN end_date date,
    IN koeff_up NUMERIC
)
RETURNS TABLE (
    customer_id int,
    average_check NUMERIC
)
LANGUAGE PLPGSQL
AS
$$
BEGIN
    RETURN QUERY
	SELECT
		c.customer_id,
		avg(t.transaction_summ) * koeff_up
	FROM
		"Cards" c
	JOIN "Transactions" t ON
		c.customer_card_id = t.customer_card_id
	WHERE
		t.transaction_datetime >= start_date
		AND t.transaction_datetime <= end_date
	GROUP BY
		c.customer_id;
END;
$$;

CREATE OR REPLACE FUNCTION average_count(
	IN transactions_count int,
	IN koeff_up NUMERIC
)
RETURNS TABLE (
    customer_id int,
    average_check NUMERIC
)
LANGUAGE PLPGSQL
AS
$$
BEGIN
    RETURN QUERY
    SELECT
		rn.customer_id,
		AVG(transaction_summ) * koeff_up AS average_check
	FROM (
		SELECT
			c.customer_id,
			t.transaction_summ,
			ROW_NUMBER() OVER (PARTITION BY c.customer_id ORDER BY t.transaction_datetime DESC) AS row_num
		FROM
			"Cards" c
		JOIN "Transactions" t ON
			c.customer_card_id = t.customer_card_id) rn
	WHERE
		row_num <= transactions_count
	GROUP BY
		rn.customer_id,
		koeff_up;
END;
$$;

CREATE OR REPLACE FUNCTION reward_group(
    IN max_churn_rate NUMERIC,
    IN max_transitions_with_discount NUMERIC,
    IN max_margin_share NUMERIC
)
RETURNS TABLE (
    customer_id int,
    group_id int,
    group_margin NUMERIC,
    group_minimum_discount NUMERIC
)
LANGUAGE PLPGSQL
AS
$$
BEGIN
    RETURN QUERY
	SELECT
		full_i."Customer_ID",
		full_i."Group_ID",
		full_i.custom_margin,
		full_i.rounded_min_discount
	FROM
		(
		SELECT
			"Customer_ID",
			"Group_ID",
			("Group_Margin" * max_margin_share) AS custom_margin,
			CEIL("Group_Minimum_Discount" / 0.05) * 0.05 AS rounded_min_discount,
			row_nums.rn AS rn,
			min(row_nums.rn) OVER (PARTITION BY "Customer_ID") AS min_rn
		FROM (
			SELECT
				*,
				ROW_NUMBER() OVER (PARTITION BY "Customer_ID" ORDER BY "Group_Affinity_Index") rn
			FROM
				GROUPS
			WHERE
				"Group_Churn_Rate" <= max_churn_rate
				AND "Group_Discount_Share" < max_transitions_with_discount
		) AS row_nums
	) AS full_i
	WHERE
		custom_margin <= rounded_min_discount
		AND rn = min_rn;
END;
$$;

CREATE OR REPLACE FUNCTION average_check_up(
    IN calc_method int,
    IN first_date date,
    IN last_date date,
    IN transactions_num int,
    IN koeff_up NUMERIC,
    IN max_churn_rate NUMERIC,
    IN max_transitions_with_discount NUMERIC,
    IN max_margin_share NUMERIC
)
RETURNS TABLE (
    "Customer_ID" int,
    "Required_Check_Measure" NUMERIC,
    "Group_Name" varchar,
    "Offer_Discount_Depth" NUMERIC
)
LANGUAGE PLPGSQL
AS
$$
DECLARE
    upper_bound date := (SELECT MAX(transaction_datetime) FROM "Transactions");
    lower_bound date := (SELECT MIN(transaction_datetime) FROM "Transactions");
BEGIN
    IF last_date < first_date THEN
        last_date = upper_bound;
    END IF;
    IF last_date > upper_bound THEN
        last_date = upper_bound;
    END IF;
    IF first_date < lower_bound THEN
        first_date = lower_bound;
    END IF;

    IF calc_method = 1 THEN
		RETURN QUERY
		SELECT
			ap.customer_id AS "Customer_ID",
			ap.average_check AS "Required_Check_Measure",
			sg.group_name AS "Group_Name",
			rg.group_margin AS "Offer_Discount_Depth"
		FROM
			average_periods(first_date, last_date, koeff_up) ap
		JOIN reward_group(max_churn_rate, max_transitions_with_discount, max_margin_share) rg ON
			ap.customer_id = rg.customer_id
		JOIN "Groups_SKU" sg ON
			rg.group_id = sg.group_id;
    ELSIF calc_method = 2 THEN
        RETURN QUERY
        SELECT
			ac.customer_id AS "Customer_ID",
			ac.average_check AS "Required_Check_Measure",
			sg.group_name AS "Group_Name",
			rg.group_margin AS "Offer_Discount_Depth"
		FROM
			average_count(transactions_num, koeff_up) ac
		JOIN reward_group(max_churn_rate, max_transitions_with_discount, max_margin_share) rg ON
			ac.customer_id = rg.customer_id
		JOIN "Groups_SKU" sg ON
			rg.group_id = sg.group_id;
    ELSE
        RAISE EXCEPTION 'WRONG calc method parameter';
    END IF;
END;
$$;

SELECT * FROM average_check_up(1, '2018-01-01'::date, '2019-01-01'::date, 2, 0.2, 200, 0.75, 1);
SELECT * FROM average_check_up(2, '2018-01-01'::date, '2019-01-01'::date, 100,  1.15, 3, 70, 30);
SELECT * FROM average_check_up(1, '2018-01-01'::date, '2020-01-01'::date, 77, 0.95, 7, 67, 11);
SELECT * FROM average_check_up(1, '2018-01-01'::date, '2019-01-01'::date, 22, 1, 10, 99, 99);
SELECT * FROM average_check_up(2, '2018-01-01'::date, '2019-01-01'::date, 22, 1, 10, 99, 99);