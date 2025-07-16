-- groups
CREATE OR REPLACE FUNCTION func_group_margin(IN mode_margin int DEFAULT 3, IN in_value int DEFAULT 100)
RETURNS TABLE (
    "Customer_ID" INT,
    "Group_ID" INT,
    "Group_Margin" NUMERIC
)
LANGUAGE PLPGSQL
AS 
$$
BEGIN
	IF mode_margin = 1 THEN
        RETURN QUERY
        SELECT
            ph."Customer_ID",
            ph."Group_ID",
            CASE
                WHEN SUM("Group_Summ_Paid") > 0 THEN SUM("Group_Summ_Paid" - "Group_Cost") / SUM("Group_Summ_Paid")
                ELSE 0
            END AS "Group_Margin"
        FROM
            purchase_history ph
        WHERE
            "Transaction_DateTime"::DATE >= (
                SELECT
                    MAX(analysis_formation)::DATE - in_value
                FROM
                    "Date_Of_Analysis_Formation"
            )
        GROUP BY
           ph."Customer_ID",
           ph."Group_ID";

    ELSIF mode_margin = 2 THEN
        RETURN QUERY
        SELECT
            lph."Customer_ID",
            lph."Group_ID",
            CASE
                WHEN SUM("Group_Summ_Paid") > 0 THEN SUM(("Group_Summ_Paid" - "Group_Cost") / "Group_Summ_Paid")
                ELSE 0
            END AS "Group_Margin"
        FROM (
            SELECT
                "Customer_ID",
                "Group_ID",
                "Group_Summ_Paid",
                "Group_Cost",
                "Group_Summ_Paid" - "Group_Cost" AS margin
            FROM
                purchase_history
            ORDER BY
                "Transaction_DateTime" DESC
            LIMIT
                1000
        ) AS lph
        GROUP BY
           lph."Customer_ID",
           lph."Group_ID";

    ELSE
        RETURN QUERY
        SELECT
            ph."Customer_ID",
            ph."Group_ID",
            CASE
                WHEN SUM("Group_Summ_Paid") > 0 THEN SUM("Group_Summ_Paid" - "Group_Cost") / SUM("Group_Summ_Paid")
                ELSE 0
            END AS "Group_Margin"
        FROM
            purchase_history ph
        GROUP BY
           ph."Customer_ID",
           ph."Group_ID";
    END IF;
END;
$$;

DROP MATERIALIZED VIEW IF EXISTS groups CASCADE;

CREATE MATERIALIZED VIEW groups AS

WITH aff_idx AS (
	SELECT
		p."Customer_ID",
		p."Group_ID",
		COALESCE(p."Group_Purchase"::NUMERIC / NULLIF(COUNT(DISTINCT ph."Transaction_ID"), 0), 0) AS "Group_Affinity_Index"
	FROM
		periods p
	LEFT JOIN
	    purchase_history ph ON
		ph."Customer_ID" = p."Customer_ID"
		AND ph."Transaction_DateTime" BETWEEN p."First_Group_Purchase_Date" AND p."Last_Group_Purchase_Date"
	GROUP BY
		p."Customer_ID",
		p."Group_ID",
		p."Group_Purchase"
),

churn_rate AS (
	SELECT
		p."Customer_ID",
		p."Group_ID",
		CASE
			WHEN p."Group_Frequency" = 0 THEN 0
			ELSE (DATE_PART('day', (SELECT MAX(analysis_formation) FROM "Date_Of_Analysis_Formation") - p."Last_Group_Purchase_Date"))::NUMERIC / p."Group_Frequency"
		END AS "Group_Churn_Rate"
	FROM
		periods p
	LEFT JOIN
	    purchase_history ph ON
		ph."Customer_ID" = p."Customer_ID"
	GROUP BY
		p."Customer_ID",
		p."Group_ID",
		p."Last_Group_Purchase_Date",
		p."Group_Frequency"
),

stab_idx AS (
    SELECT
	    "Customer_ID",
	    "Group_ID",
	    AVG(
	    	CASE
	            WHEN gr_frequency = 0 THEN 0
	            ELSE
	                CASE
	                    WHEN gr_frequency > intervals THEN gr_frequency - intervals
	                    ELSE intervals - gr_frequency
	                END::numeric / gr_frequency
	            END
	        ) AS "Group_Stability_Index"
	FROM (
		SELECT
	        ph."Customer_ID",
	        ph."Group_ID",
	        ph."Transaction_DateTime" AS tr_date,
	        COALESCE(DATE_PART('day', ph."Transaction_DateTime" - LAG(ph."Transaction_DateTime") OVER (PARTITION BY ph."Customer_ID", ph."Group_ID" ORDER BY ph."Transaction_DateTime")), 0) AS intervals,
	        p."Group_Frequency" AS gr_frequency
	    FROM
	        purchase_history ph
	    JOIN
	        periods p ON p."Customer_ID" = ph."Customer_ID" AND p."Group_ID" = ph."Group_ID")
	GROUP BY
	    "Customer_ID",
	    "Group_ID"
),

avg_discount AS (
	SELECT
	    "Customer_ID",
	    "Group_ID",
	    SUM("Group_Summ_Paid") / NULLIF(SUM("Group_Summ"), 0) AS "Group_Average_Discount"
	FROM
	    purchase_history
	GROUP BY
	    "Customer_ID",
	    "Group_ID"
),

discount_inf AS (
	SELECT
	    p."Customer_ID",
	    p."Group_ID",
	   COALESCE(dt.tr_cnt, 0)::numeric / p."Group_Purchase" AS "Group_Discount_Share",
	    p."Group_Min_Discount" AS "Group_Minimum_Discount"
	FROM (
	    SELECT
	        cd.customer_id,
	        pd.group_id,
	        COUNT(DISTINCT tr.transaction_id) AS tr_cnt
	    FROM "Transactions" tr
		JOIN "Cards" cd ON cd.customer_card_id = tr.customer_card_id
		JOIN "Checks" bl ON bl.transaction_id = tr.transaction_id
		JOIN "SKU" pd ON pd.sku_id = bl.sku_id
		JOIN "Stores" st ON st.transaction_store_id = tr.transaction_store_id AND st.sku_id = pd.sku_id
	    WHERE
	        bl.sku_discount > 0
	    GROUP BY
	        cd.customer_id,
	        pd.group_id
		) dt
	RIGHT JOIN
	    periods p ON
		p."Customer_ID" = dt.customer_id
		AND p."Group_ID" = dt.group_id
)

SELECT
    ai."Customer_ID",
    ai."Group_ID",
    ai."Group_Affinity_Index",
    cr."Group_Churn_Rate",
    si."Group_Stability_Index",
    gm."Group_Margin",
    di."Group_Discount_Share",
    di."Group_Minimum_Discount",
    ad."Group_Average_Discount"
FROM
    aff_idx ai
JOIN
    churn_rate cr ON cr."Customer_ID" = ai."Customer_ID" AND cr."Group_ID" = ai."Group_ID"
JOIN
    stab_idx si ON si."Customer_ID" = ai."Customer_ID" AND si."Group_ID" = ai."Group_ID"
JOIN
    discount_inf di ON di."Customer_ID" = ai."Customer_ID" AND di."Group_ID" = ai."Group_ID"
JOIN
    avg_discount ad ON ad."Customer_ID" = ai."Customer_ID" AND ad."Group_ID" = ai."Group_ID"
JOIN 
	func_group_margin() gm ON gm."Customer_ID" = ai."Customer_ID" AND gm."Group_ID" = ai."Group_ID";

SELECT * FROM GROUPS LIMIT 100;