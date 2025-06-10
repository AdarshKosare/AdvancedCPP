# Singleton Design Pattern

## 1. Purpose

- Ensures a class has only one instance.
- Provides a global point of access to that instance.
- Useful for managing shared resources (e.g., configurations, logging, database connections).

## 2. How it Works

- The constructor is private to prevent creating multiple instances.
- A static method provides access to the single instance, creating it if it doesn’t exist.

## 3. Real-Life Examples

- **Logging System:** A single logger instance writes logs to a file or console.
- **Configuration Manager:** Manages application-wide settings.

## 4. Why Use Static Member Variables in Singleton?

- **Shared Across Instances:** Static members belong to the class, not to any object.
- **Persistence:** Static variables are created once and persist for the program's lifetime.
- **Global Access:** Static members can be accessed via static methods (e.g., `getInstance()`).
- **Ensures a Single Instance:** Only one instance is created and shared.
- **Efficient Resource Management:** Reduces overhead by creating the instance only once.

## 5. What Happens Without Static Member Variables?

- A new instance would be created for every call to `getInstance()`, violating the Singleton principle.
- Each object would have its own copy, leading to multiple instances instead of a single shared instance.