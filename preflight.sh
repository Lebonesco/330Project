#!/bin/bash

# set local variables

DB_NAME=metafiles
PORT=5432
password=password

cd sql
cp create_tables.sql $HOME/postgres-data
cd $HOME
ls
. postgres-data/config.sh
expect: "Password:" { send "$password" }

# remove teh db if it already exists
dropdb $DB_NAME

# create db
createdb $DB_NAME

# insert tables into database
psql $DB_NAME create_tables.sql
