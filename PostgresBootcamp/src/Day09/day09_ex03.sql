TRUNCATE TABLE person_audit;

DROP TRIGGER trg_person_insert_audit ON person;
DROP TRIGGER trg_person_update_audit ON person;
DROP TRIGGER trg_person_delete_audit ON person;

DROP FUNCTION fnc_trg_person_insert_audit;
DROP FUNCTION fnc_trg_person_update_audit;
DROP FUNCTION fnc_trg_person_delete_audit;


CREATE OR REPLACE FUNCTION fnc_trg_person_audit()
  RETURNS TRIGGER 
  LANGUAGE PLPGSQL
  AS
$$
BEGIN
	IF TG_OP = 'INSERT' THEN
		INSERT INTO person_audit
		 	VALUES (current_timestamp, 'I', NEW.id, NEW.name, NEW.age, NEW.gender, NEW.address);
	ELSIF TG_OP = 'UPDATE' THEN
		INSERT INTO person_audit
		 	VALUES (current_timestamp, 'U', OLD.id, OLD.name, OLD.age, OLD.gender, OLD.address);
	ELSIF TG_OP = 'DELETE' THEN
		INSERT INTO person_audit
		 	VALUES (current_timestamp, 'D', OLD.id, OLD.name, OLD.age, OLD.gender, OLD.address);
	END IF;

	RETURN NULL;
END;
$$;


-- Let’s merge all our logic to the one main trigger with the name trg_person_audit and a new corresponding trigger function fnc_trg_person_audit.
TRUNCATE TABLE person_audit;

DROP TRIGGER trg_person_insert_audit ON person;
DROP TRIGGER trg_person_update_audit ON person;
DROP TRIGGER trg_person_delete_audit ON person;

DROP FUNCTION fnc_trg_person_insert_audit;
DROP FUNCTION fnc_trg_person_update_audit;
DROP FUNCTION fnc_trg_person_delete_audit;


CREATE OR REPLACE FUNCTION fnc_trg_person_audit()
  RETURNS TRIGGER 
  LANGUAGE PLPGSQL
  AS
$$
BEGIN
	IF TG_OP = 'INSERT' THEN
		INSERT INTO person_audit
		 	VALUES (current_timestamp, 'I', NEW.id, NEW.name, NEW.age, NEW.gender, NEW.address);
	ELSIF TG_OP = 'UPDATE' THEN
		INSERT INTO person_audit
		 	VALUES (current_timestamp, 'U', OLD.id, OLD.name, OLD.age, OLD.gender, OLD.address);
	ELSIF TG_OP = 'DELETE' THEN
		INSERT INTO person_audit
		 	VALUES (current_timestamp, 'D', OLD.id, OLD.name, OLD.age, OLD.gender, OLD.address);
	END IF;

	RETURN NULL;
END;
$$;


CREATE TRIGGER trg_person_audit
  AFTER INSERT OR UPDATE OR DELETE
  ON person
  FOR EACH ROW
  EXECUTE PROCEDURE fnc_trg_person_audit();
 
 
INSERT INTO person(id, name, age, gender, address) 
	VALUES (10,'Damir', 22, 'male', 'Irkutsk');
UPDATE person SET name = 'Bulat' WHERE id = 10;
UPDATE person SET name = 'Damir' WHERE id = 10;
DELETE FROM person WHERE id = 10;