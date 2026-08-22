#include "Observing.hpp"



// ----- Observable -----

// --- Public Members ---

void Observable::addObserver(Observer* observer) { observers.insert(observer); }


void Observable::removeObserver(Observer* observer) { observers.erase(observer); }


void Observable::notifyObservers() {

	for (auto& obs : observers) {

		obs->update();
	}
}



// ----- Observer -----

// --- Public Members ---

Observer::Observer(const Observer& observer) { 

	for (auto& observable : observer.observables) { observable->addObserver(this); }
}


Observer& Observer::operator=(const Observer& observer) {

	for (auto& observable : observables) { observable->removeObserver(this); }

	for (auto& observable : observer.observables) { this->observe(observable); }

	return *this;

}


void Observer::observe(const std::shared_ptr<Observable>& observable) {

	observable->addObserver(this);
	observables.insert(observable);
}


void Observer::stopObserving(const std::shared_ptr<Observable>& observable) {

	observable->removeObserver(this);
	observables.erase(observable);
}


void Observer::stopObservingAll() {

	for (auto& obs : observables) {

		obs->removeObserver(this);
		observables.erase(obs);
	}
}


Observer::~Observer() {

	for (auto& observable : observables) { observable->removeObserver(this); }
}
