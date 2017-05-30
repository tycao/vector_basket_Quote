#include <iostream>
#include <vector>
#include <memory>
using namespace std;

class Quote
{
public:
    Quote() = default;
    Quote(const string& book, double sale_price) :
        bookNo(book), price(sale_price) { cout << "Quote constructor is running..." << endl; }
    ~Quote() {cout << "Quote destructor is running..." <<endl; }
    std::string& isbn() const { cout << this->bookNo << endl; }
    virtual debug() const { cout << "ISBN=" << bookNo << " price=" << price << endl; }
    virtual net_price(std::size_t n) const { return n * price; }
private:
    std::string bookNo;
protected:
    double price = 0.0;
};

class Bulk_Quote : public Quote
{
public:
    Bulk_Quote() = default;
    Bulk_Quote(const string& book, double sale_price, std::size_t n, double dis_rate) :
        Quote(book, sale_price), quantity(n), discount(dis_rate) { cout << "Bulk_Quote constructor is running..." << endl; }
    ~Bulk_Quote() { cout << "Bulk_Quote destructor is running..." << endl; }
    virtual debug() const override { Quote::debug(); cout << "quantity=" << quantity << " discount=" << discount << endl; }
    virtual net_price(std::size_t n) const
    {
        if (n >= quantity)
            return n * (1 - discount) * price;
        else
            return n * price;
    }
private:
    std::size_t quantity = 0;
    double discount = 0.0;
};

vector<Quote> basket;
vector<shared_ptr<Quote>> basket2;

int main()
{
    Quote q(string("ISBN-6666-9999"), 10);
    Bulk_Quote b(string(), 10, 6, 0.5);
    Quote& qq(b);
    cout << qq.net_price(10) << endl;
    basket.push_back(q);
    basket.push_back(b);

    basket2.push_back(make_shared<Quote>(q));
    basket2.push_back(shared_ptr<Quote>(new Bulk_Quote(b)));
    basket2.push_back(make_shared<Bulk_Quote>(b));
    cout << "++++++++basket+++++++" << endl;
    for (vector<Quote>::iterator i = basket.begin(); i != basket.end(); ++i)
        cout << i->net_price(10) << endl;   //都是调用的Quote的net_price()

    cout << "+++++++basket2+++++++" << endl;
    for (vector<shared_ptr<Quote>>::iterator i = basket2.begin(); i != basket2.end(); ++i)
        cout << (*i)->net_price(10) << endl;    //根据动态类型，来确定到底调用哪个版本的虚函数net_price()
    return 0;
}
