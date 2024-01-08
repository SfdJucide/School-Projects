--Таблица Peers
CREATE TABLE IF NOT EXISTS Peers (
	Nickname VARCHAR (255) PRIMARY KEY,
	Birthday DATE
);

--Таблица Tasks
--Чтобы получить доступ к заданию, нужно выполнить задание, являющееся его условием входа.
--Для упрощения будем считать, что у каждого задания всего одно условие входа.
--В таблице должно быть одно задание, у которого нет условия входа (т.е. поле ParentTask равно null).
CREATE TABLE IF NOT EXISTS Tasks (
	Title VARCHAR (255) PRIMARY KEY,
	ParentTask VARCHAR (255),
	MaxXP INT NOT NULL,
	
	CONSTRAINT fk_tasks_parent_task FOREIGN KEY (ParentTask) 
		REFERENCES Tasks (Title)
);

--Статус проверки
--Start - начало проверки
--Success - успешное окончание проверки
--Failure - неудачное окончание проверки
DO $$
BEGIN
    IF NOT EXISTS (SELECT 1 FROM pg_type WHERE typname = 'checkstatus') THEN
        CREATE TYPE CheckStatus AS ENUM (
			'Start',
			'Success',
			'Failure'
		);
    END IF;
END$$;

--Таблица Checks
--Описывает проверку задания в целом. Проверка обязательно включает в себя один этап P2P и, возможно, этап Verter.
--Для упрощения будем считать, что пир ту пир и автотесты, относящиеся к одной проверке, всегда происходят в один день.
--Проверка считается успешной, если соответствующий P2P этап успешен, а этап Verter успешен, либо отсутствует.
--Проверка считается неуспешной, хоть один из этапов неуспешен.
--То есть проверки, в которых ещё не завершился этап P2P, или этап P2P успешен, но ещё не завершился этап Verter, не относятся ни к успешным, ни к неуспешным.
CREATE TABLE IF NOT EXISTS Checks (
	ID SERIAL PRIMARY KEY,
	Peer VARCHAR (255) NOT NULL,
	Task VARCHAR (255) NOT NULL,
	Date DATE NOT NULL,
	
	CONSTRAINT fk_checks_peer FOREIGN KEY (Peer) 
		REFERENCES Peers (Nickname),
	CONSTRAINT fk_checks_task FOREIGN KEY (Task) 
		REFERENCES Tasks (Title)
);

--Таблица XP
--За каждую успешную проверку пир, выполнивший задание, получает какое-то количество XP, отображаемое в этой таблице.
--Количество XP не может превышать максимальное доступное для проверяемой задачи.
--Первое поле этой таблицы может ссылаться только на успешные проверки.
CREATE TABLE IF NOT EXISTS XP (
	ID SERIAL PRIMARY KEY,
	"Check" INT NOT NULL,
	XPAmount INT NOT NULL,
	
	CONSTRAINT fk_xp_check FOREIGN KEY ("Check") 
		REFERENCES Checks (ID)
);

ALTER TABLE XP ADD CONSTRAINT xp_amount_check CHECK(XPAmount >= 0);

--Таблица Verter
--Каждая проверка Verter'ом состоит из 2-х записей в таблице: первая имеет статус начало, вторая - успех или неуспех. 
--Каждая проверка Verter'ом (т.е. обе записи, из которых она состоит) ссылается на проверку в таблице Checks, к которой она относится. 
--Проверка Verter'ом может ссылаться только на те проверки в таблице Checks, которые уже включают в себя успешную P2P проверку.
CREATE TABLE IF NOT EXISTS Verter (
	ID SERIAL PRIMARY KEY,
	"Check" INT NOT NULL,
	State CheckStatus NOT NULL,
	Time TIME NOT NULL,
	
	CONSTRAINT fk_verter_check FOREIGN KEY ("Check") 
		REFERENCES Checks (ID)
);

CREATE UNIQUE INDEX IF NOT EXISTS verter_unique_idx ON Verter("Check", State);

--Таблица P2P
--Каждая P2P проверка состоит из 2-х записей в таблице: первая имеет статус начало, вторая - успех или неуспех. 
--В таблице не может быть больше одной незавершенной P2P проверки, относящейся к конкретному заданию, пиру и проверяющему. 
--Каждая P2P проверка (т.е. обе записи, из которых она состоит) ссылается на проверку в таблице Checks, к которой она относится.
CREATE TABLE IF NOT EXISTS P2P (
	ID SERIAL PRIMARY KEY,
	"Check" INT NOT NULL,
	CheckingPeer VARCHAR (255) NOT NULL,
	State CheckStatus NOT NULL,
	Time TIME NOT NULL,
	
	CONSTRAINT fk_p2p_check FOREIGN KEY ("Check") 
		REFERENCES Checks (ID),
	CONSTRAINT fk_p2p_checking_peer FOREIGN KEY (CheckingPeer) 
		REFERENCES Peers (Nickname)
);

CREATE UNIQUE INDEX IF NOT EXISTS p2p_unique_idx ON P2P("Check", State);

