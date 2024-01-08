-- 1) Функция, возвращающая таблицу TransferredPoints в более человекочитаемом виде
CREATE OR REPLACE FUNCTION fnc_show_transferred_points()
  RETURNS TABLE(Peer1 varchar,  Peer2 varchar, PointsAmount int)
  LANGUAGE SQL
  AS
$$
	SELECT
		Peer1,
		Peer2,
		sum(PointsAmount) AS PointsAmount
	FROM
		(SELECT
			LEAST(checkingpeer, checkedpeer) AS Peer1,
			GREATEST(checkingpeer, checkedpeer) AS Peer2,
			CASE WHEN checkingpeer > checkedpeer THEN -pointsamount ELSE pointsamount END AS PointsAmount
		FROM
			transferredpoints)
	GROUP BY
		Peer1,
		Peer2
	ORDER BY
		Peer1,
		Peer2;
$$;

SELECT * FROM fnc_show_transferred_points();


-- 2) Функция, которая возвращает таблицу вида: ник пользователя, название проверенного задания, кол-во полученного XP
CREATE OR REPLACE FUNCTION fnc_get_peers_tasks_xp()
  RETURNS TABLE(Peer varchar, Task varchar, XP int)
  LANGUAGE SQL
  AS
$$
	SELECT 
		c.peer AS peer,
		c.task AS task,
		x.xpamount AS xp
	FROM
		checks c
	JOIN xp x ON
		x."Check" = c.id
	JOIN p2p p ON
		c.id = p."Check"
	LEFT JOIN verter v ON
		c.id = v."Check"
	WHERE
		p.state = 'Success'
		AND (v.state = 'Success'
			OR v.state IS NULL)
	ORDER BY
		peer,
		task;
$$;

SELECT * FROM fnc_get_peers_tasks_xp();


-- 3) Функция, которая определят пиров, которые не выходили из кампуса в течение всего дня
CREATE OR REPLACE FUNCTION fnc_get_not_coming_out_peers(IN check_date DATE)
  RETURNS TABLE(Peer varchar)
  LANGUAGE SQL
  AS
$$
	SELECT
		peer
	FROM
		(
		SELECT
			peer,
			count(*)
		FROM
			timetracking
		WHERE
			state = 2
			AND date = check_date
		GROUP BY
			peer)
	WHERE
		count = 1;
$$;

SELECT * FROM fnc_get_not_coming_out_peers('2022-12-03');
SELECT * FROM fnc_get_not_coming_out_peers('2023-01-08');


-- 4) Функция, которая считает изменение в количестве пир поинтов каждого пира по таблице TransferredPoints
CREATE OR REPLACE FUNCTION fnc_get_peers_peerpoints()
  RETURNS TABLE(Peer varchar, PointsChange int)
  LANGUAGE SQL
  AS
$$
	SELECT
		peer,
		sum(points) AS PointsChange
	FROM
		(
		SELECT
			checkingpeer AS peer,
			pointsamount AS points
		FROM
			transferredpoints
		UNION ALL
		SELECT
			checkedpeer AS peer,
			-pointsamount AS points
		FROM
			transferredpoints)
	GROUP BY
		peer
	ORDER BY
		PointsChange DESC;
$$;

SELECT * FROM fnc_get_peers_peerpoints();


-- 5) Функция, которая считает изменение в количестве пир поинтов каждого пира по таблице из функции fnc_show_transferred_points
CREATE OR REPLACE FUNCTION fnc_get_peers_peerpoints_from_func()
  RETURNS TABLE(Peer varchar, PointsChange int)
  LANGUAGE SQL
  AS
$$
	SELECT
		peer,
		sum(points) AS PointsChange
	FROM
		(
		SELECT
			peer1 AS peer,
			pointsamount AS points
		FROM
			fnc_show_transferred_points()
		UNION ALL
		SELECT
			peer2 AS peer,
			-pointsamount AS points
		FROM
			fnc_show_transferred_points())
	GROUP BY
		peer
	ORDER BY
		PointsChange DESC;
$$;

SELECT * FROM fnc_get_peers_peerpoints_from_func();


-- 6) Функция, которая определяет самое часто проверяемое задание за каждый день
CREATE OR REPLACE FUNCTION fnc_get_most_checking_task_per_day()
  RETURNS TABLE("Day" DATE, Task varchar)
  LANGUAGE SQL
  AS
