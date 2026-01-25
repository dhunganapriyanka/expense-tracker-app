/// <summary>
/// Implementation file for Expense Tracker
/// </summary>

#include "ExpenseTracker.h"
#include <iostream>   
#include <iomanip>   
#include <algorithm> 
#include <sstream>   
#include <fstream>   

/// <summary>
/// Date struct
/// </summary>
/// <param name="d">day</param>
/// <param name="m">month</param>
/// <param name="y">year</param>
Date::Date(int day, int month, int year) : day(day), month(month), year(year)
{
}

// operator overloads for comparison
bool Date::operator<(const Date& other) const
{
    if (year != other.year) return year < other.year;
    if (month != other.month) return month < other.month;

    return day < other.day;
}

bool Date::operator>(const Date& other) const
{
    return other < *this;  // Reuse less-than operator
}

bool Date::operator==(const Date& other) const
{
    return day == other.day && month == other.month && year == other.year;
}

/// <summary>
/// ToString for date
/// </summary>
/// <returns>string for the date to put in cout</returns>
std::string Date::ToString() const
{
    std::ostringstream oss; 
    
    // Format day with zero-padding like "05" instead of "5"
    oss << std::setfill('0') << std::setw(2) << day << "/"
        << std::setw(2) << month << "/"
        << year;
    
    return oss.str();
}

Expense::Expense(const Date& date, double amount, const std::string& category, const std::string& description)
    : date(date), amount(amount), category(category), description(description)
{
}

// Getters
Date Expense::GetDate() const
{
    return date;
}

double Expense::GetAmount() const
{
    return amount;
}

std::string Expense::GetCategory() const
{
    return category;
}

std::string Expense::GetDescription() const
{
    return description;
}

/// <summary>
/// Display it with proper padding and width and precision
/// </summary>
void Expense::Display() const
{
    std::cout << std::left                    
              << std::setw(12) << date.ToString()      
              << std::setw(12) << std::fixed           
              << std::setprecision(2) << amount       
              << std::setw(20) << category             
              << description                           
              << std::endl;
}

/// <summary>
/// Helper to just convert the variables to the json library i am using
/// </summary>
/// <returns>JSON object</returns>
json Expense::ToJSON() const
{
    json jsonObject;  // Create empty JSON object
    
    // Build nested date object
    jsonObject["date"]["day"] = date.day;
    jsonObject["date"]["month"] = date.month;
    jsonObject["date"]["year"] = date.year;

    jsonObject["amount"] = amount;
    jsonObject["category"] = category;
    jsonObject["description"] = description;
    
    return jsonObject;
}

/// <summary>
/// Get JSON from existing data
/// </summary>
/// <param name="jsonObject">JSON object</param>
/// <returns>Expense object created from JSON</returns>
Expense Expense::FromJSON(const json& jsonObject)
{
    Date date(
        jsonObject["date"]["day"].get<int>(),      
        jsonObject["date"]["month"].get<int>(),    
        jsonObject["date"]["year"].get<int>()      
    );
    
    double amount = jsonObject["amount"].get<double>();
    std::string category = jsonObject["category"].get<std::string>();
    std::string description = jsonObject["description"].get<std::string>();
    
    return Expense(date, amount, category, description);
}

ExpenseTracker::ExpenseTracker() {}

ExpenseTracker::~ExpenseTracker()
{
    expenses.clear();
}

/// <summary>
/// Add a new expense to the tracker
/// </summary>
/// <param name="date">Date of the expense</param>
/// <param name="amount">Amount of the expense</param>
/// <param name="category">Category of the expense</param>
/// <param name="description">Description of the expense</param>
void ExpenseTracker::AddExpense(const Date& date, double amount, const std::string& category, const std::string& description)
{
    // make_unique creates a unique_ptr to a new Expense object
    // This ensures automatic memory management
    expenses.push_back(std::make_unique<Expense>(date, amount, category, description));
}

/// <summary>
/// Display all expenses in a formatted table
/// </summary>
/// <remarks>Shows date, amount, category, and description for each expense</remarks>
void ExpenseTracker::ViewAllExpenses() const
{
    // Check if there are any expenses
    if (expenses.empty())
    {
        std::cout << "No expenses recorded yet." << std::endl;
        return;
    }

    // Print table header
    std::cout << "\n=== All Expenses ===" << std::endl;
    std::cout << std::left 
              << std::setw(12) << "Date"
              << std::setw(12) << "Amount"
              << std::setw(20) << "Category"
              << "Description" << std::endl;
    std::cout << std::string(60, '-') << std::endl;  // Separator line

    // Display each expense
    for (const auto& expense : expenses)
    {
        expense->Display();  // Call Display method on each expense
    }
    std::cout << std::endl;
}

