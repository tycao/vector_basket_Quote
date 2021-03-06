/*
* 本文件是对base_child2.cpp的重构，优化了Basket类的add_item()成员函数。
*/
#include <iostream>
#include <vector>
#include <memory>
#include <set>
#include <algorithm>
using namespace std;

class Quote
{
public:
    virtual Quote* clone() const & { return new Quote(*this); }
    virtual Quote* clone() && { return new Quote(std::move(*this)); }
    Quote() = default;
    Quote(const string& book, double sale_price) :
        bookNo(book), price(sale_price) { cout << "Quote constructor is running..." << endl; }
    virtual ~Quote() {cout << "Quote destructor is running..." <<endl; }
    const std::string& isbn() const { return this->bookNo; }
    virtual debug() const { cout << "ISBN=" << bookNo << " price=" << price << endl; }
    virtual net_price(std::size_t n) const { return n * price; }
private:
    std::string bookNo;
protected:
    double price = 0.0;
};

double print_total(ostream& os, const Quote& q, std::size_t n)
{
    double ret = q.net_price(n);
    os << "ISBN=" << q.isbn() << " sold=" << n << " total due=" << ret << endl;
    return ret;
}
class Bulk_Quote : public Quote
{
public:
    Bulk_Quote() = default;
    virtual Bulk_Quote* clone() const & override { cout << "Bulk_Quote* clone() const & 先被调用。" << endl; return new Bulk_Quote(*this); }
    virtual Bulk_Quote* clone() && override { cout << "Bulk_Quote* clone() && 先被调用。" << endl; return new Bulk_Quote(std::move(*this)); }
    Bulk_Quote(const string& book, double sale_price, std::size_t n, double dis_rate) :
        Quote(book, sale_price), quantity(n), discount(dis_rate) { cout << "Bulk_Quote constructor is running..." << endl; }
    virtual ~Bulk_Quote() { cout << "Bulk_Quote destructor is running..." << endl; }
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

class Basket
{
public:
    Basket() = default;
    virtual ~Basket() = default;
    void add_item(const std::shared_ptr<Quote>& sale) { items.insert(sale); }
    void add_item(const Quote& sale) { items.insert(std::shared_ptr<Quote>(sale.clone())); cout << "add_item(const Quote&)被调用。" << endl; }
    void add_item(Quote&& sale) { items.insert(std::shared_ptr<Quote>(std::move(sale).clone())); cout << "add_item(Quote&&)被调用。" << endl; }
    double total_receipt(std::ostream&) const;
private:
    static bool compare(const std::shared_ptr<Quote> &lhs, const std::shared_ptr<Quote> &rhs)
    {
        return lhs->isbn() < rhs->isbn();
    }
    std::multiset<std::shared_ptr<Quote>, decltype(compare)*> items{compare};
};
double Basket::total_receipt(std::ostream& os) const
{
    double sum = 0.0;
    for (std::multiset<std::shared_ptr<Quote>, decltype(compare)*>::iterator iter = items.cbegin(); iter != items.end(); iter = items.upper_bound(*iter))
    {
        sum += print_total(os, **iter, items.count(*iter));
    }
    os << "Total sum=" << sum << endl;
    return sum;
}

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
        cout << (*i).net_price(10) << endl;   //都是调用的Quote的net_price()

    cout << "+++++++basket2+++++++" << endl;
    for (vector<shared_ptr<Quote>>::iterator i = basket2.begin(); i != basket2.end(); ++i)
        cout << (*i)->net_price(10) << endl;    //根据动态类型，来确定到底调用哪个版本的虚函数net_price()

    cout << "++++++++++++++++++++++++++++++++++分割线++++++++++++++++++++++++++++++++++" << endl;
    Basket bk;
    Quote qqq("ISBN-99999-66666", 10);
    Bulk_Quote bbb("ISBN-99999-88888", 10, 11, 0.8);
    bk.add_item(make_shared<Quote>(qqq));
    bk.add_item(shared_ptr<Quote>(new Quote(qqq)));
    bk.add_item(make_shared<Bulk_Quote>(bbb));
    bk.add_item(shared_ptr<Quote>(new Bulk_Quote(bbb)));
    bk.total_receipt(cout);

    cout << "++++++++++++++++++++++++++++++++++++分割线+++++++++++++++++++++++++++++++++" << endl;
    Basket bk2;
    Quote qqqq("IS-999999999999-6", 10);
    Bulk_Quote bbbb("IS-99999-66666666", 10, 11, 0.8);
    bk2.add_item(qqqq);
    bk2.add_item(bbbb);
    bk2.total_receipt(cout);
    bk2.add_item(std::move(qqqq));
    bk2.add_item(std::move(bbbb));
    bk2.total_receipt(cout);



    return 0;
}