$$
	SELECT
		date,
		task
	FROM
		(
		SELECT
			date,
			task,
			DENSE_RANK() OVER (PARTITION BY date ORDER BY count(*) DESC) AS rnk
		FROM
			checks
		GROUP BY
			date,
			task)
	WHERE
		rnk = 1;
$$;

SELECT * FROM fnc_get_most_checking_task_per_day();


-- 7) Функция, которая находит всех пиров, выполнивших весь заданный блок задач и дату завершения последнего задания
CREATE OR REPLACE FUNCTION fnc_get_done_block_peers(IN block_name varchar)
  RETURNS TABLE(Peer varchar, "Day" DATE)
  LANGUAGE SQL
  AS
$$
	SELECT
		peer,
		day
	FROM
		(
		SELECT
			peer,
			count(DISTINCT task) AS succsess_count,
			max(date) AS day
		FROM
			checks c
		JOIN p2p p ON
			c.id = p."Check"
		LEFT JOIN verter v ON
			c.id = v."Check"
		WHERE
			c.task ~ concat(block_name, '[0-9]')
			AND p.state = 'Success'
			AND (v.state = 'Success'
				OR v.state IS NULL)
		GROUP BY
			peer)
	WHERE
		succsess_count = (
			SELECT
				count(title)
			FROM
				tasks
			WHERE
				title ~ concat(block_name, '[0-9]')
		);
$$;

SELECT * FROM fnc_get_done_block_peers('C');
SELECT * FROM fnc_get_done_block_peers('CPP');
SELECT * FROM fnc_get_done_block_peers('D');


-- 8) Функция, которая определяет, к какому пиру стоит идти на проверку каждому обучающемуся
CREATE OR REPLACE FUNCTION fnc_find_recommended_peer()
  RETURNS TABLE(Peer varchar, RecommendedPeer varchar)
  LANGUAGE SQL
  AS
$$
	SELECT
		p.nickname AS peer,
		r.recommendedpeer AS RecommendedPeer
	FROM
		(
		SELECT
			nickname,
			recommendedpeer,
			DENSE_RANK() OVER (PARTITION BY nickname ORDER BY count(*) DESC) AS rnk
		FROM
			(
			SELECT
				*
			FROM
				peers p
			JOIN friends f ON
				f.peer1 = p.nickname
			JOIN recommendations r ON
				r.peer = f.peer2
			WHERE
				p.nickname != r.recommendedpeer
			UNION ALL
			SELECT
				*
			FROM
				peers p
			JOIN friends f ON
				f.peer2 = p.nickname
			JOIN recommendations r ON
				r.peer = f.peer1
			WHERE
				p.nickname != r.recommendedpeer
			)
		GROUP BY
			nickname,
			recommendedpeer) r
	RIGHT JOIN peers p ON
		p.nickname = r.nickname
	WHERE
		rnk = 1
		OR rnk IS NULL;
$$;

SELECT * FROM fnc_find_recommended_peer();


-- 9) Функция, которая определяет информацию по пирам и блокам, которые они выполняют
CREATE OR REPLACE FUNCTION fnc_show_blocks_info(IN block1 varchar, IN block2 varchar)
  RETURNS TABLE(StartedBlock1 float, StartedBlock2 float, StartedBothBlocks float, DidntStartAnyBlock float)
  LANGUAGE SQL
  AS
$$
	SELECT 
		round((
		SELECT
			count(DISTINCT peer) * 1.0
		FROM
			checks
		WHERE
			task ~ concat(block1, '[0-9]')) / (
		SELECT
			count(*)
		FROM
			peers) * 100,
		2) AS StartedBlock1,
		
		round((
		SELECT
			count(DISTINCT peer) * 1.0
		FROM
			checks
		WHERE
			task ~ concat(block2, '[0-9]')) / (
		SELECT
			count(*)
		FROM
			peers) * 100,
		2) AS StartedBlock2,
		
		round((
		SELECT
			count(DISTINCT c.peer) * 1.0
		FROM
			checks c,
			checks c2
		WHERE
			c.task ~ concat(block1, '[0-9]')
				AND c2.task ~ concat(block2, '[0-9]')
				AND c.peer = c2.peer) / (
		SELECT
			count(*)
		FROM
			peers) * 100,
		2) AS StartedBothBlocks,
		
		round((
		SELECT
			count(DISTINCT nickname) * 1.0
		FROM
			peers
		WHERE
			nickname NOT IN (
			SELECT
				peer
			FROM
				checks
			WHERE
				task ~ concat(block1, '[0-9]')
					OR task ~ concat(block2, '[0-9]'))) / (
		SELECT
			count(*)
		FROM
			peers) * 100,
		2) AS DidntStartAnyBlock;
