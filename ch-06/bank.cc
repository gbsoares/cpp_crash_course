#include <cstdio>
#include <cstring>
#include <map>
#include <cstdarg>

/* Logger Interface */
struct Logger
{
    virtual ~Logger() = default;
    virtual void log_transfer(long from, long to, double amount) = 0;
    virtual void log_error(const char* fmt...) = 0;
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

    void log_error(const char* fmt, ...) override
    {
        va_list args;
        va_start(args, fmt);

        printf("[%s] Error: ", tag);
        vprintf(fmt, args);
        printf("\n");

        va_end(args);
    }

    private:
        char* tag;
};

/* Bank template */
template <typename T>
struct Bank
{
    Bank(Logger* logger) : logger{ logger }
    {
    }

    void set_logger(Logger* new_logger)
    {
        logger = new_logger;
    }

    void make_transfer(T& from, T& to, double amount)
    {
        if(from.withdraw(amount) > 0)
        {
            to.deposit(amount);
            if (logger) logger->log_transfer(from.get_id(), to.get_id(), amount);
        }
        else
        {
            if(logger) logger->log_error("not enough funds in %ld", from.get_id());
        } 
    }

    private:
        Logger* logger;
};

/* Account base class */
struct Account
{
    virtual ~Account() = default;

    Account(long id, double initial_amount) :
        id{ id },
        balance{ initial_amount }
    {
    }
    
    void deposit(double amount)
    {
        balance += amount;
        printf("Account %ld balance = $%.2f\n", id, balance);
    }

    double withdraw(double amount)
    {
        double withdrawn = 0;
        if(amount <= balance)
        {
            balance -= amount;
            withdrawn = amount;
        }
        printf("Account %ld balance = $%.2f\n", id, balance);
        return withdrawn;
    }

    long get_id()
    {
        return id;
    }
    
    protected:
        long id;
        double balance;
};

struct CheckingAccount : Account
{
    CheckingAccount(long id, double initial_amount) : Account(id, initial_amount)
    {
        printf("Creating Checkings Account (id %ld) w/ $%.2f\n", id, balance);
    }
};

struct SavingsAccount : Account
{
    SavingsAccount(long id, double initial_amount) : Account(id, initial_amount)
    {
        printf("Creating Savings Account (id %ld) w/ $%.2f\n", id, balance);
    }
};

int main(int argc, char ** arcv)
{
    ConsoleLogger console_logger("logger");
    Bank<Account> bank {&console_logger};

    CheckingAccount checkings {1000, 100};
    SavingsAccount savings {2000, 0};

    bank.make_transfer(checkings, savings, 62.0);
    bank.make_transfer(savings, checkings, 20.0);
    bank.make_transfer(savings, checkings, 50.0);

    return 0;
}

