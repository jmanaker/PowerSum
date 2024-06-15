#pragma once
//Just in case
#ifndef FRAC__proto
#define FRAC__proto
#pragma warning (disable: 4127)
#pragma warning (disable: 4800)

#define Py_LIMITED_API
#include <Python.h>

using namespace System;

namespace Math
{
	public ref struct Fraction
	{
	public:
		property Int64 Numerator
		{
			Int64 get(void);
		private:
			void set(Int64 value)
			{
				Py_CLEAR(numer);
				numer = PyLong_FromLongLong(value);
			}
		};
		property UInt64 Denominator
		{
			UInt64 get(void);
		private:
			void set(UInt64 value)
			{
				Py_CLEAR(denom);
				denom = PyLong_FromUnsignedLongLong(value);
			}
		};
		Fraction(Int64, UInt64);
		!Fraction(void);
		Fraction(Int64 num) : Fraction(num, 1)
		{
		}
		~Fraction(void)
		{
			delete this;
		}
		String ^ToString(void) override;
		Int32 GetHashCode() override;
		Boolean Equals(Object ^other) override
		{
			Fraction ^in = dynamic_cast<Fraction^>(other);
			//Use short-circuiting
			return !Object::ReferenceEquals(in, nullptr) && this == in;
		}
		static Fraction ^operator -(Fraction^);
		static Fraction ^operator +(Fraction^, Fraction^);
		static Fraction ^operator *(Fraction^, Fraction^);
		static Fraction ^operator ^(Fraction ^, Int64);
		static Fraction ^operator -(Fraction ^larg, Fraction ^rarg)
		{
			return larg + (-rarg);
		}
		static Fraction ^operator /(Fraction ^self, Fraction ^in)
		{
			if (Object::ReferenceEquals(in, nullptr))
			{
				throw gcnew NullReferenceException();
			}
			Py_INCREF(in->denom);
			Py_INCREF(in->numer);
			return self * gcnew Fraction(in->denom, in->numer, true);
		}
		static bool operator ==(Fraction ^larg, Fraction ^rarg)
		{
			return PyObject_RichCompareBool(larg->numer, rarg->denom, Py_EQ) && PyObject_RichCompareBool(larg->denom, rarg->denom, Py_EQ);
		}
		static bool operator <(Fraction ^larg, Fraction ^rarg)
		{
			return PyObject_RichCompareBool((larg - rarg)->numer, zero, Py_LT);
		}
		static bool operator <=(Fraction ^larg, Fraction ^rarg)
		{
			return PyObject_RichCompareBool((larg - rarg)->numer, zero, Py_LE);
		}
		static bool operator >(Fraction ^larg, Fraction ^rarg)
		{
			return PyObject_RichCompareBool((larg - rarg)->numer, zero, Py_GT);
		}
		static bool operator >=(Fraction ^larg, Fraction ^rarg)
		{
			return PyObject_RichCompareBool((larg - rarg)->numer, zero, Py_GE);
		}
		static operator Fraction ^ (Single);
		static operator Fraction ^ (Double);
		static operator Fraction ^ (Decimal);
		static operator Fraction ^ (Int64 in)
		{
			return gcnew Fraction(in);
		}
		static operator Double(Fraction^);
		static operator Single(Fraction ^in)
		{
			return (Single)((Double)in);
		}
		static operator Decimal(Fraction ^in)
		{
			return (Decimal)((Double)in);
		}
		static operator UInt64(Fraction ^in)
		{
			if (PyObject_RichCompareBool(in->denom, Fraction::one, Py_NE))
			{
				throw gcnew InvalidCastException();
			}
			return PyLong_AsUnsignedLongLong(in->numer);
		}
		static operator Int64(Fraction ^in)
		{
			if (PyObject_RichCompareBool(in->denom, Fraction::one, Py_NE))
			{
				throw gcnew InvalidCastException();
			}
			return PyLong_AsLongLong(in->numer);
		}
		static operator Int32(Fraction ^in)
		{
			return (Int32)((Int64)in);
		}
		static operator UInt32(Fraction ^in)
		{
			return (UInt32)((UInt64)in);
		}
	internal:
		//Refcounts of a and b are unchanged.
		static void reduce(_Inout_ PyObject**, _Inout_ PyObject**);
		//gcd(Borrowed, Borrowed, Borrowed...) => New
		static PyObject *gcd(PyObject*, PyObject*, ... array<PyObject*>^);
	private:
		//These constructors DO NOT check for zero denominators!
		//DO NOT supply PyLong_FromLong(0) to den!
		//(Stolen, Stolen)=>N/A
		Fraction(PyObject *num, PyObject *den) : Fraction(num, den, false)
		{
		}
		//(Stolen, Stolen, N/A)=>N/A
		Fraction(PyObject *, PyObject *, bool);
		static void initStatics(void);
		PyObject *numer, *denom;
		static long long instanceCount = 0;
		static PyObject *zero = NULL, *one = NULL;
	};
}

#endif