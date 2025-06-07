#include <iostream>
#include <vector>
#include <memory>
#include <string>
#include <algorithm>
using namespace std;

class Observer
{
public:
    virtual void update(float temperature, float humidity, float pressure) = 0;
    ~Observer() {}
};

class Subject
{
public:
    virtual void registerObserver(Observer *observer) = 0;
    virtual void removeObserver(Observer *observer) = 0;
    virtual void notifyObservers() = 0;
    ~Subject() {};
};

class WeatherData : public Subject
{
private:
    vector<Observer *> observers;
    float temperature;
    float humidity;
    float pressure;

public:
    void registerObserver(Observer *observer) override
    {
        observers.push_back(observer);
    }

    void removeObserver(Observer *observer) override
    {
        observers.erase(remove(observers.begin(), observers.end(), observer), observers.end());
    }

    void notifyObservers() override
    {
        for (Observer *ob : observers)
        {
            ob->update(temperature, humidity, pressure);
        }
    }

    void setMeasurements(float temp, float humid, float pr)
    {
        temperature = temp;
        humidity = humid;
        pressure = pr;
        notifyObservers();
    }
};

class CurrentConditionsDisplay : public Observer
{
public:
    void update(float temperature, float humidity, float pressure) override
    {
        std::cout << "Current conditions: " << temperature << "C degrees and " << humidity << "% humidity.\n";
    }
};

class StatisticsDisplay : public Observer
{
public:
    void update(float temperature, float humidity, float pressure) override
    {
        std::cout << "Statistics: Temp: " << temperature << "C, Humidity: " << humidity << "%, Pressure: " << pressure << " hPa.\n";
    }
};

int main()

{
    WeatherData weatherData;
    CurrentConditionsDisplay currentDisplay;
    StatisticsDisplay statsDisplay;

    weatherData.registerObserver(&currentDisplay);
    weatherData.registerObserver(&statsDisplay);

    weatherData.setMeasurements(30.0f, 65.0f, 1013.0f);
    weatherData.setMeasurements(28.0f, 70.0f, 1012.0f);
    return 0;
}