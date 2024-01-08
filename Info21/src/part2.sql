-- Процедура добавления P2P проверки
CREATE OR REPLACE PROCEDURE add_p2p(IN checked_peer varchar, IN checking_peer varchar, IN task_name varchar, IN status CheckStatus, IN p2p_time time)
LANGUAGE PLPGSQL
AS
$$
DECLARE
	last_state CheckStatus := (
		SELECT
			p.state
		FROM
			p2p p
		JOIN checks c ON
			p."Check" = c.id
		WHERE
			checkingpeer = checking_peer
			AND c.task = task_name
			AND c.peer = checked_peer
		ORDER BY
			p.id DESC
		LIMIT 1);
BEGIN
	IF (status = 'Start') THEN
		IF (last_state <> 'Start') OR (last_state IS NULL) THEN 
	    	INSERT INTO checks(id, peer, task, date)
	        VALUES((SELECT MAX(id) FROM checks) + 1, checked_peer, task_name, CURRENT_DATE);
	       
	        INSERT INTO p2p(id, "Check", checkingpeer, state, time)
	        VALUES(
	        (SELECT MAX(id) FROM p2p) + 1, 
	        (SELECT MAX(id) FROM checks WHERE peer = checked_peer AND task = task_name AND date = CURRENT_DATE),
	        checking_peer, status, p2p_time);
	    ELSE
	    	RAISE EXCEPTION 'Last p2p check (for task % by %) does not end correctly!', task_name, checked_peer;
	    END IF;
    ELSE
    	INSERT INTO p2p(id, "Check", checkingpeer, state, time)
        VALUES(
        (SELECT MAX(id) FROM p2p) + 1, 
        (SELECT MAX(id) FROM checks WHERE peer = checked_peer AND task = task_name AND date = CURRENT_DATE),
        checking_peer, status, p2p_time);
    END IF;
END;
$$;

-- Тестирование
CALL add_p2p('yngln', 'lostwill', 'C3_S21_StringPlus', 'Start', '12:37:00');
CALL add_p2p('yngln', 'lostwill', 'C3_S21_StringPlus', 'Start', '12:38:00');
CALL add_p2p('yngln', 'lostwill', 'C3_S21_StringPlus', 'Success', '12:39:00');


-- Процедура добавления проверки Verter'ом
CREATE OR REPLACE PROCEDURE add_verter(IN checked_peer varchar, IN task_name varchar, IN status CheckStatus, IN verter_time time)
LANGUAGE PLPGSQL
AS
$$
DECLARE
	last_check_id int := (
		SELECT
			p."Check"
		FROM
			checks c
		JOIN p2p p ON
			p."Check" = c.id
		WHERE
			c.peer = checked_peer
			AND c.task = task_name
			AND p.state = 'Success'
		ORDER BY
			c."date" DESC,
			p."time" DESC
		LIMIT 1);
BEGIN
	IF (last_check_id IS NULL) THEN
		RAISE EXCEPTION 'Last p2p check (for task % by %) does not end successfuly or does not exists!', task_name, checked_peer;
	ELSE 
		INSERT INTO verter(id, "Check", state, time)
	    VALUES((SELECT MAX(id) FROM verter) + 1, last_check_id, status, verter_time);
   END IF;
END;
$$;

-- Тестирование
CALL add_verter('yngln', 'C3_S21_StringPlus', 'Start', '12:37:00');
CALL add_verter('yngln', 'C3_S21_StringPlus', 'Failure', '12:37:00');
CALL add_verter('yngln', 'C2_SimpleBashUtils', 'Start', '12:37:00');


-- после добавления записи со статутом "начало" в таблицу P2P, изменяет соответствующую запись в таблице TransferredPoints
CREATE OR REPLACE FUNCTION fnc_trg_p2p_insert_audit()
  RETURNS TRIGGER 
  LANGUAGE PLPGSQL
  AS
$$
DECLARE 
	checked_peer varchar := (
		SELECT
			c.peer
		FROM checks c 
		JOIN p2p p
		ON p."Check" = c.id
		WHERE p."Check" = NEW."Check"
		AND p.state = 'Start'
	);
BEGIN
	IF NEW.state = 'Start' THEN
		UPDATE transferredpoints
		SET pointsamount = pointsamount + 1
		WHERE checkingpeer = NEW.checkingpeer
		AND checkedpeer = checked_peer;
		
		IF NOT FOUND THEN
			INSERT INTO transferredpoints(id, checkingpeer, checkedpeer, pointsamount)
			VALUES(
			(SELECT MAX(id) FROM transferredpoints) + 1,
			NEW.checkingpeer, checked_peer, 1);
		END IF;
	END IF;
	RETURN NEW;
END;
$$;

DROP TRIGGER IF EXISTS trg_p2p_insert_audit ON public.p2p;

CREATE TRIGGER trg_p2p_insert_audit
  AFTER INSERT
  ON p2p
  FOR EACH ROW
  EXECUTE PROCEDURE fnc_trg_p2p_insert_audit();

-- Тестирование
CALL add_p2p('lostwill', 'yngln', 'C5_s21_decimal', 'Start', '16:37:00');
CALL add_p2p('oakenshs', 'aboba', 'C3_S21_StringPlus', 'Start', '17:00:00');


-- перед добавлением записи в таблицу XP, проверяет корректность добавляемой записи
CREATE OR REPLACE FUNCTION fnc_trg_xp_insert_audit()
  RETURNS TRIGGER 
  LANGUAGE PLPGSQL
  AS
$$
DECLARE 
	max_xp int := (
		SELECT
			t.maxxp
		FROM checks c 
		JOIN tasks t
		ON t.title = c.task
		WHERE c.id = NEW."Check"
	);
	is_correct bool := (
		SELECT
			count(*)
		FROM
			checks c
		JOIN p2p p ON
			c.id = p."Check"
		LEFT JOIN verter v ON
			c.id = v."Check"
		WHERE
			c.id = NEW."Check"
			AND p.state = 'Success'
			AND (v.state = 'Success'
				OR v.state IS NULL)
	);
BEGIN
	IF NEW.xpamount > max_xp THEN
		RAISE EXCEPTION 'XP amount for this check (%) more than max XP for ths task (%)!', NEW.xpamount, max_xp;
	END IF;
	IF NOT is_correct THEN
		RAISE EXCEPTION 'Check with id = % does not end successfuly!', NEW."Check";
	END IF;
	RETURN NEW;
END;
$$;

DROP TRIGGER IF EXISTS trg_xp_insert_audit ON public.xp;

CREATE TRIGGER trg_xp_insert_audit
  BEFORE INSERT
  ON xp
  FOR EACH ROW
  EXECUTE PROCEDURE fnc_trg_xp_insert_audit();

-- Тестирование
INSERT INTO xp(id, "Check", xpamount)
	VALUES((SELECT MAX(id) FROM xp) + 1, 5, 320);
INSERT INTO xp(id, "Check", xpamount)
	VALUES((SELECT MAX(id) FROM xp) + 1, 6, 320);
INSERT INTO xp(id, "Check", xpamount)
	VALUES((SELECT MAX(id) FROM xp) + 1, 6, 290);
INSERT INTO xp(id, "Check", xpamount)
	VALUES((SELECT MAX(id) FROM xp) + 1, 7, 290);