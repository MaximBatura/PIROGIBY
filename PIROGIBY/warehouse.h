#ifndef WAREHOUSE_H
#define WAREHOUSE_H

class Batch {
public:
	Batch(std::string expirationDate, int quantity, double pricePerUnit) : expirationDate(expirationDate), quantity(quantity), pricePerUnit(pricePerUnit) {}

	std::string getExpirationDate() { return expirationDate; }
	int getQuantity() { return quantity; };
	double getPricePerUnit() { return pricePerUnit; }

	void substractQuantity(int part) { quantity -= part; }
private:
	std::string expirationDate;
	int quantity;
	double pricePerUnit;
};

#endif // WAREHOUSE_H