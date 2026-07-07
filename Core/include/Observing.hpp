#ifndef OBSERVING_HPP
#define OBSERVING_HPP


#include <memory>
#include <set>
#include <vector>



/* Classes following the oberver/observable design pattern. Observables notify observers when a change/event occurs. */



class Observer;


class Observable {

	// Base observable class allows objects to notify observers on events.

	private:

		std::set<Observer*> observers;

	public:

		// addObserver and removeObserver methods are not to be called manually.
		// Only to be called by observer by observer class.

		void addObserver(Observer*);
		void removeObserver(Observer*);
		void notifyObservers();
};


class Observer {

	// Base Observer class that listens to observable to update on event.

	private:

		std::set<std::shared_ptr<Observable>> observables;

	public:

		Observer() = default;
		Observer(const Observer&);
		Observer& operator=(const Observer&);

		void observe(const std::shared_ptr<Observable>&);
		void stopObserving(const std::shared_ptr<Observable>&);
		void stopObservingAll();

		// Each class that inherits the observer base class must implement this method.
		// When the observer is notified by an observable this method is called.

		virtual void update() = 0;

		~Observer();
};

#endif