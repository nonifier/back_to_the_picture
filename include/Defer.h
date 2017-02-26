#pragma once

template <class Lambda> class AtScopeExit {
	Lambda& m_lambda;
public:
	AtScopeExit(Lambda& action) : m_lambda(action) {}
	~AtScopeExit() { m_lambda(); }
};

#define DEFER_INTERNAL2(lname, aname, ...) \
    auto lname = [&]() { __VA_ARGS__; }; \
    AtScopeExit<decltype(lname)> aname(lname);
#define DEFER_TOKENPASTE(x, y) Auto_ ## x ## y
#define DEFER_INTERNAL1(ctr, ...) \
    DEFER_INTERNAL2(DEFER_TOKENPASTE(func_, ctr), \
                    DEFER_TOKENPASTE(instance_, ctr), __VA_ARGS__)
#define Defer(...) DEFER_INTERNAL1(__COUNTER__, __VA_ARGS__)
