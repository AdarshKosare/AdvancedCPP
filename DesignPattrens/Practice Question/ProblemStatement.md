## Real-Time Notification System

### Patterns Used:
- **Singleton**: Notification manager to ensure a single point of control for sending notifications.
- **Factory**: To create different types of notifications, such as email, SMS, or push notifications.
- **Observer**: To allow users to subscribe to specific event notifications.

### Description:
A system that sends notifications to users based on their preferences (e.g., weather alerts, stock price changes). Users can subscribe/unsubscribe to topics, and the notifications are sent in their preferred format (e.g., SMS, email).
