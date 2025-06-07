# Blueprint for the Observer Design Pattern

The Observer Design Pattern establishes a one-to-many dependency between objects. When one object (the **Subject**) changes its state, all its dependents (the **Observers**) are notified and updated automatically. This pattern is widely used for event handling, messaging systems, and more.

---

## Components of Observer Design Pattern

1. **Subject**:
   - Maintains a list of observers.
   - Provides methods to add, remove, and notify observers.

2. **Observer**:
   - Defines an interface for updating objects.
   - Gets notified of changes in the subject's state.

3. **ConcreteSubject**:
   - Extends the subject interface.
   - Stores the state and notifies observers when the state changes.

4. **ConcreteObserver**:
   - Implements the observer interface.
   - Updates its state based on the subject's state.

---

## Step-by-Step Blueprint

### 1. Define an Observer Interface
A base class or interface that declares the `update()` method.

```cpp
class Observer {
public:
    virtual void update(int state) = 0;
    virtual ~Observer() {}
};
```

### 2. Define a Subject Interface
Declares methods for adding, removing, and notifying observers.

```cpp
#include <vector>
class Subject {
public:
    virtual void attach(Observer* observer) = 0;
    virtual void detach(Observer* observer) = 0;
    virtual void notify() = 0;
    virtual ~Subject() {}
};
```

### 3. Create a Concrete Subject
Implements the subject interface. Holds the state and maintains a list of observers.

```cpp
class ConcreteSubject : public Subject {
private:
    std::vector<Observer*> observers;
    int state;
public:
    void attach(Observer* observer) override {
        observers.push_back(observer);
    }

    void detach(Observer* observer) override {
        observers.erase(std::remove(observers.begin(), observers.end(), observer), observers.end());
    }

    void notify() override {
        for (auto* observer : observers) {
            observer->update(state);
        }
    }

    void setState(int newState) {
        state = newState;
        notify();
    }

    int getState() const { return state; }
};
```

### 4. Create Concrete Observers
Implement the observer interface and act upon notifications from the subject.

```cpp
#include <iostream>
class ConcreteObserver : public Observer {
private:
    std::string name;
public:
    explicit ConcreteObserver(const std::string& name) : name(name) {}
    
    void update(int state) override {
        std::cout << "Observer " << name << " updated with state: " << state << "\n";
    }
};
```

### 5. Usage
Create a subject and observers. Attach observers to the subject. Change the state of the subject to see the updates in action.

```cpp
int main() {
    ConcreteSubject subject;
    ConcreteObserver observer1("A"), observer2("B");

    subject.attach(&observer1);
    subject.attach(&observer2);

    subject.setState(1);
    subject.setState(2);

    subject.detach(&observer1);
    subject.setState(3);

    return 0;
}
```

---

## Key Concepts

- **Decoupling**: Observers are decoupled from the subject. They only depend on the interface, not concrete implementations.
- **Push vs. Pull**:
  - **Push**: The subject sends detailed updates.
  - **Pull**: Observers query the subject for updates.
- **Dynamic Relationships**: Observers can be added or removed at runtime.

---

## Use Cases

1. **GUI Event Systems**: Button click notifications to multiple listeners.
2. **Logging Systems**: Notify loggers when an event occurs.
3. **Stock Market Applications**: Update subscribers when stock prices change.
4. **Data Binding in UI Frameworks**: Synchronize UI with underlying data changes.

---
