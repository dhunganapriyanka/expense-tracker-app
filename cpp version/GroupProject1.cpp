#include "ExpenseTracker.h"
#include <iostream>
#include <iomanip>
#include <limits>

// Helper function to get date input from user
Date getDateInput(const std::string& prompt)
{
    int day, month, year;
    std::cout << prompt << " (DD MM YYYY): ";
    std::cin >> day >> month >> year;
    return Date(day, month, year);
}

// UI Menu
void displayMenu()
{
    std::cout << "\n=== Expense Tracker Menu ===" << std::endl;
    std::cout << "1. Add Expense" << std::endl;
    std::cout << "2. View All Expenses" << std::endl;
    std::cout << "3. Filter by Date Range" << std::endl;
    std::cout << "4. Filter by Category" << std::endl;
    std::cout << "5. Search by Description" << std::endl;
    std::cout << "6. View Summary by Category" << std::endl;
    std::cout << "7. View Total Expenses" << std::endl;
    std::cout << "0. Exit" << std::endl;
    std::cout << "Enter your choice: ";
}

// Main loop
int main()
{
    ExpenseTracker tracker;
    int optionsChose;
    const std::string jsonFilename = "expenses.json";

    std::cout << "Welcome to Expense Tracker Application!" << std::endl;

    // Load expenses from JSON file if it exists
    if (tracker.LoadFromJSON(jsonFilename))
    {
        std::cout << "Loaded expenses from " << jsonFilename << std::endl;
    }
    else
    {
        // Adding sample data for testing purposes
        std::cout << "No existing data found. Adding sample expenses..." << std::endl;
        tracker.AddExpense(Date(15, 1, 2026), 50.00, "Food", "McDonalds");
        tracker.AddExpense(Date(16, 1, 2026), 25.50, "Transport", "Delta Airlines");
        tracker.AddExpense(Date(17, 1, 2026), 120.00, "Shopping", "Shoes");
        tracker.AddExpense(Date(18, 1, 2026), 35.75, "Food", "Groceries");
        tracker.AddExpense(Date(19, 1, 2026), 200.00, "Shopping", "Black Jacker");
        tracker.AddExpense(Date(20, 1, 2026), 15.00, "Food", "Vanilla Latte");
        tracker.AddExpense(Date(21, 1, 2026), 45.00, "Transport", "Uber to centannial");
        tracker.SaveToJSON(jsonFilename);
        std::cout << "Json not found hence sample data saved to " << jsonFilename << std::endl;
    }

    // simple do while loop for the UI
    do
    {
        displayMenu();
        std::cin >> optionsChose;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        switch (optionsChose)
        {
        case 1:
        {
            Date date = getDateInput("Enter expense date");
            double amount;
            std::cout << "Enter amount: ";
            std::cin >> amount;
            std::cin.ignore();

            std::string category;
            std::cout << "Enter category: ";
            std::getline(std::cin, category);

            std::string description;
            std::cout << "Enter description: ";
            std::getline(std::cin, description);

            tracker.AddExpense(date, amount, category, description);
            if (tracker.SaveToJSON(jsonFilename))
            {
                std::cout << "Expense added and saved to " << jsonFilename << " successfully!" << std::endl;
            }
            else
            {
                std::cout << "Expense added but failed to save to file." << std::endl;
            }
            break;
        }

        case 2:
        {
            tracker.ViewAllExpenses();
            break;
        }

        case 3:
        {
            Date startDate = getDateInput("Enter start date");
            Date endDate = getDateInput("Enter end date");

            std::cout << "\n=== Expenses from " << startDate.ToString()
                << " to " << endDate.ToString() << " ===" << std::endl;
            auto filtered = tracker.FilterByDateRange(startDate, endDate); // i like auto for faster programming
            tracker.DisplayExpenses(filtered);

            double total = tracker.GetTotalExpenses(startDate, endDate);
            std::cout << "Total: $" << total << std::endl;
            break;
        }

        case 4:
        {
            std::string category;
            std::cout << "Enter category to filter: ";
            std::getline(std::cin, category);

            std::cout << "\n=== Expenses in category: " << category << " ===" << std::endl;
            auto filtered = tracker.FilterByCategory(category);
            tracker.DisplayExpenses(filtered);

            double total = 0.0;
            for (const auto* exp : filtered)
            {
                total += exp->GetAmount();
            }
            std::cout << "Total: $" << total << std::endl;
            break;
        }

        case 5:
        {
            std::string keyword;
            std::cout << "Enter search keyword: ";
            std::getline(std::cin, keyword);

            std::cout << "\n=== Search results for: " << keyword << " ===" << std::endl;
            auto results = tracker.SearchByDescription(keyword);
            tracker.DisplayExpenses(results);
            break;
        }

        case 6:
        {
            std::cout << "\n=== Summary by Category ===" << std::endl;
            auto summary = tracker.GetSummaryByCategory();

            std::cout << std::left << std::setw(20) << "Category" << "Total" << std::endl;
            std::cout << std::string(30, '-') << std::endl;

            for (const auto& pair : summary)
            {
                std::cout << std::left << std::setw(20) << pair.first
                    << "$" << pair.second << std::endl;
            }

            std::cout << std::string(30, '-') << std::endl;
            std::cout << std::left << std::setw(20) << "Overall Total"
                << "$" << tracker.GetTotalExpenses() << std::endl;
            break;
        }

        case 7:
        {
            double total = tracker.GetTotalExpenses();
            std::cout << "\n=== Total Expenses ===" << std::endl;
            std::cout << "Total: $" << total << std::endl;
            std::cout << "Number of expenses: " << tracker.GetExpenseCount() << std::endl;
            break;
        }

        case 0:
        {
            // Save before exiting
            if (tracker.SaveToJSON(jsonFilename))
            {
                std::cout << "Expenses saved to " << jsonFilename << std::endl;
            }
            std::cout << "Thank you for using Expense Tracker!" << std::endl;
            break;
        }

        default:
        {
            std::cout << "Invalid choice. Please try again." << std::endl;
            break;
        }
        }
    } while (optionsChose != 0);

    return 0;
}
