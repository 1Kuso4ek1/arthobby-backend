psql -U postgres -f sql/setup.sql -W
psql -U arthobby_admin -d schedule_db -f sql/create_tables.sql -W
