CREATE DATABASE IF NOT EXISTS project;
USE project;
CREATE TABLE User (
    id INT AUTO_INCREMENT PRIMARY KEY,
    dormitoryroom VARCHAR(50) UNIQUE NOT NULL
);

GRANT ALL PRIVILEGES ON project.* TO 'bangme'@'localhost';

CREATE DATABASE IF NOT EXISTS washer;
USE washer;

CREATE TABLE IF NOT EXISTS washer (
    id INT AUTO_INCREMENT PRIMARY KEY,
    is_working BOOLEAN DEFAULT 0 NOT NULL,
    waitingnumber VARCHAR(50) NOT NULL,
    dormitoryroom VARCHAR(50) NULL,
    phonenumber VARCHAR(50) NULL,
    date DATE NOT NULL,
    FOREIGN KEY (dormitoryroom) REFERENCES project.User(dormitoryroom)
);

-- test
--INSERT INTO washer (waintgnumber, --dormitoryroom, date)
--VALUES ('대기1번', '101', '2025-03-28');

-- 세탁기 작동 시작
--UPDATE washer
--SET is_working = 1
--WHERE id = 1;

-- 세탁기 작동 완료
--UPDATE washer
--SET is_working = 0
--WHERE id = 1;

use washer;
drop table washer;

