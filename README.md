# Simple Order Book (C++)

A lightweight limit order book and matching engine implemented in C++.

## Overview
This project simulates a core component of a financial trading exchange. It matches Buy (Bid) and Sell (Ask) orders based on **Price-Time Priority**.

## Features
* **Order Management:** Supports Buy and Sell limit orders.
* **Matching Engine:** Automatically executes trades when bid price $\ge$ ask price.
* **Partial Fills:** Handles cases where order quantities do not match perfectly.
* **Visualization:** Prints the order book state (Bids/Asks) and real-time trade execution logs.

## How to Run
To compile and run the project using `g++`:

```bash
g++ main.cpp -o orderbook
./orderbook