$$;

SELECT * FROM fnc_show_blocks_info('CPP', 'C');
SELECT * FROM fnc_show_blocks_info('CPP', 'D');


-- 10) Функция, которая определяет процент пиров, которые когда-либо успешно проходили проверку в свой день рождения
CREATE OR REPLACE FUNCTION fnc_get_succ_on_bd_peers()
  RETURNS TABLE(SuccessfulChecks float, UnsuccessfulChecks float)
  LANGUAGE SQL
  AS
$$
	SELECT 
		round((
		SELECT
			count(DISTINCT nickname)
		FROM
			(
			SELECT
				nickname,
				concat(EXTRACT(MONTH
			FROM
				birthday),
				'-',
				EXTRACT(DAY
			FROM
				birthday)) AS birthday
			FROM
				peers) pr
		JOIN (
			SELECT
				id,
				peer,
				task,
				concat(EXTRACT(MONTH
			FROM
				date),
				'-',
				EXTRACT(DAY
			FROM
				date)) AS date
			FROM
				checks) c ON
			c.peer = pr.nickname
		JOIN p2p p ON
				c.id = p."Check"
		LEFT JOIN verter v ON
			c.id = v."Check"
		WHERE
			pr.birthday = c.date
			AND p.state = 'Success'
			AND (v.state = 'Success'
				OR v.state IS NULL)) * 1.0 / (
			SELECT
				count(DISTINCT nickname)
			FROM
				(
				SELECT
					nickname,
					concat(EXTRACT(MONTH
				FROM
					birthday),
					'-',
					EXTRACT(DAY
				FROM
					birthday)) AS birthday
				FROM
					peers) pr
			JOIN (
				SELECT
					id,
					peer,
					task,
					concat(EXTRACT(MONTH
				FROM
					date),
					'-',
					EXTRACT(DAY
				FROM
					date)) AS date
				FROM
					checks) c ON
				c.peer = pr.nickname
			WHERE
				pr.birthday = c.date) * 100,
		2) AS SuccessfulChecks,
		
		round((
		SELECT
			count(DISTINCT nickname)
		FROM
			(
			SELECT
				nickname,
				concat(EXTRACT(MONTH
			FROM
				birthday),
				'-',
				EXTRACT(DAY
			FROM
				birthday)) AS birthday
			FROM
				peers) pr
		JOIN (
			SELECT
				id,
				peer,
				task,
				concat(EXTRACT(MONTH
			FROM
				date),
				'-',
				EXTRACT(DAY
			FROM
				date)) AS date
			FROM
				checks) c ON
			c.peer = pr.nickname
		JOIN p2p p ON
				c.id = p."Check"
		LEFT JOIN verter v ON
			c.id = v."Check"
		WHERE
			pr.birthday = c.date
			AND p.state = 'Failure'
				OR v.state = 'Failure') * 1.0 / (
			SELECT
				count(DISTINCT nickname)
			FROM
				(
				SELECT
					nickname,
					concat(EXTRACT(MONTH
				FROM
					birthday),
					'-',
					EXTRACT(DAY
				FROM
					birthday)) AS birthday
				FROM
					peers) pr
			JOIN (
				SELECT
					id,
					peer,
					task,
					concat(EXTRACT(MONTH
				FROM
					date),
					'-',
					EXTRACT(DAY
				FROM
					date)) AS date
				FROM
					checks) c ON
				c.peer = pr.nickname
			WHERE
				pr.birthday = c.date) * 100,
		2) AS UnsuccessfulChecks;
$$;

SELECT * FROM fnc_get_succ_on_bd_peers();


