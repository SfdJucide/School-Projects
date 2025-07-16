-- peroids
DROP VIEW IF EXISTS periods CASCADE;

CREATE VIEW periods AS
SELECT
	pd.customer_id AS "Customer_ID",
	sk.group_id AS "Group_ID",
	min(transaction_datetime) AS "First_Group_Purchase_Date",
	max(transaction_datetime) AS "Last_Group_Purchase_Date",
	count(DISTINCT t.transaction_id) AS "Group_Purchase",
	(EXTRACT(EPOCH FROM max(transaction_datetime) - min(transaction_datetime)) / (60 * 60 * 24) + 1) / count(DISTINCT t.transaction_id) AS "Group_Frequency",
	COALESCE(MIN(NULLIF(c2.sku_discount / NULLIF(c2.sku_summ, 0), 0)), 0) AS "Group_Min_Discount"
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
GROUP BY 
	pd.customer_id,
	sk.group_id
ORDER BY
	"Customer_ID",
	"Group_ID";

SELECT * FROM periods LIMIT 100;