--Таблица TransferredPoints
--При каждой P2P проверке проверяемый пир передаёт один пир поинт проверяющему.
--Эта таблица содержит все пары проверяемый-проверяющий и кол-во переданных пир поинтов, то есть,
--другими словами, количество P2P проверок указанного проверяемого пира, данным проверяющим.
CREATE TABLE IF NOT EXISTS TransferredPoints (
	ID SERIAL PRIMARY KEY,
	CheckingPeer VARCHAR (255) NOT NULL,
	CheckedPeer VARCHAR (255) NOT NULL,
	PointsAmount INT NOT NULL,
	
	CONSTRAINT fk_transferred_points_checking_peer FOREIGN KEY (CheckingPeer) 
		REFERENCES Peers (Nickname),
	CONSTRAINT fk_transferred_points_checked_peer FOREIGN KEY (CheckedPeer) 
		REFERENCES Peers (Nickname)
);

ALTER TABLE TransferredPoints ADD CONSTRAINT not_same_peer_check CHECK(CheckingPeer <> CheckedPeer);
CREATE UNIQUE INDEX IF NOT EXISTS transferred_points_unique_idx ON TransferredPoints(CheckingPeer, CheckedPeer);

--Таблица Friends
--Дружба взаимная, т.е. первый пир является другом второго, а второй -- другом первого.
CREATE TABLE IF NOT EXISTS Friends (
	ID SERIAL PRIMARY KEY,
	Peer1 VARCHAR (255) NOT NULL,
	Peer2 VARCHAR (255) NOT NULL,
	
	CONSTRAINT fk_friends_peer1 FOREIGN KEY (Peer1) 
		REFERENCES Peers (Nickname),
	CONSTRAINT fk_friends_peer2 FOREIGN KEY (Peer2) 
		REFERENCES Peers (Nickname)
);

ALTER TABLE Friends ADD CONSTRAINT not_same_peer_check CHECK(Peer1 <> Peer2);

--Таблица Recommendations
--Каждому может понравиться, как проходила P2P проверка у того или иного пира.
--Пир, указанный в поле Peer, рекомендует проходить P2P проверку у пира из поля RecommendedPeer.
--Каждый пир может рекомендовать как ни одного, так и сразу несколько проверяющих.
CREATE TABLE IF NOT EXISTS Recommendations (
	ID SERIAL PRIMARY KEY,
	Peer VARCHAR (255) NOT NULL,
	RecommendedPeer VARCHAR (255),
	
	CONSTRAINT fk_recommendations_peer FOREIGN KEY (Peer) 
		REFERENCES Peers (Nickname),
	CONSTRAINT fk_recommendations_recommended_peer FOREIGN KEY (RecommendedPeer) 
		REFERENCES Peers (Nickname)
);

ALTER TABLE Recommendations ADD CONSTRAINT not_same_peer_check CHECK(Peer <> RecommendedPeer);

--Таблица TimeTracking
--Данная таблица содержит информация о посещениях пирами кампуса.
--Когда пир входит в кампус, в таблицу добавляется запись с состоянием 1, когда покидает - с состоянием 2.
--В заданиях, относящихся к этой таблице, под действием "выходить" подразумеваются все покидания кампуса за день, кроме последнего.
--В течение одного дня должно быть одинаковое количество записей с состоянием 1 и состоянием 2 для каждого пира.
CREATE TABLE IF NOT EXISTS TimeTracking (
	ID SERIAL PRIMARY KEY,
	Peer VARCHAR (255) NOT NULL,
	Date DATE NOT NULL,
	Time TIME NOT NULL,
	State INT NOT NULL,
	
	CONSTRAINT fk_time_tracking_peer FOREIGN KEY (Peer) 
		REFERENCES Peers (Nickname)
);

ALTER TABLE TimeTracking ADD CONSTRAINT timetracking_status_check CHECK (State IN (1, 2));


-- Экспорт данных
CREATE OR REPLACE PROCEDURE export_data(IN delim varchar)
LANGUAGE PLPGSQL
AS 
$$
DECLARE
    path_name varchar := '/home/sfd/s21Projects/SQL2_Info21_v1.0-1/src/out/';
    file_name varchar;
BEGIN
    FOR file_name IN SELECT table_name FROM information_schema.tables WHERE table_schema LIKE current_schema()
    LOOP
        EXECUTE format(
            'COPY %I TO %L DELIMITER %L CSV HEADER;', file_name, path_name || file_name || '.csv', delim);
    END LOOP;
END;
$$;

-- Импорт данных
CREATE OR REPLACE PROCEDURE import_data(IN delim varchar)
LANGUAGE PLPGSQL
AS 
$$
DECLARE
    path_name varchar := '/home/sfd/s21Projects/SQL2_Info21_v1.0-1/src/csv/';
    file_name varchar;
BEGIN
	TRUNCATE Peers, Friends, TransferredPoints, Recommendations, TimeTracking, Tasks, Checks, P2P, Verter, XP CASCADE;

    FOR file_name IN SELECT table_name FROM information_schema.tables WHERE table_schema LIKE current_schema()
    LOOP
        EXECUTE format('COPY %I FROM %L DELIMITER %L CSV HEADER;', file_name, path_name || file_name || '.csv', delim);
    END LOOP;
END;
$$;


CALL import_data(';');

CALL export_data(';');
