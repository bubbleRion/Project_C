SELECT id FROM project;

CREATE TABLE washer (
    id INT PRIMARY KEY,
    washer1 BOOLEAN DEFAULT FALSE,
    washer2 BOOLEAN DEFAULT FALSE,
    washer3 BOOLEAN DEFAULT FALSE,
    washer4 BOOLEAN DEFAULT FALSE
);

INSERT INTO washer (id, washer1, washer2, washer3, washer4)
SELECT id, 
       CASE WHEN id % 4 = 0 THEN TRUE ELSE FALSE END AS washer1,
       CASE WHEN id % 4 = 1 THEN TRUE ELSE FALSE END AS washer2,
       CASE WHEN id % 4 = 2 THEN TRUE ELSE FALSE END AS washer3,
       CASE WHEN id % 4 = 3 THEN TRUE ELSE FALSE END AS washer4
FROM project;