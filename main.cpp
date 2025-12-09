#include <iostream>
#include <vector>
#include <algorithm>
#include <string>

enum class OrderType {
    BUY,
    SELL
};

struct Order {
    private:
        int id;
        double price;
        int quantity;
        OrderType type;

    public:
        // Constructor
        Order(int i, double p, int q, OrderType t) 
            : id(i), price(p), quantity(q), type(t) {}

        // Getters
        int getId() const { return id; }
        double getPrice() const { return price; }
        int getQuantity() const { return quantity; }
        OrderType getType() const { return type; } 

        // Setters
        void setQuantity(int q) { quantity = q;}
};

class OrderBook {
    private:
        std::vector<Order> bids;
        std::vector<Order> asks;

    public:
        void addOrder(int id, double price, int quantity, OrderType type) {
            Order new_order(id, price, quantity, type);

            // 1. Handle BUY orders
            if (type == OrderType::BUY) {
                // Match with sellers offering <= price
                while (!asks.empty() && asks[0].getPrice() <= new_order.getPrice()) {
                    Order& seller = asks[0];
                    
                    // Calculate trade size
                    int trade_qty = std::min(new_order.getQuantity(), seller.getQuantity());

                    std::cout << ">>> Trade! " << trade_qty << " units @ " << seller.getPrice()
                              << " (Buyer: " << new_order.getId() << ", Seller: " << seller.getId() << ")" << std::endl;

                    // Update remaining quantities
                    new_order.setQuantity(new_order.getQuantity() - trade_qty);
                    seller.setQuantity(seller.getQuantity() - trade_qty);

                    // Remove seller if fully filled
                    if (seller.getQuantity() == 0) {
                        asks.erase(asks.begin());
                    }

                    // Stop if buy order is complete
                    if (new_order.getQuantity() == 0) {
                        return;
                    }
                } 
                
                // Add remaining order to book
                bids.push_back(new_order);  
                
                // Sort bids: Highest price first (Best Bid)
                std::sort(bids.begin(), bids.end(), [](const Order& a, const Order& b) {
                    return a.getPrice() > b.getPrice();
                });

            } else {
                // 2. Handle SELL orders
                
                // Match with buyers offering >= price
                // Note: bids[0] is the Best Bid (highest price)
                while (!bids.empty() && bids[0].getPrice() >= new_order.getPrice()) {
                    Order& buyer = bids[0]; 
                    
                    // Calculate trade size
                    int trade_qty = std::min(new_order.getQuantity(), buyer.getQuantity());

                    std::cout << ">>> Trade! " << trade_qty << " units @ " << buyer.getPrice() 
                              << " (Seller: " << new_order.getId() << ", Buyer: " << buyer.getId() << ")" << std::endl;

                    // Update remaining quantities
                    new_order.setQuantity(new_order.getQuantity() - trade_qty);
                    buyer.setQuantity(buyer.getQuantity() - trade_qty);

                    // Remove buyer if fully filled
                    if (buyer.getQuantity() == 0) {
                        bids.erase(bids.begin());
                    }

                    // Stop if sell order is complete
                    if (new_order.getQuantity() == 0) {
                        return;
                    }
                }

                // Add remaining order to book
                asks.push_back(new_order);
                
                // Sort asks: Lowest price first (Best Ask)
                std::sort(asks.begin(), asks.end(), [](const Order& a, const Order& b) {
                    return a.getPrice() < b.getPrice();
                });
            }
        }

        void printBook() const {
            std::cout << "\n--- ASKS (Sellers) ---" << std::endl;
            for (const auto& order : asks) {
                std::cout << "ID: " << order.getId() << "\t Price: " << order.getPrice() << "\t Qty: " << order.getQuantity() << std::endl;
            }

            std::cout << "----------------------" << std::endl;

            std::cout << "--- BIDS (Buyers) ---" << std::endl;
            for (const auto& order : bids) { 
                std::cout << "ID: " << order.getId() << "\t Price: " << order.getPrice() << "\t Qty: " << order.getQuantity() << std::endl;
            }
            std::cout << "----------------------\n" << std::endl;
        }
};

int main() {
    OrderBook book;

    std::cout << "=== PHASE 1: MARKET MAKERS (Building Liquidity) ===" << std::endl;
    // Setup initial Asks (Ladder)
    // ID 101: Sell 10 @ 100$
    // ID 102: Sell 20 @ 101$
    // ID 103: Sell 50 @ 102$
    book.addOrder(101, 100.0, 10, OrderType::SELL);
    book.addOrder(102, 101.0, 20, OrderType::SELL);
    book.addOrder(103, 102.0, 50, OrderType::SELL);

    // Setup initial Bids (Ladder)
    // ID 201: Buy 10 @ 98$
    // ID 202: Buy 20 @ 97$
    book.addOrder(201, 98.0, 10, OrderType::BUY);
    book.addOrder(202, 97.0, 20, OrderType::BUY);

    book.printBook();
    // Current Spread: 98.0 (Bid) - 100.0 (Ask)


    std::cout << "\n=== PHASE 2: THE WHALE BUYER (Market Sweep) ===" << std::endl;
    std::cout << "Scenario: Buyer 301 wants 35 units @ 102$." << std::endl;
    std::cout << "Expected: Takes 10 from ID 101, Takes 20 from ID 102, Takes 5 from ID 103." << std::endl;
    
    // Aggressive buy order, sweeps multiple levels
    book.addOrder(301, 102.0, 35, OrderType::BUY);

    book.printBook();


    std::cout << "\n=== PHASE 3: PANIC SELLING ===" << std::endl;
    std::cout << "Scenario: Seller 401 dumps 15 units @ 95$ (Market Order logic)." << std::endl;
    
    // Aggressive sell (effectively a Market Order), hits best bid
    book.addOrder(401, 95.0, 15, OrderType::SELL);

    book.printBook();

    return 0;
}