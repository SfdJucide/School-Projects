-- Cкрипт, создающий базу данных и таблицы, а также процедуры, 
-- позволяющие импортировать и экспортировать данные для каждой таблицы из файлов/в файлы с расширением .csv и .tsv. 
-- В качестве параметра каждой процедуры для импорта из csv файла указывается разделитель.
-- В каждую из таблиц внесится по 5 записей.



--Таблица Personal Data
CREATE TABLE IF NOT EXISTS "Personal_Data" (
	Customer_ID INT PRIMARY KEY,
	Customer_Name VARCHAR(255) NOT NULL,
	Customer_Surname VARCHAR(255) NOT NULL,
	Customer_Primary_Email VARCHAR(255) NOT NULL,
	Customer_Primary_Phone CHAR(12) NOT NULL,
	
	CONSTRAINT name_check CHECK (Customer_Name ~* '^[A-ZА-Я][a-zа-я -]+$'),
	CONSTRAINT surname_check CHECK (Customer_Surname ~* '^[A-ZА-Я][a-zа-я -]+$'),
	CONSTRAINT email_verify CHECK (Customer_Primary_Email ~* '^[A-Za-z0-9._+%-]+@[A-Za-z0-9.-]+[.][A-Za-z]+$'),
	CONSTRAINT phone_verify CHECK (Customer_Primary_Phone ~* '^[+][7][0-9]{10}$')
);


--Таблица Cards
CREATE TABLE IF NOT EXISTS "Cards" (
	Customer_Card_ID INT PRIMARY KEY,
	Customer_ID INT NOT NULL,
	
	CONSTRAINT fk_cards_customer_id FOREIGN KEY (Customer_ID) 
		REFERENCES "Personal_Data" (Customer_ID)
);


--Таблица Groups_SKU
CREATE TABLE IF NOT EXISTS "Groups_SKU" (
	Group_ID INT PRIMARY KEY,
	Group_Name VARCHAR(255) NOT NULL
);


--Таблица SKU
CREATE TABLE IF NOT EXISTS "SKU" (
	SKU_ID INT PRIMARY KEY,
	SKU_Name VARCHAR(255) NOT NULL,
	Group_ID INT NOT NULL,
	
	CONSTRAINT fk_sku_group_id FOREIGN KEY (Group_ID) 
		REFERENCES "Groups_SKU" (Group_ID)
);


--Таблица Stores
CREATE TABLE IF NOT EXISTS "Stores" (
	Transaction_Store_ID INT,
	SKU_ID INT,
	SKU_Purchase_Price DECIMAL(20,13) NOT NULL,
	SKU_Retail_Price DECIMAL(20,13) NOT NULL,
	
--	CONSTRAINT stores_pkey PRIMARY KEY (Transaction_Store_ID, SKU_ID),
	
	CONSTRAINT fk_stores_sku_id FOREIGN KEY (SKU_ID) 
		REFERENCES "SKU" (SKU_ID)
);


--Таблица Transactions
CREATE TABLE IF NOT EXISTS "Transactions" (
	Transaction_ID INT PRIMARY KEY,
	Customer_Card_ID INT NOT NULL,
	Transaction_Summ DECIMAL(20,13) NOT NULL,
	Transaction_DateTime TIMESTAMP NOT NULL,
	Transaction_Store_ID INT NOT NULL,
	
	CONSTRAINT fk_transactions_customer_card_id FOREIGN KEY (Customer_Card_ID) 
		REFERENCES "Cards" (Customer_Card_ID)
--	CONSTRAINT fk_transactions_transaction_store_id FOREIGN KEY (Transaction_Store_ID) 
--		REFERENCES Stores (Transaction_Store_ID)
);


--Таблица Checks
CREATE TABLE IF NOT EXISTS "Checks" (
	Transaction_ID INT,
	SKU_ID INT,
	SKU_Amount DECIMAL(20,13) NOT NULL,
	SKU_Summ DECIMAL(20,13) NOT NULL,
	SKU_Summ_Paid DECIMAL(20,13) NOT NULL,
	SKU_Discount DECIMAL(20,13) NOT NULL,
	
	CONSTRAINT checks_pkey PRIMARY KEY (Transaction_ID, SKU_ID),
	
	CONSTRAINT fk_checks_transaction_id FOREIGN KEY (Transaction_ID) 
		REFERENCES "Transactions" (Transaction_ID),
	CONSTRAINT fk_checks_sku_id FOREIGN KEY (SKU_ID) 
		REFERENCES "SKU" (SKU_ID)
);


--Таблица Date_Of_Analysis_Formation
CREATE TABLE IF NOT EXISTS "Date_Of_Analysis_Formation" (
	Analysis_Formation TIMESTAMP
);


--===================================IMPORT\EXPORT============================================--
-- Импорт данных
CREATE OR REPLACE PROCEDURE import_data(IN table_name varchar, IN delim varchar, IN path_name varchar)
LANGUAGE PLPGSQL
AS 
$$
BEGIN
	RAISE NOTICE 'import data to % table', table_name;
	EXECUTE format('COPY %I FROM %L DELIMITER %L CSV;', table_name, path_name || table_name || '.tsv', delim);
END;
$$;


-- Экспорт данных
CREATE OR REPLACE PROCEDURE export_data(IN delim varchar, IN path_name varchar)
LANGUAGE PLPGSQL
AS 
$$
DECLARE
    file_name varchar;
BEGIN
    FOR file_name IN SELECT table_name FROM information_schema.tables WHERE table_schema LIKE current_schema()
    LOOP
        EXECUTE format(
            'COPY %I TO %L DELIMITER %L CSV HEADER;', file_name, path_name || file_name || '.tsv', delim);
    END LOOP;
END;
$$;

-- Импорт данных
TRUNCATE "Personal_Data", "Cards", "Groups_SKU", "SKU", "Stores", "Transactions", "Checks", "Date_Of_Analysis_Formation" CASCADE;
CALL import_data('Personal_Data', '	', '../data/');
CALL import_data('Cards', '	', '../data/');
CALL import_data('Groups_SKU', '	', '../data/');
CALL import_data('SKU', '	', '../data/');
CALL import_data('Stores', '	', '../data/');
CALL import_data('Transactions', '	', '../data/');
CALL import_data('Checks', '	', '../data/');
CALL import_data('Date_Of_Analysis_Formation', '	', '../data/');

-- Экспорт данных
CALL export_data('	', '../out/');