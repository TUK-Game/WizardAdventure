#pragma once

#define DECLARE_SINGLETON(type)\
private:\
	friend class CSingleton<type>;\
	type();\
	type(const type& other) = delete;\
	type(type&& other) = delete;\
	~type();\
	type& operator = (const type& other) = delete;\
	type& operator = (type&& other) = delete;