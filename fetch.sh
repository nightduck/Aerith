#!/bin/bash

##Open tickers.txt and compile URL
read -r TICKERS<"/home/pi/aerith/tickers.txt"

echo "Polling yahoo..."

##Compile URL and wget that shit
URL="http://finance.yahoo.com/d/quotes.csv?s=${TICKERS}&f=sb"
wget -q -O ./quotes.csv $URL

##Output contents of file
echo "Data received:"
cat ./quotes.csv

##Schedule self to be run again in a minute
at now +1 minutes -M -f ./fetch.sh
