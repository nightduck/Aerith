//Aerith v1.0 - sendData
//	@author nightduck
//
//	This program handles opening the quotes.csv file and compiling the data
//	as well as controlling the radio and sending that data


#include <iostream>
#include <fstream>
#include <string>
#include <wiringPiSPI.h>
#include <wiringPi.h>
#include <nrf_API.h>
#include <unistd.h>
#include <iomanip>
using namespace std;

void flushTxFIFO() {
	digitalWrite(CSN, 0);

	unsigned char flush = 0xE1;

	wiringPiSPIDataRW(0, &flush, 1);

	digitalWrite(CSN, 1);
}

void nrfWrite(unsigned char addr, unsigned char data) {
	digitalWrite(CSN, 0);

	//Send address byte
	wiringPiSPIDataRW(0, &addr, 1);

	//Write data byte
	wiringPiSPIDataRW(0, &data, 1);


	digitalWrite(CSN, 1);
}

char nrfRead(unsigned char addr) {
	digitalWrite(CSN, 0);

	//Send the address byte
	wiringPiSPIDataRW(0, &addr, 1);

	//Receive byte back
	unsigned char x;
	wiringPiSPIDataRW(0, &x, 1);

	digitalWrite(CSN, 1);

	return x;
}

void nrfRWMulti(unsigned char addr, unsigned char * data, int len) {
	digitalWrite(CSN, 0);

	//Send address
	wiringPiSPIDataRW(0, &addr, 1);

	//Write data
	wiringPiSPIDataRW(0, data, len);

	digitalWrite(CSN, 1);
}

//Line is formatted like "TICKER",###.##
//This method extract the substring in between quotes
//Returns an empty string if it detects the line isn't formatted correctly
string getTicker(string &line) {
	int end = 1;

	while (line[end] != '"') end++;

	if (1 >= line.size() || end-1 >= line.size()) return "";
	else return line.substr(1, end - 1);
}

//Line is formatted like "TICKER",###.##
//This finds the comma and gets the number after it, rounding to 2 decimal places
//Returns an empty string if it detects the line isn't formatted correctly
string getPrice(string &line) {
	int start = 1;

	while (line[start] != ',') start++;

	int end = start + 1;
	while (line[end] != '.') end++;

	if (start+1 >= line.size() || end+2-start >= line.size()) return "";
	else return line.substr(start+1, end + 2 - start);
}

//Returns true if line is formatted correctly
bool lineValid(string &line) {
	if (line[0] != '"') return false;
	int i = 1;
}

//Adds spacing to str so it matches length len
string addSpacing(string &str, int len) {
	if (len <= str.size()) return str;
	str.append(string(len - str.size(), ' '));
	return str;
}

void init() {
	wiringPiSetupGpio();
	digitalWrite(CSN, HIGH);

	int fd = wiringPiSPISetup(0,500000);

	flushTxFIFO();
	nrfWrite(NRF_WRITE_SETUP_RETR, 0xFF);	//Set number of retransmissions to 15 with 4ms delay
	nrfWrite(NRF_WRITE_RF_CH, 0x3C); 	//Broadcast on channel 0xBC
	nrfWrite(NRF_WRITE_RF_SETUP, 0x26);	//Set data rate to 250kbps at MAXIMUM POWER!!
	nrfWrite(NRF_WRITE_PW_P0, 0x20);	//Set payload size to 32
	nrfWrite(NRF_WRITE_EN_AA, 0xFF);	//Enable auto-acknowledge on all pipes
	nrfWrite(NRF_WRITE_EN_RXADDR, 0x01);	//Enable pipe 0 to receive
	nrfWrite(NRF_WRITE_SETUP_AW, 0x03);	//Set address size to 5 bytes
	nrfWrite(NRF_WRITE_STATUS, 0xFF);	//Clear status register

	unsigned char buffer[5];
	for(int i = 0; i < 5; i++) buffer[i] = 0xF0;
	nrfRWMulti(NRF_WRITE_TX_ADDR, buffer, 5); 

	for(int i = 0; i < 5; i++) buffer[i] = 0xF0;
	nrfRWMulti(NRF_WRITE_RX_ADDR_P0, buffer, 5); 

	nrfWrite(NRF_WRITE_CONFIG, 0x0E);	//Use 2-byte CRCs, power on radio, and put in transmit mode
}

