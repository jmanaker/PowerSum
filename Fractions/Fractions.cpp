// This is the main DLL file.
#include "Fractions.h"

namespace Math
{
	Int64 Fraction::Numerator::get(void)
	{
		if (!this->numer)
		{
			throw gcnew NullReferenceException();
		}
		PyObject *error;
		unsigned long long retval(PyLong_AsLongLong(this->numer));
		if (!(error = PyErr_Occurred()))
		{
			return retval;
		}
		else
		{
			if (PyErr_GivenExceptionMatches(error, PyExc_OverflowError))
			{
				PyErr_Clear();
				throw gcnew OverflowException();
			}
			else
			{
				throw gcnew Exception();
			}
		}
	}
	UInt64 Fraction::Denominator::get(void)
	{
		if (!this->denom)
		{
			throw gcnew NullReferenceException();
		}
		PyObject *error;
		unsigned long long retval (PyLong_AsUnsignedLongLong(this->denom));
		if (!(error = PyErr_Occurred()))
		{
			return retval;
		}
		else
		{
			if (PyErr_GivenExceptionMatches(error, PyExc_OverflowError))
			{
				PyErr_Clear();
				throw gcnew OverflowException();
			}
			else
			{
				throw gcnew Exception();
			}
		}
	}
	//VERY, VERY, important!  Property accessors break if numer and denom are not valid or NULL
	Fraction::Fraction(Int64 num, UInt64 den) : numer(NULL), denom(NULL)
	{
		if (!den)
		{
			throw gcnew DivideByZeroException();
		}
		if (!Fraction::instanceCount)
		{
			initStatics();
		}
		Fraction::instanceCount++;
		this->Numerator = num;
		this->Denominator = den;
		pin_ptr<PyObject*> numerLoc(&this->numer), denomLoc(&this->denom);
		reduce(numerLoc, denomLoc);
	}
	//(Stolen, Stolen, N/A)=>N/A
	Fraction::Fraction(PyObject *num, PyObject *den, bool reduced) : numer(num), denom(den)
	{
		if (!Fraction::instanceCount)
		{
			Fraction::initStatics();
		}
		Fraction::instanceCount++;
		if (!reduced)
		{
			pin_ptr<PyObject*> numerLoc(&this->numer), denomLoc(&this->denom);
			reduce(numerLoc, denomLoc);
		}
	}
	Fraction::!Fraction(void)
	{
		Py_XDECREF(this->numer);
		Py_XDECREF(this->denom);
		Fraction::instanceCount--;
		if (!Fraction::instanceCount)
		{
			Py_CLEAR(Fraction::zero);
			Py_CLEAR(Fraction::one);
		}
	}
	String ^Fraction::ToString()
	{
		PyObject *numStr, *denStr = NULL;
		const wchar_t *num = NULL, *den = NULL;
		if (numStr = PyObject_Repr(this->numer))
		{
			try
			{
				if (num = PyUnicode_AsWideCharString(numStr, NULL))
				{
					String ^retval = gcnew String(num);
					if (PyObject_RichCompareBool(this->denom, Fraction::one, Py_EQ))
					{
						return retval;
					}
					if (denStr = PyObject_Repr(this->denom))
					{
						if (den = PyUnicode_AsWideCharString(denStr, NULL))
						{
							return retval + "/" + gcnew String(den);
						}
					}
				}
			}
			finally
			{
				Py_DECREF(numStr);
				Py_XDECREF(denStr);
				PyMem_Free((void*)num);
				PyMem_Free((void*)den);
			}
		}
		throw gcnew Exception();
	}
	Int32 Fraction::GetHashCode()
	{
		Int32 hashN(0), hashD(0);
		try
		{
			hashN = this->Numerator.GetHashCode();
		}
		catch (OverflowException^)
		{
			//Do nothing
		}
		try
		{
			hashD = this->Denominator.GetHashCode();
		}
		catch (OverflowException^)
		{
			//Do nothing
		}
		return hashN ^ hashD;
	}

	//Refcounts of a and b are unchanged.
	void Fraction::reduce(_Inout_ PyObject **a, _Inout_ PyObject **b)
	{
		PyObject *shared = gcd(*a, *b), *temp = NULL;
		try
		{
			if (temp = PyNumber_InPlaceFloorDivide(*a, shared))
			{
				if (temp == *a)
				{
					Py_CLEAR(temp);
				}
				else
				{
					Py_CLEAR(*a);
					*a = temp;
				}
				if (temp = PyNumber_InPlaceFloorDivide(*b, shared))
				{
					if (temp == *b)
					{
						Py_CLEAR(temp);
					}
					else
					{
						Py_CLEAR(*b);
						*b = temp;
						temp = NULL;
					}
					return;
				}
			}
			throw gcnew Exception();
		}
		finally
		{
			Py_XDECREF(temp);
			Py_DECREF(shared);
		}
	}
	//gcd(Borrowed, Borrowed, Borrowed...) => New
	PyObject *Fraction::gcd(PyObject *a, PyObject *b, ... array<PyObject*> ^others)
	{
		Py_INCREF(a);
		Py_INCREF(b);
		try
		{
			if (PyObject_RichCompareBool(a, b, Py_LT))
			{
				PyObject *temp;
				temp = a;
				a = b;
				b = temp;
			}
			if (PyObject_RichCompareBool(b, Fraction::zero, Py_EQ))
			{
				if (!others->Length)
				{
					Py_INCREF(a);
					return a;
				}
				else
				{
					Int32 len = others->Length - 1;
					array<PyObject*> ^nothers = gcnew array<PyObject *>(len);
					Array::Copy(others, 1, nothers, 0, len);
					return gcd(a, others[0], nothers);
				}
			}
			if (PyObject_RichCompareBool(b, Fraction::zero, Py_LT))
			{
				PyObject *nb = NULL;
				if (nb = PyNumber_Negative(b))
				{
					try
					{
						return gcd(a, nb, others);
					}
					finally
					{
						Py_DECREF(nb);
					}
				}
				PyErr_Print();
			}
			else
			{
				if (PyObject_RichCompareBool(a, Fraction::zero, Py_LT))
				{
					PyObject *na = NULL;
					if (na = PyNumber_Negative(a))
					{
						try
						{
							return gcd(na, b, others);
						}
						finally
						{
							Py_DECREF(na);
						}
					}
				}
				else
				{
					PyObject *na = NULL;
					if (na = PyNumber_Remainder(a, b))
					{
						try
						{
							return gcd(na, b, others);
						}
						finally
						{
							Py_DECREF(na);
						}
					}
					else
					{
						if (PyErr_ExceptionMatches(PyExc_ZeroDivisionError))
						{
							PyErr_Clear();
							throw gcnew DivideByZeroException();
						}
					}
				}
			}
			throw gcnew Exception();
		}
		finally
		{
			Py_DECREF(a);
			Py_DECREF(b);
		}
	}
	void Fraction::initStatics(void)
	{
		if (Fraction::zero = PyLong_FromLong(0))
		{
			if (Fraction::one = PyLong_FromLong(1))
			{
				return;
			}
			Py_DECREF(Fraction::zero);
		}
		throw gcnew Exception();
	}
}