# Author: Priyanka Dhungana
# Date: 2026-01-23
# This is the starting point for running the Expense Tracker app
# It provides a simple command-line interface for users to interact with the ExpenseTracker class
# It allows adding expenses, viewing all expenses, filtering by category or date range, and showing a summary.

from expense_tracker import ExpenseTracker

def show_menu():
    # Display the menu options
    print("\n====== Expense Tracker ======")
    print("1. Add expense")
    print("2. View all expenses")
    print("3. Filter by category")
    print("4. Filter by date range")
    print("5. Show summary")
    print("6. Exit")

def get_expense_input():
    # Get each expense details input from the user
    date = input("Enter date (YYYY-MM-DD): ")
    amount = float(input("Enter amount: $"))
    category = input("Enter category: ")
    description = input("Enter description: ")
    return date, amount, category, description

if __name__ == "__main__":
    print("\n-------- Welcome to Expense Tracker App ---------")
    print("\n")
    tracker = ExpenseTracker()
    while True:
        show_menu()
        choice = input("\nEnter your choice (1-6): ")

        if choice == "1":
            date, amount, category, description = get_expense_input()
            tracker.add_expense(date, amount, category, description)

        elif choice == "2":
            tracker.show_all_expenses()

        elif choice == "3":
            category = input("Enter category to filter: ")
            tracker.filter_by_category(category)

        elif choice == "4":
            start_date = input("Enter start date (YYYY-MM-DD): ")
            end_date = input("Enter end date (YYYY-MM-DD): ")
            tracker.filter_by_date_range(start_date, end_date)

        elif choice == "5":
            tracker.show_summary()

        elif choice == "6":
            print("Goodbye!")
            break

        else:
            print("Invalid choice. Please try again.")
