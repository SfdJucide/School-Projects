-- customers
DROP VIEW IF EXISTS customers CASCADE;

CREATE VIEW customers AS

WITH avg_checks AS (
	SELECT 
		pd.customer_id AS "Customer_ID",
		avg(t.transaction_summ) AS "Customer_Average_Check"
	FROM
		"Personal_Data" pd
	JOIN "Cards" c ON
		c.customer_id = pd.customer_id
	JOIN "Transactions" t ON
		t.customer_card_id = c.customer_card_id
	GROUP BY
		pd.customer_id
	ORDER BY
		"Customer_Average_Check" DESC
),

avg_checks_seg AS (
	SELECT 
		"Customer_ID",
		"Customer_Average_Check",
	    CASE
			WHEN ROW_NUMBER() OVER () <= ROUND(((SELECT count(*) FROM avg_checks) * 0.1), 0) THEN 'High'
			WHEN ROW_NUMBER() OVER () <= ROUND((SELECT count(*) FROM avg_checks) * 0.35, 0) THEN 'Medium'
			ELSE 'Low'
		END AS "Customer_Average_Check_Segment"
	FROM
		avg_checks
),

freq AS (
	SELECT 
		pd.customer_id AS "Customer_ID",
		(EXTRACT(EPOCH FROM max(t.transaction_datetime) - min(t.transaction_datetime)) / (60 * 60 * 24) + 1) / count(DISTINCT t.transaction_id) AS "Customer_Frequency",
		(EXTRACT(EPOCH FROM ((SELECT analysis_formation FROM "Date_Of_Analysis_Formation") - max(t.transaction_datetime))) / (60 * 60 * 24) + 1) AS "Customer_Inactive_Period"
	FROM
		"Personal_Data" pd
	JOIN "Cards" c ON
		c.customer_id = pd.customer_id
	JOIN "Transactions" t ON
		t.customer_card_id = c.customer_card_id
	GROUP BY
		pd.customer_id
	ORDER BY
		"Customer_Frequency" ASC
),

freq_seg AS (
	SELECT 
		"Customer_ID",
		"Customer_Frequency",
	    CASE
			WHEN ROW_NUMBER() OVER () <= ROUND(((SELECT count(*) FROM freq) * 0.1), 0) THEN 'Often'
			WHEN ROW_NUMBER() OVER () <= ROUND((SELECT count(*) FROM freq) * 0.35, 0) THEN 'Occasionally'
			ELSE 'Rarely'
		END AS "Customer_Frequency_Segment",
	    "Customer_Inactive_Period",
	    "Customer_Inactive_Period" / "Customer_Frequency" AS "Customer_Churn_Rate",
	    CASE
			WHEN ("Customer_Inactive_Period" / "Customer_Frequency") < 2 THEN 'Low'
			WHEN ("Customer_Inactive_Period" / "Customer_Frequency") < 5 THEN 'Medium'
			ELSE 'High'
		END AS "Customer_Churn_Segment"
	FROM
		freq
),

popular_stores AS (
	SELECT 
		customer_id,
		transaction_store_id AS top_store
	FROM
		(
		SELECT 
			pd.customer_id,
		    transaction_store_id,
		    COUNT(transaction_store_id) AS tr_count ,
		    max(transaction_datetime) AS last_tr_dt,
		    ROW_NUMBER() OVER (PARTITION BY pd.customer_id
		ORDER BY
			COUNT(transaction_store_id) DESC,
			max(transaction_datetime) DESC) AS store_rating
		FROM
			"Personal_Data" pd
		JOIN "Cards" c ON
			pd.customer_id = c.customer_id
		JOIN "Transactions" t ON
			t.customer_card_id = c.customer_card_id
		GROUP BY 
				pd.customer_id,
			    transaction_store_id
	)
	WHERE store_rating = 1
),

three_last_store AS (
	SELECT
		customer_id,
		CASE
			WHEN COUNT(DISTINCT transaction_store_id) > 1 THEN NULL
			ELSE max(transaction_store_id)
		END AS customer_popular_last_store
	FROM
		(
		SELECT
			pd.customer_id,
	        t.transaction_datetime,
	        t.Transaction_Store_ID,
	        ROW_NUMBER() OVER (PARTITION BY pd.customer_id ORDER BY t.transaction_datetime DESC) AS transaction_number
		FROM
			"Personal_Data" pd
		JOIN "Cards" c ON
			pd.customer_id = c.customer_id
		JOIN "Transactions" t ON
			t.customer_card_id = c.customer_card_id
		ORDER BY
				pd.customer_id,
				t.transaction_datetime DESC)
	WHERE
		transaction_number < 4
	GROUP BY
		customer_id
),

primary_stores AS (
	SELECT 
		ps.customer_id AS "Customer_ID",
		CASE
			WHEN customer_popular_last_store IS NULL THEN top_store
			ELSE customer_popular_last_store
		END AS "Customer_Primary_Store"
	FROM
		popular_stores ps
	NATURAL JOIN three_last_store ls
)

SELECT 
	"Customer_ID",
	"Customer_Average_Check",
	"Customer_Average_Check_Segment",
	"Customer_Frequency",
	"Customer_Frequency_Segment",
	"Customer_Inactive_Period",
	"Customer_Churn_Rate",
	"Customer_Churn_Segment",
	CASE
		WHEN "Customer_Average_Check_Segment" = 'Low' THEN 0
		WHEN "Customer_Average_Check_Segment" = 'Medium' THEN 1
		ELSE 2
	END * 9 +
	CASE
		WHEN "Customer_Frequency_Segment" = 'Rarely' THEN 0
		WHEN "Customer_Frequency_Segment" = 'Occasionally' THEN 1
		ELSE 2
	END * 3 +
	CASE
		WHEN "Customer_Churn_Segment" = 'Low' THEN 0
		WHEN "Customer_Churn_Segment" = 'Medium' THEN 1
		ELSE 2
	END + 1 AS "Customer_Segment",
	"Customer_Primary_Store"
FROM
	avg_checks_seg c
NATURAL JOIN freq_seg f
NATURAL JOIN primary_stores ps;

SELECT * FROM customers LIMIT 100;