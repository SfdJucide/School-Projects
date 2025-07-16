-- purchase_history
DROP VIEW IF EXISTS purchase_history CASCADE;

CREATE VIEW purchase_history AS
SELECT
	pd.customer_id AS "Customer_ID",
	t.transaction_id AS "Transaction_ID",
	t.transaction_datetime AS "Transaction_DateTime",
	sk.group_id AS "Group_ID",
	sum(s.sku_purchase_price * c2.sku_amount) AS "Group_Cost",
	sum(c2.sku_summ) AS "Group_Summ",
	sum(c2.sku_summ_paid) AS "Group_Summ_Paid"
FROM
	"Personal_Data" pd
JOIN "Cards" c ON
	c.customer_id = pd.customer_id
JOIN "Transactions" t ON
	t.customer_card_id = c.customer_card_id
JOIN "Checks" c2 ON
	c2.transaction_id = t.transaction_id
JOIN "SKU" sk ON
	sk.sku_id = c2.sku_id
JOIN "Stores" s ON
	s.sku_id = sk.sku_id
	AND s.transaction_store_id = t.transaction_store_id
GROUP BY 
	pd.customer_id,
	t.transaction_id,
	t.transaction_datetime,
	sk.group_id
ORDER BY
	"Customer_ID",
	"Transaction_ID",
	"Transaction_DateTime",
	"Group_ID";

SELECT * FROM purchase_history LIMIT 100;