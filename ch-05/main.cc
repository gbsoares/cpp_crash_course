#include <cstdio>
#include <cstring>
#include <map>

/* Logger Interface */
struct Logger
{
    virtual ~Logger() = default;
    virtual void log_transfer(long from, long to, double amount) = 0;
};

struct ConsoleLogger : Logger
{
    ConsoleLogger(const char* tag) :
        tag {new char[strlen(tag) + 1]}
    {
        strcpy(this->tag, tag);
    }

    ~ConsoleLogger()
    {
        delete[] tag;
    }

    void log_transfer(long from, long to, double amount) override
    {
        printf("[%s] %ld -> %ld : $%.2f\n", tag, from, to, amount);
    }

    private:
        char* tag;
};

/* Account Database Interface */
struct AccountDatabase
{
    virtual ~AccountDatabase() = default;
    virtual void setAmount(long id, double amount) = 0;
    virtual double getAmount(long id) = 0;
};

struct InMemoryAccountDatabase : AccountDatabase
{
    void setAmount(long id, double amount) override
    {
        db[id] = amount;
        printf("[db set] acct %ld = $%.2f\n", id, amount);
    }

    double getAmount(long id) override
    {
        double ret = 0.0;
        if(db.find(id) != db.end())
        {
            ret = db[id];
            printf("[db get] acct %ld = $%.2f\n", id, ret);
        }
        return ret;
    }

    private:
        std::map<long, double> db {};
};

struct Bank
{
    Bank(Logger* logger, AccountDatabase& account_db) :
        logger{ logger },
        account_db{ account_db }
    {
    }

    void set_logger(Logger* new_logger)
    {
        logger = new_logger;
    }

    void make_transfer(long from, long to, double amount)
    {
        if (logger) 
            logger->log_transfer(from, to, amount);
        
        /* update database value for account */
        account_db.setAmount(to, account_db.getAmount(to) + amount);
    }

    private:
        Logger* logger;
        AccountDatabase& account_db;
};

int main(int argc, char ** arcv)
{
    ConsoleLogger console_logger("logger");
    InMemoryAccountDatabase im_acct_db;

    /* pass logger by property injection (ptr to logger) 
     * and account_database by construction injection (reference to db) */
    Bank bank {&console_logger, im_acct_db};

    bank.make_transfer(1000, 2000, 49.95);
    bank.make_transfer(500, 2000, 20.0);

    return 0;
}

