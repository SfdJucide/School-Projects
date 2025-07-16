-- Функция, определяющая предложения, ориентированные на кросс-продажи (рост маржи)
-- Параметры функции:
--  -количество групп
--  -максимальный индекс оттока
--  -максимальный индекс стабильности потребления
--  -максимальная доля SKU (в процентах)
--  -допустимая доля маржи (в процентах)

-- Предложения, ориентированные на рост маржи за счет кросс-продаж,
-- подразумевают переключение клиента на максимально маржинальное SKU в
-- рамках востребованной им группы.

CREATE OR REPLACE FUNCTION fnc_cross_selling(
    IN cnt_group int,
    IN max_churn_rate numeric,
    IN max_stability_index numeric,
    IN max_sku_share numeric,
    IN max_margin_share numeric
)
RETURNS TABLE (
	"Customer_ID" int,
    "SKU_Name" varchar,
    "Offer_Discount_Depth" int
)
LANGUAGE PLPGSQL
AS
$$
BEGIN
    RETURN QUERY
	SELECT
		DISTINCT MD."Customer_ID" AS "Customer_ID",
		MD.prod_name AS "SKU_Name",
		CASE
			WHEN (MD."Group_Minimum_Discount" * 1.05 * 100)::int = 0 THEN 5
			ELSE (MD."Group_Minimum_Discount" * 1.05 * 100)::int
		END AS "Offer_Discount_Depth"
	FROM
		(
		SELECT
			DENSE_RANK() OVER (PARTITION BY gp."Customer_ID" ORDER BY gp."Group_ID") AS dense_r,
			FIRST_VALUE(sk.sku_name) OVER (PARTITION BY gp."Customer_ID", gp."Group_ID" ORDER BY (st.sku_retail_price - st.sku_purchase_price) DESC) AS prod_name,
			gp."Group_ID" AS g_id,
			gp."Group_Minimum_Discount",
			gp."Customer_ID",
			st.sku_retail_price,
			st.sku_purchase_price
		FROM
			"Transactions" t
		JOIN "Checks" c ON
			t.transaction_id = c.transaction_id
		JOIN "Cards" cd ON
			cd.customer_card_id = t.customer_card_id
		JOIN "Personal_Data" pd ON
			pd.customer_id = cd.customer_id
		JOIN "SKU" sk ON
			sk.sku_id = c.sku_id
		JOIN "Stores" st ON
			st.transaction_store_id = t.transaction_store_id
			AND st.sku_id = sk.sku_id
		JOIN customers ct ON
			ct."Customer_ID" = pd.customer_id
		JOIN GROUPS gp ON
			gp."Customer_ID" = pd.customer_id
			AND gp."Group_ID" = sk.group_id
		WHERE
			ct."Customer_Primary_Store" = t.transaction_store_id
			AND gp."Group_Churn_Rate" <= max_churn_rate
			AND gp."Group_Stability_Index" < max_stability_index) AS MD
	WHERE
		dense_r <= cnt_group
		AND (
			SELECT
				count(*) FILTER (WHERE sk.sku_name = MD.prod_name)::NUMERIC / count(*)
			FROM
				"Transactions" t
			JOIN "Checks" c ON
				t.transaction_id = c.transaction_id
			JOIN "Cards" cd ON
				cd.customer_card_id = t.customer_card_id
			JOIN "Personal_Data" pd ON
				pd.customer_id = cd.customer_id
			JOIN "SKU" sk ON
				sk.sku_id = c.sku_id
			JOIN "Stores" st ON
				st.transaction_store_id = t.transaction_store_id
				AND st.sku_id = sk.sku_id
			WHERE
				pd.customer_id = MD."Customer_ID"
				AND sk.group_id = MD.g_id) < max_sku_share
		AND (MD.sku_retail_price - MD.sku_purchase_price) * max_margin_share / 100.0 / MD.sku_retail_price >= MD."Group_Minimum_Discount" * 1.05;
END ;
$$;

	
SELECT * FROM fnc_cross_selling(100, 100, 100, 2, 10);
SELECT * FROM fnc_cross_selling(5, 3, 0.5, 100, 30);
SELECT * FROM fnc_cross_selling(7, 3, 0.7, 200, 30);
SELECT * FROM fnc_cross_selling(5, 5, 0.5, 50, 20);
SELECT * FROM fnc_cross_selling(4, 4, 0.4, 30, 30);