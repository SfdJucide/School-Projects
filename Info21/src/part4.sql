-- База данных для тестирования процедур
CREATE TABLE IF NOT EXISTS "TableName_human" (
	Nickname VARCHAR (255) PRIMARY KEY,
	Birthday DATE
);


CREATE TABLE IF NOT EXISTS "TableName_tasks" (
	Title VARCHAR (255) PRIMARY KEY,
	ParentTask VARCHAR (255),
	MaxXP INT NOT NULL,
	
	CONSTRAINT fk_tb_tasks_parent_task FOREIGN KEY (ParentTask) 
		REFERENCES "TableName_tasks" (Title)
);


CREATE TABLE IF NOT EXISTS "Some_Checks" (
	ID SERIAL PRIMARY KEY,
	Peer VARCHAR (255) NOT NULL,
	Task VARCHAR (255) NOT NULL,
	Date DATE NOT NULL,
	
	CONSTRAINT fk_some_checks_peer FOREIGN KEY (Peer) 
		REFERENCES "TableName_human" (Nickname),
	CONSTRAINT fk_checks_task FOREIGN KEY (Task) 
		REFERENCES "TableName_tasks" (Title)
);


CREATE TABLE IF NOT EXISTS "Useless" (
	ID SERIAL PRIMARY KEY,
	Useless_name varchar NOT NULL
);


CREATE TABLE IF NOT EXISTS "Real_P2P" (
	ID SERIAL PRIMARY KEY,
	"Check" INT NOT NULL,
	CheckingPeer VARCHAR (255) NOT NULL,
	State bool NOT NULL,
	Time TIME NOT NULL,
	
	CONSTRAINT fk_real_p2p_check FOREIGN KEY ("Check") 
		REFERENCES "Some_Checks" (ID),
	CONSTRAINT fk_real_p2p_checking_peer FOREIGN KEY (CheckingPeer) 
		REFERENCES "TableName_human" (Nickname)
);


CREATE OR REPLACE FUNCTION some_add(num1 int, num2 int)
  RETURNS int
  LANGUAGE PLPGSQL
  AS
$$
BEGIN
	RETURN num1 + num2;
END;
$$;

CREATE OR REPLACE FUNCTION some_mul(bzzz float, muuuu float)
  RETURNS float
  LANGUAGE PLPGSQL
  AS
$$
BEGIN
	RETURN bzzz * muuuu;
END;
$$;

CREATE OR REPLACE FUNCTION some_row(noneed int)
  RETURNS TABLE(Something varchar)
  LANGUAGE SQL
  AS
$$
SELECT 89 AS Something;
$$;


CREATE OR REPLACE FUNCTION fnc_trg_useless()
  RETURNS TRIGGER 
  LANGUAGE PLPGSQL
  AS
$$
BEGIN
	RAISE EXCEPTION 'no way';
	RETURN NEW;
END;
$$;

DROP TRIGGER IF EXISTS fnc_trg_useless ON public."Useless";

CREATE TRIGGER trg_useless
  BEFORE INSERT
  ON "Useless"
  FOR EACH ROW
  EXECUTE PROCEDURE fnc_trg_useless();

------------------------------------------------TASKS-------------------------------------------------------------
-- 1) Процедура, которая, не уничтожая базу данных, уничтожает все те таблицы текущей базы данных, имена которых начинаются с фразы 'TableName'.
CREATE OR REPLACE PROCEDURE drop_tablenames_from_earth()
LANGUAGE PLPGSQL
AS
$$
DECLARE
    deleting_table varchar;
BEGIN
	FOR deleting_table IN SELECT table_name FROM information_schema.tables WHERE table_schema LIKE current_schema() AND table_name LIKE 'TableName%'
    LOOP
        EXECUTE format('DROP TABLE %I CASCADE;', deleting_table);
    END LOOP;
END;
$$;

-- Тестирование
CALL drop_tablenames_from_earth();


-- 2) Процедура, которая выводит список имен и параметров всех скалярных SQL функций пользователя в текущей базе данных.
CREATE OR REPLACE PROCEDURE show_scalar_funcs_info(OUT fnc_count int)
LANGUAGE PLPGSQL
AS
$$
DECLARE
	function_info RECORD;
BEGIN
	fnc_count := 0;

    FOR function_info IN (
        SELECT
			p.proname AS function_name,
			p.proargnames AS args
		FROM
			pg_proc AS p
		JOIN pg_namespace AS n ON
			p.pronamespace = n.oid
		JOIN pg_type pt ON
			pt.oid = p.prorettype
		WHERE
			n.nspname LIKE CURRENT_SCHEMA()
			AND p.proargtypes[0] > 0
			AND pt.typlen > 0
    )
    LOOP
       fnc_count := fnc_count + 1;
       RAISE NOTICE 'Function: %() with args:%;', function_info.function_name, function_info.args;
    END LOOP;
END;
$$;

-- Тестирование
DO $$
DECLARE
	total_functions int;
BEGIN
	CALL show_scalar_funcs_info(total_functions);
	RAISE NOTICE 'Total functions found: %', total_functions;
END;
$$;


-- 3) Процедура, которая уничтожает все SQL DML триггеры в текущей базе данных.
CREATE OR REPLACE PROCEDURE drop_triggers(OUT droped_triggers_count int)
LANGUAGE PLPGSQL
AS
$$
DECLARE
    trigger_info RECORD;
BEGIN
	droped_triggers_count := 0;

	FOR trigger_info IN SELECT trigger_name, event_object_table FROM information_schema.triggers WHERE trigger_schema LIKE current_schema()
    LOOP
        EXECUTE format('DROP TRIGGER %I ON %I;', trigger_info.trigger_name, trigger_info.event_object_table);
    	droped_triggers_count := droped_triggers_count + 1;
    END LOOP;
END;
$$;

-- Тестирование
DO $$
DECLARE
	total_triggers int;
BEGIN
	CALL drop_triggers(total_triggers);
	RAISE NOTICE 'Total triggers destroyed: %', total_triggers;
END;
$$;


-- 4) Процедура, которая выводит имена и описания типа объектов, в тексте которых на языке SQL встречается строка, задаваемая параметром процедуры.
CREATE OR REPLACE PROCEDURE find_str(IN search_string varchar)
LANGUAGE PLPGSQL
AS
$$
DECLARE
    object_info RECORD;
BEGIN
    FOR object_info IN (
        SELECT
			p.proname AS obj_name,
			obj_description(pt.oid) AS description
		FROM
			pg_proc AS p
		JOIN pg_namespace AS n ON
			p.pronamespace = n.oid
		JOIN pg_type pt ON
			pt.oid = p.prorettype
		WHERE
			n.nspname LIKE CURRENT_SCHEMA()
			AND pt.typlen > 0
			AND POSITION(search_string IN p.prosrc) != 0
    )
    LOOP
        RAISE NOTICE 'Object Name: %, Description: %', object_info.obj_name, object_info.description;
    END LOOP;
END;
$$;

-- Тестирование
CALL find_str('+');
CALL find_str('RE');