-- Cоздание ролей и выдачf им прав в соответствии с описанным ниже.

-- Администратор
-- Администратор имеет полные права на редактирование и просмотр
-- любой информации, запуск и остановку процесса обработки.

-- Посетитель
-- Только просмотр информации из всех таблиц.


REVOKE ALL PRIVILEGES ON DATABASE postgres FROM administrator;
REVOKE ALL PRIVILEGES ON ALL TABLES IN SCHEMA public FROM administrator;
DROP ROLE administrator;

CREATE ROLE administrator SUPERUSER;
GRANT ALL PRIVILEGES ON DATABASE postgres TO administrator;
GRANT ALL PRIVILEGES ON ALL TABLES IN SCHEMA public TO administrator;


REVOKE SELECT ON ALL TABLES IN SCHEMA public FROM guest;
DROP ROLE guest;

CREATE ROLE guest;
GRANT SELECT ON ALL TABLES IN SCHEMA public TO guest;