-- 11) Функция, которая определяет всех пиров, которые сдали заданные задания 1 и 2, но не сдали задание 3
CREATE OR REPLACE FUNCTION fnc_get_peers_who_done_task1_task2_but_not_task3(IN task1 varchar, IN task2 varchar, IN task3 varchar)
  RETURNS TABLE(Peer varchar)
  LANGUAGE SQL
  AS
$$
	SELECT
		DISTINCT t1.peer AS Peer
	FROM
		(
		SELECT
			c.peer AS peer,
			c.task AS task
		FROM
			checks c
		JOIN p2p p ON
			c.id = p."Check"
		LEFT JOIN verter v ON
			c.id = v."Check"
		WHERE
			c.task = task1
			AND p.state = 'Success'
			AND (v.state = 'Success'
				OR v.state IS NULL)) t1,
		(
		SELECT
			c.peer AS peer,
			c.task AS task
		FROM
			checks c
		JOIN p2p p ON
			c.id = p."Check"
		LEFT JOIN verter v ON
			c.id = v."Check"
		WHERE
			c.task = task2
			AND p.state = 'Success'
			AND (v.state = 'Success'
				OR v.state IS NULL)) t2,
		(
		SELECT
			nickname AS peer
		FROM
			peers
		WHERE
			nickname NOT IN 
		(
			SELECT
				c.peer AS peer
			FROM
				checks c
			JOIN p2p p ON
				c.id = p."Check"
			LEFT JOIN verter v ON
				c.id = v."Check"
			WHERE
				c.task = task3
				AND p.state = 'Success'
				AND (v.state = 'Success'
					OR v.state IS NULL))) t3
	WHERE
		t1.peer = t2.peer
		AND t2.peer = t3.peer;
$$;

SELECT * FROM fnc_get_peers_who_done_task1_task2_but_not_task3('C2_SimpleBashUtils', 'C4_s21_math', 'C3_S21_StringPlus');
SELECT * FROM fnc_get_peers_who_done_task1_task2_but_not_task3('C2_SimpleBashUtils', 'C4_s21_math', 'D01_Linux');


-- 12) Функция, которая для каждой задачи выводит кол-во предшествующих ей задач
CREATE OR REPLACE FUNCTION fnc_get_previous_tasks_count()
  RETURNS TABLE(Task varchar, PrevCount int)
  LANGUAGE SQL
  AS
$$
	WITH RECURSIVE taskCTE AS (
	    -- стартовая часть
	    SELECT 
	        title,
	        0 AS prev
	    FROM 
	    	tasks
	    WHERE parenttask IS NULL
	    
	    UNION ALL
	
	    -- рекурсивная часть
	    SELECT
			t.title,
			taskCTE.prev + 1
		FROM
			Tasks t
		INNER JOIN
				taskCTE ON
				t.parenttask = taskCTE.title
	)
	
	SELECT
		title AS Task,
		prev AS PrevCount
	FROM 
		taskCTE;
$$;

SELECT * FROM fnc_get_previous_tasks_count();


-- 13) Функция, которая находит "удачные" для проверок дни TODO
CREATE OR REPLACE FUNCTION fnc_get_lucky_days(IN N int)
  RETURNS TABLE(Day DATE)
  LANGUAGE SQL
  AS
$$
	WITH check_states AS (
		SELECT
			id,
			date,
			min(start_time) AS start_time,
			sum(state) AS state
		FROM
			(
			SELECT
				c.id,
				c.date,
				CASE
					WHEN (p.state = 'Start') THEN
					p.time
				END AS start_time,
				CASE
					WHEN (p.state = 'Success'
					AND (v.state = 'Success'
					OR v.state IS NULL)
					AND x.xpamount >= 0.8 * t.maxxp) THEN
					1
					ELSE
					0
				END AS state
			FROM
				checks c
			JOIN p2p p ON
				p."Check" = c.id
			LEFT JOIN verter v ON
				v."Check" = c.id
			LEFT JOIN xp x ON
				x."Check" = c.id
			JOIN tasks t ON
				t.title = c.task
			ORDER BY
				c.date,
				start_time)
		GROUP BY
			id,
			date
		ORDER BY
			date,
			start_time
	)
	
	SELECT
		date AS DAY
	FROM
		(
		SELECT
			date,
			start_time,
			state,
			CASE
				WHEN LAG(state) OVER (PARTITION BY date ORDER BY start_time) = state 
				THEN ROW_NUMBER() OVER (PARTITION BY date, state ORDER BY start_time)
				ELSE 1
			END AS in_row
		FROM
			check_states
		ORDER BY
			date,
			start_time)
	WHERE
		state = 1
	GROUP BY
		date
	HAVING 
		max(in_row) >= N;
