# Author: Priyanka Dhungana
# Date: 2026-01-23
# Expense Tracker Module that tracks and manages expenses
from datetime import datetime
from expense import Expense

class ExpenseTracker:
    def __init__(self):
        # List to store all Expense objects
        self.expenses = []

    def add_expense(self, date, amount, category, description):
        # Adds a new expense to the tracker
        expense = Expense(date, amount, category, description)
        self.expenses.append(expense)
        print(f"Expense added: ${amount} for {category}")

    def show_all_expenses(self):
        # Displays all recorded expenses
        if not self.expenses:
            print("No expenses recorded yet.")
            return
        print("\n******** All Expenses *********")
        for i, expense in enumerate(self.expenses, 1):
            print(f"\nExpense #{i}")
            expense.display()

    def filter_by_category(self, category):
        # Finds all expenses in a given category.
        # Filter expenses where category matches (case-insensitive)
        filtered = []
        for expense in self.expenses:
            if expense.data["category"].lower() == category.lower():
                filtered.append(expense)

        if not filtered:
            print(f"No expenses found for category: {category}")
            return

        print(f"\n****** Expenses for '{category}' ********")
        for i, expense in enumerate(filtered, 1):
            print(f"\nExpense #{i}")
            expense.display()

    def filter_by_date_range(self, start_date_str, end_date_str):
        # Filters expenses within a specified date range. Expected format: YYYY-MM-DD
        start_date = datetime.strptime(start_date_str, "%Y-%m-%d").date()
        end_date = datetime.strptime(end_date_str, "%Y-%m-%d").date()

        filtered = []
        for expense in self.expenses:
            expense_date = expense.data["date"]
            if start_date <= expense_date <= end_date:
                filtered.append(expense)

        if not filtered:
            print(f"No expenses found between {start_date_str} and {end_date_str}")
            return

        print(f"\n****** Expenses from {start_date_str} to {end_date_str} ******")
        for i, expense in enumerate(filtered, 1):
            print(f"\nExpense #{i}: ")
            expense.display()

    def show_summary(self):
        # Displays a summary of expenses by category and overall total
        if not self.expenses:
            print("No expenses recorded yet.")
            return

        # Dictionary to store totals for each category
        category_totals = {}
        totals = 0

        # Loop through expenses and sum the amounts
        for expense in self.expenses:
            category = expense.data["category"]
            amount = expense.data["amount"]
            # Add total to category dictionary
            if category in category_totals:
                category_totals[category] += amount
            else:
                category_totals[category] = amount

            # Add to overall total
            totals += amount

        # Summary
        print("\n****** Expense Summary ********")
        print("\nTotal By Category:")
        for category, total in category_totals.items():
            print(f"  {category}: ${total:.2f}")

        print(f"\nOverall Total: ${totals:.2f}")