/// <summary>
/// Check if a date falls within a given range (inclusive)
/// </summary>
/// <param name="date">Date to check</param>
/// <param name="start">Start date of range</param>
/// <param name="end">End date of range</param>
/// <returns>true if date >= start AND date <= end</returns>
bool ExpenseTracker::IsDateInRange(const Date& date, const Date& start, const Date& end) const
{
    // Date is in range if it's not before start AND not after end
    return !(date < start) && !(date > end);
}

/// <summary>
/// Filter expenses within a date range. Inclusive range
/// </summary>
/// <param name="startDate">Start date of range</param>
/// <param name="endDate">End date of range</param>
/// <returns>Vector of pointers expenses</returns>
std::vector<const Expense*> ExpenseTracker::FilterByDateRange(const Date& startDate, const Date& endDate) const
{
    std::vector<const Expense*> filtered;  // Vector to store matching expenses

    // Iterate through all expenses
    for (const auto& expense : expenses)
    {
        // Check if expense date is within range
        if (IsDateInRange(expense->GetDate(), startDate, endDate))
        {
            // Add pointer to expense (using .get() to get raw pointer from unique_ptr)
            filtered.push_back(expense.get());
        }
    }

    return filtered;
}

/// <summary>
/// Filter expenses by category
/// </summary>
/// <param name="category">Category name to filter</param>
/// <returns>Vector of pointers expenses</returns>
std::vector<const Expense*> ExpenseTracker::FilterByCategory(const std::string& category) const
{
    std::vector<const Expense*> filtered;

    // Check each expense's category
    for (const auto& expense : expenses)
    {
        if (expense->GetCategory() == category)  // Exact match
        {
            filtered.push_back(expense.get());
        }
    }

    return filtered;
}

/// <summary>
/// Search expenses by case-insensitive description keyword 
/// </summary>
/// <param name="keyword">keyword</param>
/// <returns>Vector of pointers of expenses</returns>
std::vector<const Expense*> ExpenseTracker::SearchByDescription(const std::string& keyword) const
{
    std::vector<const Expense*> results;
    
    // Convert keyword to lowercase for case-insensitive search
    std::string lowerKeyword = keyword;
    std::transform(lowerKeyword.begin(), lowerKeyword.end(), 
                   lowerKeyword.begin(), ::tolower);

    // Search through all expenses
    for (const auto& expense : expenses)
    {
        // Get description and convert to lowercase
        std::string desc = expense->GetDescription();
        std::transform(desc.begin(), desc.end(), desc.begin(), ::tolower);

        // Check if keyword is found in description (substring search)
        if (desc.find(lowerKeyword) != std::string::npos)
        {
            results.push_back(expense.get());
        }
    }

    return results;
}

/// <summary>
/// Calculate total expenses grouped by category
/// </summary>
/// <returns>Map where keys are category names and values are total amounts</returns>
/// <remarks>Example: {"Food": 100.75, "Transport": 70.50, "Shopping": 320.00}</remarks>
std::map<std::string, double> ExpenseTracker::GetSummaryByCategory() const
{
    std::map<std::string, double> summary;  // Map to accumulate totals by category

    // Iterate through all expenses and sum amounts by category
    for (const auto& expense : expenses)
    {
        // += operator creates entry if category doesn't exist, adds to existing total
        summary[expense->GetCategory()] += expense->GetAmount();
    }

    return summary;
}

/// <summary>
/// Calculate total expenses by category within a date range
/// </summary>
/// <param name="startDate">Start date of range (inclusive)</param>
/// <param name="endDate">End date of range (inclusive)</param>
/// <returns>Map where keys are category names and values are total amounts</returns>
/// <remarks>Only includes expenses that fall within the specified date range</remarks>
std::map<std::string, double> ExpenseTracker::GetSummaryByCategory(const Date& startDate, const Date& endDate) const
{
    std::map<std::string, double> summary;

    // Only process expenses within the date range
    for (const auto& expense : expenses)
    {
        if (IsDateInRange(expense->GetDate(), startDate, endDate))
        {
            summary[expense->GetCategory()] += expense->GetAmount();
        }
    }

    return summary;
}

/// <summary>
/// Calculate total of all expenses
/// </summary>
/// <returns>Sum of all expense amounts</returns>
double ExpenseTracker::GetTotalExpenses() const
{
    double total = 0.0;
    
    // Sum up all expense amounts
    for (const auto& expense : expenses)
    {
        total += expense->GetAmount();
    }
    
    return total;
}