$$;

SELECT * FROM fnc_get_lucky_days(2);
SELECT * FROM fnc_get_lucky_days(1);

-- 14) Функция, которая определяет пира с наибольшим количеством XP
CREATE OR REPLACE FUNCTION fnc_get_most_xp_peer()
  RETURNS TABLE(Peer varchar, XP int)
  LANGUAGE SQL
  AS
$$
	SELECT
		p.nickname AS peer,
		sum(xp.xpamount) AS XP
	FROM
		peers p
	JOIN checks c ON
		p.nickname = c.peer
	JOIN xp ON
		xp."Check" = c.id
	GROUP BY
		p.nickname
	ORDER BY
		xp DESC
	LIMIT 1;
$$;

SELECT * FROM fnc_get_most_xp_peer();


-- 15) Функция, которая определяет пиров, приходивших раньше заданного времени не менее N раз за всё время
CREATE OR REPLACE FUNCTION fnc_get_peers_who_entered_earlier_n_times(IN enter_time TIME, IN N int)
  RETURNS TABLE(Peer varchar)
  LANGUAGE SQL
  AS
$$
	SELECT
		peer
	FROM
		(
		SELECT
			peer,
			date,
			min(time)
		FROM
			timetracking
		WHERE
			time < enter_time
			AND state = 1
		GROUP BY
			peer,
			date)
	GROUP BY
		peer
	HAVING
		count(*) >= N;
$$;

SELECT * FROM fnc_get_peers_who_entered_earlier_n_times('12:00:00', 2);
SELECT * FROM fnc_get_peers_who_entered_earlier_n_times('18:00:00', 1);


-- 16) Функция, которая определяет пиров, выходивших за последние N дней из кампуса больше M раз
CREATE OR REPLACE FUNCTION fnc_get_peers_who_leaved_m_times(IN N int, IN M int)
  RETURNS TABLE(Peer varchar)
  LANGUAGE SQL
  AS
$$
	SELECT
		peer
	FROM
		timetracking
	WHERE
		state = 2
		AND now()::date - date <= N
	GROUP BY
		peer
	HAVING count(*) > M;
$$;

SELECT * FROM fnc_get_peers_who_leaved_m_times(400, 2);
SELECT * FROM fnc_get_peers_who_leaved_m_times(400, 0);


-- 17) Функция, которая определяет для каждого месяца процент ранних входов
CREATE OR REPLACE FUNCTION fnc_get_monthly_early_enters()
  RETURNS TABLE(Month varchar, EarlyEntries float)
  LANGUAGE SQL
  AS
$$
	SELECT
		a.birth_month AS MONTH,
		round(b.count * 1.0 / a.count * 100, 2) AS EarlyEntries
	FROM
		(
		SELECT
			birth_month,
			month_num,
			count(*)
		FROM
			(
			SELECT
				nickname,
				EXTRACT(MONTH
			FROM
				p.birthday) AS month_num,
				TO_CHAR(birthday, 'Month') AS birth_month,
				t."date",
				min(t."time")
			FROM
				peers p
			JOIN timetracking t ON
				p.nickname = t.peer
			WHERE
				state = 1
			GROUP BY
				birth_month,
				nickname,
				t."date")
		GROUP BY
			birth_month,
			month_num) a
	JOIN (
		SELECT
			birth_month,
			month_num,
			count(*)
		FROM
			(
			SELECT
				nickname,
				EXTRACT(MONTH
			FROM
				p.birthday) AS month_num,
				TO_CHAR(birthday, 'Month') AS birth_month,
				t."date",
				min(t."time") AS time
			FROM
				peers p
			JOIN timetracking t ON
				p.nickname = t.peer
			WHERE
				state = 1
			GROUP BY
				birth_month,
				nickname,
				t."date")
		WHERE
			time < '12:00:00'
		GROUP BY
			birth_month,
			month_num) b ON
		a.month_num = b.month_num
	ORDER BY
		a.month_num;
$$;

SELECT * FROM fnc_get_monthly_early_enters();