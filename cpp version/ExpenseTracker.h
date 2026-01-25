/// <summary>
/// Header for ExpenseTracker
/// </summary>

#pragma once

#include <string>      
#include <vector>      
#include <map>         
#include <memory>      
#include "json.hpp"    // nlohmann/json library - https://github.com/nlohmann/json i use this library often so i thought it would be nice to include it

using json = nlohmann::json; // just for easier access

struct Date {
    int day;   
    int month; 
    int year;   

    // Constructor
    Date(int day = 1, int month = 1, int year = 2024);
    
    // Comparison operators for date sorting
    bool operator<(const Date& other) const; 
    bool operator>(const Date& other) const; 
    bool operator==(const Date& other) const;
    
    // Convert to string
    std::string ToString() const;
};

class Expense {
private:
    Date date;                   
    double amount;               
    std::string category;        
    std::string description;     

public:

    // Constructor
    Expense(const Date& date, double amount, const std::string& category, const std::string& description);

    // Getters
    Date GetDate() const;                   
    double GetAmount() const;               
    std::string GetCategory() const;        
    std::string GetDescription() const;     

    void Display() const;
    json ToJSON() const;

    // static because its easy to access with the scope
    static Expense FromJSON(const json& jsonObject);
};

// Main tracker application
class ExpenseTracker {
private:
    std::vector<std::unique_ptr<Expense>> expenses;

    bool IsDateInRange(const Date& date, const Date& start, const Date& end) const;

public:
    // Constructor
    ExpenseTracker();
    
    // Destructor
    ~ExpenseTracker();

    void AddExpense(const Date& date, double amount, const std::string& category, const std::string& description);
    void ViewAllExpenses() const;

    std::vector<const Expense*> FilterByDateRange(const Date& startDate, const Date& endDate) const;
    std::vector<const Expense*> FilterByCategory(const std::string& category) const;
    std::vector<const Expense*> SearchByDescription(const std::string& keyword) const;

    std::map<std::string, double> GetSummaryByCategory() const;
    std::map<std::string, double> GetSummaryByCategory(const Date& startDate, const Date& endDate) const;

    double GetTotalExpenses() const;
    double GetTotalExpenses(const Date& startDate, const Date& endDate) const;
    size_t GetExpenseCount() const;
    void DisplayExpenses(const std::vector<const Expense*>& filteredExpenses) const;
    bool DeleteExpense(size_t index);
    const Expense* GetExpenseAt(size_t index) const;

    bool SaveToJSON(const std::string& filename) const;
    bool LoadFromJSON(const std::string& filename);

};
