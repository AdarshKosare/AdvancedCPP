/*
1. Single Responsibility Principle
This principle states that "A class should have only one reason to change" which means every class should have a single responsibility or single job or single purpose. In other words, a class should have only one job or purpose within the software system.

Let's understand Single Responsibility Principle using an example:

Imagine a baker who is responsible for baking bread. The baker's role is to focus on the task of baking bread, ensuring that the bread is of high quality, properly baked, and meets the bakery's standards.

However, if the baker is also responsible for managing the inventory, ordering supplies, serving customers, and cleaning the bakery, this would violate the SRP.
Each of these tasks represents a separate responsibility, and by combining them, the baker's focus and effectiveness in baking bread could be compromised.
To adhere to the SRP, the bakery could assign different roles to different individuals or teams. For example, there could be a separate person or team responsible for managing the inventory, another for ordering supplies, another for serving customers, and another for cleaning the bakery.
Code of above Example to understand Single Responsibility Principle:




*/

#include <iostream>
#include <string>

// Class for baking bread
class BreadBaker
{
public:
    void bakeBread()
    {
        std::cout << "Baking high-quality bread..." << std::endl;
    }
};

// Class for managing inventory
class InventoryManager
{
public:
    void manageInventory()
    {
        std::cout << "Managing inventory..." << std::endl;
    }
};

// Class for ordering supplies
class SupplyOrder
{
public:
    void orderSupplies()
    {
        std::cout << "Ordering supplies..." << std::endl;
    }
};

// Class for serving customers
class CustomerService
{
public:
    void serveCustomer()
    {
        std::cout << "Serving customers..." << std::endl;
    }
};

// Class for cleaning the bakery
class BakeryCleaner
{
public:
    void cleanBakery()
    {
        std::cout << "Cleaning the bakery..." << std::endl;
    }
};

int main()
{
    BreadBaker baker;
    InventoryManager inventoryManager;
    SupplyOrder supplyOrder;
    CustomerService customerService;
    BakeryCleaner cleaner;

    // Each class focuses on its specific responsibility
    baker.bakeBread();
    inventoryManager.manageInventory();
    supplyOrder.orderSupplies();
    customerService.serveCustomer();
    cleaner.cleanBakery();

    return 0;
}