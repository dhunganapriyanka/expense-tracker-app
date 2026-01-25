# Expense Tracker Application

## Overview
The Expense Tracker Application is a simple tool that helps users record and manage their daily expenses. Users can add expenses, categorize them, view expense records, filter expenses by date or category, and calculate total spending. The application is designed to make personal finance tracking easy and organized.

## Features
- Record daily expenses with amount, date, and category
- View all recorded expenses
- Categorize expenses for better organization
- Filter expenses by date range
- Filter expenses by category
- Calculate total expenses

## Contributors
- Priyanka Dhungana

## Programming Languages Used
This application is implemented in both Python and C++.
The Python version focuses on simplicity, dynamic typing, and built in libraries for handling dates and data structures.
The C++ version focuses on performance, memory management, and the use of classes and STL containers to manage expense data.

---

## How to Run (Python Version)

Make sure you have Python installed on your computer.

1. Open a command line interface (powershell or terminal)
2. Navigate to the project folder:
   ```
   cd /expense-tracker-app
   ```
3. Run the program:
   ```
   python main.py
   ```

## How to Use

When you run the program, you'll see a menu as shown below.

```
=== Expense Tracker ===
1. Add expense
2. View all expenses
3. Filter by category
4. Filter by date range
5. Show summary
6. Exit
```

To get started, Just type a number and press Enter to choose an option.

### Adding an Expense

When adding an expense, you'll be asked for:
- **Date**: Enter in YYYY-MM-DD format (e.g., 2025-01-23)
- **Amount**: Enter the dollar amount (e.g., 25.50)
- **Category**: Enter a category like Food, Transport, Entertainment, etc.
- **Description**: A short note about the expense

### Example

```
Enter date (YYYY-MM-DD): 2025-01-23
Enter amount: $25.50
Enter category: Food
Enter description: Lunch at restaurant
```

## Project Structure

```
expense-tracker-app/
|- main.py             # Run this file to start the app
|- expense.py          # Expense class (stores expense data)
|- expense_tracker.py  # ExpenseTracker class (manages and tracks all expenses)
```

## Python Concepts Used

This project demonstrates several Python features:
- **Classes**: Expense and ExpenseTracker classes organize the code
- **Dictionaries**: Each expense stores its data in a dictionary
- **Lists**: All expenses are stored in a list
- **datetime library**: Handles date parsing and formatting
- **Dynamic typing**: Python figures out variable types automatically