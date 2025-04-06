# ArtHobby Backend

This project is the backend for the [ArtHobby application](https://arthobby-frontend.vercel.app/). It's built using modern C++, Drogon framework, and PostgreSQL for the database.

## Table of Contents

- [Prerequisites](#prerequisites)
- [Cloning the Repository](#cloning-the-repository)
- [Building the Project](#building-the-project)
- [Database Setup](#database-setup)
- [Environment Configuration](#environment-configuration)
- [Running the Application](#running-the-application)

## Prerequisites

Before you begin, ensure you have the following installed:

-   **C++ Compiler:** A C++17 compatible compiler (e.g., g++).
-   **CMake:** Version 3.10 or higher.
-   **PostgreSQL:** A running PostgreSQL server.
-   **Drogon:** The Drogon C++ web framework. Shipped as a submodule.
-   **jwt-cpp:** The JWT (JSON Web Tokens) library for C++. Shipped as a submodule.
-   **libpq-dev:** The PostgreSQL client library development headers.
-   **uuid-dev:** The UUID library development headers.

## Cloning the Repository

1.  Open your terminal and navigate to the directory where you want to clone the project.
2.  Run the following command:

    ```bash
    git clone --recursive --depth=1 --shallow-submodules https://github.com/1Kuso4ek1/arthobby-backend.git
    cd arthobby-backend
    ```

    **Note:** The `--depth=1 --shallow-submodules` flags are used for faster cloning. If you need to work with the Drogon submodule directly (e.g., switch branches), you might need to clone without these flags.

## Building the Project

1.  Create a build directory:

    ```bash
    mkdir build
    cd build
    ```

2.  Configure the project using CMake:

    ```bash
    cmake .. -DCMAKE_BUILD_TYPE=Release
    ```

3.  Build the project:

    ```bash
    make -j$(nproc)
    ```

    This will compile the project and create the `ArtHobby` executable in the `build` directory. `$(nproc)` uses all available CPU cores for faster compilation.

## Database Setup

1.  **Create a Database:**
    - Connect to your PostgreSQL server using `psql` or a GUI tool like pgAdmin.
    - Create a new user and database.
    - For example:
        ```sql
        CREATE USER your_db_user WITH PASSWORD 'your_db_password';
        CREATE DATABASE arthobby WITH OWNER your_db_user;
        GRANT ALL PRIVILEGES ON DATABASE arthobby TO your_db_user;
        ```
        - Replace `your_db_user`, `your_db_password`, and `arthobby` with your desired username, password, and database name.

2. **Create tables:**
    - You need to create tables in database.
    - In the `sql` folder you can find `create_tables.sql` file, which contains all the necessary queries.
    - Connect to your database using `psql` or pgAdmin:
        ```bash
        psql -U your_db_user -d arthobby
        ```
    - Run the `create_tables.sql` file:
        ```sql
        \i /path/to/this/project/sql/create_tables.sql
        ```
        - Replace `/path/to/this/project/sql/create_tables.sql` with the actual path to the file.
    
## Environment Configuration

1.  **Create an `env` file:**
    - In the root directory of the project, create a file named `env`.

2.  **Add Database Credentials:**
    - Open the `env` file and add the host, port, database name, username, and password, each on a new line:

    ```
    localhost
    5432
    arthobby
    your_db_user
    your_db_password
    ```

    - Replace `arthobby`, `your_db_user`, and `your_db_password` with your actual database name, username, and password.

3. *(optional)* **Set ENV_PATH:**
    - `ENV_PATH` is defined in `CMakeLists.txt` and points to `env` file.
    - If your env file is not in the root directory, you need to change this variable.

## Running the Application

1.  Navigate to the `build` directory:

    ```bash
    cd build
    ```

2.  Run the executable:

    ```bash
    ./ArtHobby
    ```

    This will start the Drogon web server.
