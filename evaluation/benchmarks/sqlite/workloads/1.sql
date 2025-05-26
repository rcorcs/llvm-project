-- 1. Create a table
CREATE TABLE users
(
    id    INTEGER PRIMARY KEY AUTOINCREMENT,
    name  TEXT NOT NULL,
    age   INTEGER,
    email TEXT UNIQUE
);

-- 2. Insert sample data
INSERT INTO users (name, age, email)
VALUES ('Alice', 30, 'alice@example.com'),
       ('Bob', 25, 'bob@example.com'),
       ('Charlie', 35, 'charlie@example.com');

-- 3. Query data
SELECT *
FROM users;

-- 4. Update a record
UPDATE users
SET age = 31
WHERE name = 'Alice';

-- 5. Query with condition
SELECT *
FROM users
WHERE age > 30;

-- 6. Delete a record
DELETE
FROM users
WHERE name = 'Bob';

-- 7. Count records
SELECT COUNT(*)
FROM users;

-- 8. Drop the table (cleanup)
DROP TABLE users;
