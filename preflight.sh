#!/bin/bash

# set local variables

DB_NAME=metafiles
PORT=5432

# remove teh db if it already exists
drop -p $PORT $DB_NAME

# create db
createdb -p $PORT $DB_NAME

# insert tables into database
cd sql
psql $DB_NAME -f create_tables.sql
