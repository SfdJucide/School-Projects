-- Please add a possibility to see additional rows with the most expensive cost to the SQL from previous exercise.
WITH all_paths AS 
(
	WITH RECURSIVE current_way AS (
	    -- стартовая часть start_way
	    SELECT 
	        point1 AS start_point,
	        point2 AS end_point,
	        cost AS path_cost,
	        2 AS lvl,
	        ARRAY [point1, point2] AS path,
	        cost AS total_cost,
	        FALSE AS is_cycled
	    FROM 
	    	paths
	    WHERE 
	    	point1 = 'a'
	    
	    UNION 
	
	    -- рекурсивная часть new_way
	    SELECT 
	        nw.point1 AS start_point,
	        nw.point2 AS end_point,
	        nw.cost AS path_cost,
	        cw.lvl + 1 AS lvl,
	        cw.path || nw.point2 AS path,
	        cw.total_cost + nw.cost AS total_cost,
	        nw.point2 = ANY(cw.path) AS is_cycled
	    FROM 
	    	current_way AS cw
	    JOIN 
	    	paths AS nw
	    ON cw.end_point = nw.point1
	    AND NOT cw.is_cycled
	)
	
	SELECT
		* 
	FROM 
		current_way 
	WHERE 
		path[array_upper(path,1)] = 'a'
		AND lvl = 5
)

SELECT 
	total_cost,
	path AS tour
FROM
	all_paths
WHERE
	total_cost IN (SELECT min(total_cost) FROM all_paths) 
	OR total_cost IN (SELECT max(total_cost) FROM all_paths)
ORDER BY
	total_cost,
	tour
