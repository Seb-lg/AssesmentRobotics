//
// Created by seb on 27/09/2019.
//

#pragma once

#include <map>
#include <string>
#include <list>
#include <functional>
#include <iostream>

/**
	Class of Event Manager
	Author: Sébastien LE GALL
**/

class EventLibrary {
public:
	EventLibrary() = default;

	/// Add an Event wich can be a lambda or a function
	/// \tparam Args Types of arguments the function need
	/// \tparam T Type of the function
	/// \param event Name of the event
	/// \param func Function or lambda wich will be fired
	template <typename ...Args, typename T>
	void addEvent(std::string const &event, T func) {
		auto cont = new Container<Args...>();
		cont->func = func;
		m_functions[event][typeid(std::function<void(Args...)>).name()].push_back(static_cast<void*>(cont));
	}

	/// Fire Events
	/// \tparam Args Types of arguments the function need
	/// \param event Name of the event to fire
	/// \param param Parameters to be passed to the function
	/// \return True if a function has been found, False otherwise
	template <typename ...Args>
	bool fire(std::string const &event, Args ...param) {
		bool entered = false;
		for (auto item : m_functions[event][typeid(std::function<void(Args...)>).name()]) {
			static_cast<Container<Args...>*>(item)->fire(param...);
			entered = true;
		}
		return entered;
	}

private:
	/// Map < event,  Map < type of args, List < functions >>
	std::map<std::string,std::map<std::string,std::list<void*>>>	m_functions;
private:
	/// Container allowing to store std::functions
	/// \tparam T args of the function
	template <typename ...T>
	struct Container {
		std::function<void(T...)> func;

		void fire(T ...args) {
			func(args...);
		}
	};
};

extern EventLibrary event;