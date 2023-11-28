-- Please create a trigger trg_person_delete_audit and corresponding trigger function fnc_trg_person_delete_audit.
CREATE OR REPLACE FUNCTION fnc_trg_person_delete_audit()
  RETURNS TRIGGER 
  LANGUAGE PLPGSQL
  AS
$$
BEGIN
	IF TG_OP = 'DELETE' THEN
		INSERT INTO person_audit
		 	VALUES (current_timestamp, 'D', OLD.id, OLD.name, OLD.age, OLD.gender, OLD.address);
	END IF;

	RETURN NULL;
END;
$$;


CREATE TRIGGER trg_person_delete_audit
  AFTER DELETE
  ON person
  FOR EACH ROW
  EXECUTE PROCEDURE fnc_trg_person_delete_audit();
 
 
DELETE FROM person WHERE id = 10;

  