int main() {
	string tickerString;
	string priceString;
	string tickers[50];
	string prices[50];
	int i;
	string line;

	init();

	while(1) {
		tickerString = "";
		priceString = "";
		i = 0;

		//Load quotes.csv into tickers and prices arrays
		ifstream quotes;

		bool readFile = true;	//Do this so it
		while(readFile) {
			try {
				readFile = false;	//Turnoff flag
				quotes.open("quotes.csv");
				while (getline(quotes, line)) {
					if (lineValid(line)) {
						tickers[i] = getTicker(line);
						prices[i] = getPrice(line);
						i++;
					}
				}
				tickers[i] = "";
				prices[i] = "";
				quotes.close();
			} catch (int e) {
				cout << "File unavailable" << endl;
				readFile = true;
				usleep(10000);
			}
		}

		i = 0;

		while (tickers[i].size() != 0) {
			//Set max to to match the length of the ticker, or the bid (whichever is higher) plus one for spacing
			int max = 1 + ((tickers[i].size() > prices[i].size()) ? tickers[i].size() : prices[i].size());

			tickerString.append(addSpacing(tickers[i], max));
			priceString.append(addSpacing(prices[i], max));

			i++;
		}


		cout << endl << "Sending..." << endl;

		for(int i = 0; i < 7; i++)
			cout << "                              |";
		cout << endl;

		cout << tickerString << endl;
		cout << priceString << endl;


		unsigned char buffer[32];

		nrfWrite(NRF_WRITE_STATUS, 0xFF);

		//Write the ticker string
		bool fail;
		for(int i = 0; i < 8; i++) {
			cout << "Writing ticker string block " << i << " - ";

			do {
				fail = false;
				buffer[0] = i;
				for(int j = 1; j < 32; j++) buffer[j] = (i*31+j-1 >= tickerString.size()) ? 0 : tickerString.at(i*31+j-1);
				nrfRWMulti(NRF_WRITE_PAYLOAD, buffer, 32);

				while(nrfRead(NRF_READ_STATUS) <= 0x0F);	//Stall until a read is successful or terminated

				if (nrfRead(NRF_READ_STATUS) & 0x10) {
					fail = true;
					cout << "Packet failed" << endl << "Resending...";
					nrfWrite(NRF_WRITE_STATUS, 0xFF);
					flushTxFIFO();
					usleep(62500);
				}
			} while (fail);

			//DEBUGGING
			if (nrfRead(NRF_READ_STATUS) & 0x20)
				cout << "Packet successful" << endl;

			nrfWrite(NRF_WRITE_STATUS, 0xFF);
			flushTxFIFO();
			usleep(62500);
		}

		//Write the prices string
		for(int i = 0; i < 8; i++) {
			cout << "Writing prices string block " << i << " - ";

			do {
				fail = false;
				buffer[0] = i + 8;
				for(int j = 1; j < 32; j++) buffer[j] = (i*31+j-1 >= priceString.size()) ? 0 : priceString.at(i*31+j-1);
				nrfRWMulti(NRF_WRITE_PAYLOAD, buffer, 32);

				while(nrfRead(NRF_READ_STATUS) <= 0x0F);	//Stall until a read is successful or terminated

				if (nrfRead(NRF_READ_STATUS) & 0x10) {
					fail = true;
					cout << "Packet failed" << endl << "Resending...";
					nrfWrite(NRF_WRITE_STATUS, 0xFF);
					flushTxFIFO();
					usleep(62500);
				}
			} while (fail);

			if (nrfRead(NRF_READ_STATUS) & 0x20)
				cout << "Packet successful" << endl;

			nrfWrite(NRF_WRITE_STATUS, 0xFF);
			flushTxFIFO();
			usleep(62500);
		}
		cout << "-------------------------------------------------------" << endl;
		sleep(32);
	}

}
