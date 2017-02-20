#!/bin/bash

git clone https://github.com/postgres/postgres.git
cd postgres
./configure  --prefix="/test"
make 
make install

