#!/bin/bash

set -ex

/etc/init.d/memcached start

#Experiment to see if this resolves the docker mysql start issue
find /var/lib/mysql/mysql -exec touch -c -a {} \;

# dump out the schema as it stands
/etc/init.d/mysql start

RETRIES=15
until mysqladmin ping --silent > /dev/null 2>&1 || [ $RETRIES -eq 0 ]; do
  echo "Waiting for mysql server to start, $((RETRIES)) remaining attempts..."
  RETRIES=$((RETRIES-=1))
  sleep 1
done

# Recreate the testing schema.
cat /var/source/tests/sql/mysql/create_test_database.sql | mysql phpunit

mysql -e 'SHOW DATABASES' && \
mysql -e 'SHOW TABLES' phpunit

# dump out the schema as it stands
/etc/init.d/postgresql start

RETRIES=15

until psql --user=zframework --host=localhost -d phpunit -c 'select 1' > /dev/null 2>&1 || [ $RETRIES -eq 0 ]; do
  echo "Waiting for postgres server to start, $((RETRIES)) remaining attempts..."
  RETRIES=$((RETRIES-=1))
  sleep 1
done

# Recreate the testing schema.
cat /var/source/tests/sql/postgresql/create_test_database.sql | psql --user=zframework --host=localhost phpunit 

echo '\d' | psql --user=zframework --host=localhost phpunit

# show the version of hhvm
hhvm --version

# swap to the source directory
cd /var/source

# setup the github token for use
if [ ! -z "$GITHUB_TOKEN" ]; then
  composer config -g github-oauth.github.com $GITHUB_TOKEN
fi

# make sure composer is configured correctly
composer validate --no-check-all --ansi

# run composer
composer update

if [ $? -ne 0 ]; then
  echo "Composer failed to update"
  exit $?
fi

# Dump the composer modules that were installed
echo "composer show"
composer show

# composer is available now run the vendor setup script
./bin/setup.sh

if [ $? -ne 0 ]; then
  echo "Dev environment failed to configure properly"
  exit $?
fi

echo "marking host as development"
mkdir -p /opt/zynga
touch /opt/zynga/is_development

echo '<?php if (!extension_loaded("pgsql")) { echo "no-pgsql\n"; exit(255); } echo "pgsql-available\n"; exit(0);' | hhvm --php

function debugMake() {
  echo "Make test failure"
  PHP_LOG=/var/source/tmp/php.log
  if [ -f "$PHP_LOG" ]; then
    cat $PHP_LOG
  else 
    echo "WARNING: no php log to work with $PHP_LOG"
  fi
  exit 255
}

trap 'debugMake' ERR
make test

echo "BUILD: OK"
exit 0

