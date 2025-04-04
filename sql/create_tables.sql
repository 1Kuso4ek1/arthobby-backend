CREATE TABLE IF NOT EXISTS product(
    id SERIAL PRIMARY KEY,
    name VARCHAR(255),
    price NUMERIC(12, 2),
    filename VARCHAR(255),
    dateOfCreation TIMESTAMP,
    type VARCHAR(255),
    material VARCHAR(255),
    size VARCHAR(255),
    article VARCHAR(255)
);
