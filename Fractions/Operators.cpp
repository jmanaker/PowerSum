#include "Fractions.h"

namespace Math
{
	Fraction ^Fraction::operator-(Fraction ^self)
	{
		if (Object::ReferenceEquals(self, nullptr))
		{
			throw gcnew NullReferenceException();
		}
		PyObject *temp;
		if (temp = PyNumber_Negative(self->numer))
		{
			Py_INCREF(self->denom);
			return gcnew Fraction(temp, self->denom, true);
		}
		throw gcnew Exception();
	}
	Fraction ^Fraction::operator+(Fraction ^larg, Fraction ^rarg)
	{
		if (Object::ReferenceEquals(larg, nullptr) || Object::ReferenceEquals(rarg, nullptr))
		{
			throw gcnew NullReferenceException();
		}
		PyObject *den;
		if (den = PyNumber_Multiply(larg->denom, rarg->denom))
		{
			try
			{
				PyObject *temp[2]
				{
					NULL, NULL
				};
				if (temp[0] = PyNumber_Multiply(larg->numer, rarg->denom))
				{
					try
					{
						if (temp[1] = PyNumber_Multiply(larg->denom, rarg->numer))
						{
							return gcnew Fraction(PyNumber_Add(temp[0], temp[1]), den);
						}
					}
					finally
					{
						Py_DECREF(temp[0]);
						Py_XDECREF(temp[1]);
					}
				}
			}
			catch (Exception^)
			{
				Py_DECREF(den);
				throw;
			}
		}
		throw gcnew Exception();
	}
	Fraction ^Fraction::operator*(Fraction ^larg, Fraction ^rarg)
	{
		if (Object::ReferenceEquals(larg, nullptr) || Object::ReferenceEquals(rarg, nullptr))
		{
			throw gcnew NullReferenceException();
		}
		PyObject *num;
		if (num = PyNumber_Multiply(larg->numer, rarg->numer))
		{
			try
			{
				PyObject *den;
				if (den = PyNumber_Multiply(larg->denom, rarg->denom))
				{
					try
					{
						return gcnew Fraction(num, den);
					}
					catch (Exception^)
					{
						Py_DECREF(den);
						throw;
					}
				}
			}
			catch (Exception^)
			{
				Py_DECREF(num);
				throw;
			}
		}
		throw gcnew Exception();
	}
	Fraction ^Fraction::operator^(Fraction ^base, Int64 exponent)
	{
		Fraction ^retval = (Fraction::one ? gcnew Fraction(Fraction::one, Fraction::one, false) : gcnew Fraction(1));
		if (!exponent)
		{
			return retval;
		}
		if (exponent < 0)
		{
			for (; exponent; exponent++)
			{
				retval /= base;
			}
			return retval;
		}
		//else exponent > 0
		retval = base;
		for (exponent--; exponent; exponent--)
		{
			retval *= base;
		}
		return retval;
	}
}