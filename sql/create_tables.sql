CREATE TABLE IF NOT EXISTS product(
    id SERIAL PRIMARY KEY,
    name VARCHAR(255),
    price INTEGER,
    filename VARCHAR(255),
    dateOfCreation TIMESTAMP,
    type VARCHAR(255),
    material VARCHAR(255),
    size VARCHAR(255),
    article VARCHAR(255),
    popularity INTEGER
);

CREATE TABLE IF NOT EXISTS "user"(
    id SERIAL PRIMARY KEY,
    username VARCHAR(255) NOT NULL,
    password VARCHAR(255) NOT NULL
);

CREATE TABLE IF NOT EXISTS cart(
    id SERIAL PRIMARY KEY,
    user_id INTEGER NOT NULL,
    product_id INTEGER NOT NULL,
    quantity INTEGER DEFAULT 1 CHECK (quantity > 0),

    FOREIGN KEY (user_id) REFERENCES "user"(id) ON DELETE CASCADE,
    FOREIGN KEY (product_id) REFERENCES product(id) ON DELETE CASCADE,

    UNIQUE(user_id, product_id)
);