/// <summary>
/// Calculate total expenses within a date range. Range is inclusive
/// </summary>
/// <param name="startDate">Start date of range</param>
/// <param name="endDate">End date of range</param>
/// <returns>Expenses in the range</returns>
double ExpenseTracker::GetTotalExpenses(const Date& startDate, const Date& endDate) const
{
    double total = 0.0;
    
    // Only sum expenses within the date range
    for (const auto& expense : expenses)
    {
        if (IsDateInRange(expense->GetDate(), startDate, endDate))
        {
            total += expense->GetAmount();
        }
    }
    
    return total;
}

/// <summary>
/// Get the total number of expenses stored
/// </summary>
/// <returns>Number of expenses</returns>
size_t ExpenseTracker::GetExpenseCount() const
{
    return expenses.size();  // Return size of the vector
}

/// <summary>
/// Display a filtered list of expenses
/// </summary>
/// <param name="filteredExpenses">Vector of expense pointers</param>
void ExpenseTracker::DisplayExpenses(const std::vector<const Expense*>& filteredExpenses) const
{
    if (filteredExpenses.empty())
    {
        std::cout << "No expenses found matching the criteria." << std::endl;
        return;
    }

    // The Header
    std::cout << std::left 
              << std::setw(12) << "Date"
              << std::setw(12) << "Amount"
              << std::setw(20) << "Category"
              << "Description" << std::endl;
    std::cout << std::string(60, '-') << std::endl;

    for (const auto* expense : filteredExpenses)
    {
        expense->Display();
    }
    std::cout << std::endl;
}

/// <summary>
/// Save all expenses to a JSON file
/// </summary>
/// <param name="filename">file name</param>
/// <returns>true if successful, false on error</returns>
bool ExpenseTracker::SaveToJSON(const std::string& filename) const
{
    try
    {
        json jsonObject;  // Create root JSON object
        jsonObject["expenses"] = json::array();  // Create empty array for expenses
        
        // Convert each expense to JSON and add to array
        for (const auto& expense : expenses)
        {
            jsonObject["expenses"].push_back(expense->ToJSON());
        }
        
        // Open file for writing
        std::ofstream file(filename);
        if (!file.is_open())
        {
            std::cerr << "Error: Could not open file " << filename << " for writing." << std::endl;
            return false;
        }
        
        // Write JSON to file with pretty printing (2-space indent)
        file << jsonObject.dump(2);
        file.close();
        return true;
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error saving JSON: " << e.what() << std::endl;
        return false;
    }
}

/// <summary>
/// Load expenses from a JSON file
/// </summary>
/// <param name="filename">file name</param>
/// <returns>true if successful, false on error</returns>
bool ExpenseTracker::LoadFromJSON(const std::string& filename)
{
    // not my usual style of handling but we just read about error handling so why not
    try
    {
        // Open file for reading
        std::ifstream file(filename);
        if (!file.is_open())
        {
            std::cerr << "Warning: Could not open file " << filename 
                      << " for reading. Starting with empty list." << std::endl;
            return false;
        }

        // Parse JSON from file
        json jsonObject;
        file >> jsonObject;  // Read entire JSON object from file
        file.close();

        // Clear existing expenses before loading new ones
        expenses.clear();

        // Check if JSON has "expenses" array
        if (jsonObject.contains("expenses") && jsonObject["expenses"].is_array())
        {
            // Iterate through each expense in the array
            for (const auto& expenseJson : jsonObject["expenses"])
            {
                try
                {
                    // Convert JSON object to Expense
                    Expense exp = Expense::FromJSON(expenseJson);
                    // Store in vector using smart pointer
                    expenses.push_back(std::make_unique<Expense>(exp));
                }
                catch (const std::exception& e)
                {
                    // Skip invalid entries but continue loading others
                    std::cerr << "Warning: Failed to parse an expense entry: " 
                              << e.what() << std::endl;
                }
            }
        }

        return true;
    }
    catch (const json::parse_error& e)
    {
        // Handle JSON syntax errors
        std::cerr << "JSON parse error: " << e.what() << std::endl;
        return false;
    }
    catch (const std::exception& e)
    {
        // Handle other errors
        std::cerr << "Error loading JSON: " << e.what() << std::endl;
        return false;
    }
}

/// <summary>
/// Get a pointer to an expense at a specific index
/// </summary>
/// <param name="index">Zero-based index of the expense</param>
/// <returns>Pointer to expense, or nullptr if index is invalid</returns>
const Expense* ExpenseTracker::GetExpenseAt(size_t index) const
{
    if (index >= expenses.size())
    {
        return nullptr;
    }
    
    // raw pointer
    return expenses[index].get();
}
