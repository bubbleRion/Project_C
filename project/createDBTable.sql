CREATE DATABASE IF NOT EXISTS project;
GRANT ALL PRIVILEGES ON project.* TO 'bangme'@'localhost';


USE project;

CREATE TABLE User (
    id INT PRIMARY KEY,
    dormitoryroom VARCHAR(50) NOT NULL,
    name VARCHAR(50) NOT NULL,
    password VARCHAR(50) NOT NULL,
    phonenumber VARCHAR(50) UNIQUE NOT NULL
);