-- We want to be stronger with data and don’t want to lose any event of changes. Let’s implement an audit feature for INSERT’s incoming changes.
-- Please create a table person_audit with the same structure like a person table but please add a few additional changes.
DROP TABLE IF EXISTS person_audit;

CREATE TABLE person_audit 
(
	created timestamp with time zone DEFAULT current_timestamp NOT NULL,
	type_event char(1) DEFAULT 'I' NOT NULL,
	row_id bigint NOT NULL,
	name varchar,
	age integer,
	gender varchar,
	address varchar
);

ALTER TABLE person_audit
ADD CONSTRAINT ch_type_event CHECK (type_event IN ('I', 'D', 'U'));


CREATE OR REPLACE FUNCTION fnc_trg_person_insert_audit()
  RETURNS TRIGGER 
  LANGUAGE PLPGSQL
  AS
$$
BEGIN
	IF TG_OP = 'INSERT' THEN
		INSERT INTO person_audit
		 	VALUES (current_timestamp, 'I', NEW.id, NEW.name, NEW.age, NEW.gender, NEW.address);
	END IF;

	RETURN NULL;
END;
$$;


CREATE TRIGGER trg_person_insert_audit
  AFTER INSERT
  ON person
  FOR EACH ROW
  EXECUTE PROCEDURE fnc_trg_person_insert_audit();
 
 
INSERT INTO person(id, name, age, gender, address) 
	VALUES (10, 'Damir', 22, 'male', 'Irkutsk');
