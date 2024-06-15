#include "Fractions.h"

namespace Math
{
	Fraction::operator Fraction ^ (Single in)
	{
		UInt64 iPart = (UInt64)System::Math::Truncate(in);
		Fraction ^holder = gcnew Fraction(iPart);
		in -= iPart;
		bool error = false;
		if (error = (in != 0))
		{
			PyObject *radix;
			//Singles are stored as base 2 decimals
			if (radix = PyLong_FromLong(2))
			{
				PyObject *temp[2]
				{
					NULL, NULL
				};
				do
				{
					if (temp[0] = PyNumber_Multiply(radix, holder->numer))
					{
						try
						{
							in *= 2;
							iPart = (UInt64)System::Math::Truncate(in);
							in -= iPart;
							if (temp[1] = PyLong_FromLongLong(iPart))
							{
								Py_CLEAR(holder->numer);
								holder->numer = temp[0];
								temp[0] = NULL;
								if (temp[0] = PyNumber_Add(temp[1], holder->numer))
								{
									Py_CLEAR(temp[1]);
									if (temp[1] = PyNumber_Multiply(radix, holder->denom))
									{
										Py_CLEAR(holder->numer);
										holder->numer = temp[0];
										temp[0] = NULL;
										Py_CLEAR(holder->denom);
										holder->denom = temp[1];
										temp[1] = NULL;
										continue;
									}
								}
							}
						}
						finally
						{
							Py_XDECREF(temp[0]);
							Py_XDECREF(temp[1]);
						}
					}
					break;
				} while (error = (in != 0));
				Py_DECREF(radix);
			}
		}
		if (!error)
		{
			return holder;
		}
		throw gcnew Exception();
	}
	Fraction::operator Fraction^(Double in)
	{
		UInt64 iPart = (UInt64)System::Math::Truncate(in);
		Fraction ^holder = gcnew Fraction(iPart);
		in -= iPart;
		bool error = false;
		if (error = (in != 0))
		{
			PyObject *radix;
			//Doubles are stored as base 2 decimals
			if (radix = PyLong_FromLong(2))
			{
				PyObject *temp[2]
				{
					NULL, NULL
				};
				do
				{
					if (temp[0] = PyNumber_Multiply(radix, holder->numer))
					{
						try
						{
							in *= 2;
							iPart = (UInt64)System::Math::Truncate(in);
							in -= iPart;
							if (temp[1] = PyLong_FromLongLong(iPart))
							{
								Py_CLEAR(holder->numer);
								holder->numer = temp[0];
								temp[0] = NULL;
								if (temp[0] = PyNumber_Add(temp[1], holder->numer))
								{
									Py_CLEAR(temp[1]);
									if (temp[1] = PyNumber_Multiply(radix, holder->denom))
									{
										Py_CLEAR(holder->numer);
										holder->numer = temp[0];
										temp[0] = NULL;
										Py_CLEAR(holder->denom);
										holder->denom = temp[1];
										temp[1] = NULL;
										continue;
									}
								}
							}
						}
						finally
						{
							Py_XDECREF(temp[0]);
							Py_XDECREF(temp[1]);
						}
					}
					break;
				} while (error = (in != 0));
				Py_DECREF(radix);
			}
		}
		if (!error)
		{
			return holder;
		}
		throw gcnew Exception();
	}
	Fraction::operator Fraction^(Decimal in)
	{
		UInt64 iPart = (UInt64)System::Math::Truncate(in);
		Fraction ^holder = gcnew Fraction(iPart);
		in -= iPart;
		bool error = false;
		if (error = (in != 0))
		{
			PyObject *radix;
			//Decimals are stored as base 10 decimals
			if (radix = PyLong_FromLong(10))
			{
				PyObject *temp[2]
				{
					NULL, NULL
				};
				do
				{
					if (temp[0] = PyNumber_Multiply(radix, holder->numer))
					{
						try
						{
							in *= 10;
							iPart = (UInt64)System::Math::Truncate(in);
							in -= iPart;
							if (temp[1] = PyLong_FromLongLong(iPart))
							{
								Py_CLEAR(holder->numer);
								holder->numer = temp[0];
								temp[0] = NULL;
								if (temp[0] = PyNumber_Add(temp[1], holder->numer))
								{
									Py_CLEAR(temp[1]);
									if (temp[1] = PyNumber_Multiply(radix, holder->denom))
									{
										Py_CLEAR(holder->numer);
										holder->numer = temp[0];
										temp[0] = NULL;
										Py_CLEAR(holder->denom);
										holder->denom = temp[1];
										temp[1] = NULL;
										continue;
									}
								}
							}
						}
						finally
						{
							Py_XDECREF(temp[0]);
							Py_XDECREF(temp[1]);
						}
					}
					break;
				} while (error = (in != 0));
				Py_DECREF(radix);
			}
		}
		if (!error)
		{
			return holder;
		}
		throw gcnew Exception();
	}
	Fraction::operator Double(Fraction ^in)
	{
		if (Object::ReferenceEquals(in, nullptr))
		{
			throw gcnew NullReferenceException();
		}
		PyObject *pyRetval;
		if (pyRetval = PyNumber_TrueDivide(in->numer, in->denom))
		{
			double retval = PyFloat_AsDouble(pyRetval);
			Py_DECREF(pyRetval);
			return retval;
		}
		throw gcnew Exception();
	}
}