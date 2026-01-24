# Expense class to store individual expense details
from datetime import datetime

class Expense:
    """Stores expense data in a dictionary."""

    def __init__(self, date_str, amount, category, description):
        # String date to date object. Only Expected format: YYYY-MM-DD
        date_obj = datetime.strptime(date_str, "%Y-%m-%d").date()
        # Expense properties
        self.data = {
            "date": date_obj,
            "amount": amount,
            "category": category,
            "description": description
        }

    def display(self):
        # Display expense details
        formatted_date = self.data['date'].strftime("%B %d, %Y")
        print(f"  Date: {formatted_date}")
        print(f"  Amount: ${self.data['amount']}")
        print(f"  Category: {self.data['category']}")
        print(f"  Description: {self.data['description']}")
